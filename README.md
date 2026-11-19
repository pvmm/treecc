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

Build the included C++ parser application with CMake:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The build fetches the Tree-sitter 0.25.10 runtime. Pass the recipe (`.rec`)
filename first and the definitions (`.def`) filename second. The app prints
their syntax trees:

```sh
build/treecc-parse sample.rec test/fixtures/functions.def
```

The application requires both filenames, checks their extensions, and returns
a nonzero exit status if a file cannot be read or has syntax errors. Parsing
does not execute compiler actions.
