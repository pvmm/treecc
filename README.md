# TreeCC

TreeCC is a language and application for describing and orchestrating compiler
toolchains through extensible, high-level project recipes. The app reads a
recipe and a definitions file, loads the requested modules, and dispatches
namespaced commands to those modules. Its goal is to provide one consistent
interface for defining project build workflows across compilers and tools.
TreeCC was designed to work around shortcomings in individual compilers and
assemblers by providing a shared, high-level way to express and orchestrate
their workflows.

## Example workflow

One workflow TreeCC is intended to orchestrate is combining assembly code with
a C program when each build step depends on information produced by another:

1. Assemble the source file once to determine the size of its machine code.
2. Generate or update the C source with an array sized to hold that code.
3. Compile the C program and inspect its output to determine the array's memory
   address.
4. Reassemble the assembly source using that address, so it knows where its
   code will reside.
5. Convert the final assembled code into a byte array, place it in the C
   source, and compile the C program again.

This example illustrates the kind of multi-stage compiler and assembler
coordination TreeCC aims to describe; executing build actions is not yet
implemented.

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
build/treecc sample.rec functions.def [rule]
```

The optional `rule` selects an action rule from the recipe's `actions` block;
it defaults to `default`. Rules are executed after the recipe is parsed.
`include <rule>` inside a rule runs that other rule completely before
continuing. Commands such as `sdcc::compile` must belong to a loaded ruleset
(for example, `sdcc-4.2.0`); otherwise running the rule fails.

The SDCC module requires a working `sdcc` executable on `PATH`, or a directory
containing it configured in `~/.config/treecc`:

```ini
[sdcc]
path = /path/to/sdcc/bin
```

## Extensions

Modules listed in a recipe's `ruleset` provide handlers for namespaced
commands. A plain string loads a dynamic module named
`libtreecc-module-<name>.so`, loaded from beside the app or from directories in
the colon-separated `TREECC_MODULE_PATH`. They implement the ABI declared in
[include/treecc/module.h](./include/treecc/module.h). A mapping such as
`"sdcc-4.2.0": sdcc` loads a module and aliases it to the `sdcc` namespace, so
`sdcc::*` commands become available.

Lua is embedded in the application. The built-in `actions.lua` module is always
loaded and implements rule selection. Lua command handlers are loaded from
`lua.lua`, and the `sdcc-4.2.0` alias (or the legacy `sdcc-bundle-4.2.0`) loads
SDCC handlers from `sdcc.lua`. Each script returns a table of handler functions,
which receive the parsed command as a Lua node table.

## Actions module

An `actions` block defines rules such as `clean`, `default`, and `run`. The
third CLI argument selects which rule to run (default: `default`). Only the
selected rule's commands are executed; `include <rule>` runs that rule fully
before continuing with the next command in the current rule. Commands with an
unloaded namespace cause the run to fail.

## SDCC module rules

The SDCC module is loaded when `ruleset` lists `sdcc-4.2.0` (aliased to `sdcc`)
or the legacy `sdcc-bundle-4.2.0`. The compiler executable is found first
through `~/.config/treecc` (`[sdcc]` section, `path` key) and then through
`PATH`.

### Target

`sdcc::target <name>` sets the active compiler target. All subsequent SDCC
compilations use `-m<name>` (for example, `sdcc::target z80` produces `-mz80`).

### Function scope (`sdcc::function`)

* A function has an ID, optional `attributes`, optional `signature`,
  `delimiters`, `contents`, and one or more `append-to` paths.
* `attributes` accepts `.OPTIONAL` and `.SIZE`; both are stored in
  `current_attributes`. An empty list enables no flags.
* Within function contents, `$sdcc::function` and `${sdcc::function}` both
  interpolate to the function name.
* `signature` is a text subnode. Its interpolated text is prepended to every
  generated `sdcc::source` file (for example,
  `signature "void $sdcc::function(char* s);"`).
* `append-to` opens the path in append mode, creates missing parent directories,
  and writes the function's extracted contents. The path extension determines
  the language: `.c` is C, anything else (such as `.asm`) is assembly.
* When `.OPTIONAL` is enabled, the contents are enclosed in a usage guard:
  * C functions: `#ifdef _USED_<name>_FUNCTION` / `#endif`
  * Assembly functions: `.ifdef _USED_<name>_FUNCTION` / `.endif`
  The function name keeps its declared case; only the `_USED_` prefix is
  uppercase.
* Each function also contributes the line
  `#define <name>(...) _USED_<name>(__VA_ARGS__)` to `out/translations.h`.

### Source scope (`sdcc::source`)

* A source has an ID and creates the file named by `filename`.
* `include-dirs` and `library-dirs` lists, `delimiters`, `contents`,
  `trim-contents`, and `contents-interpolation` are stored in the source scope.
* `trim-contents` accepts `left`, `right`, or `both` and removes whitespace from
  the selected ends of contents in source and function scopes.
* `contents-interpolation` accepts `true` or `false`; it defaults to `true`.
  Setting it to `false` preserves variable references in source and function
  contents.
* Every generated source begins with `#include "out/translations.h"`, followed
  by any function signatures and then the source contents.
* Compilation is triggered by the `sdcc::compile <source-id>` action, not by
  closing the source scope. The generated file is compiled with SDCC into
  `out/<basename>.rel` (no linking); include directories are passed with `-I`.
* The generated `out/<basename>.lst` is scanned for `_USED_<name>` symbols. Each
  match produces `#define USED_<name>_FUNCTION 1` in `out/defs.h` and
  `.define _USED_<name>_FUNCTION /1/` in `out/defs.asm`, making the usage guard
  visible to both C and assembly code.

### Commands

* `sdcc::compile <source-id>` finds the `sdcc::source` with that ID and calls
  `sdcc -m<target>` with the source's `filename`, producing
  `out/<basename>.rel` and updating `out/defs.h` and `out/defs.asm` from the
  `.lst`.
* `sdcc::clean`, `sdcc::link`, `sdcc::ihx2rom`, `sdcc::openmsx`, and
  `sdcc::optional` are placeholders that print a recognition message.

## Tree-sitter

Tree-sitter provides the syntax parser used by TreeCC. The grammar is defined
in [grammar.js](./grammar.js), with [src/parser.c](./src/parser.c) generated
from it and [src/scanner.c](./src/scanner.c) implementing custom-delimited
blobs.
