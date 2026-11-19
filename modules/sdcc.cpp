#include "treecc/module.h"

#include <iostream>
#include <string>

namespace {

std::string text(TSNode node, const char *source) {
  const uint32_t start = ts_node_start_byte(node);
  const uint32_t end = ts_node_end_byte(node);
  return std::string(source + start, end - start);
}

TSNode argument_value(TSNode argument) {
  if (std::string(ts_node_type(argument)) != "argument" ||
      ts_node_named_child_count(argument) == 0) {
    return TSNode();
  }
  return ts_node_named_child(argument, 0);
}

bool process_source_property(TSNode node, const char *source) {
  if (std::string(ts_node_type(node)) == "function_call") {
    const TSNode function = ts_node_child_by_field_name(node, "function", 8);
    if (!ts_node_is_null(function) && text(function, source) == "filename") {
      for (uint32_t i = 0; i < ts_node_named_child_count(node); ++i) {
        const TSNode value = argument_value(ts_node_named_child(node, i));
        if (!ts_node_is_null(value) &&
            std::string(ts_node_type(value)) == "string") {
          std::string filename = text(value, source);
          if (filename.size() < 2) {
            std::cerr << "sdcc module: malformed filename string\n";
            return false;
          }
          std::cout << "source filename: "
                    << filename.substr(1, filename.size() - 2) << '\n';
          return true;
        }
      }
      std::cerr << "sdcc module: filename command requires a string\n";
      return false;
    }
  }

  const uint32_t child_count = ts_node_child_count(node);
  for (uint32_t i = 0; i < child_count; ++i) {
    if (!process_source_property(ts_node_child(node, i), source)) {
      return false;
    }
  }
  return true;
}

int handle_source(TSNode command, const char *source, uint32_t) {
  std::string context_id;
  TSNode context = TSNode();
  for (uint32_t i = 0; i < ts_node_named_child_count(command); ++i) {
    const TSNode argument = ts_node_named_child(command, i);
    if (std::string(ts_node_type(argument)) != "argument" ||
        ts_node_named_child_count(argument) == 0) {
      continue;
    }

    const TSNode value = ts_node_named_child(argument, 0);
    if (std::string(ts_node_type(value)) == "identifier" && context_id.empty()) {
      context_id = text(value, source);
    } else if (std::string(ts_node_type(value)) == "context") {
      context = value;
    }
  }

  if (context_id.empty() || ts_node_is_null(context)) {
    std::cerr << "sdcc module: sdcc::source requires an id and context\n";
    return 1;
  }

  std::cout << "sdcc::source: " << context_id << '\n';
  return process_source_property(context, source) ? 0 : 1;
}

int handle_function(TSNode command, const char *source, uint32_t) {
  for (uint32_t i = 0; i < ts_node_named_child_count(command); ++i) {
    const TSNode value = argument_value(ts_node_named_child(command, i));
    if (!ts_node_is_null(value) &&
        std::string(ts_node_type(value)) == "identifier") {
      std::cout << "sdcc::function: " << text(value, source) << '\n';
      return 0;
    }
  }

  std::cerr << "sdcc module: sdcc::function requires an id\n";
  return 1;
}

int handle_other_command(TSNode command, const char *source, uint32_t) {
  const TSNode function = ts_node_child_by_field_name(command, "function", 8);
  if (ts_node_is_null(function)) {
    std::cerr << "sdcc module: namespaced command has no name\n";
    return 1;
  }
  std::cout << text(function, source) << ": recognized\n";
  return 0;
}

int register_command(const TreeCCModuleRegistryV1 *registry, const char *name,
                    TreeCCCommandHandlerV1 handler) {
  return registry->register_command(registry->userdata, name, handler);
}

}  // namespace

extern "C" const char *treecc_module_name_v1(void) {
  return "sdcc-bundle-4.2.0";
}

extern "C" int treecc_module_register_v1(
    const TreeCCModuleRegistryV1 *registry) {
  if (registry == nullptr || registry->abi_version != TREECC_MODULE_ABI_V1 ||
      registry->register_command == nullptr) {
    return 1;
  }
  if (register_command(registry, "sdcc::source", handle_source) != 0 ||
      register_command(registry, "sdcc::function", handle_function) != 0) {
    return 1;
  }

  const char *const other_commands[] = {
      "sdcc::append-to", "sdcc::clean",   "sdcc::compile",
      "sdcc::ihx2rom",   "sdcc::link",    "sdcc::openmsx",
      "sdcc::optional",
  };
  for (const char *name : other_commands) {
    if (register_command(registry, name, handle_other_command) != 0) {
      return 1;
    }
  }
  return 0;
}
