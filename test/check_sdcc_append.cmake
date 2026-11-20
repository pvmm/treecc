if(NOT DEFINED TREECC_EXECUTABLE OR NOT DEFINED RECIPE OR
   NOT DEFINED DEFINITIONS OR NOT DEFINED WORKING_DIRECTORY OR
   NOT DEFINED TEST_MODE)
  message(FATAL_ERROR "Missing required SDCC append test parameter")
endif()

file(MAKE_DIRECTORY "${WORKING_DIRECTORY}")
set(output_file "${WORKING_DIRECTORY}/nested/include/output.asm")
if(TEST_MODE STREQUAL "create")
  file(REMOVE_RECURSE "${WORKING_DIRECTORY}/nested")
elseif(TEST_MODE STREQUAL "append")
  file(MAKE_DIRECTORY "${WORKING_DIRECTORY}/nested/include")
  file(WRITE "${output_file}" "existing\n")
else()
  message(FATAL_ERROR "Unknown SDCC append test mode: ${TEST_MODE}")
endif()

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

file(READ "${output_file}" actual)
if(TEST_MODE STREQUAL "create")
  set(expected ".ifdef _USED_EXAMPLE_\nPAYLOAD\n.endif\n")
else()
  set(expected "existing\n.ifdef _USED_EXAMPLE_\nPAYLOAD\n.endif\n")
endif()
if(NOT actual STREQUAL expected)
  message(FATAL_ERROR "Unexpected appended file contents: [${actual}]")
endif()
