execute_process(
  COMMAND "${TREECC}" "${RECIPE}" "${DEFINITIONS}"
  RESULT_VARIABLE result
  OUTPUT_VARIABLE output
  ERROR_VARIABLE error
)
set(all_output "${output}${error}")

if(MODE STREQUAL "error")
  if(result EQUAL 0)
    message(FATAL_ERROR "expected the included file's unknown command to fail")
  endif()
  if(NOT all_output MATCHES "include_error_child.rec:2: no loaded module registered command `missing::command`")
    message(FATAL_ERROR "error was not attributed to the included file:\n${all_output}")
  endif()
elseif(MODE STREQUAL "cycle")
  if(result EQUAL 0)
    message(FATAL_ERROR "expected a recursive include to fail")
  endif()
  if(NOT all_output MATCHES "recursive include")
    message(FATAL_ERROR "recursive include was not reported:\n${all_output}")
  endif()
else()
  message(FATAL_ERROR "unknown include check mode: ${MODE}")
endif()
