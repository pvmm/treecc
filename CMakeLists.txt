cmake_minimum_required(VERSION 3.16)
project(treecc LANGUAGES C CXX)

include(FetchContent)

set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
  tree_sitter_runtime
  GIT_REPOSITORY https://github.com/tree-sitter/tree-sitter.git
  GIT_TAG v0.25.10
  GIT_SHALLOW TRUE
  SOURCE_SUBDIR lib
)
FetchContent_MakeAvailable(tree_sitter_runtime)

add_executable(
  treecc-parse
  app/main.cpp
  src/parser.c
  src/scanner.c
)
target_compile_features(treecc-parse PRIVATE cxx_std_17)
target_include_directories(
  treecc-parse
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${tree_sitter_runtime_SOURCE_DIR}/lib/include"
)
target_link_libraries(treecc-parse PRIVATE tree-sitter)

include(CTest)
if(BUILD_TESTING)
  add_test(
    NAME parse_examples
    COMMAND treecc-parse
      "${CMAKE_CURRENT_SOURCE_DIR}/sample.rec"
      "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/functions.def"
  )
  set_tests_properties(parse_examples PROPERTIES PASS_REGULAR_EXPRESSION "source filename: main.c")
endif()
