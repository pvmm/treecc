#ifndef TREECC_MODULE_H
#define TREECC_MODULE_H

#include <tree_sitter/api.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TREECC_MODULE_ABI_V1 1

typedef int (*TreeCCCommandHandlerV1)(TSNode command, const char *source,
                                      uint32_t source_length);

typedef struct {
  uint32_t abi_version;
  void *userdata;
  int (*register_command)(void *userdata, const char *name,
                          TreeCCCommandHandlerV1 handler);
} TreeCCModuleRegistryV1;

typedef const char *(*TreeCCModuleNameV1)(void);
typedef int (*TreeCCModuleRegisterV1)(const TreeCCModuleRegistryV1 *registry);

#ifdef __cplusplus
}
#endif

#endif
