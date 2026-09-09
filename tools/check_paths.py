#!/usr/bin/env python3
"""Refuse tracked paths that no reviewer would have chosen deliberately.

An agent's `git add -A` swept an empty file literally named " " into a
commit on 2026-09-09. Every gate passed it: it is not ROM-derived, so the
clean-room sweep is silent; it is zero bytes, so no size rule fires; and it
breaks no build. It surfaced only as a GitHub listing saying "1 entry was
omitted from the list", because the web UI cannot render an entry with that
name.

The failure class is a stray path created by a shell mishap -- a redirect
with a stray space, an unquoted variable that expanded to nothing, an
editor swap file -- and then committed wholesale. None of it is malicious
and none of it is caught by a content rule, because the *content* is fine.
The name is the defect.

    python3 tools/check_paths.py

Exit 1 on a suspicious tracked path, 0 otherwise. Reports the rule each one
broke. Reads only the index/tree via git; no file contents are examined.
"""

from __future__ import annotations

import re
import subprocess
import sys

# Names that are almost always an accident rather than a decision. Kept
# deliberately narrow: a false positive here blocks a commit, so each entry
# has to be something no one would type on purpose.
RULES = (
    (re.compile(r"(^|/)\s+($|/)"), "path component is only whitespace"),
    (re.compile(r"\s($|/)"), "path component ends in whitespace"),
    (re.compile(r"[\x00-\x1f\x7f]"), "path contains a control character"),
    (re.compile(r"(^|/)\.($|/)|(^|/)\.\.($|/)"), "path contains a . or .. component"),
    (re.compile(r"\.(sw[a-p]|orig|rej)($|/)"), "editor swap or merge leftover"),
    (re.compile(r"(^|/)(core|a\.out)($|/)"), "build or crash artifact"),
    (re.compile(r"(^|/)~"), "path component starts with ~"),
)


def tracked_paths() -> list[str]:
    """Every tracked path, with -z so a name containing anything is safe."""
    data = subprocess.check_output(["git", "ls-files", "-z"])
    return [p.decode("utf-8", "surrogateescape") for p in data.split(b"\0") if p]


def offenders(paths: list[str]) -> list[tuple[str, str]]:
    found = []
    for path in paths:
        for pattern, why in RULES:
            if pattern.search(path):
                found.append((path, why))
                break
    return found


def main() -> int:
    paths = tracked_paths()
    bad = offenders(paths)
    if not bad:
        print(f"tracked paths OK -- {len(paths)} checked")
        return 0
    print(f"{len(bad)} tracked path(s) look accidental:", file=sys.stderr)
    for path, why in bad:
        print(f"  {path!r}: {why}", file=sys.stderr)
    print(
        "\nThese are usually swept in by `git add -A` after a shell mishap.\n"
        "Remove them with `git rm -- <path>` (quote it), or add a deliberate\n"
        "exception to RULES in tools/check_paths.py with a written reason.",
        file=sys.stderr,
    )
    return 1


if __name__ == "__main__":
    sys.exit(main())
