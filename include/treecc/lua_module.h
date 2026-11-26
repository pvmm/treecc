#ifndef TREECC_LUA_MODULE_H
#define TREECC_LUA_MODULE_H

#include "treecc/module.h"

#ifdef __cplusplus
extern "C" {
#endif

int treecc_initialize_lua_module_v1(void);
void treecc_set_rule_v1(const char *rule);
int treecc_load_lua_handlers_v1(const TreeCCModuleRegistryV1 *registry,
                                const char *script_path,
                                const char *default_namespace);
void treecc_shutdown_lua_module(void);

#ifdef __cplusplus
}
#endif

#endif
