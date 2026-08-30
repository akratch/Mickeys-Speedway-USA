#!/usr/bin/env python3
"""Report unintegrated lane commits without reading sibling worktrees.

The report is a coordination hint. A commit message such as ``Match foo`` is
shown as a *claim* until the integration lane repeats the project's exact
object, relocation, linked-range, and ROM checks (ADR 0011).
"""

from __future__ import annotations

import argparse
from functools import lru_cache
import json
import re
import subprocess
import sys
from dataclasses import asdict, dataclass
from pathlib import PurePosixPath

import finalize_plateau


MATCH_RE = re.compile(r"^match(?:ed)?\s+([A-Za-z_][A-Za-z0-9_]*)\b", re.I)
REMOVED_GLOBAL_ASM_RE = re.compile(
    r'^-\s*#pragma\s+GLOBAL_ASM\("(?P<path>[^"]+)"\)', re.MULTILINE,
)
DISPOSITIONS_PATH = "config/lane-claim-dispositions.us.json"
LEGACY_TRIAGE_PATH = "docs/matching-triage.md"
SYMBOL_TOKEN_TEMPLATE = r"(?<![A-Za-z0-9_]){symbol}(?![A-Za-z0-9_])"
FUNCTION_DEFINITION_TEMPLATE = (
    r"(?<![A-Za-z0-9_]){symbol}\s*\([^;{{}}]*\)\s*\{{"
)
PLATEAU_BLOCK_RE = re.compile(
    r"/\*\s*PLATEAU-HANDOFF(?::[A-Za-z_][A-Za-z0-9_]*:start)?\b"
    r".*?\*/",
    re.DOTALL,
)
PLATEAU_SUBJECT_RE = re.compile(
    r"\b(?:plateaus?|park(?:ed|ing)?)\b", re.I
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


@lru_cache(maxsize=2048)
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


@lru_cache(maxsize=2048)
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


def blob_ids(refs: list[str], path: str) -> dict[str, str | None]:
    """Resolve one path for many refs with one Git object-database process."""
    if not refs:
        return {}
    queries = "".join(f"{ref}:{path}\n" for ref in refs)
    result = subprocess.run(
        ["git", "cat-file", "--batch-check=%(objectname) %(objecttype)"],
        input=queries,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    if result.returncode != 0:
        raise RuntimeError(f"git cat-file batch failed: {result.stderr.strip()}")
    lines = result.stdout.splitlines()
    if len(lines) != len(refs):
        raise RuntimeError("git cat-file batch returned the wrong row count")
    resolved: dict[str, str | None] = {}
    for ref, line in zip(refs, lines, strict=True):
        if line.endswith(" missing"):
            resolved[ref] = None
            continue
        fields = line.split()
        if len(fields) != 2 or fields[1] != "blob":
            raise RuntimeError(f"{ref}:{path} did not resolve to one blob")
        resolved[ref] = fields[0]
    return resolved


def blob_contents(
    refs: list[str], path: str,
) -> dict[str, tuple[str, str] | None]:
    """Read one source path from many committed refs in one Git process."""
    if not refs:
        return {}
    queries = "".join(f"{ref}:{path}\n" for ref in refs).encode()
    result = subprocess.run(
        ["git", "cat-file", "--batch"], input=queries,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE,
    )
    if result.returncode != 0:
        detail = result.stderr.decode(errors="replace").strip()
        raise RuntimeError(f"git cat-file batch failed: {detail}")
    data = result.stdout
    cursor = 0
    resolved: dict[str, tuple[str, str] | None] = {}
    for ref in refs:
        newline = data.find(b"\n", cursor)
        if newline < 0:
            raise RuntimeError("git cat-file batch omitted an object header")
        header = data[cursor:newline].decode("ascii", errors="replace")
        cursor = newline + 1
        if header.endswith(" missing"):
            resolved[ref] = None
            continue
        fields = header.split()
        if len(fields) != 3 or fields[1] != "blob":
            raise RuntimeError(f"{ref}:{path} did not resolve to one blob")
        try:
            size = int(fields[2], 10)
        except ValueError as error:
            raise RuntimeError("git cat-file returned an invalid blob size") from error
        end = cursor + size
        if end >= len(data) or data[end:end + 1] != b"\n":
            raise RuntimeError("git cat-file batch returned a truncated blob")
        try:
            text = data[cursor:end].decode("utf-8")
        except UnicodeDecodeError as error:
            raise RuntimeError(f"{ref}:{path} is not UTF-8 source") from error
        resolved[ref] = (fields[0], text)
        cursor = end + 1
    if cursor != len(data):
        raise RuntimeError("git cat-file batch returned unexpected trailing data")
    return resolved


def exact_symbol_pattern(symbol: str) -> re.Pattern[str]:
    return re.compile(
        SYMBOL_TOKEN_TEMPLATE.format(symbol=re.escape(symbol)), re.MULTILINE,
    )


def exact_symbol_rows(text: str | None, symbol: str) -> list[str]:
    if text is None:
        return []
    token = exact_symbol_pattern(symbol)
    return [line for line in text.splitlines() if token.search(line)]


def legacy_evidence_signature(
    text: str | None, symbol: str,
) -> tuple[tuple[str, ...], tuple[str, ...]]:
    """Select this symbol's rows and generated blocks from the shared ledger."""
    if text is None:
        return (), ()
    marker = re.escape(f"plateau-handoff:{symbol}")
    pattern = re.compile(
        rf"<!-- {marker}:start -->.*?<!-- {marker}:end -->\n?",
        re.DOTALL,
    )
    return tuple(exact_symbol_rows(text, symbol)), tuple(pattern.findall(text))


def malformed_legacy_marker(text: str | None, symbol: str) -> bool:
    """Reject a target generated block whose paired markers are damaged."""
    if text is None:
        return False
    start = f"<!-- plateau-handoff:{symbol}:start -->"
    end = f"<!-- plateau-handoff:{symbol}:end -->"
    if start not in text and end not in text:
        return False
    _rows, blocks = legacy_evidence_signature(text, symbol)
    return text.count(start) != 1 or text.count(end) != 1 or len(blocks) != 1


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
    return bool(plateau_handoff_signature(text, symbol))


def plateau_handoff_signature(text: str, symbol: str) -> tuple[str, ...]:
    """Return only this symbol's inline handoff blocks, byte for byte."""
    token = exact_symbol_pattern(symbol)
    found = []
    for block in PLATEAU_BLOCK_RE.findall(text):
        if re.search(rf"\bsymbol:\s*{re.escape(symbol)}\s*(?:\n|$)", block):
            found.append(block)
        elif block.startswith(f"/* PLATEAU-HANDOFF:{symbol}:start") and token.search(block):
            found.append(block)
    return tuple(found)


def shard_path(symbol: str) -> str:
    return finalize_plateau.handoff_shard_path(symbol)


def validated_shard_source(text: str | None, symbol: str) -> str | None:
    if text is None:
        return None
    try:
        return finalize_plateau.handoff_shard_source(text, symbol)
    except finalize_plateau.PlateauError as error:
        raise RuntimeError(str(error)) from error


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


def path_plateau_record(ref: str, path: str) -> tuple[str, str] | None:
    """Return the latest plateau commit for a single-candidate source path."""
    raw = git("log", "--format=%H%x00%s%x1e", ref, "--", path)
    for record in raw.split("\x1e"):
        record = record.strip("\n")
        if not record:
            continue
        commit, subject = record.split("\0", 1)
        if PLATEAU_SUBJECT_RE.search(subject):
            return commit, subject
    return None


def ledger_source_plateau_record(
    ref: str, symbol: str, source_path: str,
) -> tuple[str, str] | None:
    """Find exact-symbol plateau evidence committed with a mixed-TU source.

    Older consolidated translation units often used a shortened generated
    name (or only an overlay number) in the commit subject.  The exact symbol
    still appears in the triage row changed by that commit.  Requiring the
    same commit to change both that exact row and the owning source path keeps
    this fallback unambiguous without treating every path-wide plateau as
    evidence for every guarded function in the file.
    """
    raw = git(
        "log", "--format=%H%x00%s%x1e", ref, "--", LEGACY_TRIAGE_PATH,
    )
    token = exact_symbol_pattern(symbol)
    for record in raw.split("\x1e"):
        record = record.strip("\n")
        if not record:
            continue
        commit, subject = record.split("\0", 1)
        if not PLATEAU_SUBJECT_RE.search(subject):
            continue
        changed = set(git(
            "diff-tree", "--no-commit-id", "--name-only", "-r", commit,
            "--", source_path, LEGACY_TRIAGE_PATH,
        ).splitlines())
        if source_path not in changed or LEGACY_TRIAGE_PATH not in changed:
            continue
        triage = show_file(commit, LEGACY_TRIAGE_PATH)
        if triage is None:
            continue
        parent = first_parent(commit)
        previous = show_file(parent, LEGACY_TRIAGE_PATH) if parent else None
        if legacy_evidence_signature(triage, symbol) == legacy_evidence_signature(
            previous, symbol,
        ):
            continue
        rows = (line for line in triage.splitlines() if token.search(line))
        if any(PLATEAU_SUBJECT_RE.search(line) for line in rows):
            return commit, subject
    return None


def shard_source_plateau_record(
    ref: str, symbol: str, source_path: str,
) -> tuple[str, str] | None:
    """Find a plateau commit that changed one source and its exact shard."""
    ledger_path = shard_path(symbol)
    raw = git("log", "--format=%H%x00%s%x1e", ref, "--", ledger_path)
    for record in raw.split("\x1e"):
        record = record.strip("\n")
        if not record:
            continue
        commit, subject = record.split("\0", 1)
        if not PLATEAU_SUBJECT_RE.search(subject):
            continue
        changed = set(git(
            "diff-tree", "--no-commit-id", "--name-only", "-r", commit,
            "--", source_path, ledger_path,
        ).splitlines())
        if source_path not in changed or ledger_path not in changed:
            continue
        shard = show_file(commit, ledger_path)
        if shard is None:
            continue
        try:
            recorded_source = finalize_plateau.handoff_shard_source(shard, symbol)
        except finalize_plateau.PlateauError:
            continue
        if recorded_source == source_path:
            return commit, subject
    return None


def newest_related_commit(commits: list[str]) -> str | None:
    """Choose the descendant when exact evidence exists in both ledgers."""
    if not commits:
        return None
    selected = commits[0]
    for commit in commits[1:]:
        if is_ancestor(selected, commit):
            selected = commit
    return selected


def first_parent(commit: str) -> str | None:
    fields = git("rev-list", "--parents", "-n", "1", commit).split()
    return fields[1] if len(fields) > 1 else None


def latest_legacy_evidence_commit(ref: str, symbol: str) -> str | None:
    """Find the newest commit owning a current target row or block line."""
    text = show_file(ref, LEGACY_TRIAGE_PATH)
    if text is None:
        return None
    token = exact_symbol_pattern(symbol)
    start = f"<!-- plateau-handoff:{symbol}:start -->"
    end = f"<!-- plateau-handoff:{symbol}:end -->"
    selected_lines: set[int] = set()
    inside = False
    for index, line in enumerate(text.splitlines(), 1):
        if line == start:
            inside = True
        if inside or token.search(line):
            selected_lines.add(index)
        if line == end:
            inside = False
    if not selected_lines:
        return None

    blame = git(
        "blame", "--line-porcelain", ref, "--", LEGACY_TRIAGE_PATH,
    )
    commits: list[str] = []
    current_commit: str | None = None
    current_line = 0
    for line in blame.splitlines():
        header = re.fullmatch(r"\^?([0-9a-f]{40}) \d+ \d+(?: \d+)?", line)
        if header:
            current_commit = header.group(1)
        elif line.startswith("\t"):
            current_line += 1
            if current_line in selected_lines and current_commit is not None:
                commits.append(current_commit)
    if current_line != len(text.splitlines()):
        raise RuntimeError("git blame returned the wrong legacy-ledger line count")
    return newest_related_commit(list(dict.fromkeys(commits)))


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


def lane_refs(
    *, containing: str | None = None, unmerged_into: str | None = None
) -> list[tuple[str, str]]:
    rows = []
    args = ["for-each-ref", "--format=%(refname:short)%00%(objectname)"]
    if containing:
        args.append(f"--contains={containing}")
    if unmerged_into:
        args.append(f"--no-merged={unmerged_into}")
    args.append("refs/heads/lane/")
    output = git(*args)
    for line in output.splitlines():
        branch, head = line.split("\0", 1)
        rows.append((branch, head))
    return rows


def active_lanes_for_source(
    base: str, symbol: str, base_path: str, base_blob: str,
    base_source_commit: str, base_text: str | None = None,
) -> list[str]:
    """Return lanes with committed work on this exact guarded candidate.

    A translation unit may contain many independently assignable NON_MATCHING
    functions.  Comparing only the whole-file blob made an old edit to any one
    function reserve every other function in that file.  Compare the validated
    target guard instead, while still failing closed when a lane removes or
    damages the guard, or adds a target handoff.
    """
    active = []
    refs = lane_refs(
        containing=base_source_commit, unmerged_into=base
    )
    branches = [branch for branch, _head in refs]
    objects = blob_contents(branches, base_path)
    legacy_objects = blob_contents(branches, LEGACY_TRIAGE_PATH)
    target_shard_path = shard_path(symbol)
    shard_objects = blob_contents(branches, target_shard_path)
    if base_text is None:
        base_text = show_file(base, base_path)
    base_legacy_evidence = legacy_evidence_signature(
        show_file(base, LEGACY_TRIAGE_PATH), symbol,
    )
    base_shard = show_file(base, target_shard_path)
    if all(
        objects[branch] is not None
        and objects[branch][0] == base_blob
        and legacy_evidence_signature(
            legacy_objects[branch][1]
            if legacy_objects[branch] is not None else None,
            symbol,
        ) == base_legacy_evidence
        and (
            shard_objects[branch][1]
            if shard_objects[branch] is not None else None
        ) == base_shard
        for branch in branches
    ):
        return []
    try:
        base_candidate = finalize_plateau.require_guarded_candidate(
            base_text or "", symbol
        )
    except finalize_plateau.PlateauError:
        base_candidate = None
    base_lines = (base_text or "").splitlines(keepends=True)
    base_region = (
        "".join(base_lines[base_candidate.ifdef_line:base_candidate.endif_line + 1])
        if base_candidate is not None else None
    )
    base_handoff = plateau_handoff_signature(base_text or "", symbol)
    for branch, _head in refs:
        # The contains filter already excludes old refs that predate the
        # current target source. Most retained lane refs have not changed this
        # path at all; compare its blob before paying for a full symbol/path
        # resolution on the small differing remainder.
        lane_object = objects[branch]
        lane_legacy_object = legacy_objects[branch]
        lane_shard_object = shard_objects[branch]
        lane_legacy = lane_legacy_object[1] if lane_legacy_object is not None else None
        lane_shard = lane_shard_object[1] if lane_shard_object is not None else None
        if legacy_evidence_signature(lane_legacy, symbol) != base_legacy_evidence:
            active.append(branch)
            continue
        if lane_shard != base_shard:
            active.append(branch)
            continue
        if lane_object is None:
            active.append(branch)
            continue
        lane_blob, lane_text = lane_object
        if lane_blob == base_blob:
            continue
        if base_region is None:
            active.append(branch)
            continue
        try:
            lane_candidate = finalize_plateau.require_guarded_candidate(
                lane_text, symbol
            )
        except finalize_plateau.PlateauError:
            active.append(branch)
            continue
        lane_lines = lane_text.splitlines(keepends=True)
        lane_region = "".join(
            lane_lines[
                lane_candidate.ifdef_line:lane_candidate.endif_line + 1
            ]
        )
        if lane_region != base_region:
            active.append(branch)
            continue
        if plateau_handoff_signature(lane_text, symbol) != base_handoff:
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
        base, symbol, path, current_blob, base_source_commit, text,
    )
    if active:
        return Assignment(
            symbol, "active", path, None, None, active,
            "an unintegrated lane has a different committed target guard or handoff",
        )

    if not guarded_fallback(text, symbol):
        return Assignment(
            symbol, "already-integrated/exhausted", path, None, None, [],
            "base has a committed definition without this target's fallback",
        )

    source_record = target_history_record(
        base, symbol, [path], require_plateau=False,
    )
    named_plateau_record = (
        source_record
        if source_record and PLATEAU_SUBJECT_RE.search(source_record[1])
        else None
    )
    # Historical standalone candidate files often predate the structured EOF
    # handoff marker and use friendly names only in commit subjects. A plateau
    # or park commit on a path containing exactly one NON_MATCHING guard is
    # still unambiguous target evidence; treating it as fresh work caused the
    # ready queue to reassign already exhausted o57/o79/o22 routes.
    guard_count = len(re.findall(r"#\s*ifdef\s+NON_MATCHING\b", text))
    path_record = (
        path_plateau_record(base, path)
        if guard_count == 1
        else None
    )
    triage_text = show_file(base, LEGACY_TRIAGE_PATH)
    if malformed_legacy_marker(triage_text, symbol):
        return Assignment(
            symbol, "stale-ledger", path, None,
            latest_legacy_evidence_commit(base, symbol), [],
            "malformed target-specific handoff block in the legacy triage ledger",
        )
    ledger_rows = exact_symbol_rows(triage_text, symbol)
    legacy_has_symbol = bool(ledger_rows)
    legacy_marks_plateau = any(
        PLATEAU_SUBJECT_RE.search(line) for line in ledger_rows
    )
    legacy_source_record = (
        ledger_source_plateau_record(base, symbol, path)
        if guard_count > 1 and legacy_marks_plateau
        else None
    )
    target_shard_path = shard_path(symbol)
    shard_text = show_file(base, target_shard_path)
    try:
        shard_source = validated_shard_source(shard_text, symbol)
    except RuntimeError as error:
        return Assignment(
            symbol, "stale-ledger", path, None,
            latest_path_commit(base, target_shard_path), [], str(error),
        )
    if shard_source is not None and shard_source != path:
        return Assignment(
            symbol, "stale-ledger", path, None,
            latest_path_commit(base, target_shard_path), [],
            f"symbol handoff shard records source {shard_source}, expected {path}",
        )
    shard_record = (
        shard_source_plateau_record(base, symbol, path)
        if shard_source is not None
        else None
    )
    if (
        not has_plateau_handoff(text, symbol)
        and named_plateau_record is None
        and path_record is None
        and legacy_source_record is None
        and shard_source is None
    ):
        return Assignment(
            symbol, "base-only", path, None, None, [],
            "base retains the fallback and has no committed plateau handoff",
        )

    source_commit = (
        named_plateau_record[0]
        if named_plateau_record is not None
        else path_record[0]
        if path_record is not None
        else legacy_source_record[0]
        if legacy_source_record is not None
        else shard_record[0]
        if shard_record is not None
        else None
    )
    ledger_commits = []
    if shard_source is not None:
        shard_commit = latest_path_commit(base, target_shard_path)
        if shard_commit is not None:
            ledger_commits.append(shard_commit)
    if legacy_has_symbol:
        legacy_commit = latest_legacy_evidence_commit(base, symbol)
        if legacy_commit is not None:
            ledger_commits.append(legacy_commit)
    current_ledger_commits = (
        [commit for commit in ledger_commits if is_ancestor(source_commit, commit)]
        if source_commit is not None
        else []
    )
    ledger_commit = newest_related_commit(
        current_ledger_commits or ledger_commits
    )
    if source_commit is None:
        return Assignment(
            symbol, "stale-ledger", path, None, ledger_commit, [],
            "source has a plateau handoff but no target-named plateau commit",
        )
    if ledger_commit is None:
        return Assignment(
            symbol, "stale-ledger", path, source_commit, None, [],
            "source plateau is missing exact-symbol handoff ledger evidence",
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
    for branch, head in lane_refs(unmerged_into=base):
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
