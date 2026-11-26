if(NOT DEFINED TREECC_EXECUTABLE OR NOT DEFINED RECIPE OR
   NOT DEFINED DEFINITIONS OR NOT DEFINED WORKING_DIRECTORY)
  message(FATAL_ERROR "Missing required SDCC function include test parameter")
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

set(asm_file "${WORKING_DIRECTORY}/out/from_asm.asm")
set(c_file "${WORKING_DIRECTORY}/out/from_c.c")
foreach(path IN ITEMS "${asm_file}" "${c_file}")
  if(NOT EXISTS "${path}")
    message(FATAL_ERROR "Expected function file was not created: ${path}")
  endif()
endforeach()

file(READ "${asm_file}" assembly)
file(READ "${c_file}" c_source)
if(NOT assembly MATCHES "^\\.include \"out/defs\\.asm\"\n")
  message(FATAL_ERROR "Assembly function does not start with defs include: [${assembly}]")
endif()
if(NOT c_source MATCHES "^#include \"out/defs\\.h\"\n")
  message(FATAL_ERROR "C function does not start with defs include: [${c_source}]")
endif()
