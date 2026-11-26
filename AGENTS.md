# Agent rules for TreeCC

This file records the behavior rules implemented in this repository. Read it
before changing the parser, SDCC handlers, or the actions module.

## CLI

* `treecc <recipe.rec> <definitions.def> [rule]`
* The optional third argument selects an action rule and defaults to `default`.
* The recipe and definitions files must have `.rec` and `.def` extensions.

## Ruleset and module loading

* `ruleset` lists modules. A plain string loads a dynamic module named
  `libtreecc-module-<name>.so` from beside the executable or from
  `TREECC_MODULE_PATH`.
* A mapping `"<module>": <namespace>` loads `<module>` and aliases it to the
  given namespace. `"sdcc-4.2.0": sdcc` (and the legacy
  `"sdcc-bundle-4.2.0": sdcc`) loads the built-in SDCC Lua handlers, making
  `sdcc::*` commands available.
* `lua` loads the built-in `lua.lua` handlers.
* Commands with a `::` prefix must have a loaded handler; otherwise the run
  fails. Actions are dispatched only when their rule is selected.

## SDCC module

The compiler executable is found through `~/.config/treecc` (`[sdcc]` section,
`path` key) and then through `PATH`.

### Target

`sdcc::target <name>` sets the active compiler target. Subsequent SDCC
compilations use `-m<name>` (for example, `sdcc::target z80` → `-mz80`).

### Source scope (`sdcc::source`)

* A source has an ID and creates the file named by `filename`.
* `include-dirs`, `library-dirs`, `delimiters`, `contents`, `trim-contents`, and
  `contents-interpolation` are stored in the source scope.
* `trim-contents` accepts `left`, `right`, or `both`.
* `contents-interpolation` defaults to `true`; `false` preserves variable
  references in contents.
* Every generated source begins with `#include "out/translations.h"`, followed
  by any function signatures and then the source contents.
* Compilation is triggered by the `sdcc::compile <source-id>` action, not by
  closing the source scope. The generated file is compiled with SDCC into
  `out/<basename>.rel` (no linking). Include directories are passed with `-I`.
* The generated `out/<basename>.lst` is scanned for `_USED_<name>` symbols.
  Each match produces `#define _USED_<name>_FUNCTION 1` in `out/defs.h` and
  `.define _USED_<name>_FUNCTION /1/` in `out/defs.asm`, so C and assembly
  code can both test the usage guard.

### Function scope (`sdcc::function`)

* A function has an ID, optional `attributes`, optional `signature`,
  `delimiters`, `contents`, and one or more `append-to` paths.
* `attributes` accepts `.OPTIONAL` and `.SIZE`.
* `$sdcc::function` and `${sdcc::function}` interpolate to the function name.
* `signature` text is prepended to every generated `sdcc::source` file.
* `append-to` appends the function contents to the path (creating parents).
  A `.c` extension means C, anything else (for example `.asm`) means assembly.
* Function contents always start with a defs include: `#include "out/defs.h"`
  for C and `.include "out/defs.asm"` for assembly (valid ASxxxx syntax). This
  line is inserted before any `.OPTIONAL` guard.
* With `.OPTIONAL`, contents are wrapped in a usage guard:
  * C: `#ifdef _USED_<name>_FUNCTION` / `#endif`
  * Assembly: `.ifdef _USED_<name>_FUNCTION` / `.endif`
  The function name keeps its declared case; only `_USED_` is uppercase.
* Each function contributes
  `#define <name>(...) _USED_<name>(__VA_ARGS__)` to `out/translations.h`.

### Commands

* `sdcc::compile <source-id>` finds the `sdcc::source` with that ID and calls
  `sdcc -m<target>` with its `filename`, producing `out/<basename>.rel` and
  updating `out/defs.h` and `out/defs.asm` from the `.lst`.
* `sdcc::clean`, `sdcc::link`, `sdcc::ihx2rom`, `sdcc::openmsx`, and
  `sdcc::optional` are command placeholders that print a recognition message.

## Actions module

* Implemented in `modules/actions.lua`, always loaded as a built-in module.
* `actions` contains rule subcontexts such as `clean`, `default`, and `run`.
* Only the selected rule (third CLI argument, default `default`) is executed;
  commands inside other rules are not run during parsing.
* Commands inside a rule run in order.
* `include <rule>` runs that rule completely before continuing with the next
  command in the current rule.
* If a command's namespace has no loaded handler (for example `sdcc::compile`
  without an SDCC ruleset), running the rule fails.

## Tests

* Use the fake SDCC in `test/fixtures/fake-bin` for tests that should not need
  a real compiler. It creates `.rel`/`.lst` outputs for `-c -o`.
* Run `ctest --test-dir build --output-on-failure` after changes.
