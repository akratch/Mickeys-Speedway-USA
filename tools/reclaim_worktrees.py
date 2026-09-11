#!/usr/bin/env python3
"""Remove lane worktrees whose work is already integrated.

    tools/reclaim_worktrees.py             # report only (the default)
    tools/reclaim_worktrees.py --apply     # actually remove them

A lane worktree carries its own `build/` tree, so the fleet grows by hundreds of
megabytes per lane and does not shrink on its own. One sweep reclaimed **31 GB**
across 106 worktrees. Left alone it will fill the disk, and a full disk fails a
build in ways that look like a compiler problem.

**A worktree is removed only when all three hold**, each re-checked at the
moment of deletion rather than trusted from an earlier survey:

1. it is not running a lane -- a `.codex-run.log` without a `.codex-status`
   means a detached Codex worker is still writing to it, `--exclude` covers
   in-process agents by name, and **nothing inside it has been modified within
   `--min-idle-minutes`** (default 45). That last one is not redundant: the
   other three conditions cannot tell a just-created lane from a fully
   integrated one, because a new worktree has no commits of its own, a clean
   tree, and a branch level with the integration ref. A sweep run while a lane
   was starting up deleted its worktree for exactly that reason, so the idle
   probe is the condition that actually protects live work;
2. `git status --porcelain` is empty, so nothing uncommitted is lost;
3. `git rev-list --count <integration>..<branch>` is 0, so every commit it
   carries is already merged.

**Branches are never touched.** Only the checkout goes. A branch costs nothing,
`tools/new_lane.sh` can recreate the worktree, and deleting a lane branch before
its merge commit has cost this project real work before.

`git worktree remove` refuses outright here -- the checkouts contain submodules
-- so removal is a directory delete followed by `git worktree prune`. That is
why the three conditions are re-verified immediately before the delete.
"""
from __future__ import annotations

import argparse
import pathlib
import shutil
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]


def git(*args: str, cwd: pathlib.Path | None = None) -> str:
    out = subprocess.run(["git", *args], cwd=cwd or ROOT,
                         capture_output=True, text=True)
    return out.stdout.strip()


def worktrees() -> list[pathlib.Path]:
    paths = []
    for line in git("worktree", "list", "--porcelain").splitlines():
        if line.startswith("worktree "):
            path = pathlib.Path(line[len("worktree "):])
            if path != ROOT:
                paths.append(path)
    return paths


def lane_name(path: pathlib.Path) -> str:
    return path.name.removeprefix("mickey-lane-").removesuffix(".noindex")


def recently_touched(path: pathlib.Path, minutes: int) -> str | None:
    """The first file modified inside `path` within `minutes`, or None.

    This is the condition a freshly-created lane fails and a finished one
    passes. It exists because the other three conditions CANNOT distinguish a
    lane that has just been created from one whose work is fully integrated: a
    new worktree has no commits of its own, a clean tree, and a branch level
    with the integration ref. A sweep run while a lane was starting up deleted
    its worktree out from under it for exactly that reason.

    `find -newermt ... -print -quit` short-circuits on the first hit, so the
    common case (an active lane, something written seconds ago) is fast.
    """
    try:
        out = subprocess.run(
            ["find", str(path), "-newermt", f"-{minutes} minutes",
             "-print", "-quit"],
            capture_output=True, text=True, timeout=120)
    except (OSError, subprocess.TimeoutExpired):
        # Cannot tell -> assume it is live. Refusing to delete is always the
        # recoverable direction; a stale worktree costs disk, a deleted live
        # one costs a lane's work.
        return "(probe failed)"
    hit = out.stdout.strip().splitlines()
    return hit[0] if hit else None


def status(path: pathlib.Path, integration: str, exclude: set[str],
           min_idle_minutes: int = 45) -> tuple[bool, str]:
    """(removable, why-not)."""
    name = lane_name(path)
    if name in exclude:
        return False, "excluded by name"
    if (path / ".codex-run.log").is_file() and not (path / ".codex-status").is_file():
        return False, "a Codex worker is still running here"
    if not path.is_dir():
        return False, "already gone"
    dirty = git("status", "--porcelain", cwd=path)
    if dirty:
        return False, f"{len(dirty.splitlines())} uncommitted change(s)"
    branch = git("rev-parse", "--abbrev-ref", "HEAD", cwd=path)
    if not branch or branch == "HEAD":
        return False, "detached or unreadable HEAD"
    ahead = git("rev-list", "--count", f"{integration}..{branch}")
    if ahead != "0":
        return False, f"{ahead} commit(s) not in {integration}"
    touched = recently_touched(path, min_idle_minutes)
    if touched is not None:
        return False, f"active within {min_idle_minutes}m ({touched})"
    return True, ""


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(
        description="Remove lane worktrees whose work is already integrated.")
    ap.add_argument("--apply", action="store_true",
                    help="remove them; without this the tool only reports")
    ap.add_argument("--integration", default="campaign/unchain")
    ap.add_argument("--exclude", default="",
                    help="comma-separated lane names to keep (in-process agents)")
    ap.add_argument("--min-idle-minutes", type=int, default=45,
                    help="refuse a worktree with any file modified this "
                         "recently; a lane that has just been created is "
                         "otherwise indistinguishable from an integrated one "
                         "(default: 45)")
    args = ap.parse_args(argv)
    exclude = {n.strip() for n in args.exclude.split(",") if n.strip()}

    removable, kept = [], []
    for path in worktrees():
        ok, why = status(path, args.integration, exclude, args.min_idle_minutes)
        (removable if ok else kept).append((path, why))

    for path, why in kept:
        print(f"  keep   {lane_name(path):<16} {why}")
    if not args.apply:
        print(f"\n{len(removable)} worktree(s) removable, {len(kept)} kept.")
        for path, _ in removable:
            print(f"  would remove {lane_name(path)}")
        print("\nRe-run with --apply to remove them. Branches are never touched.")
        return 0

    removed = 0
    for path, _ in removable:
        ok, why = status(path, args.integration, exclude, args.min_idle_minutes)   # re-verify at delete time
        if not ok:
            print(f"  refuse {lane_name(path):<16} changed since the survey: {why}")
            continue
        shutil.rmtree(path, ignore_errors=True)
        removed += 1
    subprocess.run(["git", "worktree", "prune"], cwd=ROOT, check=False)
    print(f"\nremoved {removed} worktree(s); {len(kept)} kept; branches untouched")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
