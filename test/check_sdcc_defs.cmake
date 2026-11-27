if(NOT DEFINED TREECC_EXECUTABLE OR NOT DEFINED RECIPE OR
   NOT DEFINED DEFINITIONS OR NOT DEFINED WORKING_DIRECTORY)
  message(FATAL_ERROR "Missing required SDCC defs test parameter")
endif()

file(MAKE_DIRECTORY "${WORKING_DIRECTORY}")
file(REMOVE_RECURSE "${WORKING_DIRECTORY}/out")
execute_process(
  COMMAND "${CMAKE_COMMAND}" -E env
    "FAKE_SDCC_LST_CONTENT=  _USED_example  0001  _USED_example_c  0001"
    "${TREECC_EXECUTABLE}" "${RECIPE}" "${DEFINITIONS}"
  WORKING_DIRECTORY "${WORKING_DIRECTORY}"
  RESULT_VARIABLE result
  OUTPUT_VARIABLE stdout
  ERROR_VARIABLE stderr
)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "TreeCC failed (${result}):\n${stdout}\n${stderr}")
endif()

foreach(name IN ITEMS defs.h defs.asm)
  set(defs_file "${WORKING_DIRECTORY}/out/${name}")
  if(NOT EXISTS "${defs_file}")
    message(FATAL_ERROR "Expected out/${name} was not created")
  endif()
endforeach()

file(READ "${WORKING_DIRECTORY}/out/defs.h" header)
file(READ "${WORKING_DIRECTORY}/out/defs.asm" assembly)
set(expected_header "#pragma once\n#define _USED_example_c_FUNCTION 1\n")
set(expected_assembly
  ".ifndef _DEFS_ASM_INCLUDED\n.define _DEFS_ASM_INCLUDED /1/\n.define _USED_example_FUNCTION /1/\n.endif\n")
if(NOT header STREQUAL expected_header)
  message(FATAL_ERROR "Unexpected defs.h contents: [${header}]")
endif()
if(NOT assembly STREQUAL expected_assembly)
  message(FATAL_ERROR "Unexpected defs.asm contents: [${assembly}]")
endif()
