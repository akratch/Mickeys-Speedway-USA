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
from pathlib import PurePosixPath


MATCH_RE = re.compile(r"^match(?:ed)?\s+([A-Za-z_][A-Za-z0-9_]*)\b", re.I)
REMOVED_GLOBAL_ASM_RE = re.compile(
    r'^-\s*#pragma\s+GLOBAL_ASM\("(?P<path>[^"]+)"\)', re.MULTILINE,
)
DISPOSITIONS_PATH = "config/lane-claim-dispositions.us.json"
TRIAGE_PATH = "docs/matching-triage.md"
SYMBOL_TOKEN_TEMPLATE = r"(?<![A-Za-z0-9_]){symbol}(?![A-Za-z0-9_])"
FUNCTION_DEFINITION_TEMPLATE = (
    r"(?<![A-Za-z0-9_]){symbol}\s*\([^;{{}}]*\)\s*\{{"
)
PLATEAU_BLOCK_RE = re.compile(
    r"/\*\s*PLATEAU-HANDOFF(?::[A-Za-z_][A-Za-z0-9_]*:start)?\b"
    r".*?\*/",
    re.DOTALL,
)


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


def has_text(ref: str, text: str) -> bool:
    result = subprocess.run(
        ["git", "grep", "-q", "-F", text, ref, "--", "src"],
        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
    )
    if result.returncode not in (0, 1):
        raise RuntimeError(f"git grep failed for {ref}:{text}")
    return result.returncode == 0


def show_file(ref: str, path: str) -> str | None:
    result = subprocess.run(
        ["git", "show", f"{ref}:{path}"], text=True,
        stdout=subprocess.PIPE, stderr=subprocess.DEVNULL,
    )
    if result.returncode == 0:
        return result.stdout
    if result.returncode == 128:
        return None
    raise RuntimeError(f"git show failed for {ref}:{path}")


def blob_id(ref: str, path: str) -> str | None:
    result = subprocess.run(
        ["git", "rev-parse", "--verify", f"{ref}:{path}"], text=True,
        stdout=subprocess.PIPE, stderr=subprocess.DEVNULL,
    )
    if result.returncode == 0:
        return result.stdout.strip()
    if result.returncode == 128:
        return None
    raise RuntimeError(f"git rev-parse failed for {ref}:{path}")


def exact_symbol_pattern(symbol: str) -> re.Pattern[str]:
    return re.compile(
        SYMBOL_TOKEN_TEMPLATE.format(symbol=re.escape(symbol)), re.MULTILINE,
    )


def source_identity(ref: str, symbol: str) -> tuple[str | None, str | None]:
    """Return the one committed definition path, or a fail-closed reason."""
    output = git(
        "grep", "-l", "-w", "-e", symbol, ref, "--", "src", check=False,
    )
    definition = re.compile(
        FUNCTION_DEFINITION_TEMPLATE.format(symbol=re.escape(symbol)),
        re.DOTALL,
    )
    paths = []
    for row in output.splitlines():
        path = row.split(":", 1)[1] if ":" in row else row
        text = show_file(ref, path)
        if text is not None and definition.search(text):
            paths.append(path)
    paths = sorted(set(paths))
    if len(paths) == 1:
        return paths[0], None
    if not paths:
        return None, f"no exact committed source definition for {symbol}"
    return None, "ambiguous exact source definitions: " + ", ".join(paths)


def guarded_fallback(text: str, symbol: str) -> bool:
    """Recognise an exact-symbol NON_MATCHING body and its one fallback."""
    definition = FUNCTION_DEFINITION_TEMPLATE.format(symbol=re.escape(symbol))
    guard = re.compile(
        rf"#\s*ifdef\s+NON_MATCHING\b(?P<body>.*?)"
        rf"#\s*else\b(?P<fallback>.*?)#\s*endif\b",
        re.DOTALL,
    )
    for match in guard.finditer(text):
        if not re.search(definition, match.group("body"), re.DOTALL):
            continue
        fallbacks = re.findall(
            r'#\s*pragma\s+GLOBAL_ASM\s*\(\s*"([^"]+)"\s*\)',
            match.group("fallback"),
        )
        if len(fallbacks) != 1:
            return False
        name = PurePosixPath(fallbacks[0]).name
        return name == f"{symbol}.s" or bool(re.fullmatch(
            r"func_overlay_[0-9]{3}_F[0-9A-Fa-f]{7}_[0-9A-Fa-f]+\.s",
            name,
        ))
    return False


def has_plateau_handoff(text: str, symbol: str) -> bool:
    token = exact_symbol_pattern(symbol)
    for block in PLATEAU_BLOCK_RE.findall(text):
        if re.search(rf"\bsymbol:\s*{re.escape(symbol)}\s*(?:\n|$)", block):
            return True
        if block.startswith(f"/* PLATEAU-HANDOFF:{symbol}:start") and token.search(block):
            return True
    return False


def target_history_record(
    ref: str, symbol: str, paths: list[str], *, require_plateau: bool,
) -> tuple[str, str] | None:
    if not paths:
        return None
    raw = git(
        "log", "--format=%H%x00%s%x1e", ref, "--", *paths,
    )
    token = exact_symbol_pattern(symbol)
    for record in raw.split("\x1e"):
        record = record.strip("\n")
        if not record:
            continue
        commit, subject = record.split("\0", 1)
        if not token.search(subject):
            continue
        if require_plateau and "plateau" not in subject.lower():
            continue
        return commit, subject
    return None


def target_history_commit(
    ref: str, symbol: str, paths: list[str], *, require_plateau: bool,
) -> str | None:
    record = target_history_record(
        ref, symbol, paths, require_plateau=require_plateau,
    )
    return record[0] if record else None


def latest_path_commit(ref: str, path: str, *, exclude: str | None = None) -> str | None:
    args = ["log", "-1", "--format=%H", ref]
    if exclude:
        args.extend(("--not", exclude))
    args.extend(("--", path))
    value = git(*args).strip()
    return value or None


def is_ancestor(older: str, newer: str) -> bool:
    result = subprocess.run(
        ["git", "merge-base", "--is-ancestor", older, newer],
        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
    )
    if result.returncode not in (0, 1):
        raise RuntimeError(f"cannot compare commits {older} and {newer}")
    return result.returncode == 0


def removed_global_asms(commit: str) -> list[str]:
    patch = git(
        "show", "--format=", "--no-ext-diff", "--unified=0", commit,
        "--", "src",
    )
    return sorted(set(REMOVED_GLOBAL_ASM_RE.findall(patch)))


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
    reason: str | None = None
    decision_commit: str | None = None


@dataclass
class Lane:
    branch: str
    head: str
    ahead: int
    claims: list[Claim]


@dataclass
class Assignment:
    symbol: str
    state: str
    source_path: str | None
    source_commit: str | None
    ledger_commit: str | None
    active_lanes: list[str]
    reason: str


def lane_refs(*, containing: str | None = None) -> list[tuple[str, str]]:
    rows = []
    args = ["for-each-ref", "--format=%(refname:short)%00%(objectname)"]
    if containing:
        args.append(f"--contains={containing}")
    args.append("refs/heads/lane/")
    output = git(*args)
    for line in output.splitlines():
        branch, head = line.split("\0", 1)
        rows.append((branch, head))
    return rows


def active_lanes_for_source(
    base: str, symbol: str, base_path: str, base_blob: str,
    base_source_commit: str,
) -> list[str]:
    active = []
    for branch, _head in lane_refs(containing=base_source_commit):
        # A writable lane starts from the committed target version it owns.
        # Old divergent refs that predate a later canonical target change are
        # history, not active ownership, even when their whole-file blob is
        # different from today's base.
        if latest_path_commit(branch, base_path, exclude=base) is None:
            continue
        lane_path, lane_error = source_identity(branch, symbol)
        if lane_error or lane_path != base_path:
            active.append(branch)
            continue
        if blob_id(branch, lane_path) != base_blob:
            active.append(branch)
    return sorted(active)


def assignment_status(base: str, symbol: str) -> Assignment:
    """Classify whether one exact target is safe to assign.

    Only ``base-only`` is assignable. Every other state is deliberately
    fail-closed so stale evidence cannot become duplicate matching work.
    """
    path, identity_error = source_identity(base, symbol)
    if identity_error or path is None:
        return Assignment(
            symbol, "stale-ledger", path, None, None, [],
            identity_error or "source identity is unavailable",
        )
    text = show_file(base, path)
    current_blob = blob_id(base, path)
    if text is None or current_blob is None:
        return Assignment(
            symbol, "stale-ledger", path, None, None, [],
            "exact source path is absent from the base object",
        )

    base_source_commit = target_history_commit(
        base, symbol, [path], require_plateau=False,
    ) or latest_path_commit(base, path)
    if base_source_commit is None:
        return Assignment(
            symbol, "stale-ledger", path, None, None, [],
            "exact source path has no committed history",
        )
    active = active_lanes_for_source(
        base, symbol, path, current_blob, base_source_commit,
    )
    if active:
        return Assignment(
            symbol, "active", path, None, None, active,
            "an unintegrated lane has a different committed source blob",
        )

    if not guarded_fallback(text, symbol):
        return Assignment(
            symbol, "already-integrated/exhausted", path, None, None, [],
            "base has a committed definition without this target's fallback",
        )

    source_record = target_history_record(
        base, symbol, [path], require_plateau=False,
    )
    committed_plateau = bool(
        source_record and "plateau" in source_record[1].lower()
    )
    if not has_plateau_handoff(text, symbol) and not committed_plateau:
        return Assignment(
            symbol, "base-only", path, None, None, [],
            "base retains the fallback and has no committed plateau handoff",
        )

    source_commit = target_history_commit(
        base, symbol, [path], require_plateau=True,
    )
    triage_text = show_file(base, TRIAGE_PATH)
    ledger_has_symbol = bool(
        triage_text is not None and exact_symbol_pattern(symbol).search(triage_text)
    )
    ledger_commit = target_history_commit(
        base, symbol, [TRIAGE_PATH], require_plateau=False,
    ) if ledger_has_symbol else None
    if source_commit is None:
        return Assignment(
            symbol, "stale-ledger", path, None, ledger_commit, [],
            "source has a plateau handoff but no target-named plateau commit",
        )
    if ledger_commit is None:
        return Assignment(
            symbol, "stale-ledger", path, source_commit, None, [],
            f"source plateau is missing exact-symbol evidence in {TRIAGE_PATH}",
        )
    if not is_ancestor(source_commit, ledger_commit):
        return Assignment(
            symbol, "stale-ledger", path, source_commit, ledger_commit, [],
            "triage evidence predates the committed source plateau",
        )
    return Assignment(
        symbol, "already-integrated/exhausted", path, source_commit,
        ledger_commit, [], "source plateau and triage evidence are current",
    )


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


def claim_dispositions(base: str) -> dict[str, dict[str, str]]:
    raw = git("show", f"{base}:{DISPOSITIONS_PATH}", check=False)
    if not raw:
        return {}
    try:
        document = json.loads(raw)
    except json.JSONDecodeError as error:
        raise RuntimeError(f"{base}:{DISPOSITIONS_PATH}: {error}") from error
    if document.get("schema_version") != 1:
        raise RuntimeError(f"{base}:{DISPOSITIONS_PATH}: unsupported schema")
    claims = document.get("claims")
    if not isinstance(claims, dict):
        raise RuntimeError(f"{base}:{DISPOSITIONS_PATH}: claims must be an object")
    for commit, row in claims.items():
        if not re.fullmatch(r"[0-9a-f]{40}", commit):
            raise RuntimeError(
                f"{base}:{DISPOSITIONS_PATH}: invalid commit {commit!r}"
            )
        if not isinstance(row, dict):
            raise RuntimeError(
                f"{base}:{DISPOSITIONS_PATH}: {commit} must be an object"
            )
        if row.get("state") not in {"rejected", "superseded"}:
            raise RuntimeError(
                f"{base}:{DISPOSITIONS_PATH}: {commit} has invalid state"
            )
        if not isinstance(row.get("symbol"), str) or not row["symbol"]:
            raise RuntimeError(
                f"{base}:{DISPOSITIONS_PATH}: {commit} needs a symbol"
            )
        if not isinstance(row.get("reason"), str) or not row["reason"]:
            raise RuntimeError(
                f"{base}:{DISPOSITIONS_PATH}: {commit} needs a reason"
            )
        decision_commit = row.get("decision_commit")
        if not isinstance(decision_commit, str) or not re.fullmatch(
            r"[0-9a-f]{40}", decision_commit
        ):
            raise RuntimeError(
                f"{base}:{DISPOSITIONS_PATH}: {commit} needs a full "
                "decision_commit"
            )
        decision_is_ancestor = subprocess.run(
            ["git", "merge-base", "--is-ancestor", decision_commit, base],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
        )
        if decision_is_ancestor.returncode != 0:
            raise RuntimeError(
                f"{base}:{DISPOSITIONS_PATH}: decision {decision_commit} "
                f"is not an ancestor of {base}"
            )
    return claims


def collect(base: str, symbol_filter: str | None) -> list[Lane]:
    git("rev-parse", "--verify", base)
    dispositions = claim_dispositions(base)
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
            reason = None
            decision_commit = None
            disposition = dispositions.get(commit)
            if disposition:
                if disposition["symbol"] != symbol:
                    raise RuntimeError(
                        f"{base}:{DISPOSITIONS_PATH}: {commit} names "
                        f"{disposition['symbol']}, subject names {symbol}"
                    )
                state = disposition["state"]
                reason = disposition["reason"]
                decision_commit = disposition["decision_commit"]
            else:
                base_fallback = has_global_asm(base, symbol)
                lane_fallback = has_global_asm(branch, symbol)
                # Friendly C names and splat's generated fallback names can
                # differ. A match commit records the authoritative association
                # by deleting the exact GLOBAL_ASM path, so use that path when
                # the friendly-name probe cannot see the fallback.
                fallback_paths = removed_global_asms(commit)
                if not base_fallback and fallback_paths:
                    base_fallback = any(
                        has_text(base, f'GLOBAL_ASM("{path}")')
                        for path in fallback_paths
                    )
                    lane_fallback = any(
                        has_text(branch, f'GLOBAL_ASM("{path}")')
                        for path in fallback_paths
                    )
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
                reason=reason,
                decision_commit=decision_commit,
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


def print_assignment(assignment: Assignment) -> None:
    details = []
    if assignment.source_path:
        details.append(f"source={assignment.source_path}")
    if assignment.source_commit:
        details.append(f"source-commit={assignment.source_commit[:12]}")
    if assignment.ledger_commit:
        details.append(f"ledger-commit={assignment.ledger_commit[:12]}")
    if assignment.active_lanes:
        details.append("lanes=" + ",".join(assignment.active_lanes))
    suffix = " " + " ".join(details) if details else ""
    print(
        f"assignment: {assignment.state} {assignment.symbol}{suffix}\n"
        f"  reason: {assignment.reason}"
    )


def print_text(
    base: str, lanes: list[Lane], pending_only: bool,
    assignment: Assignment | None,
) -> None:
    print(f"base: {base}")
    if assignment:
        print_assignment(assignment)
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
            if claim.reason:
                print(
                    f"    decision: {claim.decision_commit[:12]} "
                    f"reason: {claim.reason}"
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
        assignment = assignment_status(args.base, args.symbol) if args.symbol else None
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
            "assignment": asdict(assignment) if assignment else None,
            "lanes": [asdict(lane) for lane in lanes],
        }, indent=2, sort_keys=True))
    else:
        print_text(args.base, lanes, args.pending_only, assignment)
    if assignment and assignment.state != "base-only":
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
