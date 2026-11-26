if(NOT DEFINED TREECC_EXECUTABLE OR NOT DEFINED RECIPE OR
   NOT DEFINED DEFINITIONS OR NOT DEFINED WORKING_DIRECTORY)
  message(FATAL_ERROR "Missing required SDCC optional include test parameter")
endif()

file(MAKE_DIRECTORY "${WORKING_DIRECTORY}")
file(REMOVE_RECURSE "${WORKING_DIRECTORY}/out")
execute_process(
  COMMAND "${TREECC_EXECUTABLE}" "${RECIPE}" "${DEFINITIONS}"
  WORKING_DIRECTORY "${WORKING_DIRECTORY}"
  RESULT_VARIABLE result
  OUTPUT_VARIABLE stdout
  ERROR_VARIABLE stderr
)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "TreeCC failed (${result}):\n${stdout}\n${stderr}")
endif()

file(READ "${WORKING_DIRECTORY}/out/optional_c.c" c_source)
file(READ "${WORKING_DIRECTORY}/out/optional_asm.asm" assembly)
set(expected_c
  "#include \"out/defs.h\"\n#include \"out/translations.h\"\n#ifdef _USED_optional_c_FUNCTION\nvoid optional_c(void) {\n}\n#endif\n")
set(expected_asm
  ".include \"out/defs.asm\"\n.include \"out/translations.asm\"\n.ifdef _USED_optional_asm_FUNCTION\nnop\n.endif\n")
if(NOT c_source STREQUAL expected_c)
  message(FATAL_ERROR "Unexpected optional C function: [${c_source}]")
endif()
if(NOT assembly STREQUAL expected_asm)
  message(FATAL_ERROR "Unexpected optional assembly function: [${assembly}]")
endif()

file(READ "${WORKING_DIRECTORY}/out/translations.asm" translations)
set(expected_translations
  ".define optional_c /_USED_optional_c/\n.define optional_asm /_USED_optional_asm/\n")
if(NOT translations STREQUAL expected_translations)
  message(FATAL_ERROR "Unexpected translations.asm: [${translations}]")
endif()
