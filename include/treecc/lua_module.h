#ifndef TREECC_LUA_MODULE_H
#define TREECC_LUA_MODULE_H

#include "treecc/module.h"

#ifdef __cplusplus
extern "C" {
#endif

int treecc_register_lua_module_v1(const TreeCCModuleRegistryV1 *registry,
                                  const char *script_path);
void treecc_shutdown_lua_module(void);

#ifdef __cplusplus
}
#endif

#endif
