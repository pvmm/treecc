if(NOT DEFINED TREECC_EXECUTABLE OR NOT DEFINED RECIPE OR
   NOT DEFINED DEFINITIONS OR NOT DEFINED WORKING_DIRECTORY)
  message(FATAL_ERROR "Missing required SDCC contents include test parameter")
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

set(c_file "${WORKING_DIRECTORY}/out/from_file.c")
if(NOT EXISTS "${c_file}")
  message(FATAL_ERROR "Expected function file was not created: ${c_file}")
endif()

file(READ "${c_file}" contents)
if(NOT contents MATCHES "#include \"out/defs\\.h\"\n")
  message(FATAL_ERROR "Generated C file is missing the defs include:\n${contents}")
endif()
if(NOT contents MATCHES "void from_file\\(void\\)")
  message(FATAL_ERROR "Contents include did not interpolate the function name:\n${contents}")
endif()
if(NOT contents MATCHES "from file")
  message(FATAL_ERROR "Contents include did not interpolate variables:\n${contents}")
endif()
