#!/usr/bin/env python3
"""Publish and restore verified, commit-keyed lane bootstrap artifacts.

The cache lives below Git's common directory, never in a sibling worktree.
Publishing always proves the current tracked tree is clean and runs the normal
ROM verification gate.  Restoring requires the exact same commit and an empty
destination for every cached artifact.  The copied build remains lane-local;
on APFS the copy uses clonefile-backed ``cp -c`` to avoid duplicate storage.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
import time
from pathlib import Path


SCHEMA = "mickey-verified-lane-cache-v1"
CACHE_DIRNAME = "mickey-lane-cache"
MANIFEST = "manifest.json"
ARTIFACTS = (
    "asm",
    "assets",
    ".splat_cache",
    "build/.splat-stamp",
    "build/asm",
    "build/assets",
    "build/src",
    "build/mickey.us.elf",
    "build/mickey.us.map",
    "build/mickey.us.bin",
    "build/mickey.us.z64",
    "mickey.us.ld",
    "undefined_funcs_auto.us.txt",
    "undefined_syms_auto.us.txt",
    "tools/n64crc",
)


class CacheError(RuntimeError):
    pass


def _run(
    argv: list[str], root: Path, *, check: bool = True, capture: bool = True
) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        argv,
        cwd=root,
        check=check,
        text=True,
        stdout=subprocess.PIPE if capture else None,
        stderr=subprocess.PIPE if capture else None,
    )


def _repo_root(path: Path) -> Path:
    result = _run(["git", "rev-parse", "--show-toplevel"], path)
    return Path(result.stdout.strip()).resolve()


def _common_dir(root: Path) -> Path:
    result = _run(
        ["git", "rev-parse", "--path-format=absolute", "--git-common-dir"], root
    )
    return Path(result.stdout.strip()).resolve()


def _head(root: Path) -> str:
    return _run(["git", "rev-parse", "HEAD"], root).stdout.strip()


def _assert_clean(root: Path) -> None:
    status = _run(
        ["git", "status", "--porcelain=v1", "--untracked-files=no"], root
    ).stdout
    if status.strip():
        names = ", ".join(line[3:] for line in status.splitlines()[:5])
        raise CacheError(f"tracked worktree is not clean ({names})")


def _expected_sha1(root: Path) -> str:
    path = root / "mickey.us.sha1"
    if not path.is_file():
        raise CacheError("missing mickey.us.sha1")
    value = path.read_text(encoding="utf-8").split()[0].lower()
    if len(value) != 40 or any(char not in "0123456789abcdef" for char in value):
        raise CacheError("mickey.us.sha1 does not begin with a SHA-1 digest")
    return value


def _file_sha1(path: Path) -> str:
    digest = hashlib.sha1()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def _cache_path(root: Path, commit: str) -> Path:
    return _common_dir(root) / CACHE_DIRNAME / commit


def _copy(source: Path, destination: Path) -> None:
    destination.parent.mkdir(parents=True, exist_ok=True)
    if sys.platform == "darwin":
        command = ["cp", "-cR", str(source), str(destination)]
        result = subprocess.run(command, check=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        if result.returncode == 0:
            return
    if source.is_dir():
        shutil.copytree(source, destination, copy_function=shutil.copy2)
    else:
        shutil.copy2(source, destination)


def _present_artifacts(root: Path) -> list[str]:
    return [relative for relative in ARTIFACTS if (root / relative).exists()]


def _manifest(cache: Path) -> dict[str, object]:
    path = cache / MANIFEST
    try:
        payload = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise CacheError(f"invalid cache manifest {path}: {exc}") from exc
    if payload.get("schema") != SCHEMA:
        raise CacheError(f"unsupported cache schema in {path}")
    return payload


def publish(root: Path, *, jobs: int, niceness: int) -> Path:
    root = _repo_root(root)
    _assert_clean(root)
    commit = _head(root)
    expected = _expected_sha1(root)
    command = ["nice", "-n", str(niceness), "gmake", f"-j{jobs}", "verify"]
    result = _run(command, root, check=False, capture=False)
    if result.returncode:
        raise CacheError(f"verification failed with exit status {result.returncode}")
    _assert_clean(root)

    rom = root / "build/mickey.us.z64"
    if not rom.is_file() or _file_sha1(rom) != expected:
        raise CacheError("verified ROM artifact is absent or has the wrong SHA-1")
    artifacts = _present_artifacts(root)
    required = {"asm", "assets", "build/.splat-stamp", "build/mickey.us.elf", "build/mickey.us.z64"}
    missing = sorted(required.difference(artifacts))
    if missing:
        raise CacheError("verified build lacks cache prerequisites: " + ", ".join(missing))

    destination = _cache_path(root, commit)
    if destination.exists():
        payload = _manifest(destination)
        if payload.get("commit") != commit or payload.get("rom_sha1") != expected:
            raise CacheError(f"cache collision at {destination}")
        return destination

    parent = destination.parent
    parent.mkdir(parents=True, exist_ok=True)
    temporary = Path(tempfile.mkdtemp(prefix=f".{commit[:12]}-", dir=parent))
    try:
        for relative in artifacts:
            _copy(root / relative, temporary / relative)
        payload = {
            "schema": SCHEMA,
            "commit": commit,
            "rom_sha1": expected,
            "artifacts": artifacts,
            "proof": f"nice -n {niceness} gmake -j{jobs} verify",
        }
        (temporary / MANIFEST).write_text(
            json.dumps(payload, indent=2, sort_keys=True) + "\n", encoding="utf-8"
        )
        os.replace(temporary, destination)
    except BaseException:
        shutil.rmtree(temporary, ignore_errors=True)
        raise
    return destination


def restore(root: Path) -> Path:
    root = _repo_root(root)
    _assert_clean(root)
    commit = _head(root)
    cache = _cache_path(root, commit)
    if not cache.is_dir():
        raise FileNotFoundError(f"no verified lane cache for {commit}")
    payload = _manifest(cache)
    if payload.get("commit") != commit:
        raise CacheError("cache commit does not match the lane HEAD")
    if payload.get("rom_sha1") != _expected_sha1(root):
        raise CacheError("cache ROM identity does not match this tree")
    artifacts = payload.get("artifacts")
    if not isinstance(artifacts, list) or not artifacts:
        raise CacheError("cache manifest has no artifact list")
    if any(relative not in ARTIFACTS for relative in artifacts):
        raise CacheError("cache manifest contains an unapproved artifact path")
    collisions = [relative for relative in artifacts if (root / relative).exists()]
    if collisions:
        raise CacheError("restore destinations already exist: " + ", ".join(collisions[:5]))

    for relative in artifacts:
        source = cache / relative
        if not source.exists():
            raise CacheError(f"cache artifact is missing: {relative}")
    for relative in artifacts:
        _copy(cache / relative, root / relative)

    # Git just populated tracked prerequisites. Make's timestamp graph should
    # therefore see this exact-commit snapshot as current, not rebuild it all.
    stamp = time.time_ns()
    for relative in artifacts:
        path = root / relative
        if path.is_dir():
            for child in path.rglob("*"):
                if child.is_file() and not child.is_symlink():
                    os.utime(child, ns=(stamp, stamp))
        elif path.is_file() and not path.is_symlink():
            os.utime(path, ns=(stamp, stamp))

    rom = root / "build/mickey.us.z64"
    if _file_sha1(rom) != payload["rom_sha1"]:
        raise CacheError("restored ROM artifact failed its SHA-1 check")
    query = _run(
        ["gmake", "--no-print-directory", "-q", "build/mickey.us.elf"],
        root,
        check=False,
    )
    if query.returncode != 0:
        raise CacheError("restored build is not current according to Make")
    return cache


def _parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("action", choices=("publish", "restore", "path"))
    parser.add_argument("--repo", type=Path, default=Path.cwd())
    parser.add_argument("--jobs", type=int, default=2)
    parser.add_argument("--nice", type=int, default=10)
    parser.add_argument("--quiet", action="store_true")
    return parser


def main(argv: list[str] | None = None) -> int:
    args = _parser().parse_args(argv)
    if args.jobs < 1:
        raise SystemExit("--jobs must be positive")
    if not 0 <= args.nice <= 19:
        raise SystemExit("--nice must be between 0 and 19")
    try:
        root = _repo_root(args.repo.resolve())
        if args.action == "publish":
            path = publish(root, jobs=args.jobs, niceness=args.nice)
            message = f"published verified lane cache: {path}"
        elif args.action == "restore":
            path = restore(root)
            message = f"restored verified lane cache: {path}"
        else:
            path = _cache_path(root, _head(root))
            if not path.is_dir():
                raise FileNotFoundError(f"no verified lane cache for {_head(root)}")
            _manifest(path)
            message = str(path)
    except FileNotFoundError as exc:
        if not args.quiet:
            print(f"lane-cache: {exc}", file=sys.stderr)
        return 3
    except (CacheError, OSError, subprocess.SubprocessError) as exc:
        print(f"lane-cache: {exc}", file=sys.stderr)
        return 2
    if not args.quiet:
        print(message)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
