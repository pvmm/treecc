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

struct ParsedFile {
  std::string path;
  std::string source;
  Tree tree;
};

std::unique_ptr<ParsedFile> parse_file(TSParser *parser,
                                      const std::string &path) {
  try {
    auto parsed = std::make_unique<ParsedFile>();
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

  bool load_from_ruleset(TSNode root, const std::string &source,
                         const std::filesystem::path &executable_path) {
    std::vector<std::string> modules;
    collect_modules(root, source, modules);

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
      if (!load_module(module, search_paths)) {
        return false;
      }
    }
    return true;
  }

  TreeCCCommandHandlerV1 find(const std::string &name) const {
    return registry_.find(name);
  }

  bool register_builtin_lua(const std::filesystem::path &script_path) {
    ModuleRegistrationContext context{&registry_};
    const TreeCCModuleRegistryV1 api{TREECC_MODULE_ABI_V1, &context,
                                     register_command_callback};
    if (treecc_register_lua_module_v1(&api, script_path.c_str()) != 0) {
      return false;
    }
    std::cout << "Loaded built-in module: lua\n";
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
    if (name != "lua" && !name.empty() && seen.insert(name).second) {
      modules.push_back(name);
    }
  }

  static void collect_ruleset_modules(
      TSNode ruleset, const std::string &source,
      std::vector<std::string> &modules,
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
        const TSNode key_argument = ts_node_named_child(value, 0);
        add_module(string_value(argument_value(key_argument), source), modules,
                   seen);
      }
    }
  }

  static void collect_modules(TSNode node, const std::string &source,
                              std::vector<std::string> &modules) {
    std::unordered_set<std::string> seen;
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
              collect_ruleset_modules(context, source, modules, seen);
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
};

bool dispatch_commands(TSNode node, const std::string &source,
                       const ModuleManager &modules, const std::string &path) {
  bool success = true;
  if (std::string(ts_node_type(node)) == "function_call") {
    const TSNode function = ts_node_child_by_field_name(node, "function", 8);
    if (!ts_node_is_null(function)) {
      const std::string name = node_text(function, source);
      if (name.find("::") != std::string::npos) {
        const auto handler = modules.find(name);
        if (handler == nullptr) {
          std::cerr << path << ':' << ts_node_start_point(node).row + 1
                    << ": no loaded module registered command `" << name
                    << "`\n";
          success = false;
        } else if (handler(node, source.data(),
                           static_cast<std::uint32_t>(source.size())) != 0) {
          std::cerr << path << ':' << ts_node_start_point(node).row + 1
                    << ": command `" << name << "` failed\n";
          success = false;
        }
      }
    }
  }

  for (uint32_t i = 0; i < ts_node_child_count(node); ++i) {
    success = dispatch_commands(ts_node_child(node, i), source, modules, path) &&
              success;
  }
  return success;
}

bool contains_lua_command(TSNode node, const std::string &source) {
  if (std::string(ts_node_type(node)) == "function_call") {
    const TSNode function = ts_node_child_by_field_name(node, "function", 8);
    if (!ts_node_is_null(function) &&
        node_text(function, source).compare(0, 5, "lua::") == 0) {
      return true;
    }
  }

  for (uint32_t i = 0; i < ts_node_child_count(node); ++i) {
    if (contains_lua_command(ts_node_child(node, i), source)) {
      return true;
    }
  }
  return false;
}

}  // namespace

int main(int argc, char **argv) {
  LuaModuleGuard lua_module_guard;
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0]
              << " <recipe.rec> <definitions.def>\n";
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

  auto recipe = parse_file(parser.get(), recipe_path);
  auto definitions = parse_file(parser.get(), definitions_path);
  if (!recipe || !definitions) {
    return 1;
  }

  std::error_code path_error;
  const auto executable_path =
      std::filesystem::absolute(argv[0], path_error);
  if (path_error) {
    std::cerr << "could not resolve executable path: " << path_error.message()
              << '\n';
    return 2;
  }

  ModuleManager modules;
  const TSNode recipe_root = ts_tree_root_node(recipe->tree.get());
  if (!modules.load_from_ruleset(recipe_root, recipe->source, executable_path)) {
    return 1;
  }

  const TSNode definitions_root =
      ts_tree_root_node(definitions->tree.get());
  if ((contains_lua_command(recipe_root, recipe->source) ||
       contains_lua_command(definitions_root, definitions->source)) &&
      !modules.register_builtin_lua(executable_path.parent_path() / "lua.lua")) {
    return 1;
  }

  const bool recipe_commands = dispatch_commands(
      recipe_root, recipe->source, modules, recipe->path);
  const bool definition_commands =
      dispatch_commands(definitions_root, definitions->source, modules,
                        definitions->path);
  return recipe_commands && definition_commands ? 0 : 1;
}
