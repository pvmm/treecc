#!/usr/bin/env python3
"""Rebuild the lib/sdcc/functions/ directory from MSXGL headers and sources.

By default the script scans the headers listed in msxgl.h, plus msx-music.h,
and reads the matching .c file for each header when one exists. It writes one
file per function to lib/sdcc/functions/<header-stem>/<function-name>.c.

Usage:
    ./generate-msxgl.py [--src DIR] [--out DIR]
"""

from __future__ import annotations

import argparse
import re
import shutil
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent
DEFAULT_SRC = Path("/home/pedro/projects/msx/msxgl/engine/src")
DEFAULT_OUT = REPO_ROOT / "lib" / "sdcc" / "functions"
MAIN_HEADER = "msxgl.h"
EXTRA_HEADERS = ["msx-music.h"]

FUNCTION_COMMENT = re.compile(r"^\s*//\s*Function:\s*([A-Za-z_]\w*)\s*$")
DECLARATION = re.compile(
    r"^\s*(?:inline\s+)?(?:[A-Za-z_]\w*\s+)*"
    r"([A-Za-z_]\w*)\s*\([^;{}]*\)\s*(?:;|\{)"
)
DEFINITION = re.compile(r"^\s*(?:[A-Za-z_]\w*\s+)*([A-Za-z_]\w*)\s*\([^;{}]*\)\s*$")

KEYWORDS = {
    "if", "else", "for", "while", "do", "switch", "case", "return",
    "sizeof", "typedef", "struct", "enum", "union", "asm", "typeof",
}


def read_lines(path: Path) -> list[str]:
    return path.read_text(encoding="utf-8", errors="replace").splitlines()


def discover_headers(src: Path) -> list[Path]:
    names: list[str] = []
    seen: set[str] = set()

    def add(name: str) -> None:
        if name not in seen:
            seen.add(name)
            names.append(name)

    for line in read_lines(src / MAIN_HEADER):
        match = re.match(r'^\s*#\s*include\s*"([^"]+)"', line)
        if match:
            add(match.group(1))
    for name in EXTRA_HEADERS:
        add(name)

    return [src / name for name in names if (src / name).is_file()]


def collect_declaration(lines: list[str], start: int) -> tuple[list[str], int]:
    """Collect a prototype or inline definition starting at start."""
    collected: list[str] = []
    index = start
    while index < len(lines):
        line = lines[index]
        collected.append(line.rstrip())
        stripped = line.strip()
        if ";" in stripped and "{" not in stripped:
            return collected, index
        if "{" in stripped:
            depth = stripped.count("{") - stripped.count("}")
            index += 1
            while index < len(lines) and depth > 0:
                current = lines[index].rstrip()
                collected.append(current)
                depth += current.count("{") - current.count("}")
                index += 1
            return collected, index - 1
        index += 1
    return collected, index - 1


def preceding_comment(lines: list[str], start: int) -> list[str]:
    """Return the contiguous // comment block immediately before start."""
    comment: list[str] = []
    index = start - 1
    while index >= 0 and lines[index].lstrip().startswith("//"):
        comment.append(lines[index].rstrip())
        index -= 1
    comment.reverse()
    return comment


def hash_name(name: str) -> str:
    return name.strip().lstrip("#").strip()


def parse_header(path: Path) -> dict[str, dict[str, str]]:
    lines = read_lines(path)
    functions: dict[str, dict[str, str]] = {}
    documented: set[str] = set()

    index = 0
    while index < len(lines):
        match = FUNCTION_COMMENT.match(lines[index])
        if not match:
            index += 1
            continue
        name = match.group(1)
        documented.add(name)
        comment: list[str] = []
        while index < len(lines) and lines[index].lstrip().startswith("//"):
            comment.append(lines[index].rstrip())
            index += 1
        declaration, last = collect_declaration(lines, index)
        functions[name] = {
            "comment": "\n".join(comment),
            "declaration": "\n".join(declaration),
        }
        index = last + 1

    for index, line in enumerate(lines):
        stripped = line.strip()
        if not stripped or stripped.startswith(("#", "//", "/*", "*")):
            continue
        if any(stripped.startswith(keyword) for keyword in KEYWORDS):
            continue
        match = DECLARATION.match(line)
        if not match:
            continue
        name = match.group(1)
        if name in functions or name in KEYWORDS:
            continue
        comment = preceding_comment(lines, index)
        declaration, last = collect_declaration(lines, index)
        functions[name] = {
            "comment": "\n".join(comment),
            "declaration": "\n".join(declaration),
        }

    return functions


def parse_source(path: Path) -> dict[str, str]:
    lines = read_lines(path)
    definitions: dict[str, str] = {}

    index = 0
    while index < len(lines):
        line = lines[index]
        stripped = line.strip()
        match = DEFINITION.match(line) if not stripped.startswith("#") else None
        if not match:
            index += 1
            continue
        name = match.group(1)
        if name in KEYWORDS:
            index += 1
            continue

        if "{" in line:
            open_line = index
        else:
            cursor = index + 1
            while cursor < len(lines) and (
                not lines[cursor].strip() or lines[cursor].lstrip().startswith("//")
            ):
                cursor += 1
            if cursor >= len(lines) or "{" not in lines[cursor]:
                index += 1
                continue
            open_line = cursor

        depth = lines[open_line].count("{") - lines[open_line].count("}")
        end = open_line
        while depth > 0:
            end += 1
            if end >= len(lines):
                break
            depth += lines[end].count("{") - lines[end].count("}")
        if depth != 0:
            index += 1
            continue

        comment = preceding_comment(lines, index)
        body = "\n".join(
            line.rstrip() for line in lines[index:end + 1]
        )
        if comment:
            body = "\n".join(comment) + "\n" + body
        definitions[name] = body
        index = end + 1

    return definitions


def render_function(
    name: str,
    header: Path,
    source: Path | None,
    header_info: dict[str, str] | None,
    implementation: str | None,
) -> str:
    parts = [f"// Function: {name}"]
    if header_info is not None:
        parts.append(f"// Header: {header.name}")
    if source is not None:
        parts.append(f"// Source: {source.name}")
    parts.append("")

    if header_info:
        if header_info["comment"]:
            parts.append(header_info["comment"])
        parts.append(header_info["declaration"])
        parts.append("")
    if implementation:
        parts.append(implementation)
        parts.append("")

    return "\n".join(parts).rstrip() + "\n"


def build(src: Path, out: Path) -> tuple[int, list[str]]:
    headers = discover_headers(src)
    if not headers:
        raise SystemExit(f"no headers found from {src / MAIN_HEADER}")

    if out.exists():
        shutil.rmtree(out)
    out.mkdir(parents=True)

    written = 0
    missing_sources: list[str] = []
    for header in headers:
        source = src / f"{header.stem}.c"
        if not source.is_file():
            missing_sources.append(source.name)
            source = None

        header_functions = parse_header(header)
        source_functions = parse_source(source) if source else {}
        names = list(header_functions)
        names.extend(name for name in source_functions if name not in header_functions)
        if not names:
            continue

        group = out / header.stem
        group.mkdir(parents=True, exist_ok=True)
        for name in names:
            content = render_function(
                name,
                header,
                source,
                header_functions.get(name),
                source_functions.get(name),
            )
            (group / f"{name}.c").write_text(content, encoding="utf-8")
            written += 1

    return written, missing_sources


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--src", type=Path, default=DEFAULT_SRC,
                        help="directory containing msxgl.h and sources")
    parser.add_argument("--out", type=Path, default=DEFAULT_OUT,
                        help="directory to rebuild (default: ./lib/sdcc/functions)")
    args = parser.parse_args()

    written, missing_sources = build(args.src.resolve(), args.out.resolve())
    print(f"generated {written} function files in {args.out}")
    if missing_sources:
        print("headers without a matching .c file: " +
              ", ".join(sorted(missing_sources)))


if __name__ == "__main__":
    main()
