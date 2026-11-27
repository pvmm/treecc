#include "tree_sitter/parser.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum TokenType {
  BLOB_START,
  BLOB_TEXT,
  BLOB_END,
  CONTENTS_INCLUDE,
};

enum { MAX_DELIMITER_LENGTH = 1022 };

typedef struct {
  uint16_t delimiter_length;
  char delimiter[MAX_DELIMITER_LENGTH];
} Scanner;

void *tree_sitter_treecc_external_scanner_create(void) {
  return calloc(1, sizeof(Scanner));
}

void tree_sitter_treecc_external_scanner_destroy(void *payload) {
  free(payload);
}

unsigned tree_sitter_treecc_external_scanner_serialize(void *payload, char *buffer) {
  Scanner *scanner = (Scanner *)payload;
  memcpy(buffer, &scanner->delimiter_length, sizeof(scanner->delimiter_length));
  memcpy(buffer + sizeof(scanner->delimiter_length), scanner->delimiter,
         scanner->delimiter_length);
  return (unsigned)(sizeof(scanner->delimiter_length) + scanner->delimiter_length);
}

void tree_sitter_treecc_external_scanner_deserialize(void *payload, const char *buffer,
                                                     unsigned length) {
  Scanner *scanner = (Scanner *)payload;
  scanner->delimiter_length = 0;
  memset(scanner->delimiter, 0, sizeof(scanner->delimiter));
  if (length < sizeof(scanner->delimiter_length)) {
    return;
  }

  uint16_t delimiter_length;
  memcpy(&delimiter_length, buffer, sizeof(delimiter_length));
  if (delimiter_length > MAX_DELIMITER_LENGTH ||
      length != sizeof(delimiter_length) + delimiter_length) {
    return;
  }

  scanner->delimiter_length = delimiter_length;
  memcpy(scanner->delimiter, buffer + sizeof(delimiter_length), delimiter_length);
}

static void skip_blob_prefix(TSLexer *lexer) {
  while (lexer->lookahead == ' ' || lexer->lookahead == '\t' ||
         lexer->lookahead == '\r' || lexer->lookahead == '\n' ||
         lexer->lookahead == ':') {
    lexer->advance(lexer, true);
  }
}

static bool scan_contents_include(TSLexer *lexer) {
  static const char keyword[] = ".INCLUDE";
  for (uint16_t i = 0; keyword[i] != '\0'; ++i) {
    if (lexer->lookahead != (unsigned char)keyword[i]) {
      return false;
    }
    lexer->advance(lexer, false);
  }
  lexer->mark_end(lexer);
  lexer->result_symbol = CONTENTS_INCLUDE;
  return true;
}

static bool scan_blob_start(Scanner *scanner, TSLexer *lexer) {
  if (lexer->lookahead == '{') {
    lexer->advance(lexer, false);
    if (lexer->lookahead == '{') {
      lexer->advance(lexer, false);
      scanner->delimiter[0] = '}';
      scanner->delimiter[1] = '}';
      scanner->delimiter_length = 2;
      lexer->mark_end(lexer);
      lexer->result_symbol = BLOB_START;
      return true;
    }
    return false;
  }

  uint16_t length = 0;
  while (lexer->lookahead != 0 && lexer->lookahead != ' ' &&
         lexer->lookahead != '\t' && lexer->lookahead != '\r' &&
         lexer->lookahead != '\n') {
    if (length == MAX_DELIMITER_LENGTH) {
      return false;
    }
    scanner->delimiter[length++] = (char)lexer->lookahead;
    lexer->advance(lexer, false);
  }

  if (length == 0) {
    return false;
  }

  scanner->delimiter_length = length;
  lexer->mark_end(lexer);
  lexer->result_symbol = BLOB_START;
  return true;
}

static bool scan_blob_content(Scanner *scanner, TSLexer *lexer,
                              const bool *valid_symbols) {
  bool consumed = false;
  while (lexer->lookahead != 0) {
    if (lexer->lookahead == (unsigned char)scanner->delimiter[0]) {
      lexer->mark_end(lexer);
      uint16_t matched = 0;
      while (matched < scanner->delimiter_length &&
             lexer->lookahead == (unsigned char)scanner->delimiter[matched]) {
        lexer->advance(lexer, false);
        matched++;
      }

      if (matched == scanner->delimiter_length) {
        if (consumed && valid_symbols[BLOB_TEXT]) {
          lexer->result_symbol = BLOB_TEXT;
          return true;
        }
        if (valid_symbols[BLOB_END]) {
          scanner->delimiter_length = 0;
          memset(scanner->delimiter, 0, sizeof(scanner->delimiter));
          lexer->mark_end(lexer);
          lexer->result_symbol = BLOB_END;
          return true;
        }
        return false;
      }

      consumed = true;
      lexer->mark_end(lexer);
      continue;
    }

    lexer->advance(lexer, false);
    consumed = true;
    lexer->mark_end(lexer);
  }
  return consumed && valid_symbols[BLOB_TEXT];
}

bool tree_sitter_treecc_external_scanner_scan(void *payload, TSLexer *lexer,
                                             const bool *valid_symbols) {
  Scanner *scanner = (Scanner *)payload;

  if (scanner->delimiter_length == 0) {
    skip_blob_prefix(lexer);
    if (valid_symbols[CONTENTS_INCLUDE] && scan_contents_include(lexer)) {
      return true;
    }
    if (!valid_symbols[BLOB_START]) {
      return false;
    }
    return scan_blob_start(scanner, lexer);
  }
  return scan_blob_content(scanner, lexer, valid_symbols);
}
