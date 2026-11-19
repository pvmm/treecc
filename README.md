# TreeCC Tree-sitter grammar

This repository contains the Tree-sitter grammar for TreeCC recipe (`.rec`) and
definition (`.def`) files. The grammar recognizes the syntax; project ordering,
function signatures, variable interpolation, and action execution remain
semantic/runtime responsibilities.

## Generate and test

Install the Node.js development dependency, then run:

```sh
npm install
npm run generate
npm test
```

Replace the placeholder repository URL in `tree-sitter.json` before publishing
the grammar.

The sample recipe is [sample.rec](./sample.rec), and the language notes are in
[specs.txt](./specs.txt). Definition syntax has a focused example at
[test/fixtures/functions.def](./test/fixtures/functions.def).

## C++ consumers

Tree-sitter generates the parser as C. A C++ application can link the generated
`src/parser.c` and the Tree-sitter runtime, then obtain the grammar through the
C API:

```cpp
#include <tree_sitter/api.h>

extern "C" const TSLanguage *tree_sitter_treecc();
```

Use the returned language with `ts_parser_set_language`. The grammar does not
implement a compiler executor.
