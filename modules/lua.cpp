#include "treecc/lua_module.h"

#include <lua.hpp>

#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>

namespace {

lua_State *lua = nullptr;
int handlers_reference = LUA_NOREF;
std::string rule_name;

int set_rule(lua_State *state) {
  rule_name = luaL_checkstring(state, 1);
  return 0;
}

int get_rule(lua_State *state) {
  lua_pushstring(state, rule_name.c_str());
  return 1;
}

int call_handler(lua_State *state) {
  if (lua == nullptr || handlers_reference == LUA_NOREF) {
    lua_pushboolean(state, 0);
    return 1;
  }
  const char *name = luaL_checkstring(state, 1);
  luaL_checktype(state, 2, LUA_TTABLE);

  lua_rawgeti(state, LUA_REGISTRYINDEX, handlers_reference);
  lua_getfield(state, -1, name);
  lua_remove(state, -2);
  if (!lua_isfunction(state, -1)) {
    lua_pop(state, 1);
    return luaL_error(state, "no handler registered for command `%s`", name);
  }

  lua_pushvalue(state, 2);
  if (lua_pcall(state, 1, 1, 0) != LUA_OK) {
    return lua_error(state);
  }
  const bool succeeded = !lua_isboolean(state, -1) || lua_toboolean(state, -1);
  lua_pop(state, 1);
  lua_pushboolean(state, succeeded ? 1 : 0);
  return 1;
}

int create_parent_directories(lua_State *state) {
  std::size_t path_length = 0;
  const char *path = luaL_checklstring(state, 1, &path_length);
  const std::filesystem::path parent =
      std::filesystem::path(std::string(path, path_length)).parent_path();
  std::error_code error;
  if (!parent.empty()) {
    std::filesystem::create_directories(parent, error);
  }
  if (error) {
    lua_pushnil(state);
    lua_pushstring(state, error.message().c_str());
    return 2;
  }
  lua_pushboolean(state, true);
  return 1;
}

std::string node_text(TSNode node, const char *source) {
  const uint32_t start = ts_node_start_byte(node);
  const uint32_t end = ts_node_end_byte(node);
  return std::string(source + start, end - start);
}

void set_string(lua_State *state, const char *key, const std::string &value) {
  lua_pushlstring(state, value.data(), value.size());
  lua_setfield(state, -2, key);
}

void push_node(lua_State *state, TSNode node, const char *source) {
  const std::string type = ts_node_type(node);
  const std::string raw = node_text(node, source);
  const TSPoint start = ts_node_start_point(node);
  const TSPoint end = ts_node_end_point(node);

  lua_createtable(state, 0, 9);
  set_string(state, "type", type);
  set_string(state, "text", raw);
  lua_pushinteger(state, static_cast<lua_Integer>(start.row + 1));
  lua_setfield(state, -2, "line");
  lua_pushinteger(state, static_cast<lua_Integer>(start.column + 1));
  lua_setfield(state, -2, "column");
  lua_pushinteger(state, static_cast<lua_Integer>(end.row + 1));
  lua_setfield(state, -2, "end_line");
  lua_pushinteger(state, static_cast<lua_Integer>(end.column + 1));
  lua_setfield(state, -2, "end_column");

  if (type == "string" && raw.size() >= 2) {
    set_string(state, "value", raw.substr(1, raw.size() - 2));
  } else if (type == "identifier" || type == "number" ||
             type == "variable") {
    set_string(state, "value", raw);
  }

  if (type == "function_call") {
    const TSNode function = ts_node_child_by_field_name(node, "function", 8);
    if (!ts_node_is_null(function)) {
      set_string(state, "name", node_text(function, source));
    }

    lua_createtable(state, static_cast<int>(ts_node_named_child_count(node)), 0);
    int argument_index = 1;
    for (uint32_t i = 0; i < ts_node_named_child_count(node); ++i) {
      const TSNode child = ts_node_named_child(node, i);
      if (std::string(ts_node_type(child)) != "argument" ||
          ts_node_named_child_count(child) == 0) {
        continue;
      }
      push_node(state, ts_node_named_child(child, 0), source);
      lua_rawseti(state, -2, argument_index++);
    }
    lua_setfield(state, -2, "args");
  }

  lua_createtable(state, static_cast<int>(ts_node_named_child_count(node)), 0);
  int child_index = 1;
  for (uint32_t i = 0; i < ts_node_named_child_count(node); ++i) {
    push_node(state, ts_node_named_child(node, i), source);
    lua_rawseti(state, -2, child_index++);
  }
  lua_setfield(state, -2, "children");
}

void push_command(lua_State *state, TSNode command, const char *source) {
  push_node(state, command, source);

  const TSNode function = ts_node_child_by_field_name(command, "function", 8);
  set_string(state, "name", node_text(function, source));

  lua_createtable(state, static_cast<int>(ts_node_named_child_count(command)), 0);
  int argument_index = 1;
  for (uint32_t i = 0; i < ts_node_named_child_count(command); ++i) {
    const TSNode child = ts_node_named_child(command, i);
    if (std::string(ts_node_type(child)) != "argument" ||
        ts_node_named_child_count(child) == 0) {
      continue;
    }
    push_node(state, ts_node_named_child(child, 0), source);
    lua_rawseti(state, -2, argument_index++);
  }
  lua_setfield(state, -2, "args");
}

int dispatch_to_lua(TSNode command, const char *source, uint32_t) {
  if (lua == nullptr || handlers_reference == LUA_NOREF) {
    std::cerr << "Lua handlers are not initialized\n";
    return 1;
  }

  const TSNode function = ts_node_child_by_field_name(command, "function", 8);
  const std::string command_name = node_text(function, source);
  lua_rawgeti(lua, LUA_REGISTRYINDEX, handlers_reference);
  lua_getfield(lua, -1, command_name.c_str());
  lua_remove(lua, -2);
  if (!lua_isfunction(lua, -1)) {
    lua_pop(lua, 1);
    std::cerr << "no Lua handler registered for `" << command_name << "`\n";
    return 1;
  }

  push_command(lua, command, source);
  if (lua_pcall(lua, 1, 1, 0) != LUA_OK) {
    std::cerr << "Lua handler `" << command_name
              << "` failed: " << lua_tostring(lua, -1) << '\n';
    lua_pop(lua, 1);
    return 1;
  }

  const bool succeeded = !lua_isboolean(lua, -1) || lua_toboolean(lua, -1);
  lua_pop(lua, 1);
  return succeeded ? 0 : 1;
}

bool register_handlers(const TreeCCModuleRegistryV1 *registry,
                       const char *script_path,
                       const char *default_namespace) {
  const int initial_top = lua_gettop(lua);
  if (luaL_loadfile(lua, script_path) != LUA_OK ||
      lua_pcall(lua, 0, 1, 0) != LUA_OK) {
    std::cerr << "could not load Lua handlers from " << script_path << ": "
              << lua_tostring(lua, -1) << '\n';
    lua_settop(lua, initial_top);
    return false;
  }
  if (!lua_istable(lua, -1)) {
    std::cerr << script_path << " must return a table of handler functions\n";
    lua_settop(lua, initial_top);
    return false;
  }

  const int script_handlers = lua_gettop(lua);
  lua_pushnil(lua);
  while (lua_next(lua, script_handlers) != 0) {
    if (lua_type(lua, -2) != LUA_TSTRING || !lua_isfunction(lua, -1)) {
      std::cerr << script_path << " must map command names to functions\n";
      lua_settop(lua, initial_top);
      return false;
    }

    std::size_t name_length = 0;
    const char *name = lua_tolstring(lua, -2, &name_length);
    if (name_length == 0) {
      std::cerr << script_path << " contains an empty handler name\n";
      lua_settop(lua, initial_top);
      return false;
    }

    const std::string handler_name(name, name_length);
    const bool namespaced = handler_name.find("::") != std::string::npos;
    const bool bare = default_namespace == nullptr || default_namespace[0] == '\0';
    const std::string command_name =
        namespaced || bare
            ? handler_name
            : std::string(default_namespace) + "::" + handler_name;
    if (registry->register_command(registry->userdata, command_name.c_str(),
                                   dispatch_to_lua) != 0) {
      std::cerr << "failed to register Lua handler `" << command_name << "`\n";
      lua_settop(lua, initial_top);
      return false;
    }

    lua_rawgeti(lua, LUA_REGISTRYINDEX, handlers_reference);
    lua_pushstring(lua, command_name.c_str());
    lua_pushvalue(lua, -3);
    lua_settable(lua, -3);
    lua_pop(lua, 1);
    lua_pop(lua, 1);
  }
  lua_settop(lua, initial_top);
  return true;
}

}  // namespace

extern "C" int treecc_initialize_lua_module_v1(void) {
  if (lua != nullptr) {
    return 0;
  }
  lua = luaL_newstate();
  if (lua == nullptr) {
    std::cerr << "could not create Lua state\n";
    return 1;
  }
  luaL_openlibs(lua);
  lua_newtable(lua);
  lua_pushcfunction(lua, create_parent_directories);
  lua_setfield(lua, -2, "create_parent_directories");
  lua_pushcfunction(lua, set_rule);
  lua_setfield(lua, -2, "set_rule");
  lua_pushcfunction(lua, get_rule);
  lua_setfield(lua, -2, "rule");
  lua_pushcfunction(lua, call_handler);
  lua_setfield(lua, -2, "call_handler");
  lua_setglobal(lua, "treecc");
  lua_newtable(lua);
  handlers_reference = luaL_ref(lua, LUA_REGISTRYINDEX);
  return 0;
}

extern "C" void treecc_set_rule_v1(const char *rule) {
  rule_name = rule == nullptr ? "" : rule;
}

extern "C" int treecc_load_lua_handlers_v1(
    const TreeCCModuleRegistryV1 *registry, const char *script_path,
    const char *default_namespace) {
  if (registry == nullptr || registry->abi_version != TREECC_MODULE_ABI_V1 ||
      registry->register_command == nullptr || script_path == nullptr ||
      default_namespace == nullptr || lua == nullptr ||
      handlers_reference == LUA_NOREF) {
    return 1;
  }
  return register_handlers(registry, script_path, default_namespace) ? 0 : 1;
}

extern "C" void treecc_shutdown_lua_module(void) {
  if (lua != nullptr) {
    if (handlers_reference != LUA_NOREF) {
      luaL_unref(lua, LUA_REGISTRYINDEX, handlers_reference);
      handlers_reference = LUA_NOREF;
    }
    lua_close(lua);
    lua = nullptr;
  }
}
