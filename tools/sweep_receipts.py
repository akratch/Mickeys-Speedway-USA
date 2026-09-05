#!/usr/bin/env python3
"""Untracked, content-addressed scalar receipts for bounded permuter searches.

Receipts live in Git's common directory so independent lanes share completed
search knowledge without opening one another's worktrees. A kernel file lock
owns each search key; crashes release it without stale PID bookkeeping.
"""
from __future__ import annotations

import contextlib
import fcntl
import hashlib
import json
import os
import subprocess
import tempfile
import time
import uuid
from pathlib import Path

SCHEMA = 1


def digest(value: object) -> str:
    return hashlib.sha256(json.dumps(value, sort_keys=True, separators=(",", ":")).encode()).hexdigest()


def file_digest(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def tree_digest(root: Path, *, source_only: bool = False) -> str:
    """Hash actual files, including dirty source, without build/cache churn."""
    excluded = {".git", ".venv", "__pycache__", ".pytest_cache", "build", "nonmatchings", "test"}
    rows = []
    for directory, dirs, names in os.walk(root, followlinks=True):
        dirs[:] = sorted(d for d in dirs if d not in excluded)
        for name in sorted(names):
            path = Path(directory) / name
            if source_only and path.suffix not in {".py", ".toml", ".inc", ".h", ".c", ".txt", ".sh"}:
                continue
            if path.is_file() and not name.startswith("debug_"):
                rows.append((path.relative_to(root).as_posix(), file_digest(path)))
    if not rows:
        raise ValueError(f"no tool identity inputs under {root}")
    return digest(rows)


def atomic_json(path: Path, value: object) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = None
    try:
        with tempfile.NamedTemporaryFile(mode="w", dir=path.parent, delete=False) as stream:
            temporary = Path(stream.name)
            json.dump(value, stream, sort_keys=True, indent=2)
            stream.write("\n")
            stream.flush()
            os.fsync(stream.fileno())
        os.replace(temporary, path)
    finally:
        if temporary is not None:
            temporary.unlink(missing_ok=True)


def reusable(result: dict) -> bool:
    """A failed, interrupted, or unpromoted zero-score search is retryable."""
    return (isinstance(result, dict) and result.get("ok") is True
            and type(result.get("base_score")) is int and result["base_score"] >= 0
            and not any(result.get(k) for k in
                        ("error", "promote_error", "commit_error", "stopped_batch"))
            and (not result.get("zero_found") or result.get("promoted") is True))


class ReceiptStore:
    def __init__(self, root: Path):
        self.root = root

    @classmethod
    def for_repo(cls, repo: Path) -> "ReceiptStore":
        common = subprocess.check_output(
            ["git", "rev-parse", "--path-format=absolute", "--git-common-dir"],
            cwd=repo, text=True).strip()
        return cls(Path(common) / "mickey-sweep-receipts" / f"v{SCHEMA}")

    def directory(self, key: str) -> Path:
        if len(key) != 64 or any(c not in "0123456789abcdef" for c in key):
            raise ValueError("invalid receipt key")
        return self.root / key[:2] / key

    @contextlib.contextmanager
    def claim(self, key: str):
        directory = self.directory(key)
        directory.mkdir(parents=True, exist_ok=True)
        with (directory / "search.lock").open("a") as lock:
            try:
                fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
            except BlockingIOError:
                yield False
                return
            try:
                yield True
            finally:
                fcntl.flock(lock, fcntl.LOCK_UN)

    def completed(self, key: str) -> dict | None:
        try:
            value = json.loads((self.directory(key) / "complete.json").read_text())
        except (OSError, ValueError):
            return None
        if (not isinstance(value, dict) or value.get("schema") != SCHEMA or value.get("key") != key
                or digest(value.get("inputs")) != key
                or not reusable(value.get("result", {}))):
            return None
        return value

    def record(self, inputs: dict, result: dict) -> str:
        """Called while holding claim(). Retain attempts and best scalar score."""
        key = digest(inputs)
        directory = self.directory(key)
        # No source, target bytes, objects, logs, or exception text belongs in
        # the receipt. The scratch path points to private local diagnostics.
        scalar_fields = ("func", "c_file", "overlay", "ok", "base_score", "best_score",
                         "zero_found", "promoted", "seconds", "extended", "stopped_flat",
                         "stopped_batch", "annotated_relocs", "scratch_path")
        scalar = {k: result.get(k) for k in scalar_fields}
        for name in ("error", "promote_error", "commit_error"):
            scalar[name] = bool(result.get(name))
        value = {"schema": SCHEMA, "key": key, "inputs": inputs,
                 "finished": time.time(), "result": scalar}
        atomic_json(directory / "attempts" / f"{time.time_ns()}-{uuid.uuid4().hex}.json", value)
        if reusable(scalar):
            atomic_json(directory / "complete.json", value)
            # Each settings key owns its own context-index file; writers do
            # not share a read/modify/write index and cannot lose one another.
            context_path = self.root / "contexts" / digest(inputs["context"]) / f"{key}.json"
            try:
                previous = json.loads(context_path.read_text())["result"]
                old_score = min(v for v in (previous.get("base_score"), previous.get("best_score"))
                                if isinstance(v, int))
            except (OSError, ValueError, KeyError, TypeError):
                old_score = None
            new_score = min(v for v in (scalar["base_score"], scalar["best_score"])
                            if isinstance(v, int))
            if old_score is None or new_score < old_score:
                atomic_json(context_path, value)
        scores = [scalar[k] for k in ("best_score", "base_score") if isinstance(scalar[k], int)]
        if scores:
            best_path = directory / "best.json"
            try:
                previous = json.loads(best_path.read_text())
                previous_score = previous["score"]
            except (OSError, ValueError, KeyError):
                previous_score = None
            if previous_score is None or min(scores) < previous_score:
                atomic_json(best_path, {**value, "score": min(scores)})
        return key

    def descending(self, context: dict) -> bool:
        """Find a successful improvement under any caps for this exact input."""
        for path in (self.root / "contexts" / digest(context)).glob("*.json"):
            try:
                value = json.loads(path.read_text())
                result = value["result"]
                if (value["inputs"]["context"] == context and reusable(result)
                        and isinstance(result.get("best_score"), int)
                        and 0 < result["best_score"] < result["base_score"]):
                    return True
            except (OSError, ValueError, KeyError, TypeError):
                continue
        return False
