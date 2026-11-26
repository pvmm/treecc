if(NOT DEFINED TREECC_EXECUTABLE OR NOT DEFINED RECIPE OR
   NOT DEFINED DEFINITIONS OR NOT DEFINED WORKING_DIRECTORY)
  message(FATAL_ERROR "Missing required SDCC source file test parameter")
endif()

file(MAKE_DIRECTORY "${WORKING_DIRECTORY}")
file(REMOVE_RECURSE "${WORKING_DIRECTORY}/nested")
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

set(output_file "${WORKING_DIRECTORY}/nested/main.c")
if(NOT EXISTS "${output_file}")
  message(FATAL_ERROR "Expected source file was not created")
endif()
file(READ "${output_file}" actual)
set(expected "\nint main(void) {\n  return $name[0];\n}\n\n")
if(NOT actual STREQUAL expected)
  message(FATAL_ERROR "Unexpected source file contents: [${actual}]")
endif()
