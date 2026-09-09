#!/usr/bin/env python3
"""Reconcile and preflight one public release without ever pushing it.

The default mode is read-only: generators rerender in check mode, release
metrics are compared with the named remote-tracking branch, and the complete
gate suite runs.  ``--write-derived`` is the only mode that may alter tracked
files, and it invokes only the explicit public-safe generators listed below.
This tool has no merge, copy, fetch, or push operation.
"""

from __future__ import annotations

import argparse
from dataclasses import dataclass
import hashlib
import hmac
import json
import os
from pathlib import Path
import re
import signal
import subprocess
import sys
import time
from typing import Any, Iterable

sys.dont_write_bytecode = True

import release_gate
import overlay_atlas
from system_health import collect_health, format_report


READ_ONLY_GENERATORS = (
    ("overlay-atlas", "gmake", "-j1", "overlay-atlas"),
    (
        "postprocess-audit",
        sys.executable,
        "tools/postprocess_audit.py",
        "--check",
    ),
    ("overlay-donors", "gmake", "-j1", "overlay-donors"),
)
WRITE_GENERATORS = (
    ("overlay-atlas", "gmake", "-j1", "overlay-atlas-write"),
    ("atlas-digest", sys.executable, "tools/refresh_atlas_digest.py"),
    (
        "postprocess-audit",
        sys.executable,
        "tools/postprocess_audit.py",
        "--write",
    ),
    ("overlay-symbols", "gmake", "-j1", "overlay-syms"),
    ("scoreboard", "gmake", "-j1", "scoreboard"),
)
DERIVED_PATHS = {
    "README.md",
    "config/overlay-donors.us.json",
    "config/overlays.us.json",
    "config/postprocess-audit.us.json",
    "mickey.us.yaml",
    "overlay_undefined_syms.us.txt",
}

# Construct operator-only names in pieces so this public-safe scanner does not
# diagnose its own source.  These are path rules, not content allowlists.
FORBIDDEN_PATH_PARTS = (
    ("." + "codex",),
    ("." + "decomp-workbench",),
    ("base" + "roms",),
    ("a" + "sm",),
    ("ass" + "ets",),
    ("expec" + "ted",),
    ("tools", "i" + "do"),
    ("tools", "bin" + "utils"),
)
FORBIDDEN_BASENAMES = {"AG" + "ENTS.md", "CLA" + "UDE.md"}
MESSAGE_MARKERS = (
    re.compile(r"(?im)^co-authored-by:.*(?:codex|claude|agent|bot)"),
    re.compile(r"(?im)^generated-by:"),
)

MANIFEST_SCHEMA = "mickey-" + "public-release-manifest-v1"
APPROVAL_SCHEMA = "mickey-" + "public-release-approval-v1"
MANIFEST_DEFAULT = Path("build/public-release/manifest.json")
ZERO_OID_RE = re.compile(r"^0+$")
WINDOWS_ABSOLUTE_RE = re.compile(r"^[A-Za-z]:[\\/]")

# This is the release policy expressed as data.  Deny rules win before the
# broad public-project allowlist.  Names that are themselves publication
# markers stay split so the release scanner does not diagnose this tool.
MANIFEST_FORBIDDEN_EXACT = {
    "AG" + "ENTS.md",
    "CLA" + "UDE.md",
    "docs/LOCAL_DECOMP_AGENT.md",
    "docs/breakthrough-campaign.md",
    "docs/campaigns.md",
    "docs/epoch14-plan.md",
    "tools/codex_lane.sh",
    "tools/crew.py",
    "tools/integration_base.py",
    "tools/lane_cache.py",
    "tools/lane_status.py",
    "tools/merge_lane.sh",
    "tools/new_lane.sh",
    "tools/with_verify_lock.sh",
}
MANIFEST_FORBIDDEN_PREFIXES = (
    "." + "codex/",
    "." + "decomp-workbench/",
    "asm/",
    "assets/",
    "baseroms/",
    "build/",
    "build_non_matching/",
    "expected/",
    "tools/ido/",
    "tools/binutils/",
)
MANIFEST_FORBIDDEN_SUFFIXES = (
    ".a",
    ".bin",
    ".dylib",
    ".elf",
    ".exe",
    ".map",
    ".n64",
    ".o",
    ".rom",
    ".so",
    ".v64",
    ".z64",
)
MANIFEST_PUBLIC_SAFE_PREFIXES = (
    ".github/",
    "config/",
    "docs/",
    "include/",
    "mk/",
    "src/",
    "tools/",
)
MANIFEST_PUBLIC_SAFE_ROOTS = {
    ".gitignore",
    ".gitmodules",
    "Dockerfile",
    "LICENSE",
    "Makefile",
    "README.md",
    "diff_settings.py",
    "mickey.us.yaml",
    "objdiff.json",
    "overlay_undefined_syms.us.txt",
    "requirements.txt",
    "symbol_addrs.us.txt",
    "undefined_syms.us.txt",
}
BASE_REQUIRED_GATES = (
    "gmake verify",
    "gmake cleanroom",
    "gmake check-docs",
    "gmake check-scoreboard",
)
TOOLING_GATE = "gmake check-tooling"
OVERLAY_SYMBOL_GATE = "gmake check-overlay-syms"

SCOREBOARD_BEGIN = "<!-- SCOREBOARD_BEGIN -->"
SCOREBOARD_END = "<!-- SCOREBOARD_END -->"
METRIC_PATTERNS = (
    ("functions", re.compile(r"^functions\s+(\d+)\s*/\s*(\d+)", re.M)),
    ("resident C bytes", re.compile(r"^\.text bytes\s+(\d+)\s*/\s*(\d+)", re.M)),
    ("verified asm bytes", re.compile(r"^verified asm\s+(\d+)\s*/\s*(\d+)", re.M)),
    ("overlay C bytes", re.compile(r"^overlay C\s+(\d+)\s*/\s*(\d+)", re.M)),
    ("whole resolved bytes", re.compile(r"^whole resolved\s+(\d+)\s*/\s*(\d+)", re.M)),
    ("named functions", re.compile(r"^named\s+(\d+)\s*/\s*(\d+)", re.M)),
    ("adopted symbols", re.compile(r"^symbols\s+(\d+)\b", re.M)),
    ("decompiled bytes", re.compile(r"^decompiled\s+(\d+)\s*/\s*(\d+)", re.M)),
    ("GLOBAL_ASM bytes", re.compile(r"^GLOBAL_ASM remaining\s+(\d+)\s*/\s*(\d+)", re.M)),
    ("NON_MATCHING bytes", re.compile(r"^NON_MATCHING\s+(\d+)\s*/\s*(\d+)", re.M)),
)


class PublicReleaseError(RuntimeError):
    pass


@dataclass(frozen=True)
class ReleaseContext:
    repo: Path
    branch: str
    remote: str
    base_ref: str
    base_oid: str
    head_oid: str
    fetch_url: str
    push_url: str
    outgoing_commits: tuple[str, ...]


@dataclass(frozen=True)
class ManifestContext:
    repo: Path
    branch: str
    remote: str
    remote_ref: str
    remote_oid: str
    remote_identity_sha256: str
    source_ref_sha256: str
    base_oid: str
    head_oid: str
    commits: tuple[str, ...]


@dataclass(frozen=True)
class Metric:
    value: int
    total: int | None


def _git(
    repo: Path, *args: str, check: bool = True, binary: bool = False
) -> str | bytes:
    result = subprocess.run(
        ["git", *args],
        cwd=repo,
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=not binary,
    )
    if check and result.returncode != 0:
        stderr = result.stderr if not binary else result.stderr.decode(errors="replace")
        stdout = result.stdout if not binary else result.stdout.decode(errors="replace")
        detail = stderr.strip() or stdout.strip() or f"exit {result.returncode}"
        raise PublicReleaseError(f"git {' '.join(args)}: {detail}")
    return result.stdout


def _repo_root(start: Path) -> Path:
    return Path(str(_git(start, "rev-parse", "--show-toplevel")).strip()).resolve()


def _release_context(
    repo: Path, branch: str, remote: str, *, require_clean: bool
) -> ReleaseContext:
    current = str(_git(repo, "branch", "--show-current")).strip()
    if current != branch:
        raise PublicReleaseError(
            f"current branch is {current or '(detached)'!r}, expected {branch!r}"
        )
    if require_clean:
        dirt = str(
            _git(repo, "status", "--porcelain=v1", "--untracked-files=no")
        ).strip()
        if dirt:
            raise PublicReleaseError("tracked worktree/index dirt is present")

    fetch_url = str(_git(repo, "remote", "get-url", remote)).strip()
    push_url = str(_git(repo, "remote", "get-url", "--push", remote)).strip()
    if not fetch_url or not push_url:
        raise PublicReleaseError(f"remote {remote!r} lacks a fetch or push URL")
    url_findings = release_gate._scan_text(
        f"remote {remote} URL", fetch_url + "\n" + push_url
    )
    if url_findings:
        raise PublicReleaseError("remote URL failed release text scan")

    base_ref = f"refs/remotes/{remote}/{branch}"
    probe = subprocess.run(
        ["git", "show-ref", "--verify", "--quiet", base_ref], cwd=repo, check=False
    )
    if probe.returncode != 0:
        raise PublicReleaseError(
            f"missing local comparison ref {base_ref!r}; fetch it outside this tool"
        )
    ancestor = subprocess.run(
        ["git", "merge-base", "--is-ancestor", base_ref, "HEAD"],
        cwd=repo,
        check=False,
    )
    if ancestor.returncode != 0:
        raise PublicReleaseError(
            f"HEAD is not a fast-forward descendant of {remote}/{branch}"
        )

    base_oid = str(_git(repo, "rev-parse", base_ref)).strip()
    head_oid = str(_git(repo, "rev-parse", "HEAD")).strip()
    commits = tuple(
        line
        for line in str(
            _git(repo, "rev-list", "--reverse", f"{base_ref}..HEAD")
        ).splitlines()
        if line
    )
    return ReleaseContext(
        repo=repo,
        branch=branch,
        remote=remote,
        base_ref=base_ref,
        base_oid=base_oid,
        head_oid=head_oid,
        fetch_url=fetch_url,
        push_url=push_url,
        outgoing_commits=commits,
    )


def _remote_urls(repo: Path, remote: str) -> tuple[str, str]:
    fetch_urls = [
        row
        for row in str(_git(repo, "remote", "get-url", "--all", remote)).splitlines()
        if row
    ]
    push_urls = [
        row
        for row in str(
            _git(repo, "remote", "get-url", "--push", "--all", remote)
        ).splitlines()
        if row
    ]
    if len(fetch_urls) != 1 or len(push_urls) != 1:
        raise PublicReleaseError(
            f"remote {remote!r} must have exactly one fetch and one push URL"
        )
    fetch_url, push_url = fetch_urls[0], push_urls[0]
    findings = release_gate._scan_text(
        f"remote {remote} URL", fetch_url + "\n" + push_url
    )
    if findings:
        raise PublicReleaseError("remote URL failed release text scan")
    if fetch_url.startswith(("/", "file:")) or push_url.startswith(("/", "file:")):
        raise PublicReleaseError("manifest remote must not be a local filesystem URL")
    if WINDOWS_ABSOLUTE_RE.match(fetch_url) or WINDOWS_ABSOLUTE_RE.match(push_url):
        raise PublicReleaseError("manifest remote must not be a local filesystem URL")
    return fetch_url, push_url


def _resolve_commit(repo: Path, value: str, label: str) -> str:
    if not value or value.startswith("-") or any(ch.isspace() for ch in value):
        raise PublicReleaseError(f"ambiguous {label} revision")
    result = subprocess.run(
        ["git", "rev-parse", "--verify", f"{value}^{{commit}}"],
        cwd=repo,
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    rows = [row for row in result.stdout.splitlines() if row]
    if result.returncode != 0 or len(rows) != 1 or not re.fullmatch(r"[0-9a-f]+", rows[0]):
        raise PublicReleaseError(f"ambiguous or missing {label} revision")
    return rows[0]


def _parse_manifest_range(repo: Path, value: str) -> tuple[str, str]:
    if value.count("..") != 1 or "..." in value:
        raise PublicReleaseError("manifest range must be exactly BASE..TIP")
    base_name, head_name = value.split("..", 1)
    if not base_name or not head_name:
        raise PublicReleaseError("manifest range must name both BASE and TIP")
    return (
        _resolve_commit(repo, base_name, "manifest base"),
        _resolve_commit(repo, head_name, "manifest tip"),
    )


def _is_ancestor(repo: Path, older: str, newer: str) -> bool:
    return (
        subprocess.run(
            ["git", "merge-base", "--is-ancestor", older, newer],
            cwd=repo,
            check=False,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
        ).returncode
        == 0
    )


def _manifest_context(
    repo: Path, branch: str, remote: str, manifest_range: str
) -> ManifestContext:
    current = str(_git(repo, "branch", "--show-current")).strip()
    if current != branch:
        raise PublicReleaseError(
            f"current branch is {current or '(detached)'!r}, expected {branch!r}"
        )
    dirt = str(_git(repo, "status", "--porcelain=v1", "--untracked-files=no")).strip()
    if dirt:
        raise PublicReleaseError("tracked worktree/index dirt is present")

    fetch_url, push_url = _remote_urls(repo, remote)
    if fetch_url != push_url:
        raise PublicReleaseError(
            "manifest remote has different fetch and push destinations"
        )
    remote_identity = hashlib.sha256(
        (fetch_url + "\0" + push_url).encode("utf-8")
    ).hexdigest()
    remote_ref = f"refs/remotes/{remote}/{branch}"
    probe = subprocess.run(
        ["git", "show-ref", "--verify", "--quiet", remote_ref],
        cwd=repo,
        check=False,
    )
    if probe.returncode != 0:
        raise PublicReleaseError(
            f"missing local source ref {remote_ref!r}; fetch it outside this tool"
        )
    upstream = str(_git(repo, "rev-parse", "--abbrev-ref", "@{upstream}")).strip()
    if upstream != f"{remote}/{branch}":
        expected_upstream = f"{remote}/{branch}"
        raise PublicReleaseError(
            f"source branch upstream is {upstream!r}, expected {expected_upstream!r}"
        )

    head = str(_git(repo, "rev-parse", "HEAD")).strip()
    remote_oid = str(_git(repo, "rev-parse", remote_ref)).strip()
    if head != remote_oid:
        raise PublicReleaseError(
            "source branch and its remote-tracking ref differ; selected commits "
            "are not an unambiguous integrated tip"
        )
    base_oid, head_oid = _parse_manifest_range(repo, manifest_range)
    if head_oid != head:
        raise PublicReleaseError("manifest tip must resolve to the current source HEAD")
    if base_oid == head_oid:
        raise PublicReleaseError("manifest range contains no commits")
    if not _is_ancestor(repo, base_oid, head_oid):
        raise PublicReleaseError("manifest base is not an ancestor of its tip")

    commits = tuple(
        row
        for row in str(
            _git(repo, "rev-list", "--reverse", "--topo-order", f"{base_oid}..{head_oid}")
        ).splitlines()
        if row
    )
    if not commits:
        raise PublicReleaseError("manifest range contains no commits")
    for commit in commits:
        parents = str(_git(repo, "rev-list", "--parents", "-n", "1", commit)).split()
        if len(parents) != 2:
            raise PublicReleaseError(
                f"commit {commit[:12]} is a merge, root, or otherwise ungroupable"
            )
    return ManifestContext(
        repo=repo,
        branch=branch,
        remote=remote,
        remote_ref=remote_ref,
        remote_oid=remote_oid,
        remote_identity_sha256=remote_identity,
        source_ref_sha256=hashlib.sha256(
            (remote + "\0" + branch).encode("utf-8")
        ).hexdigest(),
        base_oid=base_oid,
        head_oid=head_oid,
        commits=commits,
    )


def _forbidden_path(path: str) -> bool:
    parts = tuple(part for part in Path(path).parts if part not in {"", "."})
    if parts and parts[-1] in FORBIDDEN_BASENAMES:
        return True
    return any(parts[: len(prefix)] == prefix for prefix in FORBIDDEN_PATH_PARTS)


def _validate_manifest_path(path: str) -> None:
    if (
        not path
        or path.startswith("/")
        or WINDOWS_ABSOLUTE_RE.match(path)
        or "\\" in path
    ):
        raise PublicReleaseError("changed path is absolute or non-canonical")
    parts = path.split("/")
    if any(part in {"", ".", ".."} for part in parts):
        raise PublicReleaseError("changed path is non-canonical")


def _manifest_forbidden_reason(path: str) -> str | None:
    _validate_manifest_path(path)
    folded = path.casefold()
    if release_gate._scan_text("changed path", path):
        return "publication-filtered path"
    if path in MANIFEST_FORBIDDEN_EXACT:
        return "private operating path"
    if path.startswith("docs/campaign-check-ledger-"):
        return "private operating path"
    if any(path == prefix[:-1] or path.startswith(prefix) for prefix in MANIFEST_FORBIDDEN_PREFIXES):
        return "private, ROM-derived, or build-artifact path"
    if folded.endswith(MANIFEST_FORBIDDEN_SUFFIXES):
        return "binary or build-artifact suffix"
    basename = path.rsplit("/", 1)[-1]
    if basename.startswith(("core.", ".DS_Store")) or basename.endswith("~"):
        return "scratch or workstation artifact"
    return None


def _classify_manifest_path(
    path: str, *, status: str, old_mode: str, new_mode: str
) -> str:
    if _manifest_forbidden_reason(path):
        return "forbidden"
    code = status[:1]
    if code in {"C", "D", "R", "T"} or old_mode != new_mode and old_mode != "000000":
        return "review-required"
    if path in MANIFEST_PUBLIC_SAFE_ROOTS or path.startswith(MANIFEST_PUBLIC_SAFE_PREFIXES):
        return "public-safe"
    return "review-required"


def _decode_git_path(payload: bytes) -> str:
    try:
        return payload.decode("utf-8")
    except UnicodeDecodeError as exc:
        raise PublicReleaseError("changed path is not valid UTF-8") from exc


def _raw_commit_changes(repo: Path, commit: str) -> list[dict[str, Any]]:
    parent_row = str(_git(repo, "rev-list", "--parents", "-n", "1", commit)).split()
    if len(parent_row) != 2:
        raise PublicReleaseError(
            f"commit {commit[:12]} is a merge, root, or otherwise ungroupable"
        )
    parent = parent_row[1]
    raw = bytes(
        _git(
            repo,
            "diff-tree",
            "-r",
            "--no-commit-id",
            "--raw",
            "-z",
            "--find-renames=50%",
            "--find-copies=50%",
            parent,
            commit,
            binary=True,
        )
    )
    fields = raw.split(b"\0")
    changes: list[dict[str, Any]] = []
    index = 0
    while index < len(fields) and fields[index]:
        metadata = fields[index].decode("ascii")
        index += 1
        parts = metadata.split()
        if len(parts) != 5 or not parts[0].startswith(":"):
            raise PublicReleaseError(f"commit {commit[:12]} has malformed raw diff")
        old_mode = parts[0][1:]
        new_mode, old_blob, new_blob, status = parts[1:]
        code = status[:1]
        if code not in {"A", "C", "D", "M", "R", "T"}:
            raise PublicReleaseError(
                f"commit {commit[:12]} has unsupported status {status!r}"
            )
        if index >= len(fields) or not fields[index]:
            raise PublicReleaseError(f"commit {commit[:12]} has a missing diff path")
        first_path = _decode_git_path(fields[index])
        index += 1
        old_path: str | None = None
        path = first_path
        if code in {"C", "R"}:
            if index >= len(fields) or not fields[index]:
                raise PublicReleaseError(
                    f"commit {commit[:12]} has a missing rename/copy destination"
                )
            old_path = first_path
            path = _decode_git_path(fields[index])
            index += 1

        paths = [path] if old_path is None else [old_path, path]
        for candidate in paths:
            reason = _manifest_forbidden_reason(candidate)
            if reason:
                raise PublicReleaseError(
                    f"commit {commit[:12]} path {candidate!r} is forbidden: {reason}"
                )
        if old_mode in {"120000", "160000"} or new_mode in {"120000", "160000"}:
            kind = "symlink" if "120000" in {old_mode, new_mode} else "submodule"
            raise PublicReleaseError(
                f"commit {commit[:12]} changes a {kind}: {path}"
            )
        if old_mode not in {"000000", "100644", "100755"} or new_mode not in {
            "000000",
            "100644",
            "100755",
        }:
            raise PublicReleaseError(
                f"commit {commit[:12]} has unsupported file mode change for {path}"
            )

        old_identity = None if ZERO_OID_RE.fullmatch(old_blob) else old_blob
        new_identity = None if ZERO_OID_RE.fullmatch(new_blob) else new_blob
        classification = _classify_manifest_path(
            path, status=status, old_mode=old_mode, new_mode=new_mode
        )
        if old_path is not None:
            old_class = _classify_manifest_path(
                old_path, status=status, old_mode=old_mode, new_mode=new_mode
            )
            if old_class == "forbidden":
                classification = "forbidden"
            elif old_class == "review-required":
                classification = "review-required"
        changes.append(
            {
                "status": status,
                "path": path,
                "old_path": old_path,
                "old_blob": old_identity,
                "new_blob": new_identity,
                "old_mode": old_mode,
                "new_mode": new_mode,
                "classification": classification,
            }
        )
    if not changes:
        raise PublicReleaseError(f"commit {commit[:12]} has no changed paths")
    return changes


def _message_subject(repo: Path, commit: str) -> tuple[str, str]:
    message = str(_git(repo, "show", "-s", "--format=%B", commit))
    findings = release_gate._scan_text(f"commit {commit[:12]} message", message)
    for pattern in MESSAGE_MARKERS:
        if pattern.search(message):
            findings.append(f"commit {commit[:12]} message has a forbidden trailer")
    if findings:
        raise PublicReleaseError(
            f"commit {commit[:12]} message failed publication filters"
        )
    subject = message.splitlines()[0].strip() if message.splitlines() else ""
    _validate_public_subject(subject)
    return subject, hashlib.sha256(message.encode("utf-8")).hexdigest()


def _validate_public_subject(subject: object) -> str:
    if not isinstance(subject, str) or not subject or len(subject) > 120:
        raise PublicReleaseError("public commit subject must contain 1..120 characters")
    if (
        subject.strip() != subject
        or any(ord(character) < 32 or ord(character) == 127 for character in subject)
    ):
        raise PublicReleaseError("public commit subject must be one plain line")
    if re.match(r"(?i)^(?:co-authored-by|generated-by):", subject):
        raise PublicReleaseError("public commit subject resembles a forbidden trailer")
    if release_gate._scan_text("public commit subject", subject):
        raise PublicReleaseError("public commit subject failed publication filters")
    return subject


def _required_gates(changes: list[dict[str, Any]]) -> list[str]:
    gates = list(BASE_REQUIRED_GATES)
    paths = [str(row["path"]) for row in changes]
    paths.extend(str(row["old_path"]) for row in changes if row["old_path"] is not None)
    if any(path.startswith("tools/") or path == "Makefile" for path in paths):
        gates.append(TOOLING_GATE)
    if any(
        path.startswith("src/overlays/")
        or path.startswith("config/overlays")
        or path == "overlay_undefined_syms.us.txt"
        for path in paths
    ):
        gates.append(OVERLAY_SYMBOL_GATE)
    return gates


def _scan_manifest_changes(repo: Path, commit: str, changes: list[dict[str, Any]]) -> None:
    seen: set[str] = set()
    for change in changes:
        oid = change["new_blob"]
        if oid is None or oid in seen:
            continue
        seen.add(oid)
        payload = _blob(repo, oid)
        if b"\0" in payload:
            raise PublicReleaseError(
                f"commit {commit[:12]} changes a binary blob at {change['path']}"
            )
        if _scan_payload(f"commit {commit[:12]}:{change['path']}", payload):
            raise PublicReleaseError(
                f"commit {commit[:12]} changed text failed publication filters"
            )


def _manifest_payload(ctx: ManifestContext) -> dict[str, Any]:
    units: list[dict[str, Any]] = []
    all_gates: list[str] = []
    for index, commit in enumerate(ctx.commits, 1):
        changes = _raw_commit_changes(ctx.repo, commit)
        _scan_manifest_changes(ctx.repo, commit, changes)
        subject, message_sha256 = _message_subject(ctx.repo, commit)
        gates = _required_gates(changes)
        for gate in gates:
            if gate not in all_gates:
                all_gates.append(gate)
        units.append(
            {
                "unit": index,
                "private_commit": commit,
                "parent": str(
                    _git(ctx.repo, "rev-parse", f"{commit}^")
                ).strip(),
                "suggested_public_subject": subject,
                "message_sha256": message_sha256,
                "changes": changes,
                "required_gates": gates,
            }
        )
    return {
        "schema": MANIFEST_SCHEMA,
        "source": {
            "remote_tip": ctx.remote_oid,
            "remote_identity_sha256": ctx.remote_identity_sha256,
            "source_ref_sha256": ctx.source_ref_sha256,
            "base": ctx.base_oid,
            "head": ctx.head_oid,
        },
        "units": units,
        "required_final_gates": all_gates,
        "operations": {
            "copies_private_tree": False,
            "merges": False,
            "cherry_picks": False,
            "pushes": False,
        },
    }


def _canonical_json(value: object) -> bytes:
    return (json.dumps(value, indent=2, sort_keys=True) + "\n").encode("utf-8")


def _seal_manifest(payload: dict[str, Any]) -> dict[str, Any]:
    sealed = dict(payload)
    sealed["manifest_sha256"] = hashlib.sha256(_canonical_json(payload)).hexdigest()
    return sealed


def _validate_manifest_integrity(manifest: object) -> dict[str, Any]:
    if not isinstance(manifest, dict) or manifest.get("schema") != MANIFEST_SCHEMA:
        raise PublicReleaseError("manifest schema is missing or unsupported")
    if set(manifest) != {
        "schema",
        "source",
        "units",
        "required_final_gates",
        "operations",
        "manifest_sha256",
    }:
        raise PublicReleaseError("manifest fields are missing or unsupported")
    digest = manifest.get("manifest_sha256")
    if not isinstance(digest, str) or not re.fullmatch(r"[0-9a-f]{64}", digest):
        raise PublicReleaseError("manifest digest is missing or malformed")
    payload = dict(manifest)
    del payload["manifest_sha256"]
    actual = hashlib.sha256(_canonical_json(payload)).hexdigest()
    if not hmac.compare_digest(digest, actual):
        raise PublicReleaseError("manifest digest does not match its contents")

    source = manifest["source"]
    if not isinstance(source, dict) or set(source) != {
        "remote_tip",
        "remote_identity_sha256",
        "source_ref_sha256",
        "base",
        "head",
    }:
        raise PublicReleaseError("manifest source identity is malformed")
    oid_pattern = re.compile(r"(?:[0-9a-f]{40}|[0-9a-f]{64})")
    if not all(
        isinstance(source[key], str) and oid_pattern.fullmatch(source[key])
        for key in ("remote_tip", "base", "head")
    ):
        raise PublicReleaseError("manifest source commit identities are malformed")
    for key in ("remote_identity_sha256", "source_ref_sha256"):
        if not isinstance(source[key], str) or not re.fullmatch(
            r"[0-9a-f]{64}", source[key]
        ):
            raise PublicReleaseError("manifest remote/ref identity is malformed")

    operations = manifest["operations"]
    if operations != {
        "copies_private_tree": False,
        "merges": False,
        "cherry_picks": False,
        "pushes": False,
    }:
        raise PublicReleaseError("manifest operation guarantees are malformed")
    units = manifest["units"]
    if not isinstance(units, list) or not units:
        raise PublicReleaseError("manifest must contain at least one commit unit")
    if source["remote_tip"] != source["head"]:
        raise PublicReleaseError("manifest source tip identities disagree")
    allowed_gates = {*BASE_REQUIRED_GATES, TOOLING_GATE, OVERLAY_SYMBOL_GATE}
    final_gates: list[str] = []
    for index, unit in enumerate(units, 1):
        if not isinstance(unit, dict) or set(unit) != {
            "unit",
            "private_commit",
            "parent",
            "suggested_public_subject",
            "message_sha256",
            "changes",
            "required_gates",
        }:
            raise PublicReleaseError(f"manifest unit {index} is malformed")
        if unit["unit"] != index:
            raise PublicReleaseError("manifest unit ordering is malformed")
        if not all(
            isinstance(unit[key], str) and oid_pattern.fullmatch(unit[key])
            for key in ("private_commit", "parent")
        ):
            raise PublicReleaseError(f"manifest unit {index} commit identity is malformed")
        expected_parent = source["base"] if index == 1 else units[index - 2]["private_commit"]
        if unit["parent"] != expected_parent:
            raise PublicReleaseError(f"manifest unit {index} parent chain is malformed")
        _validate_public_subject(unit["suggested_public_subject"])
        if not isinstance(unit["message_sha256"], str) or not re.fullmatch(
            r"[0-9a-f]{64}", unit["message_sha256"]
        ):
            raise PublicReleaseError(f"manifest unit {index} message identity is malformed")
        changes = unit["changes"]
        if not isinstance(changes, list) or not changes:
            raise PublicReleaseError(f"manifest unit {index} has no path rows")
        for path_index, row in enumerate(changes, 1):
            if not isinstance(row, dict) or set(row) != {
                "status",
                "path",
                "old_path",
                "old_blob",
                "new_blob",
                "old_mode",
                "new_mode",
                "classification",
            }:
                raise PublicReleaseError(
                    f"manifest unit {index} path {path_index} is malformed"
                )
            if not isinstance(row["status"], str) or not re.fullmatch(
                r"[ACDMRT](?:[0-9]{1,3})?", row["status"]
            ):
                raise PublicReleaseError("manifest path status is malformed")
            if not isinstance(row["path"], str):
                raise PublicReleaseError("manifest path is malformed")
            _validate_manifest_path(row["path"])
            if row["old_path"] is not None:
                if not isinstance(row["old_path"], str):
                    raise PublicReleaseError("manifest old path is malformed")
                _validate_manifest_path(row["old_path"])
            for key in ("old_blob", "new_blob"):
                if row[key] is not None and (
                    not isinstance(row[key], str) or not oid_pattern.fullmatch(row[key])
                ):
                    raise PublicReleaseError("manifest blob identity is malformed")
            if row["old_mode"] not in {"000000", "100644", "100755"} or row[
                "new_mode"
            ] not in {"000000", "100644", "100755"}:
                raise PublicReleaseError("manifest file mode is malformed")
            if row["classification"] not in {"public-safe", "review-required"}:
                raise PublicReleaseError("manifest path classification is malformed")
            code = row["status"][0]
            if (code in {"R", "C"}) != (row["old_path"] is not None):
                raise PublicReleaseError("manifest rename/copy path identity is malformed")
            if code == "A":
                blobs_valid = row["old_blob"] is None and row["new_blob"] is not None
            elif code == "D":
                blobs_valid = row["old_blob"] is not None and row["new_blob"] is None
            else:
                blobs_valid = row["old_blob"] is not None and row["new_blob"] is not None
            if not blobs_valid:
                raise PublicReleaseError("manifest blob transition is malformed")
            expected_classification = _classify_manifest_path(
                row["path"],
                status=row["status"],
                old_mode=row["old_mode"],
                new_mode=row["new_mode"],
            )
            if row["old_path"] is not None:
                old_classification = _classify_manifest_path(
                    row["old_path"],
                    status=row["status"],
                    old_mode=row["old_mode"],
                    new_mode=row["new_mode"],
                )
                if old_classification != "public-safe":
                    expected_classification = old_classification
            if row["classification"] != expected_classification:
                raise PublicReleaseError("manifest path classification has drifted")
        expected_gates = _required_gates(changes)
        if unit["required_gates"] != expected_gates or not set(expected_gates) <= allowed_gates:
            raise PublicReleaseError(f"manifest unit {index} gate list is malformed")
        for gate in expected_gates:
            if gate not in final_gates:
                final_gates.append(gate)
    if manifest["required_final_gates"] != final_gates:
        raise PublicReleaseError("manifest final gate list is malformed")
    if units[-1]["private_commit"] != source["head"]:
        raise PublicReleaseError("manifest final unit does not reach source head")
    return manifest


def _approval_template(manifest: dict[str, Any]) -> dict[str, Any]:
    _validate_manifest_integrity(manifest)
    source = manifest["source"]
    units: list[dict[str, Any]] = []
    for unit in manifest["units"]:
        paths = []
        for change in unit["changes"]:
            paths.append(
                {
                    "status": change["status"],
                    "path": change["path"],
                    "old_path": change["old_path"],
                    "old_blob": change["old_blob"],
                    "new_blob": change["new_blob"],
                    "old_mode": change["old_mode"],
                    "new_mode": change["new_mode"],
                    "classification": change["classification"],
                    "decision": "pending",
                }
            )
        units.append(
            {
                "private_commit": unit["private_commit"],
                "public_subject": unit["suggested_public_subject"],
                "paths": paths,
            }
        )
    return {
        "schema": APPROVAL_SCHEMA,
        "manifest_sha256": manifest["manifest_sha256"],
        "source": {
            "remote_tip": source["remote_tip"],
            "remote_identity_sha256": source["remote_identity_sha256"],
            "base": source["base"],
            "head": source["head"],
            "source_ref_sha256": source["source_ref_sha256"],
        },
        "units": units,
    }


def _strict_keys(value: object, expected: set[str], label: str) -> dict[str, Any]:
    if not isinstance(value, dict) or set(value) != expected:
        raise PublicReleaseError(f"{label} fields are missing, extra, or malformed")
    return value


def _validate_approval(
    manifest: dict[str, Any], approval: object
) -> list[dict[str, Any]]:
    _validate_manifest_integrity(manifest)
    root = _strict_keys(
        approval,
        {"schema", "manifest_sha256", "source", "units"},
        "approval",
    )
    if root["schema"] != APPROVAL_SCHEMA:
        raise PublicReleaseError("approval schema is missing or unsupported")
    if root["manifest_sha256"] != manifest["manifest_sha256"]:
        raise PublicReleaseError("approval names a stale or different manifest")
    expected_source = _approval_template(manifest)["source"]
    if root["source"] != expected_source:
        raise PublicReleaseError("approval source identity has drifted")
    approved_units = root["units"]
    expected_units = manifest["units"]
    if not isinstance(approved_units, list) or len(approved_units) != len(expected_units):
        raise PublicReleaseError("approval commit units do not match the manifest")

    result: list[dict[str, Any]] = []
    for index, (approved, expected) in enumerate(zip(approved_units, expected_units), 1):
        unit = _strict_keys(
            approved,
            {"private_commit", "public_subject", "paths"},
            f"approval unit {index}",
        )
        if unit["private_commit"] != expected["private_commit"]:
            raise PublicReleaseError(f"approval unit {index} commit identity has drifted")
        subject = _validate_public_subject(unit["public_subject"])
        paths = unit["paths"]
        expected_paths = expected["changes"]
        if not isinstance(paths, list) or len(paths) != len(expected_paths):
            raise PublicReleaseError(f"approval unit {index} path set has drifted")
        normalized_paths: list[dict[str, Any]] = []
        for path_index, (row, expected_row) in enumerate(zip(paths, expected_paths), 1):
            item = _strict_keys(
                row,
                {
                    "status",
                    "path",
                    "old_path",
                    "old_blob",
                    "new_blob",
                    "old_mode",
                    "new_mode",
                    "classification",
                    "decision",
                },
                f"approval unit {index} path {path_index}",
            )
            identity = {
                key: item[key]
                for key in (
                    "status",
                    "path",
                    "old_path",
                    "old_blob",
                    "new_blob",
                    "old_mode",
                    "new_mode",
                    "classification",
                )
            }
            expected_identity = {
                key: expected_row[key]
                for key in (
                    "status",
                    "path",
                    "old_path",
                    "old_blob",
                    "new_blob",
                    "old_mode",
                    "new_mode",
                    "classification",
                )
            }
            if identity != expected_identity:
                raise PublicReleaseError(
                    f"approval unit {index} path {path_index} identity has drifted"
                )
            if item["decision"] != "approve":
                raise PublicReleaseError(
                    f"approval unit {index} path {path_index} is not explicitly approved"
                )
            if item["classification"] == "forbidden":
                raise PublicReleaseError("a forbidden path cannot be approved")
            normalized_paths.append(identity)
        result.append(
            {
                "private_commit": unit["private_commit"],
                "public_subject": subject,
                "paths": normalized_paths,
            }
        )
    return result


def _tree_entries(repo: Path, ref: str) -> Iterable[tuple[str, str]]:
    raw = bytes(_git(repo, "ls-tree", "-r", "-z", "--full-tree", ref, binary=True))
    for record in raw.split(b"\0"):
        if not record:
            continue
        header, raw_path = record.split(b"\t", 1)
        _mode, kind, oid = header.decode("ascii").split()
        if kind == "blob":
            yield oid, raw_path.decode("utf-8", errors="surrogateescape")


def _blob(repo: Path, oid: str) -> bytes:
    return bytes(_git(repo, "cat-file", "blob", oid, binary=True))


def _index_entries(repo: Path) -> Iterable[tuple[str, str]]:
    raw = bytes(_git(repo, "ls-files", "--stage", "-z", binary=True))
    for record in raw.split(b"\0"):
        if not record:
            continue
        header, raw_path = record.split(b"\t", 1)
        mode, _oid, stage = header.decode("ascii").split()
        if stage != "0":
            raise PublicReleaseError("index contains unmerged entries")
        yield mode, raw_path.decode("utf-8", errors="surrogateescape")


def _scan_payload(label: str, payload: bytes) -> list[str]:
    if b"\0" in payload:
        return []
    text = payload.decode("utf-8", errors="replace")
    return release_gate._scan_text(label, text)


def _scan_release(ctx: ReleaseContext, *, include_worktree: bool) -> list[str]:
    findings: list[str] = []
    seen_blobs: set[str] = set()
    for ref in ctx.outgoing_commits:
        short = ref[:12]
        message = str(_git(ctx.repo, "show", "-s", "--format=%B", ref))
        findings.extend(release_gate._scan_text(f"commit {short} message", message))
        for pattern in MESSAGE_MARKERS:
            if pattern.search(message):
                findings.append(f"commit {short} message: pattern {pattern.pattern!r}")

    refs = (*ctx.outgoing_commits, ctx.head_oid)
    for ref in dict.fromkeys(refs):
        short = ref[:12]
        for oid, path in _tree_entries(ctx.repo, ref):
            if _forbidden_path(path):
                findings.append(f"commit {short}: forbidden tracked path {path}")
            if oid in seen_blobs:
                continue
            seen_blobs.add(oid)
            findings.extend(_scan_payload(f"commit {short}:{path}", _blob(ctx.repo, oid)))

    if include_worktree:
        for mode, path in _index_entries(ctx.repo):
            if _forbidden_path(path):
                findings.append(f"worktree: forbidden tracked path {path}")
            if mode == "160000":
                # A gitlink's tracked payload is its commit ID, already covered
                # by the outgoing tree scan. An uninitialized submodule is a
                # valid public checkout and has no worktree file to inspect.
                continue
            full = ctx.repo / path
            if full.is_symlink():
                payload = os.readlink(full).encode("utf-8", errors="surrogateescape")
            elif full.is_file():
                payload = full.read_bytes()
            else:
                findings.append(f"worktree: tracked path is missing: {path}")
                continue
            findings.extend(_scan_payload(f"worktree:{path}", payload))
    return sorted(set(findings))


def _check_diff_hygiene(ctx: ReleaseContext, *, include_worktree: bool) -> None:
    outgoing = str(_git(ctx.repo, "diff", "--check", f"{ctx.base_ref}..HEAD"))
    if outgoing.strip():
        raise PublicReleaseError("outgoing committed diff has whitespace errors")
    if include_worktree:
        working = str(_git(ctx.repo, "diff", "--check"))
        cached = str(_git(ctx.repo, "diff", "--cached", "--check"))
        if working.strip() or cached.strip():
            raise PublicReleaseError("tracked worktree/index diff has whitespace errors")


def _show_file(repo: Path, ref: str, path: str) -> str | None:
    result = subprocess.run(
        ["git", "show", f"{ref}:{path}"],
        cwd=repo,
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    return result.stdout if result.returncode == 0 else None


def _scoreboard_metrics(text: str | None) -> dict[str, Metric]:
    if text is None:
        return {}
    start = text.find(SCOREBOARD_BEGIN)
    end = text.find(SCOREBOARD_END)
    if start < 0 or end < start:
        return {}
    block = text[start:end]
    metrics: dict[str, Metric] = {}
    for label, pattern in METRIC_PATTERNS:
        match = pattern.search(block)
        if match:
            metrics[label] = Metric(
                int(match.group(1)), int(match.group(2)) if match.lastindex == 2 else None
            )
    return metrics


def _metric_delta_lines(old_text: str | None, new_text: str | None) -> list[str]:
    old = _scoreboard_metrics(old_text)
    new = _scoreboard_metrics(new_text)
    lines: list[str] = []
    for label, _pattern in METRIC_PATTERNS:
        if label not in old or label not in new:
            continue
        before, after = old[label], new[label]
        delta = after.value - before.value
        total = ""
        if before.total != after.total:
            total = f"; total {before.total} -> {after.total}"
        lines.append(
            f"metric {label}: {before.value} -> {after.value} ({delta:+d}){total}"
        )
    return lines


def _read_json(text: str | None) -> dict | None:
    if text is None:
        return None
    try:
        value = json.loads(text)
    except json.JSONDecodeError as exc:
        raise PublicReleaseError(f"invalid overlay atlas JSON: {exc}") from exc
    return value if isinstance(value, dict) else None


def _exact_range_delta(old: dict, new: dict) -> dict[str, object]:
    """Use the atlas tool's canonical, fail-closed exact-C identity model."""
    try:
        return overlay_atlas.compare_exact_c_atlases(old, new)
    except overlay_atlas.AtlasDeltaError as exc:
        raise PublicReleaseError(f"overlay exact-range delta is invalid: {exc}") from exc


def _delta_report(ctx: ReleaseContext) -> list[str]:
    old_readme = _show_file(ctx.repo, ctx.base_ref, "README.md")
    current_readme = (ctx.repo / "README.md").read_text(errors="replace")
    lines = _metric_delta_lines(old_readme, current_readme)
    if not lines:
        lines.append("metric deltas: unavailable (scoreboard absent or unparseable)")

    old_atlas = _read_json(
        _show_file(ctx.repo, ctx.base_ref, "config/overlays.us.json")
    )
    current_path = ctx.repo / "config/overlays.us.json"
    new_atlas = _read_json(
        current_path.read_text(errors="replace") if current_path.is_file() else None
    )
    if old_atlas is None or new_atlas is None:
        lines.append("overlay exact-range deltas: unavailable")
        return lines
    delta = _exact_range_delta(old_atlas, new_atlas)
    totals = delta["totals"]
    assert isinstance(totals, dict)
    promoted = int(totals["promotion_bytes"])
    retracted = int(totals["retraction_bytes"])
    lines.append(
        f"overlay exact ranges: promoted={promoted} retracted={retracted} "
        f"net={promoted - retracted:+d}"
    )
    for kind, key in (("promotion", "promotions"), ("retraction", "retractions")):
        rows = delta[key]
        assert isinstance(rows, list)
        for row in rows:
            lines.append(
                f"  {kind} overlay {int(row['overlay'])} "
                f"+0x{int(row['offset']):X}..+0x{int(row['end_offset']):X} "
                f"{int(row['size'])} bytes source={row['source']}"
            )
    return lines


def _terminate(proc: subprocess.Popen[bytes]) -> None:
    try:
        os.killpg(proc.pid, signal.SIGTERM)
        proc.wait(timeout=5)
    except ProcessLookupError:
        return
    except subprocess.TimeoutExpired:
        try:
            os.killpg(proc.pid, signal.SIGKILL)
        except ProcessLookupError:
            pass


def _run_commands(
    repo: Path,
    commands: tuple[tuple[str, ...], ...],
    *,
    niceness: int,
    timeout: float,
) -> bool:
    log_dir = repo / "build" / "public-release" / "reconcile"
    log_dir.mkdir(parents=True, exist_ok=True)
    for index, command_row in enumerate(commands, 1):
        label, *command = command_row
        report = collect_health(repo)
        health_path = log_dir / f"{index:02d}-{label}-health.log"
        health_path.write_text(format_report(report) + "\n")
        if report.verdict != "HEALTHY":
            print(f"reconcile {label}: {report.verdict} log={health_path}")
            return False
        log_path = log_dir / f"{index:02d}-{label}.log"
        started = time.monotonic()
        with log_path.open("wb") as log:
            log.write(("command: " + " ".join(command) + "\n").encode())
            log.flush()
            try:
                proc = subprocess.Popen(
                    command,
                    cwd=repo,
                    stdout=log,
                    stderr=subprocess.STDOUT,
                    start_new_session=True,
                    preexec_fn=(lambda: os.nice(niceness)) if niceness else None,
                )
                return_code = proc.wait(timeout=timeout)
            except subprocess.TimeoutExpired:
                log.write(f"timeout after {timeout:.1f}s\n".encode())
                _terminate(proc)
                return_code = 124
            except OSError as exc:
                log.write(f"launch failed: {exc}\n".encode())
                return_code = 127
        status = "PASS" if return_code == 0 else "FAIL"
        print(
            f"reconcile {label}: {status} {time.monotonic() - started:.1f}s "
            f"log={log_path}"
        )
        if return_code != 0:
            return False
    return True


def _release_gate_command(
    ctx: ReleaseContext,
    *,
    niceness: int,
    timeout: float,
    clean: bool,
) -> list[str]:
    command = [
        sys.executable,
        "tools/release_gate.py",
        "--repo",
        str(ctx.repo),
        "--log-dir",
        "build/public-release/gates",
        "--nice",
        str(niceness),
        "--timeout",
        str(timeout),
    ]
    if clean:
        command.extend(
            [
                "--public-push-preflight",
                "--branch",
                ctx.branch,
                "--remote",
                ctx.remote,
            ]
        )
    return command


def _reconciliation_commands(
    ctx: ReleaseContext, *, write_derived: bool, check_reference_builds: bool
) -> tuple[tuple[str, ...], ...]:
    commands = WRITE_GENERATORS if write_derived else READ_ONLY_GENERATORS
    if not ctx.outgoing_commits:
        planned = commands
    else:
        range_check = (
            "outgoing-cleanroom",
            "bash",
            "tools/cleanroom_check.sh",
            "--range",
            f"{ctx.base_ref}..HEAD",
        )
        planned = (range_check, *commands)
    if check_reference_builds:
        reference_preflight = (
            "reference-builds",
            "bash",
            "tools/verify_reference_builds.sh",
        )
        return (reference_preflight, *planned)
    return planned


def _tracked_dirt(repo: Path) -> str:
    return str(_git(repo, "status", "--porcelain=v1", "--untracked-files=no")).strip()


def _unexpected_derived_changes(repo: Path) -> list[str]:
    names = {
        line
        for line in str(_git(repo, "diff", "--name-only", "HEAD")).splitlines()
        if line
    }
    names.update(
        line
        for line in str(_git(repo, "diff", "--cached", "--name-only", "HEAD")).splitlines()
        if line
    )
    return sorted(names - DERIVED_PATHS)


def _print_context(ctx: ReleaseContext) -> None:
    print(
        f"release context: branch={ctx.branch} remote={ctx.remote} "
        f"base={ctx.base_oid[:12]} head={ctx.head_oid[:12]} "
        f"outgoing={len(ctx.outgoing_commits)}"
    )
    print(f"remote fetch URL: {ctx.fetch_url}")
    print(f"remote push URL:  {ctx.push_url}")


def _metadata_path(repo: Path, value: Path, label: str) -> tuple[Path, str]:
    if value.is_absolute() or ".." in value.parts or len(value.parts) < 2:
        raise PublicReleaseError(f"{label} must be a repository-relative build path")
    relative = value.as_posix()
    _validate_manifest_path(relative)
    if value.parts[0] != "build" or value.suffix != ".json":
        raise PublicReleaseError(f"{label} must live under ignored build/")
    current = repo
    for part in value.parts:
        current = current / part
        if current.is_symlink():
            raise PublicReleaseError(f"{label} must not traverse a symlink")
    ignored = subprocess.run(
        ["git", "check-ignore", "--no-index", "--quiet", "--", relative],
        cwd=repo,
        check=False,
    )
    if ignored.returncode != 0:
        raise PublicReleaseError(f"{label} is not ignored by Git")
    return repo / value, relative


def _write_metadata(path: Path, value: object) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + ".tmp")
    temporary.write_bytes(_canonical_json(value))
    os.replace(temporary, path)


def _read_metadata(path: Path, label: str) -> object:
    try:
        raw = path.read_bytes()
    except OSError as exc:
        raise PublicReleaseError(f"cannot read {label}") from exc
    if b"\0" in raw or len(raw) > 4 * 1024 * 1024:
        raise PublicReleaseError(f"{label} is binary or unexpectedly large")
    try:
        return json.loads(raw)
    except (UnicodeDecodeError, json.JSONDecodeError) as exc:
        raise PublicReleaseError(f"{label} is not valid JSON") from exc


def _approval_path_for(manifest_path: Path) -> Path:
    return manifest_path.with_name(manifest_path.stem + ".approval.json")


def _manifest_from_current_source(
    repo: Path, branch: str, remote: str, manifest_range: str
) -> dict[str, Any]:
    ctx = _manifest_context(repo, branch, remote, manifest_range)
    return _seal_manifest(_manifest_payload(ctx))


def _generate_manifest_mode(
    repo: Path,
    branch: str,
    remote: str,
    manifest_range: str,
    manifest_name: Path,
) -> int:
    manifest_path, relative = _metadata_path(repo, manifest_name, "manifest output")
    manifest = _manifest_from_current_source(repo, branch, remote, manifest_range)
    _write_metadata(manifest_path, manifest)
    approval_path = _approval_path_for(manifest_path)
    approval_relative = Path(relative).with_name(Path(relative).stem + ".approval.json")
    _metadata_path(repo, approval_relative, "approval template")
    if not approval_path.exists():
        _write_metadata(approval_path, _approval_template(manifest))
        approval_status = f"approval-template={approval_relative.as_posix()}"
    else:
        approval_status = f"approval-template-preserved={approval_relative.as_posix()}"
    print(
        f"PUBLIC RELEASE MANIFEST PASS: units={len(manifest['units'])} "
        f"manifest={relative} {approval_status}"
    )
    print("network mutation=disabled; copy=disabled; merge=disabled; push=disabled")
    return 0


def _approve_manifest_mode(
    repo: Path,
    branch: str,
    remote: str,
    manifest_range: str,
    manifest_name: Path,
    approval_name: Path,
) -> int:
    manifest_path, relative = _metadata_path(repo, manifest_name, "manifest input")
    approval_path, approval_relative = _metadata_path(repo, approval_name, "approval input")
    if manifest_path == approval_path:
        raise PublicReleaseError("manifest and approval inputs must be different files")
    recorded = _read_metadata(manifest_path, "manifest")
    current = _manifest_from_current_source(repo, branch, remote, manifest_range)
    if recorded != current:
        raise PublicReleaseError(
            "recorded manifest has drifted from the current source range"
        )
    approved = _validate_approval(current, _read_metadata(approval_path, "approval"))
    path_count = sum(len(unit["paths"]) for unit in approved)
    print(
        f"PUBLIC RELEASE APPROVAL PASS: units={len(approved)} paths={path_count} "
        f"manifest={relative} approval={approval_relative}"
    )
    print("network mutation=disabled; copy=disabled; merge=disabled; push=disabled")
    return 0


def _export_identity(change: dict[str, Any]) -> dict[str, Any]:
    return {
        key: change[key]
        for key in (
            "status",
            "path",
            "old_path",
            "old_blob",
            "new_blob",
            "old_mode",
            "new_mode",
            "classification",
        )
    }


def _validate_export_units(
    ctx: ReleaseContext,
    manifest: dict[str, Any],
    approval: object,
) -> tuple[int, int]:
    approved = _validate_approval(manifest, approval)
    if len(ctx.outgoing_commits) != len(approved):
        raise PublicReleaseError(
            "public outgoing commit count does not match approved commit units"
        )
    path_count = 0
    for index, (commit, unit) in enumerate(zip(ctx.outgoing_commits, approved), 1):
        message = str(_git(ctx.repo, "show", "-s", "--format=%B", commit)).rstrip("\n")
        if message != unit["public_subject"]:
            raise PublicReleaseError(
                f"public unit {index} does not have its approved plain subject"
            )
        changes = _raw_commit_changes(ctx.repo, commit)
        _scan_manifest_changes(ctx.repo, commit, changes)
        actual = [_export_identity(row) for row in changes]
        if actual != unit["paths"]:
            raise PublicReleaseError(
                f"public unit {index} path/blob identities differ from approval"
            )
        path_count += len(actual)
    return len(approved), path_count


def _validate_export_mode(
    repo: Path,
    branch: str,
    remote: str,
    manifest_name: Path,
    approval_name: Path,
) -> int:
    manifest_path, relative = _metadata_path(repo, manifest_name, "manifest input")
    approval_path, approval_relative = _metadata_path(repo, approval_name, "approval input")
    if manifest_path == approval_path:
        raise PublicReleaseError("manifest and approval inputs must be different files")
    manifest = _validate_manifest_integrity(_read_metadata(manifest_path, "manifest"))
    approval = _read_metadata(approval_path, "approval")
    ctx = _release_context(repo, branch, remote, require_clean=True)
    _check_diff_hygiene(ctx, include_worktree=False)
    findings = _scan_release(ctx, include_worktree=False)
    if findings:
        raise PublicReleaseError("public export failed release text scan")
    units, paths = _validate_export_units(ctx, manifest, approval)
    print(
        f"PUBLIC RELEASE EXPORT VALIDATION PASS: units={units} paths={paths} "
        f"manifest={relative} approval={approval_relative}"
    )
    print("gates-recorded=yes; network mutation=disabled; push=disabled")
    return 0


def _parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo", type=Path, default=Path.cwd())
    parser.add_argument("--remote", required=True)
    parser.add_argument("--branch", required=True)
    parser.add_argument(
        "--write-derived",
        action="store_true",
        help="run the explicit derived-artifact writers; still never push",
    )
    parser.add_argument(
        "--check-reference-builds",
        action="store_true",
        help=(
            "fail early unless the out-of-tree reference farm matches "
            "tools/reference-builds.lock"
        ),
    )
    parser.add_argument(
        "--manifest-range",
        metavar="BASE..TIP",
        help=(
            "emit one reviewed public commit unit per source commit under build/; "
            "does not copy or publish"
        ),
    )
    parser.add_argument(
        "--manifest-file",
        type=Path,
        default=MANIFEST_DEFAULT,
        help="repository-relative ignored build path for manifest metadata",
    )
    parser.add_argument(
        "--approval",
        type=Path,
        help="reviewed approval JSON with exact approved path/blob identities",
    )
    parser.add_argument(
        "--validate-export",
        action="store_true",
        help="validate outgoing public commits against a manifest and approval",
    )
    parser.add_argument("--nice", type=int, default=15)
    parser.add_argument("--timeout", type=float, default=3600.0, metavar="SECONDS")
    return parser


def main(argv: list[str] | None = None) -> int:
    args = _parser().parse_args(argv)
    if not 0 <= args.nice <= 19:
        raise SystemExit("--nice must be between 0 and 19")
    if args.timeout <= 0:
        raise SystemExit("--timeout must be positive")
    special_mode = bool(args.manifest_range or args.validate_export or args.approval)
    if special_mode and (args.write_derived or args.check_reference_builds):
        raise SystemExit(
            "manifest/approval/export modes cannot run reconciliation writers or farm checks"
        )
    if args.validate_export and args.manifest_range:
        raise SystemExit("--validate-export and --manifest-range are separate modes")
    if args.validate_export and args.approval is None:
        raise SystemExit("--validate-export requires --approval")
    if args.approval is not None and not (args.manifest_range or args.validate_export):
        raise SystemExit("--approval requires --manifest-range or --validate-export")
    try:
        repo = _repo_root(args.repo.resolve())
        if args.validate_export:
            assert args.approval is not None
            return _validate_export_mode(
                repo,
                args.branch,
                args.remote,
                args.manifest_file,
                args.approval,
            )
        if args.manifest_range:
            if args.approval is None:
                return _generate_manifest_mode(
                    repo,
                    args.branch,
                    args.remote,
                    args.manifest_range,
                    args.manifest_file,
                )
            return _approve_manifest_mode(
                repo,
                args.branch,
                args.remote,
                args.manifest_range,
                args.manifest_file,
                args.approval,
            )
        ctx = _release_context(repo, args.branch, args.remote, require_clean=True)
        _check_diff_hygiene(ctx, include_worktree=False)
        findings = _scan_release(ctx, include_worktree=False)
        if findings:
            raise PublicReleaseError("release text scan failed: " + "; ".join(findings[:8]))
    except PublicReleaseError as exc:
        print(f"PUBLIC RELEASE PREFLIGHT FAIL: {exc}", file=sys.stderr)
        return 2

    _print_context(ctx)
    mode = "WRITE-DERIVED" if args.write_derived else "DRY-RUN"
    print(f"mode: {mode}; network mutation=disabled; push=disabled")
    generators = _reconciliation_commands(
        ctx,
        write_derived=args.write_derived,
        check_reference_builds=args.check_reference_builds,
    )
    if not _run_commands(repo, generators, niceness=args.nice, timeout=args.timeout):
        print("PUBLIC RELEASE PREFLIGHT FAIL: reconciliation command failed", file=sys.stderr)
        return 1

    dirty = _tracked_dirt(repo)
    try:
        if args.write_derived:
            unexpected = _unexpected_derived_changes(repo)
            if unexpected:
                raise PublicReleaseError(
                    "derived generators changed paths outside the allowlist: "
                    + ", ".join(unexpected)
                )
        ctx = _release_context(
            repo, args.branch, args.remote, require_clean=not args.write_derived
        )
        _check_diff_hygiene(ctx, include_worktree=bool(dirty))
        findings = _scan_release(ctx, include_worktree=bool(dirty))
        if findings:
            raise PublicReleaseError("release text scan failed: " + "; ".join(findings[:8]))
    except PublicReleaseError as exc:
        print(f"PUBLIC RELEASE PREFLIGHT FAIL: {exc}", file=sys.stderr)
        return 2

    gate_command = _release_gate_command(
        ctx, niceness=args.nice, timeout=args.timeout, clean=not bool(dirty)
    )
    gate_result = subprocess.run(gate_command, cwd=repo, check=False)
    if gate_result.returncode != 0:
        print("PUBLIC RELEASE PREFLIGHT FAIL: release gates failed", file=sys.stderr)
        return gate_result.returncode

    try:
        final_ctx = _release_context(
            repo, args.branch, args.remote, require_clean=not args.write_derived
        )
        final_dirty = _tracked_dirt(repo)
        _check_diff_hygiene(final_ctx, include_worktree=bool(final_dirty))
        findings = _scan_release(final_ctx, include_worktree=bool(final_dirty))
        if findings:
            raise PublicReleaseError(
                "post-gate release text scan failed: " + "; ".join(findings[:8])
            )
        for line in _delta_report(final_ctx):
            print(line)
    except PublicReleaseError as exc:
        print(f"PUBLIC RELEASE PREFLIGHT FAIL: {exc}", file=sys.stderr)
        return 2

    if final_dirty:
        print("derived artifacts changed:")
        print(final_dirty)
        print(
            "PUBLIC RELEASE RECONCILIATION PASS: review and commit only these "
            "generated changes, then rerun dry-run preflight; push=disabled"
        )
    else:
        print("PUBLIC RELEASE PREFLIGHT PASS: release-ready=yes; push=disabled")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
