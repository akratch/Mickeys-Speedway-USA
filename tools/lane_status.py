#!/usr/bin/env python3
"""Report unintegrated lane commits without reading sibling worktrees.

The report is a coordination hint. A commit message such as ``Match foo`` is
shown as a *claim* until the integration lane repeats the project's exact
object, relocation, linked-range, and ROM checks (ADR 0011).
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from dataclasses import asdict, dataclass


MATCH_RE = re.compile(r"^match(?:ed)?\s+([A-Za-z_][A-Za-z0-9_]*)\b", re.I)


def git(*args: str, check: bool = True) -> str:
    result = subprocess.run(
        ["git", *args], text=True, stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    if check and result.returncode != 0:
        message = result.stderr.strip() or "git command failed"
        raise RuntimeError(f"git {' '.join(args)}: {message}")
    return result.stdout


def has_global_asm(ref: str, symbol: str) -> bool:
    result = subprocess.run(
        ["git", "grep", "-q", "-F", f"{symbol}.s", ref, "--", "src"],
        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
    )
    if result.returncode not in (0, 1):
        raise RuntimeError(f"git grep failed for {ref}:{symbol}")
    return result.returncode == 0


def source_paths(commit: str) -> list[str]:
    paths = git(
        "diff-tree", "--no-commit-id", "--name-only", "-r", commit,
        "--", "src",
    ).splitlines()
    return sorted(path for path in paths if path.endswith((".c", ".h")))


@dataclass
class Claim:
    symbol: str
    commit: str
    committed_at: str
    subject: str
    state: str
    source_paths: list[str]


@dataclass
class Lane:
    branch: str
    head: str
    ahead: int
    claims: list[Claim]


def lane_refs() -> list[tuple[str, str]]:
    rows = []
    output = git(
        "for-each-ref", "--format=%(refname:short)%00%(objectname)",
        "refs/heads/lane/",
    )
    for line in output.splitlines():
        branch, head = line.split("\0", 1)
        rows.append((branch, head))
    return rows


def unique_commits(branch: str, base: str) -> list[tuple[str, str, str]]:
    raw = git(
        "log", "--no-merges", "--format=%H%x00%cI%x00%s%x1e",
        branch, "--not", base,
    )
    rows = []
    for record in raw.split("\x1e"):
        record = record.strip("\n")
        if not record:
            continue
        commit, committed_at, subject = record.split("\0", 2)
        rows.append((commit, committed_at, subject.strip()))
    return rows


def collect(base: str, symbol_filter: str | None) -> list[Lane]:
    git("rev-parse", "--verify", base)
    lanes = []
    for branch, head in lane_refs():
        commits = unique_commits(branch, base)
        if not commits:
            continue
        claims = []
        for commit, committed_at, subject in commits:
            match = MATCH_RE.match(subject)
            if not match:
                continue
            symbol = match.group(1)
            if symbol_filter and symbol != symbol_filter:
                continue
            base_fallback = has_global_asm(base, symbol)
            lane_fallback = has_global_asm(branch, symbol)
            if base_fallback and not lane_fallback:
                state = "pending"
            elif not base_fallback:
                state = "already-in-base"
            else:
                state = "claim-only"
            claims.append(Claim(
                symbol=symbol,
                commit=commit,
                committed_at=committed_at,
                subject=subject,
                state=state,
                source_paths=source_paths(commit),
            ))
        if symbol_filter and not claims:
            continue
        lanes.append(Lane(
            branch=branch,
            head=head,
            ahead=len(commits),
            claims=claims,
        ))
    return sorted(lanes, key=lambda lane: lane.branch)


def print_text(base: str, lanes: list[Lane], pending_only: bool) -> None:
    print(f"base: {base}")
    for lane in lanes:
        claims = [c for c in lane.claims if not pending_only or c.state == "pending"]
        if pending_only and not claims:
            continue
        print(f"{lane.branch} {lane.head[:12]} ahead={lane.ahead}")
        for claim in claims:
            paths = ",".join(claim.source_paths) or "-"
            print(
                f"  {claim.state:15s} {claim.symbol:42s} "
                f"{claim.commit[:12]} {paths}"
            )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--base", default="campaign/unchain")
    parser.add_argument("--symbol", help="Show claims for one exact symbol")
    parser.add_argument("--pending-only", action="store_true")
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()

    try:
        lanes = collect(args.base, args.symbol)
    except RuntimeError as error:
        print(f"lane_status: {error}", file=sys.stderr)
        return 2

    if args.pending_only:
        lanes = [
            Lane(
                branch=lane.branch,
                head=lane.head,
                ahead=lane.ahead,
                claims=[claim for claim in lane.claims if claim.state == "pending"],
            )
            for lane in lanes
        ]
        lanes = [lane for lane in lanes if lane.claims]

    if args.json:
        print(json.dumps({
            "base": args.base,
            "lanes": [asdict(lane) for lane in lanes],
        }, indent=2, sort_keys=True))
    else:
        print_text(args.base, lanes, args.pending_only)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
