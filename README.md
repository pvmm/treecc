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

The build requires the Lua 5.4 development package, fetches the Tree-sitter
0.25.10 runtime, and builds the `shell` module. Lua is compiled into the app as
a built-in module. Recipes using the SDCC ruleset require `sdcc` to be on
`PATH`, or in the optional `[sdcc]` `path` setting in `~/.config/treecc`;
startup stops with an error if neither location contains a working compiler.
The configured path is a directory containing the `sdcc` executable. Pass the
recipe (`.rec`) filename first and the definitions (`.def`) filename second:

```ini
[sdcc]
path = /path/to/sdcc/bin
```

```sh
build/treecc-parse sample.rec test/fixtures/functions.def
```

The app loads each external module named in the recipe's `ruleset` from beside
the executable. The `sdcc-bundle-4.2.0` ruleset entry is handled internally by
the built-in Lua module, which loads SDCC handlers from `sdcc.lua` rather than a
separate plugin. Additional module directories can be supplied as colon-separated
paths in `TREECC_MODULE_PATH`. A module is a shared library named
`libtreecc-module-<ruleset-name>.so`; it exports `treecc_module_name_v1` and
`treecc_module_register_v1` as defined in [module.h](./include/treecc/module.h).
Registered namespaced commands are dispatched to their module handlers. The
Lua script handles the `sdcc::source` context and its ID parameter.

When a recipe or definitions file contains a `lua::` command, the app
automatically loads `lua.lua` beside the executable. The
`sdcc-bundle-4.2.0` ruleset entry loads SDCC handlers from `sdcc.lua`. Each
script returns a table mapping handler names to functions. Unqualified keys
like `greet` or `source` are registered under the appropriate namespace
(`lua::greet` or `sdcc::source`). Handlers receive a node table containing
`name`, `args`, source positions, and nested `children`. For example:

```lua
return {
  greet = function(command)
    print(command.args[1].value)
  end,
}
```

The scripts are [modules/lua.lua](./modules/lua.lua) and
[modules/sdcc.lua](./modules/sdcc.lua); CMake copies them beside the executable.
Each `sdcc::function` creates a function object containing its name,
`append-to` paths, delimiters, and extracted contents. These objects are kept
in the SDCC module's `functions` list.

The application requires both filenames, checks their extensions, and returns
a nonzero exit status if a file cannot be read, has syntax errors, or references
an unavailable namespaced command. Compiler actions are not executed yet.
