#include "treecc/lua_module.h"

#include <lua.hpp>

#include <cstdint>
#include <iostream>
#include <string>

namespace {

lua_State *lua = nullptr;
int handlers_reference = LUA_NOREF;

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

  lua_createtable(state, 0, 7);
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
    std::cerr << "lua module: Lua handlers are not initialized\n";
    return 1;
  }

  const TSNode function = ts_node_child_by_field_name(command, "function", 8);
  const std::string command_name = node_text(function, source);
  constexpr char prefix[] = "lua::";
  if (command_name.compare(0, sizeof(prefix) - 1, prefix) != 0) {
    std::cerr << "lua module: unexpected command `" << command_name << "`\n";
    return 1;
  }

  lua_rawgeti(lua, LUA_REGISTRYINDEX, handlers_reference);
  lua_getfield(lua, -1, command_name.c_str() + sizeof(prefix) - 1);
  lua_remove(lua, -2);
  if (!lua_isfunction(lua, -1)) {
    lua_pop(lua, 1);
    std::cerr << "lua module: no handler registered for `" << command_name
              << "`\n";
    return 1;
  }

  push_command(lua, command, source);
  if (lua_pcall(lua, 1, 1, 0) != LUA_OK) {
    std::cerr << "lua module: " << lua_tostring(lua, -1) << '\n';
    lua_pop(lua, 1);
    return 1;
  }

  const bool succeeded = !lua_isboolean(lua, -1) || lua_toboolean(lua, -1);
  lua_pop(lua, 1);
  return succeeded ? 0 : 1;
}

bool register_handlers(const TreeCCModuleRegistryV1 *registry) {
  lua_rawgeti(lua, LUA_REGISTRYINDEX, handlers_reference);
  lua_pushnil(lua);
  while (lua_next(lua, -2) != 0) {
    if (lua_type(lua, -2) != LUA_TSTRING || !lua_isfunction(lua, -1)) {
      std::cerr << "lua module: handlers table must map names to functions\n";
      lua_pop(lua, 3);
      return false;
    }

    std::size_t name_length = 0;
    const char *name = lua_tolstring(lua, -2, &name_length);
    if (name_length == 0 ||
        std::string(name, name_length).find("::") != std::string::npos) {
      std::cerr << "lua module: handler names must be nonempty and unqualified\n";
      lua_pop(lua, 3);
      return false;
    }

    const std::string command_name = "lua::" + std::string(name, name_length);
    if (registry->register_command(registry->userdata, command_name.c_str(),
                                   dispatch_to_lua) != 0) {
      std::cerr << "lua module: failed to register `" << command_name << "`\n";
      lua_pop(lua, 3);
      return false;
    }
    lua_pop(lua, 1);
  }
  lua_pop(lua, 1);
  return true;
}

}  // namespace

extern "C" int treecc_register_lua_module_v1(
    const TreeCCModuleRegistryV1 *registry, const char *script_path) {
  if (registry == nullptr || registry->abi_version != TREECC_MODULE_ABI_V1 ||
      registry->register_command == nullptr || script_path == nullptr) {
    return 1;
  }

  lua = luaL_newstate();
  if (lua == nullptr) {
    std::cerr << "lua module: could not create Lua state\n";
    return 1;
  }
  luaL_openlibs(lua);
  if (luaL_loadfile(lua, script_path) != LUA_OK ||
      lua_pcall(lua, 0, 1, 0) != LUA_OK) {
    std::cerr << "lua module: could not load " << script_path << ": "
              << lua_tostring(lua, -1) << '\n';
    lua_close(lua);
    lua = nullptr;
    return 1;
  }
  if (!lua_istable(lua, -1)) {
    std::cerr << "lua module: " << script_path
              << " must return a table of handler functions\n";
    lua_pop(lua, 1);
    lua_close(lua);
    lua = nullptr;
    return 1;
  }

  handlers_reference = luaL_ref(lua, LUA_REGISTRYINDEX);
  if (!register_handlers(registry)) {
    luaL_unref(lua, LUA_REGISTRYINDEX, handlers_reference);
    handlers_reference = LUA_NOREF;
    lua_close(lua);
    lua = nullptr;
    return 1;
  }
  return 0;
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
