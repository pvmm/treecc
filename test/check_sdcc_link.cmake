if(NOT DEFINED TREECC_EXECUTABLE OR NOT DEFINED RECIPE OR
   NOT DEFINED DEFINITIONS OR NOT DEFINED WORKING_DIRECTORY)
  message(FATAL_ERROR "Missing required SDCC link test parameter")
endif()

file(MAKE_DIRECTORY "${WORKING_DIRECTORY}")
file(REMOVE_RECURSE "${WORKING_DIRECTORY}/out")
set(log_file "${WORKING_DIRECTORY}/sdcc.log")
file(REMOVE "${log_file}")
execute_process(
  COMMAND "${CMAKE_COMMAND}" -E env
    "FAKE_SDCC_LOG=${log_file}"
    "${TREECC_EXECUTABLE}" "${RECIPE}" "${DEFINITIONS}"
  WORKING_DIRECTORY "${WORKING_DIRECTORY}"
  RESULT_VARIABLE result
  OUTPUT_VARIABLE stdout
  ERROR_VARIABLE stderr
)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "TreeCC failed (${result}):\n${stdout}\n${stderr}")
endif()

foreach(fragment IN ITEMS "-c -mz80" "-o 'out/main.ihx'" "-o 'out/stdio.rel'" "-o 'out/stdio_c.rel'")
  if(NOT stderr MATCHES "${fragment}")
    message(FATAL_ERROR "stderr missing command [${fragment}]:\n${stderr}")
  endif()
endforeach()

set(ihx_file "${WORKING_DIRECTORY}/out/main.ihx")
if(NOT EXISTS "${ihx_file}")
  message(FATAL_ERROR "Expected out/main.ihx was not created")
endif()

file(READ "${log_file}" log)
string(REGEX REPLACE "\n$" "" log_lines "${log}")
string(REGEX MATCHALL "[^\n]+" calls "${log}")
list(LENGTH calls call_count)
if(NOT call_count EQUAL 4)
  message(FATAL_ERROR "Expected four SDCC invocations, got ${call_count}:\n${log}")
endif()

list(GET calls 1 assemble_call)
if(NOT assemble_call MATCHES "-o out/stdio.rel stdio.asm")
  message(FATAL_ERROR "Assembly call missing output or input: ${assemble_call}")
endif()

list(GET calls 2 compile_c_call)
if(NOT compile_c_call MATCHES "-c -mz80 -o out/stdio_c.rel stdio.c")
  message(FATAL_ERROR "C compilation call missing output or input: ${compile_c_call}")
endif()

list(GET calls 3 link_call)
set(required
  "-mz80"
  "--no-std-crt0"
  "-DTEST=1"
  "-o out/main.ihx"
  "out/main.rel"
  "out/stdio.rel"
  "out/stdio_c.rel"
)
foreach(fragment IN LISTS required)
  if(NOT link_call MATCHES "${fragment}")
    message(FATAL_ERROR "Link call missing [${fragment}]: ${link_call}")
  endif()
endforeach()
