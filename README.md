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
coordination TreeCC is designed to describe. The SDCC module currently handles
the source generation, compilation, linking, and generated usage-header steps;
later stages such as ROM conversion and emulator launch remain placeholders.

TreeCC is an early-stage implementation. The current app parses its input,
loads Lua handlers and dynamic modules, and executes selected action rules.
Full language evaluation and variable semantics are still evolving, and
commands such as `sdcc::clean`, `sdcc::ihx2rom`, `sdcc::openmsx`, and
`sdcc::optional` only print a recognition message.

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

Recipe and definitions files may use a global `include "<path>"` statement to
process another file at that point, like a C `#include`. Paths are resolved
relative to the file containing the `include`, and syntax or command errors are
reported with the included file's path and line. Recursive include cycles fail.

### Installing a relocatable bundle

```sh
cmake --install build --prefix /path/to/treecc
```

This installs the executable, Lua scripts, dynamic modules and Tree-sitter
runtime into `<prefix>/bin`. The installed `RUNPATH` is `$ORIGIN`, so the whole
`bin` directory can be moved or symlinked elsewhere. SDCC itself remains an
external tool.

The SDCC module requires a working `sdcc` executable on `PATH`, a directory
containing it configured in `~/.config/treecc`, or the system installation at
`/usr/libexec/sdcc/sdcc`:

```ini
[sdcc]
path = /path/to/sdcc/bin
```

### Generating MSXGL function files

`./generate-msxgl.py [--src DIR] [--out DIR]` rebuilds the
`lib/sdcc/functions/` directory. It scans the headers included by `msxgl.h`,
plus `msx-music.h`, and pairs each header with the same-named `.c` file when one
exists. One file per function is written to
`lib/sdcc/functions/<header-stem>/<function-name>.c`, containing
the header declaration and documentation followed by the implementation.
Headers without a matching source (`core.h`, `system_port.h`, `color.h`) still
produce files for any inline functions they declare.

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
through `~/.config/treecc` (`[sdcc]` section, `path` key), then through `PATH`,
and finally falls back to `/usr/libexec/sdcc/sdcc`.

### Target

`sdcc::target <name>` sets the active compiler target. All subsequent SDCC
compilations use `-m<name>` (for example, `sdcc::target z80` produces `-mz80`).

### Link options

`sdcc::link_opt [ <option> ... ]` registers additional SDCC link parameters.
For example, `sdcc::link_opt [ --no-std-crt0 ]` makes every later `sdcc::link`
pass `--no-std-crt0` to SDCC.

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
* Function contents always start with a defs include: `#include "out/defs.h"`
  for C and `.include "out/defs.asm"` for assembly (valid ASxxxx syntax), placed
  before any `.OPTIONAL` usage guard.
* With `.OPTIONAL`, a translations include follows the defs include:
  `#include "out/translations.h"` for C or
  `.include "out/translations.asm"` for assembly. Registering an optional
  function also generates `out/translations.asm`, containing one
  `.define <name> /__USED_<name>/` line per optional function. The double
  underscore matches SDCC's C-to-assembly mangling of `_USED_<name>`.
* Assembly functions emit `.globl <name>` before their label so the symbol is
  exported to the linker; keyword substitution renames it to `__USED_<name>`.
* When `.OPTIONAL` is enabled, the contents are enclosed in a usage guard:
  * C functions: `#ifdef _USED_<name>_FUNCTION` / `#endif`
  * Assembly functions: `.ifdef _USED_<name>_FUNCTION` / `.endif`
  The function name keeps its declared case; only the `_USED_` prefix is
  uppercase.
* Each function also contributes the line
  `#define <name>(...) _USED_<name>(__VA_ARGS__)` to `out/translations.h`.
* `out/defs.h` and `out/translations.h` always begin with `#pragma once`.
  `out/defs.asm` and `out/translations.asm` use equivalent ASxxxx include
  guards (`.ifndef` / `.define` / `.endif`).

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
* The generated `out/<basename>.lst` is scanned for `_USED_<name>` symbols. A
  function's language is remembered from its `append-to` path: a `.c` path means
  C, anything else means assembly. Each match produces
  `#define _USED_<name>_FUNCTION 1` in `out/defs.h` only for C functions and
  `.define _USED_<name>_FUNCTION /1/` in `out/defs.asm` only for assembly
  functions.

### Commands

* `sdcc::compile <source-id>` finds the `sdcc::source` with that ID and calls
  `sdcc -m<target>` with the source's `filename`, producing
  `out/<basename>.rel` and updating `out/defs.h` and `out/defs.asm` from the
  `.lst`.
* `sdcc::link <source-id-or-.rel>` invokes `sdcc -m<target>` with the source's
  `out/<basename>.rel` (or the given `.rel` path) and the inputs from every
  `append-to` path declared by `sdcc::function` nodes. `.c` files are compiled
  separately to `out/<basename>_c.rel`; `.asm` files are assembled first to
  `out/<basename>.rel` with the matching SDCC assembler (`sdasz80` for Z80
  targets). Only `.rel` files reach the linker, avoiding SDCC overwriting the
  main object. It writes `out/<basename>.ihx`; source include directories are
  passed with `-I`.
* Compilation, assembly, and linking commands are echoed to stderr before they
  run.
* `sdcc::clean`, `sdcc::ihx2rom`, `sdcc::openmsx`, and `sdcc::optional` are
  placeholders that print a recognition message.

## Tree-sitter

Tree-sitter provides the syntax parser used by TreeCC. The grammar is defined
in [grammar.js](./grammar.js), with [src/parser.c](./src/parser.c) generated
from it and [src/scanner.c](./src/scanner.c) implementing custom-delimited
blobs.
