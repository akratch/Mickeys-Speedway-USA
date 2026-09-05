#!/usr/bin/env python3
"""Untracked receipts and immutable evidence for bounded permuter searches.

Receipts live in Git's common directory so independent lanes share completed
search knowledge without opening one another's worktrees. A kernel file lock
owns each search key; crashes release it without stale PID bookkeeping.
"""
from __future__ import annotations

import contextlib
import fcntl
import hashlib
import io
import json
import os
import subprocess
import stat
import tempfile
import time
import uuid
import zipfile
from pathlib import Path

SCHEMA = 2
MAX_ARTIFACT_BYTES = 128 * 1024 * 1024
REQUIRED_ARTIFACTS = {"baseline/base.c", "baseline/base.o", "baseline/compile.sh",
                      "baseline/target.s", "baseline/settings.toml", "baseline/recipe.json",
                      "baseline/tu.c", "baseline/permuter_settings.toml",
                      "best/source.c", "best/object.o"}


def owned_bytes(root: Path, relative: str) -> bytes:
    """Read only regular files below an owned directory, without following links."""
    parts = relative.split("/")
    if any(p in ("", ".", "..") for p in parts) or relative.startswith("/"):
        raise ValueError("unsafe artifact path")
    fd = os.open(root, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW)
    try:
        for part in parts[:-1]:
            child = os.open(part, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW, dir_fd=fd)
            os.close(fd)
            fd = child
        file_fd = os.open(parts[-1], os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK, dir_fd=fd)
        with os.fdopen(file_fd, "rb") as stream:
            if not stat.S_ISREG(os.fstat(stream.fileno()).st_mode):
                raise ValueError("artifact is not a regular file")
            data = stream.read(MAX_ARTIFACT_BYTES + 1)
            if len(data) > MAX_ARTIFACT_BYTES:
                raise ValueError("artifact exceeds preservation byte cap")
            return data
    finally:
        os.close(fd)


def attempt_files(root: Path) -> tuple[dict[str, bytes], list[str]]:
    files, errors = {}, []
    total = 0
    for directory, dirs, names in os.walk(root, followlinks=False):
        for name in sorted(dirs + names):
            path = Path(directory) / name
            rel = path.relative_to(root).as_posix()
            if name in dirs and not path.is_symlink():
                continue
            try:
                data = owned_bytes(root, rel)
                total += len(data)
                if total > MAX_ARTIFACT_BYTES:
                    raise ValueError("attempt exceeds preservation byte cap")
                files["attempt/" + rel] = data
            except (OSError, ValueError) as error:
                errors.append(f"{rel}: {error}")
    return files, errors


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

    def save_bundle(self, files: dict[str, bytes], *, complete: bool) -> str:
        """Immutable deterministic archive; never extract or execute archive paths."""
        if complete and not REQUIRED_ARTIFACTS <= files.keys():
            raise ValueError("incomplete artifact bundle")
        if sum(map(len, files.values())) > MAX_ARTIFACT_BYTES:
            raise ValueError("bundle exceeds preservation byte cap")
        manifest = {"schema": SCHEMA, "complete": complete, "files": {}}
        output = io.BytesIO()
        with zipfile.ZipFile(output, "w", compression=zipfile.ZIP_STORED) as archive:
            for name, data in sorted(files.items()):
                if any(p in ("", ".", "..") for p in name.split("/")) or "\\" in name or name == "manifest.json":
                    raise ValueError("unsafe bundle member")
                manifest["files"][name] = {"size": len(data), "sha256": hashlib.sha256(data).hexdigest()}
                archive.writestr(zipfile.ZipInfo(name), data)
            archive.writestr(zipfile.ZipInfo("manifest.json"), json.dumps(manifest, sort_keys=True).encode())
        data = output.getvalue()
        key = hashlib.sha256(data).hexdigest()
        directory = self.root / "bundles"
        directory.mkdir(parents=True, exist_ok=True)
        if directory.is_symlink():
            raise ValueError("bundle directory must not be a symlink")
        temporary = None
        try:
            with tempfile.NamedTemporaryFile(dir=directory, delete=False) as stream:
                temporary = Path(stream.name)
                stream.write(data)
                stream.flush()
                os.fsync(stream.fileno())
            try:
                os.link(temporary, directory / (key + ".zip"))
            except FileExistsError:
                if owned_bytes(directory, key + ".zip") != data:
                    raise ValueError("existing content-addressed bundle is corrupt; preserved for review")
            dir_fd = os.open(directory, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW)
            try:
                os.fsync(dir_fd)
            finally:
                os.close(dir_fd)
        finally:
            if temporary is not None:
                temporary.unlink(missing_ok=True)
        return key

    def read_bundle(self, key: str, *, require_complete: bool = True) -> dict[str, bytes]:
        self.directory(key)  # Validate digest before constructing a filesystem path.
        data = owned_bytes(self.root / "bundles", key + ".zip")
        if hashlib.sha256(data).hexdigest() != key:
            raise ValueError("artifact bundle digest mismatch")
        with zipfile.ZipFile(io.BytesIO(data)) as archive:
            if (sum(info.file_size for info in archive.infolist()) > MAX_ARTIFACT_BYTES
                    or any(info.compress_type != zipfile.ZIP_STORED for info in archive.infolist())):
                raise ValueError("unsupported or oversized artifact archive")
            manifest = json.loads(archive.read("manifest.json"))
            if not isinstance(manifest, dict) or not isinstance(manifest.get("files"), dict):
                raise ValueError("invalid artifact manifest")
            files = manifest["files"]
            if manifest.get("schema") != SCHEMA or (require_complete and
                    (manifest.get("complete") is not True or not REQUIRED_ARTIFACTS <= files.keys())):
                raise ValueError("incomplete artifact bundle")
            if len(archive.namelist()) != len(files) + 1 or set(archive.namelist()) != {*files, "manifest.json"}:
                raise ValueError("unexpected artifact bundle members")
            result = {}
            for name, metadata in files.items():
                if not isinstance(metadata, dict):
                    raise ValueError("invalid artifact member metadata")
                if any(p in ("", ".", "..") for p in name.split("/")) or "\\" in name:
                    raise ValueError("unsafe artifact bundle member")
                content = archive.read(name)
                if len(content) != metadata["size"] or hashlib.sha256(content).hexdigest() != metadata["sha256"]:
                    raise ValueError("artifact member digest mismatch")
                result[name] = content
            return result

    def artifacts_valid(self, value: dict) -> bool:
        try:
            self.read_bundle(value["result"]["artifact_bundle"])
            return True
        except (OSError, ValueError, KeyError, TypeError, zipfile.BadZipFile):
            return False

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
                or not reusable(value.get("result", {})) or not self.artifacts_valid(value)):
            return None
        return value

    def record(self, inputs: dict, result: dict) -> str:
        """Called while holding claim(). Retain attempts and best scalar score."""
        key = digest(inputs)
        directory = self.directory(key)
        # Scalar JSON only references the separate ignored evidence bundle.
        scalar_fields = ("func", "c_file", "overlay", "ok", "base_score", "best_score",
                         "zero_found", "promoted", "seconds", "extended", "stopped_flat",
                         "stopped_batch", "annotated_relocs", "scratch_path", "artifact_bundle")
        scalar = {k: result.get(k) for k in scalar_fields}
        for name in ("error", "promote_error", "commit_error"):
            scalar[name] = bool(result.get(name))
        value = {"schema": SCHEMA, "key": key, "inputs": inputs,
                 "finished": time.time(), "result": scalar}
        atomic_json(directory / "attempts" / f"{time.time_ns()}-{uuid.uuid4().hex}.json", value)
        if reusable(scalar) and self.artifacts_valid(value):
            atomic_json(directory / "complete.json", value)
            # Each settings key owns its own context-index file; writers do
            # not share a read/modify/write index and cannot lose one another.
            context_path = self.root / "contexts" / digest(inputs["context"]) / f"{key}.json"
            try:
                old_value = json.loads(context_path.read_text())
                if not self.artifacts_valid(old_value):
                    raise ValueError("previous artifacts unavailable")
                previous = old_value["result"]
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
                if (value.get("schema") == SCHEMA and value.get("key") == digest(value["inputs"])
                        and value["inputs"]["context"] == context and reusable(result) and self.artifacts_valid(value)
                        and isinstance(result.get("best_score"), int)
                        and 0 < result["best_score"] < result["base_score"]):
                    return True
            except (OSError, ValueError, KeyError, TypeError):
                continue
        return False
