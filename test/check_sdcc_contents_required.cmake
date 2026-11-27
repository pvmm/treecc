if(NOT DEFINED TREECC_EXECUTABLE OR NOT DEFINED RECIPE OR
   NOT DEFINED DEFINITIONS OR NOT DEFINED WORKING_DIRECTORY)
  message(FATAL_ERROR "Missing required SDCC contents test parameter")
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
if(result EQUAL 0)
  message(FATAL_ERROR "TreeCC unexpectedly succeeded:\n${stdout}\n${stderr}")
endif()
if(NOT stderr MATCHES "sdcc::function requires contents")
  message(FATAL_ERROR "Expected missing-contents error, got:\n${stdout}\n${stderr}")
endif()
if(EXISTS "${WORKING_DIRECTORY}/out/missing_contents.c")
  message(FATAL_ERROR "Function file was written without contents")
endif()
