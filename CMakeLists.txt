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

# The installed bundle keeps the Lua scripts, modules and Tree-sitter runtime
# beside the executable. Relative RUNPATHs let the whole directory be moved.
set(TREECC_INSTALL_RPATH "$ORIGIN")
set_target_properties(treecc PROPERTIES INSTALL_RPATH "${TREECC_INSTALL_RPATH}")

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
      INSTALL_RPATH "${TREECC_INSTALL_RPATH}"
  )
  install(TARGETS ${target} LIBRARY DESTINATION bin)
endfunction()

add_treecc_module(treecc_module_shell modules/shell.cpp "treecc-module-shell")
configure_file(modules/lua.lua "${CMAKE_CURRENT_BINARY_DIR}/lua.lua" COPYONLY)
configure_file(modules/actions.lua "${CMAKE_CURRENT_BINARY_DIR}/actions.lua" COPYONLY)
configure_file(modules/sdcc.lua "${CMAKE_CURRENT_BINARY_DIR}/sdcc.lua" COPYONLY)

install(TARGETS treecc RUNTIME DESTINATION bin)
install(FILES
  modules/actions.lua
  modules/lua.lua
  modules/sdcc.lua
  "$<TARGET_FILE:tree-sitter>"
  DESTINATION bin
)

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
  if(UNIX)
    set(TREECC_SYMLINK_DIR "${CMAKE_CURRENT_BINARY_DIR}/symlink-bin")
    file(MAKE_DIRECTORY "${TREECC_SYMLINK_DIR}")
    file(REMOVE "${TREECC_SYMLINK_DIR}/treecc")
    file(CREATE_LINK
      "${CMAKE_CURRENT_BINARY_DIR}/treecc"
      "${TREECC_SYMLINK_DIR}/treecc"
      SYMBOLIC
    )
    add_test(
      NAME treecc_symlinked_executable
      COMMAND "${TREECC_SYMLINK_DIR}/treecc"
        "${CMAKE_CURRENT_SOURCE_DIR}/sample.rec"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
    )
    set_tests_properties(treecc_symlinked_executable PROPERTIES
      PASS_REGULAR_EXPRESSION "Loaded Lua handlers: actions.lua"
      ENVIRONMENT
        "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
    )
  endif()
  add_test(
    NAME include_commands
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/include_commands.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(include_commands PROPERTIES
    PASS_REGULAR_EXPRESSION "sdcc::target: z80"
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME include_error
    COMMAND "${CMAKE_COMMAND}"
      "-DTREECC=$<TARGET_FILE:treecc>"
      "-DRECIPE=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/include_error.rec"
      "-DDEFINITIONS=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
      -DMODE=error
      -P "${CMAKE_CURRENT_SOURCE_DIR}/test/check_include.cmake"
  )
  add_test(
    NAME include_cycle
    COMMAND "${CMAKE_COMMAND}"
      "-DTREECC=$<TARGET_FILE:treecc>"
      "-DRECIPE=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/include_cycle_a.rec"
      "-DDEFINITIONS=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
      -DMODE=cycle
      -P "${CMAKE_CURRENT_SOURCE_DIR}/test/check_include.cmake"
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
  set_tests_properties(sdcc_source_file PROPERTIES
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME sdcc_defs
    COMMAND "${CMAKE_COMMAND}"
      "-DTREECC_EXECUTABLE=$<TARGET_FILE:treecc>"
      "-DRECIPE=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/sdcc_defs.rec"
      "-DDEFINITIONS=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
      "-DWORKING_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/sdcc-defs"
      -P "${CMAKE_CURRENT_SOURCE_DIR}/test/check_sdcc_defs.cmake"
  )
  set_tests_properties(sdcc_defs PROPERTIES
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME sdcc_trim_contents
    COMMAND "${CMAKE_COMMAND}"
      "-DTREECC_EXECUTABLE=$<TARGET_FILE:treecc>"
      "-DRECIPE=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/sdcc_trim_contents.rec"
      "-DDEFINITIONS=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
      "-DWORKING_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/sdcc-trim-contents"
      -P "${CMAKE_CURRENT_SOURCE_DIR}/test/check_sdcc_trim_contents.cmake"
  )
  set_tests_properties(sdcc_trim_contents PROPERTIES
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME sdcc_append_to
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/sample.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(sdcc_append_to PROPERTIES
    PASS_REGULAR_EXPRESSION "sdcc::append-to: received [./include/]*stdio\\.asm"
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
    NAME sdcc_function_includes
    COMMAND "${CMAKE_COMMAND}"
      "-DTREECC_EXECUTABLE=$<TARGET_FILE:treecc>"
      "-DRECIPE=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/sdcc_function_includes.rec"
      "-DDEFINITIONS=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
      "-DWORKING_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/sdcc-function-includes"
      -P "${CMAKE_CURRENT_SOURCE_DIR}/test/check_sdcc_function_includes.cmake"
  )
  set_tests_properties(sdcc_function_includes PROPERTIES
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME sdcc_optional_includes
    COMMAND "${CMAKE_COMMAND}"
      "-DTREECC_EXECUTABLE=$<TARGET_FILE:treecc>"
      "-DRECIPE=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/sdcc_optional_includes.rec"
      "-DDEFINITIONS=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
      "-DWORKING_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/sdcc-optional-includes"
      -P "${CMAKE_CURRENT_SOURCE_DIR}/test/check_sdcc_optional_includes.cmake"
  )
  set_tests_properties(sdcc_optional_includes PROPERTIES
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  foreach(link_mode IN ITEMS id rel)
    if(link_mode STREQUAL "rel")
      set(link_recipe "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/sdcc_link_rel.rec")
    else()
      set(link_recipe "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/sdcc_link.rec")
    endif()
    add_test(
      NAME "sdcc_link_${link_mode}"
      COMMAND "${CMAKE_COMMAND}"
        "-DTREECC_EXECUTABLE=$<TARGET_FILE:treecc>"
        "-DRECIPE=${link_recipe}"
        "-DDEFINITIONS=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
        "-DWORKING_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}/sdcc-link-${link_mode}"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/test/check_sdcc_link.cmake"
    )
    set_tests_properties("sdcc_link_${link_mode}" PROPERTIES
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
    PASS_REGULAR_EXPRESSION "sdcc::contents: received (\\.include \"out/defs\\.asm\"\n\\.include \"out/translations\\.asm\"\n)?\\.ifdef _USED_print_"
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
    NAME sdcc_fallback
    COMMAND "${CMAKE_COMMAND}" -E env "PATH="
      "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/sdcc_fallback.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(sdcc_fallback PROPERTIES
    PASS_REGULAR_EXPRESSION "sdcc::target: z80"
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
  add_test(
    NAME actions_default
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/sample.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(actions_default PROPERTIES
    PASS_REGULAR_EXPRESSION "sdcc::compile: main"
    FAIL_REGULAR_EXPRESSION "sdcc::clean: recognized"
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME actions_clean
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/sample.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
      "clean"
  )
  set_tests_properties(actions_clean PROPERTIES
    PASS_REGULAR_EXPRESSION "sdcc::clean: recognized"
    FAIL_REGULAR_EXPRESSION "sdcc::compile: main"
  )
  add_test(
    NAME actions_run
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/sample.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
      "run"
  )
  set_tests_properties(actions_run PROPERTIES
    PASS_REGULAR_EXPRESSION "sdcc::openmsx: recognized"
    ENVIRONMENT
      "PATH=${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/fake-bin;HOME=${CMAKE_CURRENT_BINARY_DIR}/no-config-home"
  )
  add_test(
    NAME actions_unknown_rule
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/sample.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
      "nonexistent"
  )
  set_tests_properties(actions_unknown_rule PROPERTIES
    WILL_FAIL TRUE
  )
  add_test(
    NAME actions_missing_sdcc
    COMMAND "$<TARGET_FILE:treecc>"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/actions_missing_sdcc.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(actions_missing_sdcc PROPERTIES
    WILL_FAIL TRUE
  )
  foreach(test_name IN ITEMS
      parse_examples
      include_commands
      include_error
      include_cycle
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
      sdcc_fallback
      actions_default
      actions_clean
      actions_run
      actions_unknown_rule
      actions_missing_sdcc
  )
    set(test_working_directory "${CMAKE_CURRENT_BINARY_DIR}/test-work/${test_name}")
    file(MAKE_DIRECTORY "${test_working_directory}/include")
    set_tests_properties("${test_name}" PROPERTIES
      WORKING_DIRECTORY "${test_working_directory}"
    )
  endforeach()
endif()
