#!/usr/bin/env python3
"""Resolve the freshest unambiguous campaign integration base."""

from __future__ import annotations

import argparse
from pathlib import Path
import subprocess
import sys


LOCAL_BASE = "campaign/unchain"
REMOTE_BASE = "origin/campaign/unchain"


class IntegrationBaseError(RuntimeError):
    """The local and remote integration histories cannot be ordered safely."""


def _git(repo: Path, *args: str, check: bool = True) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(
        ["git", "-C", str(repo), *args],
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    if check and result.returncode != 0:
        raise IntegrationBaseError(
            result.stderr.strip() or f"git {' '.join(args)} failed"
        )
    return result


def _commit(repo: Path, ref: str) -> str | None:
    result = _git(repo, "rev-parse", "--verify", f"{ref}^{{commit}}", check=False)
    if result.returncode == 0:
        return result.stdout.strip()
    if result.returncode == 128:
        return None
    raise IntegrationBaseError(result.stderr.strip() or f"cannot resolve {ref}")


def _is_ancestor(repo: Path, older: str, newer: str) -> bool:
    result = _git(repo, "merge-base", "--is-ancestor", older, newer, check=False)
    if result.returncode in (0, 1):
        return result.returncode == 0
    raise IntegrationBaseError(
        result.stderr.strip() or f"cannot compare {older} and {newer}"
    )


def resolve(repo: Path) -> str:
    """Return the newest linearly ordered integration ref, or HEAD."""
    repo = repo.resolve()
    local = _commit(repo, LOCAL_BASE)
    remote = _commit(repo, REMOTE_BASE)
    if local is None and remote is None:
        if _commit(repo, "HEAD") is None:
            raise IntegrationBaseError("repository has no integration ref or HEAD")
        return "HEAD"
    if local is None:
        return REMOTE_BASE
    if remote is None or local == remote:
        return LOCAL_BASE
    if _is_ancestor(repo, local, remote):
        return REMOTE_BASE
    if _is_ancestor(repo, remote, local):
        return LOCAL_BASE
    raise IntegrationBaseError(
        f"{LOCAL_BASE} and {REMOTE_BASE} have diverged; pass an explicit base"
    )


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo", type=Path, default=Path.cwd())
    args = parser.parse_args(argv)
    try:
        print(resolve(args.repo))
    except IntegrationBaseError as exc:
        print(f"integration-base: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
