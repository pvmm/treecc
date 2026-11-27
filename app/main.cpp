#include <tree_sitter/api.h>
#include "treecc/module.h"
#include "treecc/lua_module.h"

#include <cstdint>
#include <cstdlib>
#include <dlfcn.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

extern "C" const TSLanguage *tree_sitter_treecc();

namespace {

struct ParserDeleter {
  void operator()(TSParser *parser) const { ts_parser_delete(parser); }
};

struct TreeDeleter {
  void operator()(TSTree *tree) const { ts_tree_delete(tree); }
};

struct StringDeleter {
  void operator()(char *value) const { std::free(value); }
};

using Parser = std::unique_ptr<TSParser, ParserDeleter>;
using Tree = std::unique_ptr<TSTree, TreeDeleter>;
using CStr = std::unique_ptr<char, StringDeleter>;

struct LuaModuleGuard {
  ~LuaModuleGuard() { treecc_shutdown_lua_module(); }
};

std::string node_text(TSNode node, const std::string &source) {
  const uint32_t start = ts_node_start_byte(node);
  const uint32_t end = ts_node_end_byte(node);
  return source.substr(start, end - start);
}

std::string string_node_value(TSNode node, const std::string &source) {
  const std::string raw = node_text(node, source);
  if (raw.size() < 2 || raw.front() != '"' || raw.back() != '"') {
    return {};
  }
  return raw.substr(1, raw.size() - 2);
}

std::filesystem::path resolve_executable_path(const char *argv0) {
  std::error_code error;
#if defined(__linux__)
  const auto proc_path = std::filesystem::read_symlink("/proc/self/exe", error);
  if (!error) {
    return proc_path;
  }
#endif
  const auto absolute_path = std::filesystem::absolute(argv0, error);
  if (error) {
    return argv0;
  }
  std::error_code canonical_error;
  const auto canonical_path =
      std::filesystem::canonical(absolute_path, canonical_error);
  return canonical_error ? absolute_path : canonical_path;
}

std::string read_file(const std::string &path) {
  std::ifstream input(path, std::ios::binary);
  if (!input) {
    throw std::runtime_error("cannot open file");
  }

  std::string source((std::istreambuf_iterator<char>(input)),
                     std::istreambuf_iterator<char>());
  if (input.bad()) {
    throw std::runtime_error("failed while reading file");
  }
  if (source.size() > std::numeric_limits<std::uint32_t>::max()) {
    throw std::runtime_error("file exceeds Tree-sitter's input size limit");
  }
  return source;
}

void report_errors(TSNode node, const std::string &path, int &error_count) {
  if (ts_node_is_error(node) || ts_node_is_missing(node)) {
    const TSPoint point = ts_node_start_point(node);
    std::cerr << path << ':' << point.row + 1 << ':' << point.column + 1 << ": "
              << (ts_node_is_missing(node) ? "missing syntax" : "syntax error")
              << " near `" << ts_node_type(node) << "`\n";
    ++error_count;
  }

  const uint32_t child_count = ts_node_child_count(node);
  for (uint32_t i = 0; i < child_count; ++i) {
    report_errors(ts_node_child(node, i), path, error_count);
  }
}

struct Document;

struct DocumentItem {
  TSNode node{};
  std::unique_ptr<Document> included;
};

struct Document {
  std::string path;
  std::string source;
  Tree tree;
  std::vector<DocumentItem> items;
};

struct IncludeGuard {
  std::unordered_set<std::string> &active;
  std::string key;

  ~IncludeGuard() { active.erase(key); }
};

std::unique_ptr<Document> parse_document(
    TSParser *parser, const std::string &path,
    std::unordered_set<std::string> &active) {
  std::error_code path_error;
  const auto absolute = std::filesystem::absolute(path, path_error);
  const std::string key =
      path_error ? path : absolute.lexically_normal().string();
  if (!active.insert(key).second) {
    std::cerr << path << ": recursive include detected\n";
    return nullptr;
  }
  IncludeGuard guard{active, key};

  try {
    auto parsed = std::make_unique<Document>();
    parsed->path = path;
    parsed->source = read_file(path);
    Tree tree(ts_parser_parse_string(
        parser, nullptr, parsed->source.data(),
        static_cast<std::uint32_t>(parsed->source.size())));
    if (!tree) {
      std::cerr << path << ": parsing failed\n";
      return nullptr;
    }

    const TSNode root = ts_tree_root_node(tree.get());
    CStr tree_text(ts_node_string(root));
    std::cout << "== " << path << " ==\n" << tree_text.get() << '\n';

    int error_count = 0;
    report_errors(root, path, error_count);
    if (error_count != 0 || ts_node_has_error(root)) {
      if (error_count == 0) {
        std::cerr << path << ": syntax error\n";
      }
      return nullptr;
    }

    std::cout << path << ": syntax OK\n";
    parsed->tree = std::move(tree);

    for (uint32_t i = 0; i < ts_node_named_child_count(root); ++i) {
      const TSNode child = ts_node_named_child(root, i);
      DocumentItem item;
      if (std::string(ts_node_type(child)) == "include") {
        const TSNode path_node = ts_node_child_by_field_name(child, "path", 4);
        if (ts_node_is_null(path_node)) {
          std::cerr << path << ':' << ts_node_start_point(child).row + 1
                    << ": include requires a path\n";
          return nullptr;
        }
        const std::string include_path =
            string_node_value(path_node, parsed->source);
        if (include_path.empty()) {
          std::cerr << path << ':' << ts_node_start_point(child).row + 1
                    << ": include requires a string path\n";
          return nullptr;
        }
        const auto resolved =
            std::filesystem::path(path).parent_path() / include_path;
        std::error_code resolved_error;
        const auto resolved_absolute =
            std::filesystem::absolute(resolved, resolved_error);
        const std::string resolved_key =
            resolved_error ? resolved.string()
                           : resolved_absolute.lexically_normal().string();
        if (active.count(resolved_key) != 0) {
          std::cerr << path << ':' << ts_node_start_point(child).row + 1
                    << ": recursive include of `" << include_path << "`\n";
          return nullptr;
        }
        item.included = parse_document(parser, resolved.string(), active);
        if (!item.included) {
          return nullptr;
        }
      } else {
        item.node = child;
      }
      parsed->items.push_back(std::move(item));
    }

    return parsed;
  } catch (const std::exception &error) {
    std::cerr << path << ": " << error.what() << '\n';
    return nullptr;
  }
}

class CommandRegistry {
 public:
  int register_command(const char *name, TreeCCCommandHandlerV1 handler) {
    if (name == nullptr || handler == nullptr) {
      return 1;
    }
    return handlers_.emplace(name, handler).second ? 0 : 1;
  }

  TreeCCCommandHandlerV1 find(const std::string &name) const {
    const auto command = handlers_.find(name);
    return command == handlers_.end() ? nullptr : command->second;
  }

 private:
  std::unordered_map<std::string, TreeCCCommandHandlerV1> handlers_;
};

struct ModuleRegistrationContext {
  CommandRegistry *registry;
};

int register_command_callback(void *userdata, const char *name,
                              TreeCCCommandHandlerV1 handler) {
  auto *context = static_cast<ModuleRegistrationContext *>(userdata);
  return context->registry->register_command(name, handler);
}

class ModuleManager {
 public:
  ~ModuleManager() {
    for (void *handle : handles_) {
      dlclose(handle);
    }
  }

  bool load_from_ruleset(const Document &recipe, const Document &definitions,
                         const std::filesystem::path &executable_path) {
    std::vector<std::string> modules;
    std::unordered_map<std::string, std::string> aliases;
    std::unordered_set<std::string> seen_modules;
    collect_modules(recipe, modules, aliases, seen_modules);
    collect_modules(definitions, modules, aliases, seen_modules);
    builtin_lua_requested_ = false;
    sdcc_module_requested_ = false;
    sdcc_namespace_ = "sdcc";

    std::vector<std::filesystem::path> search_paths{
        executable_path.parent_path()};
    if (const char *environment = std::getenv("TREECC_MODULE_PATH")) {
      std::string paths(environment);
      std::size_t start = 0;
      while (start <= paths.size()) {
        const std::size_t end = paths.find(':', start);
        const std::string path = paths.substr(start, end - start);
        if (!path.empty()) {
          search_paths.emplace_back(path);
        }
        if (end == std::string::npos) {
          break;
        }
        start = end + 1;
      }
    }

    for (const std::string &module : modules) {
      if (module == "lua") {
        builtin_lua_requested_ = true;
        continue;
      }
      if (module == "sdcc-bundle-4.2.0" || module == "sdcc-4.2.0") {
        sdcc_module_requested_ = true;
        const auto alias = aliases.find(module);
        if (alias != aliases.end()) {
          sdcc_namespace_ = alias->second;
        }
        continue;
      }
      if (!load_module(module, search_paths)) {
        return false;
      }
    }
    return true;
  }

  TreeCCCommandHandlerV1 find(const std::string &name) const {
    return registry_.find(name);
  }

  bool builtin_lua_requested() const { return builtin_lua_requested_; }

  bool sdcc_module_requested() const { return sdcc_module_requested_; }

  const std::string &sdcc_namespace() const { return sdcc_namespace_; }

  bool initialize_builtin_lua() {
    if (treecc_initialize_lua_module_v1() != 0) {
      return false;
    }
    return true;
  }

  bool load_builtin_lua_handlers(const std::filesystem::path &script_path,
                                 const char *default_namespace) {
    ModuleRegistrationContext context{&registry_};
    const TreeCCModuleRegistryV1 api{TREECC_MODULE_ABI_V1, &context,
                                     register_command_callback};
    if (treecc_load_lua_handlers_v1(&api, script_path.c_str(),
                                    default_namespace) != 0) {
      return false;
    }
    std::cout << "Loaded Lua handlers: " << script_path.filename().string()
              << '\n';
    return true;
  }

 private:
  static TSNode argument_value(TSNode argument) {
    if (std::string(ts_node_type(argument)) != "argument" ||
        ts_node_named_child_count(argument) == 0) {
      return TSNode();
    }
    return ts_node_named_child(argument, 0);
  }

  static std::string string_value(TSNode node, const std::string &source) {
    if (std::string(ts_node_type(node)) != "string") {
      return {};
    }
    std::string value = node_text(node, source);
    if (value.size() < 2) {
      return {};
    }
    return value.substr(1, value.size() - 2);
  }

  static void add_module(const std::string &name,
                         std::vector<std::string> &modules,
                         std::unordered_set<std::string> &seen) {
    if (!name.empty() && seen.insert(name).second) {
      modules.push_back(name);
    }
  }

  static std::string value_text(TSNode node, const std::string &source) {
    if (ts_node_is_null(node)) {
      return {};
    }
    TSNode value = node;
    if (std::string(ts_node_type(value)) == "argument") {
      value = argument_value(value);
    }
    if (ts_node_is_null(value)) {
      return {};
    }
    if (std::string(ts_node_type(value)) == "string") {
      return string_value(value, source);
    }
    return node_text(value, source);
  }

  static void collect_ruleset_modules(
      TSNode ruleset, const std::string &source,
      std::vector<std::string> &modules,
      std::unordered_map<std::string, std::string> &aliases,
      std::unordered_set<std::string> &seen) {
    for (uint32_t i = 0; i < ts_node_named_child_count(ruleset); ++i) {
      TSNode value = ts_node_named_child(ruleset, i);
      if (std::string(ts_node_type(value)) == "argument") {
        value = argument_value(value);
      }
      if (ts_node_is_null(value)) {
        continue;
      }
      if (std::string(ts_node_type(value)) == "string") {
        add_module(string_value(value, source), modules, seen);
      } else if (std::string(ts_node_type(value)) == "mapping" &&
                 ts_node_named_child_count(value) >= 1) {
        const std::string name =
            value_text(ts_node_named_child(value, 0), source);
        add_module(name, modules, seen);
        if (ts_node_named_child_count(value) >= 2) {
          const std::string alias =
              value_text(ts_node_named_child(value, 1), source);
          if (!alias.empty()) {
            aliases[name] = alias;
          }
        }
      }
    }
  }

  static void collect_modules(
      TSNode node, const std::string &source, std::vector<std::string> &modules,
      std::unordered_map<std::string, std::string> &aliases,
      std::unordered_set<std::string> &seen) {
    std::function<void(TSNode)> visit = [&](TSNode current) {
      if (std::string(ts_node_type(current)) == "function_call") {
        const TSNode function =
            ts_node_child_by_field_name(current, "function", 8);
        if (!ts_node_is_null(function) &&
            node_text(function, source) == "ruleset") {
          for (uint32_t i = 0; i < ts_node_named_child_count(current); ++i) {
            const TSNode context = argument_value(ts_node_named_child(current, i));
            if (!ts_node_is_null(context) &&
                std::string(ts_node_type(context)) == "context") {
              collect_ruleset_modules(context, source, modules, aliases, seen);
            }
          }
        }
      }

      for (uint32_t i = 0; i < ts_node_child_count(current); ++i) {
        visit(ts_node_child(current, i));
      }
    };
    visit(node);
  }

  static void collect_modules(
      const Document &document, std::vector<std::string> &modules,
      std::unordered_map<std::string, std::string> &aliases,
      std::unordered_set<std::string> &seen) {
    for (const auto &item : document.items) {
      if (item.included) {
        collect_modules(*item.included, modules, aliases, seen);
      } else {
        collect_modules(item.node, document.source, modules, aliases, seen);
      }
    }
  }

  bool load_module(const std::string &name,
                   const std::vector<std::filesystem::path> &search_paths) {
    if (name.find_first_not_of(
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.") !=
        std::string::npos) {
      std::cerr << "invalid module name in ruleset: " << name << '\n';
      return false;
    }

    const std::string filename = "libtreecc-module-" + name + ".so";
    std::filesystem::path module_path;
    for (const auto &directory : search_paths) {
      const auto candidate = directory / filename;
      if (std::filesystem::is_regular_file(candidate)) {
        module_path = candidate;
        break;
      }
    }
    if (module_path.empty()) {
      std::cerr << "module `" << name << "` not found as " << filename
                << "; searched beside the app and in TREECC_MODULE_PATH\n";
      return false;
    }

    void *handle = dlopen(module_path.c_str(), RTLD_NOW | RTLD_LOCAL);
    if (handle == nullptr) {
      std::cerr << "could not load module `" << name << "`: " << dlerror()
                << '\n';
      return false;
    }

    dlerror();
    auto module_name = reinterpret_cast<TreeCCModuleNameV1>(
        dlsym(handle, "treecc_module_name_v1"));
    const char *symbol_error = dlerror();
    if (symbol_error != nullptr || module_name == nullptr) {
      std::cerr << "module `" << name
                << "` does not export treecc_module_name_v1\n";
      dlclose(handle);
      return false;
    }
    if (name != module_name()) {
      std::cerr << "module name mismatch: ruleset requested `" << name
                << "` but library registers `" << module_name() << "`\n";
      dlclose(handle);
      return false;
    }

    dlerror();
    auto register_module = reinterpret_cast<TreeCCModuleRegisterV1>(
        dlsym(handle, "treecc_module_register_v1"));
    symbol_error = dlerror();
    if (symbol_error != nullptr || register_module == nullptr) {
      std::cerr << "module `" << name
                << "` does not export treecc_module_register_v1\n";
      dlclose(handle);
      return false;
    }

    ModuleRegistrationContext context{&registry_};
    const TreeCCModuleRegistryV1 api{TREECC_MODULE_ABI_V1, &context,
                                     register_command_callback};
    if (register_module(&api) != 0) {
      std::cerr << "module `" << name << "` failed to register commands\n";
      dlclose(handle);
      return false;
    }

    handles_.push_back(handle);
    std::cout << "Loaded module: " << name << '\n';
    return true;
  }

  CommandRegistry registry_;
  std::vector<void *> handles_;
  bool builtin_lua_requested_ = false;
  bool sdcc_module_requested_ = false;
  std::string sdcc_namespace_ = "sdcc";
};

bool dispatch_commands(TSNode node, const std::string &source,
                       const ModuleManager &modules, const std::string &path) {
  bool success = true;
  bool handled_actions = false;
  if (std::string(ts_node_type(node)) == "function_call") {
    const TSNode function = ts_node_child_by_field_name(node, "function", 8);
    if (!ts_node_is_null(function)) {
      const std::string name = node_text(function, source);
      const auto handler = modules.find(name);
      if (handler != nullptr) {
        treecc_set_source_path_v1(path.c_str());
        if (handler(node, source.data(),
                    static_cast<std::uint32_t>(source.size())) != 0) {
          std::cerr << path << ':' << ts_node_start_point(node).row + 1
                    << ": command `" << name << "` failed\n";
          success = false;
        }
        handled_actions = name == "actions";
      } else if (name.find("::") != std::string::npos) {
        std::cerr << path << ':' << ts_node_start_point(node).row + 1
                  << ": no loaded module registered command `" << name
                  << "`\n";
        success = false;
      }
    }
  }

  if (!handled_actions) {
    for (uint32_t i = 0; i < ts_node_child_count(node); ++i) {
      success =
          dispatch_commands(ts_node_child(node, i), source, modules, path) &&
          success;
    }
  }
  return success;
}

bool dispatch_commands(const Document &document, const ModuleManager &modules) {
  bool success = true;
  for (const auto &item : document.items) {
    if (item.included) {
      success = dispatch_commands(*item.included, modules) && success;
    } else {
      success =
          dispatch_commands(item.node, document.source, modules,
                            document.path) &&
          success;
    }
  }
  return success;
}

bool contains_command_prefix(TSNode node, const std::string &source,
                             const std::string &prefix) {
  if (std::string(ts_node_type(node)) == "function_call") {
    const TSNode function = ts_node_child_by_field_name(node, "function", 8);
    if (!ts_node_is_null(function)) {
      const std::string name = node_text(function, source);
      if (name.compare(0, prefix.size(), prefix) == 0) {
        return true;
      }
    }
  }

  for (uint32_t i = 0; i < ts_node_child_count(node); ++i) {
    if (contains_command_prefix(ts_node_child(node, i), source, prefix)) {
      return true;
    }
  }
  return false;
}

bool contains_command_prefix(const Document &document,
                             const std::string &prefix) {
  for (const auto &item : document.items) {
    if (item.included) {
      if (contains_command_prefix(*item.included, prefix)) {
        return true;
      }
    } else if (contains_command_prefix(item.node, document.source, prefix)) {
      return true;
    }
  }
  return false;
}

}  // namespace

int main(int argc, char **argv) {
  LuaModuleGuard lua_module_guard;
  if (argc < 3 || argc > 4) {
    std::cerr << "Usage: " << argv[0]
              << " <recipe.rec> <definitions.def> [rule]\n";
    return 2;
  }

  const std::string recipe_path = argv[1];
  const std::string definitions_path = argv[2];
  if (std::filesystem::path(recipe_path).extension() != ".rec") {
    std::cerr << "recipe file must have a .rec extension: " << recipe_path << '\n';
    return 2;
  }
  if (std::filesystem::path(definitions_path).extension() != ".def") {
    std::cerr << "definitions file must have a .def extension: "
              << definitions_path << '\n';
    return 2;
  }

  Parser parser(ts_parser_new());
  if (!parser) {
    std::cerr << "could not create Tree-sitter parser\n";
    return 2;
  }
  if (!ts_parser_set_language(parser.get(), tree_sitter_treecc())) {
    std::cerr << "TreeCC grammar is incompatible with the Tree-sitter runtime\n";
    return 2;
  }

  std::unordered_set<std::string> active_includes;
  auto recipe = parse_document(parser.get(), recipe_path, active_includes);
  auto definitions =
      parse_document(parser.get(), definitions_path, active_includes);
  if (!recipe || !definitions) {
    return 1;
  }

  const auto executable_path = resolve_executable_path(argv[0]);

  ModuleManager modules;
  if (!modules.load_from_ruleset(*recipe, *definitions, executable_path)) {
    return 1;
  }

  const bool lua_handlers_requested =
      modules.builtin_lua_requested() ||
      contains_command_prefix(*recipe, "lua::") ||
      contains_command_prefix(*definitions, "lua::");
  if (!modules.initialize_builtin_lua()) {
    return 1;
  }
  treecc_set_rule_v1(argc == 4 ? argv[3] : "default");
  if (!modules.load_builtin_lua_handlers(executable_path.parent_path() /
                                             "actions.lua",
                                         "")) {
    return 1;
  }
  if (lua_handlers_requested &&
      !modules.load_builtin_lua_handlers(executable_path.parent_path() /
                                             "lua.lua",
                                         "lua")) {
    return 1;
  }
  if (modules.sdcc_module_requested() &&
      !modules.load_builtin_lua_handlers(executable_path.parent_path() /
                                             "sdcc.lua",
                                         modules.sdcc_namespace().c_str())) {
    return 1;
  }

  const bool recipe_commands = dispatch_commands(*recipe, modules);
  const bool definition_commands = dispatch_commands(*definitions, modules);
  return recipe_commands && definition_commands ? 0 : 1;
}
