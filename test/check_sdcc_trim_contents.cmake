if(NOT DEFINED TREECC_EXECUTABLE OR NOT DEFINED RECIPE OR
   NOT DEFINED DEFINITIONS OR NOT DEFINED WORKING_DIRECTORY)
  message(FATAL_ERROR "Missing required SDCC trim test parameter")
endif()

file(MAKE_DIRECTORY "${WORKING_DIRECTORY}")
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

file(READ "${WORKING_DIRECTORY}/trim/left.c" left)
file(READ "${WORKING_DIRECTORY}/trim/right.c" right)
file(READ "${WORKING_DIRECTORY}/trim/both.c" both)
if(NOT left STREQUAL "#include \"out/translations.h\"\nPAYLOAD\n")
  message(FATAL_ERROR "Unexpected left trim output: [${left}]")
endif()
if(NOT right STREQUAL "#include \"out/translations.h\"\n  PAYLOAD")
  message(FATAL_ERROR "Unexpected right trim output: [${right}]")
endif()
if(NOT both STREQUAL "#include \"out/translations.h\"\nPAYLOAD")
  message(FATAL_ERROR "Unexpected both trim output: [${both}]")
endif()
