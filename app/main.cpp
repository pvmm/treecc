#include <tree_sitter/api.h>

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

extern "C" const TSLanguage *tree_sitter_treecc();

namespace {

struct ParserDeleter {
  void operator()(TSParser *parser) const { ts_parser_delete(parser); }
};

struct TreeDeleter {
  void operator()(TSTree *tree) const { ts_tree_delete(tree); }
};

struct StringDeleter {
  void operator()(char *value) const { std::free(value); }
};

using Parser = std::unique_ptr<TSParser, ParserDeleter>;
using Tree = std::unique_ptr<TSTree, TreeDeleter>;
using CStr = std::unique_ptr<char, StringDeleter>;

std::string node_text(TSNode node, const std::string &source) {
  const uint32_t start = ts_node_start_byte(node);
  const uint32_t end = ts_node_end_byte(node);
  return source.substr(start, end - start);
}

TSNode find_function_call(TSNode node, const std::string &name,
                          const std::string &source) {
  if (std::string(ts_node_type(node)) == "function_call") {
    const TSNode function = ts_node_child_by_field_name(node, "function", 8);
    if (!ts_node_is_null(function) && node_text(function, source) == name) {
      return node;
    }
  }

  const uint32_t child_count = ts_node_child_count(node);
  for (uint32_t i = 0; i < child_count; ++i) {
    const TSNode match =
        find_function_call(ts_node_child(node, i), name, source);
    if (!ts_node_is_null(match)) {
      return match;
    }
  }
  return TSNode();
}

void print_source_filenames(TSNode node, const std::string &source) {
  if (std::string(ts_node_type(node)) == "function_call") {
    const TSNode function = ts_node_child_by_field_name(node, "function", 8);
    if (!ts_node_is_null(function) &&
        node_text(function, source) == "sdcc::source") {
      const TSNode filename_call = find_function_call(node, "filename", source);
      if (!ts_node_is_null(filename_call)) {
        for (uint32_t i = 0; i < ts_node_named_child_count(filename_call); ++i) {
          const TSNode argument = ts_node_named_child(filename_call, i);
          if (std::string(ts_node_type(argument)) != "argument") {
            continue;
          }

          const TSNode value = ts_node_named_child(argument, 0);
          if (!ts_node_is_null(value) &&
              std::string(ts_node_type(value)) == "string") {
            std::string filename = node_text(value, source);
            filename = filename.substr(1, filename.size() - 2);
            std::cout << "source filename: " << filename << '\n';
          }
          break;
        }
      }
    }
  }

  const uint32_t child_count = ts_node_child_count(node);
  for (uint32_t i = 0; i < child_count; ++i) {
    print_source_filenames(ts_node_child(node, i), source);
  }
}

std::string read_file(const std::string &path) {
  std::ifstream input(path, std::ios::binary);
  if (!input) {
    throw std::runtime_error("cannot open file");
  }

  std::string source((std::istreambuf_iterator<char>(input)),
                     std::istreambuf_iterator<char>());
  if (input.bad()) {
    throw std::runtime_error("failed while reading file");
  }
  if (source.size() > std::numeric_limits<std::uint32_t>::max()) {
    throw std::runtime_error("file exceeds Tree-sitter's input size limit");
  }
  return source;
}

void report_errors(TSNode node, const std::string &path, int &error_count) {
  if (ts_node_is_error(node) || ts_node_is_missing(node)) {
    const TSPoint point = ts_node_start_point(node);
    std::cerr << path << ':' << point.row + 1 << ':' << point.column + 1 << ": "
              << (ts_node_is_missing(node) ? "missing syntax" : "syntax error")
              << " near `" << ts_node_type(node) << "`\n";
    ++error_count;
  }

  const uint32_t child_count = ts_node_child_count(node);
  for (uint32_t i = 0; i < child_count; ++i) {
    report_errors(ts_node_child(node, i), path, error_count);
  }
}

bool parse_file(TSParser *parser, const std::string &path) {
  try {
    const std::string source = read_file(path);
    Tree tree(ts_parser_parse_string(
        parser, nullptr, source.data(), static_cast<std::uint32_t>(source.size())));
    if (!tree) {
      std::cerr << path << ": parsing failed\n";
      return false;
    }

    const TSNode root = ts_tree_root_node(tree.get());
    CStr tree_text(ts_node_string(root));
    std::cout << "== " << path << " ==\n" << tree_text.get() << '\n';
    print_source_filenames(root, source);

    int error_count = 0;
    report_errors(root, path, error_count);
    if (error_count != 0 || ts_node_has_error(root)) {
      if (error_count == 0) {
        std::cerr << path << ": syntax error\n";
      }
      return false;
    }

    std::cout << path << ": syntax OK\n";
    return true;
  } catch (const std::exception &error) {
    std::cerr << path << ": " << error.what() << '\n';
    return false;
  }
}

}  // namespace

int main(int argc, char **argv) {
  Parser parser(ts_parser_new());
  if (!parser) {
    std::cerr << "could not create Tree-sitter parser\n";
    return 2;
  }
  if (!ts_parser_set_language(parser.get(), tree_sitter_treecc())) {
    std::cerr << "TreeCC grammar is incompatible with the Tree-sitter runtime\n";
    return 2;
  }

  std::vector<std::string> paths;
  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {
      paths.emplace_back(argv[i]);
    }
  } else {
    paths.emplace_back(std::string(TREECC_SOURCE_DIR) + "/sample.rec");
    paths.emplace_back(std::string(TREECC_SOURCE_DIR) +
                       "/test/fixtures/functions.def");
  }

  bool success = true;
  for (const std::string &path : paths) {
    success = parse_file(parser.get(), path) && success;
  }
  return success ? 0 : 1;
}
