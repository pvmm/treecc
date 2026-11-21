cmake_minimum_required(VERSION 3.16)
project(treecc LANGUAGES C CXX)

include(FetchContent)
find_package(Lua 5.4 REQUIRED)

set(BUILD_SHARED_LIBS ON)
FetchContent_Declare(
  tree_sitter_runtime
  GIT_REPOSITORY https://github.com/tree-sitter/tree-sitter.git
  GIT_TAG v0.25.10
  GIT_SHALLOW TRUE
  SOURCE_SUBDIR lib
)
FetchContent_MakeAvailable(tree_sitter_runtime)

add_executable(
  treecc
  app/main.cpp
  modules/lua.cpp
  src/parser.c
  src/scanner.c
)
target_compile_features(treecc PRIVATE cxx_std_17)
target_include_directories(
  treecc
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${tree_sitter_runtime_SOURCE_DIR}/lib/include"
    "${LUA_INCLUDE_DIR}"
)
target_link_libraries(treecc PRIVATE tree-sitter ${LUA_LIBRARIES})
if(UNIX AND NOT APPLE)
  target_link_libraries(treecc PRIVATE ${CMAKE_DL_LIBS})
endif()

function(add_treecc_module target source output_name)
  add_library(${target} MODULE ${source})
  target_compile_features(${target} PRIVATE cxx_std_17)
  target_include_directories(
    ${target}
    PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/include"
      "${tree_sitter_runtime_SOURCE_DIR}/lib/include"
  )
  target_link_libraries(${target} PRIVATE tree-sitter)
  set_target_properties(
    ${target}
    PROPERTIES
      OUTPUT_NAME "${output_name}"
      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  )
endfunction()

add_treecc_module(treecc_module_shell modules/shell.cpp "treecc-module-shell")
configure_file(modules/lua.lua "${CMAKE_CURRENT_BINARY_DIR}/lua.lua" COPYONLY)
configure_file(modules/sdcc.lua "${CMAKE_CURRENT_BINARY_DIR}/sdcc.lua" COPYONLY)

include(CTest)
if(BUILD_TESTING)
  set(TREECC_TEST_HOME "${CMAKE_CURRENT_BINARY_DIR}/test-home")
  file(MAKE_DIRECTORY "${TREECC_TEST_HOME}/.config")
  configure_file(
    test/fixtures/treecc.conf.in
    "${TREECC_TEST_HOME}/.config/treecc"
    @ONLY
  )

  add_test(
    NAME parse_examples
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/sample.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(parse_examples PROPERTIES
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME sdcc_source_context
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/sample.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(sdcc_source_context PROPERTIES
    PASS_REGULAR_EXPRESSION "sdcc::source:"
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME sdcc_source_filename
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/sample.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(sdcc_source_filename PROPERTIES
    PASS_REGULAR_EXPRESSION "source filename: main.c"
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME sdcc_source_file
    COMMAND "${CMAKE_COMMAND}"
      "-DTREECC_EXECUTABLE=$<TARGET_FILE:treecc>"
      "-DRECIPE=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/sdcc_source_file.rec"
      "-DDEFINITIONS=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
      "-DWORKING_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/sdcc-source-file"
      -P "${CMAKE_CURRENT_SOURCE_DIR}/test/check_sdcc_source_file.cmake"
  )
  add_test(
    NAME sdcc_append_to
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/sample.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(sdcc_append_to PROPERTIES
    PASS_REGULAR_EXPRESSION "sdcc::append-to: received ./include/stdio\\.asm"
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  foreach(append_mode IN ITEMS create append)
    set(append_working_directory
      "${CMAKE_CURRENT_BINARY_DIR}/sdcc-append-${append_mode}"
    )
    file(MAKE_DIRECTORY "${append_working_directory}")
    add_test(
      NAME "sdcc_append_file_${append_mode}"
      COMMAND "${CMAKE_COMMAND}"
        "-DTREECC_EXECUTABLE=$<TARGET_FILE:treecc>"
        "-DRECIPE=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/sdcc_append_file.rec"
        "-DDEFINITIONS=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
        "-DWORKING_DIRECTORY=${append_working_directory}"
        "-DTEST_MODE=${append_mode}"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/test/check_sdcc_append.cmake"
    )
    set_tests_properties("sdcc_append_file_${append_mode}" PROPERTIES
      ENVIRONMENT
        "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
    )
  endforeach()
  add_test(
    NAME sdcc_delimiters
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/sample.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(sdcc_delimiters PROPERTIES
    PASS_REGULAR_EXPRESSION "sdcc::delimiters: received \\[---------------\\]"
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME sdcc_contents
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/sdcc_contents.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(sdcc_contents PROPERTIES
    PASS_REGULAR_EXPRESSION "sdcc::contents: received \\.ifdef _USED_PRINT_"
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME sdcc_multiple_functions
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/sdcc_contents.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(sdcc_multiple_functions PROPERTIES
    PASS_REGULAR_EXPRESSION "sdcc::functions: registered \\[print, second\\]"
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME sdcc_debug_interpolation
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/sdcc_debug.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(sdcc_debug_interpolation PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello, Pedro!"
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME sdcc_attributes
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/sdcc_attributes.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(sdcc_attributes PROPERTIES
    PASS_REGULAR_EXPRESSION "sdcc::current_attributes: \\[\\.OPTIONAL, \\.SIZE\\]"
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME sdcc_empty_attributes
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/sdcc_attributes.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(sdcc_empty_attributes PROPERTIES
    PASS_REGULAR_EXPRESSION "sdcc::current_attributes: \\[\\]"
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME sdcc_config_path
    COMMAND "${CMAKE_COMMAND}" -E env "PATH=" "HOME=${TREECC_TEST_HOME}"
      "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/sample.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(sdcc_config_path PROPERTIES
    PASS_REGULAR_EXPRESSION "sdcc::source:"
  )
  add_test(
    NAME sdcc_missing_from_path
    COMMAND "${CMAKE_COMMAND}" -E env "PATH="
      "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/sample.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(sdcc_missing_from_path PROPERTIES
    WILL_FAIL TRUE
    ENVIRONMENT "HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME lua_node_handler
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/lua_handlers.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(lua_node_handler PROPERTIES
    PASS_REGULAR_EXPRESSION "Lua handled lua::greet: TreeCC"
  )
  foreach(test_name IN ITEMS
      parse_examples
      sdcc_source_context
      sdcc_source_filename
      sdcc_append_to
      sdcc_delimiters
      sdcc_contents
      sdcc_multiple_functions
      sdcc_debug_interpolation
      sdcc_attributes
      sdcc_empty_attributes
      sdcc_config_path
      sdcc_missing_from_path
  )
    set(test_working_directory "${CMAKE_CURRENT_BINARY_DIR}/test-work/${test_name}")
    file(MAKE_DIRECTORY "${test_working_directory}/include")
    set_tests_properties("${test_name}" PROPERTIES
      WORKING_DIRECTORY "${test_working_directory}"
    )
  endforeach()
endif()
