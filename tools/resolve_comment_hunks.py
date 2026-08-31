#!/usr/bin/env python3
r"""Resolve merge-conflict hunks that carry no code.

  mickey.us.yaml   hunks whose both sides are `#` comment lines: take ours.
  *.c / *.h        hunks whose both sides are C comment lines only (plateau
                   notes above a `#ifdef NON_MATCHING`): take theirs, the lane
                   that just worked the function.
  docs/*.md        hunks whose both sides are ledger table rows for the same
                   function(s) (`| \`name\` | ...`): take theirs.

Any hunk with a code line on either side is left for a human. Usage: FILE...
Exit 1 if any hunk is left.
"""
from pathlib import Path
import re, sys

pat = re.compile(r"<<<<<<< [^\n]*\n(.*?)=======\n(.*?)>>>>>>> [^\n]*\n", re.S)


def yaml_comments(block):
    return all(l.strip() == "" or l.lstrip().startswith("#") for l in block.split("\n"))


def c_comments(block):
    inside = False
    for l in block.split("\n"):
        t = l.strip()
        if t == "":
            continue
        if inside:
            if "*/" in t:
                inside = False
                if t.split("*/", 1)[1].strip():
                    return False
            continue
        if t.startswith("//"):
            continue
        if t.startswith("/*"):
            if "*/" in t:
                if t.split("*/", 1)[1].strip():
                    return False
            else:
                inside = True
            continue
        if t.startswith("*"):  # continuation line of a block comment
            continue
        return False
    return not inside


def open_plateau_names(block):
    """Return disjoint EOF plateau blocks whose final close follows the hunk."""
    lines = [line.strip() for line in block.splitlines() if line.strip()]
    if not lines:
        return None
    names = []
    index = 0
    while index < len(lines):
        start = re.fullmatch(
            r"/\* PLATEAU-HANDOFF:([^:]+):start", lines[index]
        )
        if not start:
            return None
        name = start.group(1)
        names.append(name)
        index += 1
        while index < len(lines) and lines[index] != f"* PLATEAU-HANDOFF:{name}:end":
            if "*/" in lines[index]:
                return None
            index += 1
        if index == len(lines):
            return None
        index += 1
        if index == len(lines):
            return names
        if lines[index] != "*/":
            return None
        index += 1
    return None


def row_names(block):
    names = []
    for l in block.split("\n"):
        if l.strip() == "":
            continue
        m = re.match(r"\|\s*`([^`]+)`\s*\|", l)
        if not m:
            return None
        names.append(m.group(1))
    return names


def resolve(path):
    source_path = Path(path)
    s = source_path.read_text()
    left = 0

    def sub(m):
        nonlocal left
        ours, theirs = m.group(1), m.group(2)
        if path.endswith((".yaml", ".yml")):
            if yaml_comments(ours) and yaml_comments(theirs):
                return ours
        elif path.endswith((".c", ".h")):
            ours_names = open_plateau_names(ours)
            theirs_names = open_plateau_names(theirs)
            if ours_names and theirs_names and set(ours_names).isdisjoint(theirs_names):
                # finalize_plateau appends blocks at EOF. When two lanes add
                # different blocks, Git puts the common closing delimiter
                # after the conflict hunk. Close ours here and let that shared
                # delimiter close theirs.
                return ours.rstrip("\n") + "\n */\n\n" + theirs
            if c_comments(ours) and c_comments(theirs):
                return theirs
        elif path.endswith(".md"):
            a, b = row_names(ours), row_names(theirs)
            if a and b and set(a) == set(b):
                return theirs
        left += 1
        return m.group(0)

    s = pat.sub(sub, s)
    source_path.write_text(s)
    print(f"{path}: {'resolved' if left == 0 else f'{left} hunk(s) left'}")
    return left


if __name__ == "__main__":
    sys.exit(1 if sum(resolve(p) for p in sys.argv[1:]) else 0)
