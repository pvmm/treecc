# TreeCC

TreeCC is a language and application for describing and orchestrating compiler
toolchains through extensible, high-level project recipes. The app reads a
recipe and a definitions file, loads the requested modules, and dispatches
namespaced commands to those modules. Its goal is to provide one consistent
interface for defining project build workflows across compilers and tools.

TreeCC is an early-stage implementation. The current app parses its input,
loads Lua handlers and dynamic modules, and collects some SDCC function
metadata. Full language evaluation, variable and context semantics, and
execution of build actions are not implemented yet; compiler operations such
as compile and link are placeholders.

## Build and run

The application requires CMake, a C/C++ toolchain, and Lua 5.4 development
files. CMake fetches the Tree-sitter 0.25.10 runtime. Node.js and the project
npm dependencies are needed to generate and test the grammar.

```sh
npm install
npm run generate
npm test

cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Run TreeCC with a recipe (`.rec`) followed by a definitions file (`.def`):

```sh
build/treecc sample.rec functions.def
```

The SDCC module requires a working `sdcc` executable on `PATH`, or a directory
containing it configured in `~/.config/treecc`:

```ini
[sdcc]
path = /path/to/sdcc/bin
```

## Extensions

Modules listed in a recipe's `ruleset` provide handlers for namespaced
commands. Dynamic modules are shared libraries named
`libtreecc-module-<ruleset-name>.so`, loaded from beside the app or from
directories in the colon-separated `TREECC_MODULE_PATH`. They implement the
ABI declared in [include/treecc/module.h](./include/treecc/module.h).

Lua is embedded in the application. Lua command handlers are loaded from
`lua.lua`, and the `sdcc-bundle-4.2.0` ruleset loads SDCC handlers from
`sdcc.lua`. Each script returns a table of handler functions, which receive
the parsed command as a Lua node table.

The SDCC module recognizes `.OPTIONAL` and `.SIZE` in a function's `attributes`
list and stores the enabled flags in that function's `current_attributes` list.
An empty list leaves no flags enabled.
Each `append-to` path is opened in append mode and receives the function's
extracted contents, creating the file if it does not already exist.

## Tree-sitter

Tree-sitter provides the syntax parser used by TreeCC. The grammar is defined
in [grammar.js](./grammar.js), with [src/parser.c](./src/parser.c) generated
from it and [src/scanner.c](./src/scanner.c) implementing custom-delimited
blobs.
