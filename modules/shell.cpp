#include "treecc/module.h"

extern "C" const char *treecc_module_name_v1(void) { return "shell"; }

extern "C" int treecc_module_register_v1(
    const TreeCCModuleRegistryV1 *registry) {
  return registry == nullptr || registry->abi_version != TREECC_MODULE_ABI_V1 ||
                 registry->register_command == nullptr
             ? 1
             : 0;
}
