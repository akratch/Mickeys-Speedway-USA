#!/usr/bin/env python3
"""Reversible tracked-file journal for one sweep promotion in its own lane."""
from __future__ import annotations

import os
import subprocess
import tempfile
import time
from pathlib import Path


def contents(path: Path) -> bytes | None:
    return path.read_bytes() if path.exists() else None


def replace(path: Path, data: bytes | None) -> None:
    if data is None:
        path.unlink(missing_ok=True)
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    mode = path.stat().st_mode if path.exists() else 0o644
    with tempfile.NamedTemporaryFile(dir=path.parent, delete=False) as stream:
        stream.write(data)
        temporary = Path(stream.name)
    temporary.chmod(mode)
    os.replace(temporary, path)


class FileJournal:
    def __init__(self, root: Path, paths: list[Path], evidence: Path):
        self.root = root
        self.evidence = evidence
        self.before = {path: contents(path) for path in paths}
        self.owned = dict(self.before)
        evidence.mkdir(parents=True, exist_ok=True)
        for path, data in self.before.items():
            if data is not None:
                backup = evidence / "before" / path.relative_to(root)
                backup.parent.mkdir(parents=True, exist_ok=True)
                backup.write_bytes(data)

    def check(self, paths=None):
        for path in self.owned if paths is None else paths:
            if contents(path) != self.owned[path]:
                raise RuntimeError(f"concurrent edit preserved: {path.relative_to(self.root)}")

    def write(self, path: Path, data: bytes | None):
        self.check([path])
        replace(path, data)
        self.owned[path] = data

    def capture(self, paths: list[Path]):
        for path in paths:
            self.owned[path] = contents(path)

    def changed(self) -> list[Path]:
        return [path for path in self.before if self.before[path] != self.owned[path]]

    def rollback(self, deadline: float | None = None) -> list[str]:
        """Reverse our writes; three-way merge independent subsequent edits.

        A conflict preserves the current file plus before/owned snapshots for
        review. It never guesses through or overwrites a concurrent edit.
        """
        conflicts = []
        for index, path in enumerate(self.changed()):
            before, owned, current = self.before[path], self.owned[path], contents(path)
            if current == owned:
                replace(path, before)
                continue
            if None not in (before, owned, current):
                timeout = 5 if deadline is None else min(5, deadline - time.monotonic())
                if timeout <= 0:
                    conflicts.append(str(path.relative_to(self.root)))
                    continue
                folder = self.evidence / f"rollback-{index}"
                folder.mkdir(exist_ok=True)
                for name, data in (("current", current), ("owned", owned), ("before", before)):
                    (folder / name).write_bytes(data)
                merged = subprocess.run(
                    ["git", "merge-file", "-p", str(folder / "current"),
                     str(folder / "owned"), str(folder / "before")],
                    capture_output=True, timeout=timeout)
                if merged.returncode == 0 and contents(path) == current:
                    replace(path, merged.stdout)
                    continue
            conflicts.append(str(path.relative_to(self.root)))
        return conflicts
