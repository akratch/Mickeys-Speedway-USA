#!/usr/bin/env python3
"""Shared, file-based coordination for the three-session Codex crew.

Runtime state lives under the repository's Git common directory, so every
worktree sees the same mailbox while no coordination message can be committed.
See ADRs 0013 and 0014 for the ownership and lifecycle rules.
"""

from __future__ import annotations

import argparse
import datetime as dt
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import uuid


SCHEMA_VERSION = 1
STATES = (
    "ACTIVE",
    "READY",
    "ASSIGNED",
    "ACKED",
    "WORKING",
    "HANDOFF",
    "PIPELINED",
    "INTEGRATED",
    "RELEASED",
    "PLATEAU",
    "BLOCKED",
    "IDLE",
)
MESSAGE_TYPES = (
    "ASSIGN",
    "ACK",
    "UPDATE",
    "QUESTION",
    "ANSWER",
    "HANDOFF",
    "PIPELINE",
    "INTEGRATED",
    "RELEASE",
    "RELEASED",
    "BLOCKED",
)
NAME_RE = re.compile(r"^[a-z0-9][a-z0-9-]{0,31}$")
TASK_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9._-]{0,63}$")


class CrewError(RuntimeError):
    """A concise, user-actionable crew-control error."""


def run_git(*args: str) -> str:
    result = subprocess.run(
        ["git", *args],
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    if result.returncode != 0:
        detail = result.stderr.strip() or "git command failed"
        raise CrewError(f"git {' '.join(args)}: {detail}")
    return result.stdout.strip()


def default_crew_root() -> Path:
    common = run_git("rev-parse", "--path-format=absolute", "--git-common-dir")
    return Path(common).resolve() / "codex-crew"


def primary_checkout_root() -> Path:
    common = Path(run_git("rev-parse", "--path-format=absolute", "--git-common-dir")).resolve()
    if common.name != ".git":
        raise CrewError(f"expected a non-bare repository with a .git common directory, got {common}")
    return common.parent


def utc_now() -> dt.datetime:
    return dt.datetime.now(dt.timezone.utc)


def timestamp(value: dt.datetime | None = None) -> str:
    return (value or utc_now()).isoformat(timespec="seconds").replace("+00:00", "Z")


def clean_line(value: str) -> str:
    return " ".join(value.replace("\x00", "").splitlines()).strip()


def validate_name(value: str, label: str) -> str:
    if not NAME_RE.fullmatch(value):
        raise CrewError(f"invalid {label} {value!r}; use lowercase letters, digits, and hyphens")
    return value


def validate_task(value: str) -> str:
    if not TASK_RE.fullmatch(value):
        raise CrewError(f"invalid task id {value!r}")
    return value


def atomic_write(path: Path, content: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    fd, temporary = tempfile.mkstemp(prefix=f".{path.name}.", dir=path.parent)
    temp_path = Path(temporary)
    try:
        with os.fdopen(fd, "w", encoding="utf-8", newline="\n") as handle:
            handle.write(content)
            handle.flush()
            os.fsync(handle.fileno())
        os.replace(temp_path, path)
    finally:
        temp_path.unlink(missing_ok=True)


def load_config(root: Path) -> dict[str, object]:
    path = root / "config.json"
    if not path.is_file():
        raise CrewError(f"crew is not initialized; run tools/crew.py init (expected {path})")
    try:
        config = json.loads(path.read_text(encoding="utf-8"))
    except (json.JSONDecodeError, OSError) as error:
        raise CrewError(f"cannot read {path}: {error}") from error
    if config.get("schema") != SCHEMA_VERSION:
        raise CrewError(f"unsupported crew schema in {path}")
    return config


def configured_roles(config: dict[str, object]) -> list[str]:
    workers = config.get("workers")
    if not isinstance(workers, list):
        raise CrewError("crew config has no worker list")
    roles = ["leader"]
    for row in workers:
        if not isinstance(row, dict) or not isinstance(row.get("role"), str):
            raise CrewError("crew config contains an invalid worker")
        roles.append(row["role"])
    return roles


def require_role(config: dict[str, object], role: str) -> str:
    if role not in configured_roles(config):
        raise CrewError(f"unknown role {role!r}")
    return role


def status_document(
    role: str,
    state: str,
    task: str,
    branch: str,
    ownership: str,
    compile_permit: str,
    summary: str,
) -> str:
    return (
        f"# {role} status\n\n"
        f"- state: {state}\n"
        f"- task: {task}\n"
        f"- branch: {branch}\n"
        f"- ownership: {ownership}\n"
        f"- compile-permit: {compile_permit}\n"
        f"- updated-at: {timestamp()}\n"
        f"- summary: {summary}\n"
    )


def parse_worker(value: str) -> tuple[str, str]:
    role, separator, lane = value.partition("=")
    role = validate_name(role, "worker role")
    lane = validate_name(lane if separator else f"crew-{role}", "lane name")
    return role, lane


def command_init(args: argparse.Namespace, root: Path) -> None:
    workers = [parse_worker(value) for value in args.worker]
    roles = [role for role, _ in workers]
    if len(roles) != 2 or len(set(roles)) != 2:
        raise CrewError("this protocol requires exactly two distinct workers")
    if "leader" in roles:
        raise CrewError("leader is reserved and cannot be a worker role")

    repo = primary_checkout_root()
    rows = []
    for role, lane in workers:
        rows.append({
            "role": role,
            "lane": lane,
            "parking_branch": f"lane/{lane}",
            "worktree": str(repo.parent / f"{repo.name}-lane-{lane}"),
        })
    desired = {
        "schema": SCHEMA_VERSION,
        "integration_branch": args.integration_branch,
        "leader": "leader",
        "workers": rows,
    }

    config_path = root / "config.json"
    if config_path.exists():
        existing = load_config(root)
        for key in ("schema", "integration_branch", "leader", "workers"):
            if existing.get(key) != desired.get(key):
                raise CrewError(f"existing {config_path} disagrees on {key}; refusing to overwrite")
    else:
        root.mkdir(parents=True, exist_ok=True)
        desired["initialized_at"] = timestamp()
        atomic_write(config_path, json.dumps(desired, indent=2, sort_keys=True) + "\n")

    all_roles = ["leader", *roles]
    for role in all_roles:
        (root / "inbox" / role).mkdir(parents=True, exist_ok=True)
        (root / "archive" / role).mkdir(parents=True, exist_ok=True)

    queue_path = root / "queue.md"
    if not queue_path.exists():
        atomic_write(
            queue_path,
            "# Crew queue\n\n"
            "Leader-owned. Assign disjoint work; one isolated pipeline task may follow a handoff.\n\n"
            "| Priority | Task | Owner | State | Ownership | Base | Notes |\n"
            "|---:|---|---|---|---|---|---|\n"
        )

    leader_status = root / "status" / "leader.md"
    if not leader_status.exists():
        atomic_write(
            leader_status,
            status_document("leader", "ACTIVE", "none", args.integration_branch,
                            "queue and integration", "idle", "crew initialized"),
        )
    for row in rows:
        path = root / "status" / f"{row['role']}.md"
        if not path.exists():
            atomic_write(
                path,
                status_document(
                    str(row["role"]), "READY", "none", str(row["parking_branch"]),
                    "unassigned", "idle", "waiting for assignment",
                ),
            )
    print(root)


def body_from_args(args: argparse.Namespace) -> str:
    if getattr(args, "body", None) is not None:
        return args.body.strip()
    body_file = getattr(args, "body_file", None)
    if body_file:
        if body_file == "-":
            return sys.stdin.read().strip()
        return Path(body_file).read_text(encoding="utf-8").strip()
    if not sys.stdin.isatty():
        return sys.stdin.read().strip()
    return ""


def create_message(
    root: Path,
    config: dict[str, object],
    sender: str,
    recipient: str,
    kind: str,
    task: str,
    subject: str,
    body: str,
) -> Path:
    require_role(config, sender)
    require_role(config, recipient)
    validate_task(task)
    if kind not in MESSAGE_TYPES:
        raise CrewError(f"invalid message type {kind!r}")
    now = utc_now()
    message_id = (
        now.strftime("%Y%m%dT%H%M%S.%fZ")
        + f"-{uuid.uuid4().hex[:8]}-{sender}-{kind.lower()}-{task}"
    )
    path = root / "inbox" / recipient / f"{message_id}.md"
    content = (
        "---\n"
        f"schema: {SCHEMA_VERSION}\n"
        f"id: {message_id}\n"
        f"from: {sender}\n"
        f"to: {recipient}\n"
        f"type: {kind}\n"
        f"task: {task}\n"
        f"created-at: {timestamp(now)}\n"
        f"subject: {clean_line(subject) or kind}\n"
        "---\n\n"
        f"{body}\n"
    )
    path.parent.mkdir(parents=True, exist_ok=True)
    try:
        with path.open("x", encoding="utf-8", newline="\n") as handle:
            handle.write(content)
            handle.flush()
            os.fsync(handle.fileno())
    except FileExistsError as error:
        raise CrewError(f"message collision at {path}; retry") from error
    return path


def command_send(args: argparse.Namespace, root: Path) -> None:
    config = load_config(root)
    path = create_message(
        root,
        config,
        args.sender,
        args.recipient,
        args.kind,
        args.task,
        args.subject,
        body_from_args(args),
    )
    print(path.name)


def message_path(root: Path, role: str, message: str, archive: bool = False) -> Path:
    if Path(message).name != message:
        raise CrewError("message must be a file name from tools/crew.py inbox")
    folder = "archive" if archive else "inbox"
    path = root / folder / role / message
    if not path.is_file():
        raise CrewError(f"message not found: {path}")
    return path


def parse_front_matter(path: Path) -> dict[str, str]:
    lines = path.read_text(encoding="utf-8").splitlines()
    if not lines or lines[0] != "---":
        raise CrewError(f"invalid message header: {path}")
    fields: dict[str, str] = {}
    for line in lines[1:]:
        if line == "---":
            return fields
        key, separator, value = line.partition(":")
        if not separator:
            raise CrewError(f"invalid message header: {path}")
        fields[key.strip()] = value.strip()
    raise CrewError(f"unterminated message header: {path}")


def command_inbox(args: argparse.Namespace, root: Path) -> None:
    config = load_config(root)
    require_role(config, args.role)
    paths = sorted((root / "inbox" / args.role).glob("*.md"))
    if not paths:
        print(f"{args.role} inbox empty")
        return
    for path in paths:
        fields = parse_front_matter(path)
        print(
            "\t".join((
                path.name,
                fields.get("type", "?"),
                fields.get("task", "?"),
                fields.get("from", "?"),
                fields.get("subject", ""),
            ))
        )


def command_read(args: argparse.Namespace, root: Path) -> None:
    config = load_config(root)
    require_role(config, args.role)
    print(message_path(root, args.role, args.message, args.archive).read_text(encoding="utf-8"), end="")


def archive_message(root: Path, role: str, message: str) -> Path:
    source = message_path(root, role, message)
    destination = root / "archive" / role / message
    if destination.exists():
        raise CrewError(f"archive already contains {message}")
    os.replace(source, destination)
    return destination


def command_archive(args: argparse.Namespace, root: Path) -> None:
    config = load_config(root)
    require_role(config, args.role)
    print(archive_message(root, args.role, args.message))


def command_ack(args: argparse.Namespace, root: Path) -> None:
    config = load_config(root)
    require_role(config, args.role)
    source = message_path(root, args.role, args.message)
    fields = parse_front_matter(source)
    sender = fields.get("from")
    task = fields.get("task")
    if not sender or not task:
        raise CrewError(f"message lacks sender or task: {source}")
    ack = create_message(
        root,
        config,
        args.role,
        sender,
        "ACK",
        task,
        f"ACK {fields.get('type', 'message')}: {fields.get('subject', '')}",
        body_from_args(args) or f"Received {source.name}.",
    )
    archive_message(root, args.role, args.message)
    print(ack.name)


def command_status(args: argparse.Namespace, root: Path) -> None:
    config = load_config(root)
    roles = [require_role(config, args.role)] if args.role else configured_roles(config)
    for index, role in enumerate(roles):
        if index:
            print()
        path = root / "status" / f"{role}.md"
        if not path.is_file():
            raise CrewError(f"missing status file: {path}")
        print(path.read_text(encoding="utf-8"), end="")


def command_set_status(args: argparse.Namespace, root: Path) -> None:
    config = load_config(root)
    role = require_role(config, args.role)
    if args.state not in STATES:
        raise CrewError(f"invalid state {args.state!r}")
    task = args.task if args.task == "none" else validate_task(args.task)
    document = status_document(
        role,
        args.state,
        task,
        clean_line(args.branch),
        clean_line(args.ownership),
        args.compile_permit,
        clean_line(args.summary),
    )
    atomic_write(root / "status" / f"{role}.md", document)
    print(root / "status" / f"{role}.md")


def command_queue(args: argparse.Namespace, root: Path) -> None:
    load_config(root)
    path = root / "queue.md"
    if not path.is_file():
        raise CrewError(f"missing queue: {path}")
    print(path.read_text(encoding="utf-8"), end="")


def command_set_queue(args: argparse.Namespace, root: Path) -> None:
    load_config(root)
    if args.role != "leader":
        raise CrewError("only leader may replace queue.md")
    body = body_from_args(args)
    if not body:
        raise CrewError("queue body is empty")
    atomic_write(root / "queue.md", body.rstrip() + "\n")
    print(root / "queue.md")


def registered_worktrees() -> dict[str, str]:
    rows: dict[str, str] = {}
    path: str | None = None
    for line in run_git("worktree", "list", "--porcelain").splitlines():
        if line.startswith("worktree "):
            path = str(Path(line.removeprefix("worktree ")).resolve())
        elif line.startswith("branch refs/heads/") and path:
            rows[path] = line.removeprefix("branch refs/heads/")
        elif not line:
            path = None
    return rows


def command_doctor(args: argparse.Namespace, root: Path) -> None:
    config = load_config(root)
    problems: list[str] = []
    roles = configured_roles(config)
    for role in roles:
        for path in (root / "inbox" / role, root / "archive" / role):
            if not path.is_dir():
                problems.append(f"missing directory {path}")
        path = root / "status" / f"{role}.md"
        if not path.is_file():
            problems.append(f"missing status {path}")
    if not (root / "queue.md").is_file():
        problems.append(f"missing queue {root / 'queue.md'}")

    worktrees = registered_worktrees()
    for row in config["workers"]:
        path = str(Path(str(row["worktree"])).resolve())
        expected = str(row["parking_branch"])
        actual = worktrees.get(path)
        if actual is None:
            problems.append(f"worker worktree is not registered: {path}")
        elif actual != expected:
            problems.append(f"{row['role']} is on {actual}, expected {expected}")

    if problems:
        for problem in problems:
            print(f"ERROR: {problem}")
        raise CrewError(f"doctor found {len(problems)} problem(s)")
    print(f"OK: {root} ({', '.join(roles)})")


def add_body_arguments(parser: argparse.ArgumentParser) -> None:
    group = parser.add_mutually_exclusive_group()
    group.add_argument("--body", help="Message body as one argument")
    group.add_argument("--body-file", help="Read body from this file, or - for stdin")


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, help="Override the Git-common crew directory")
    commands = parser.add_subparsers(dest="command", required=True)

    init = commands.add_parser("init", help="Initialize the leader and two worker mailboxes")
    init.add_argument(
        "--worker",
        action="append",
        default=[],
        metavar="ROLE=LANE",
        help="Add one worker and its permanent lane (exactly two required)",
    )
    init.add_argument("--integration-branch", default="campaign/unchain")
    init.set_defaults(function=command_init)

    root = commands.add_parser("root", help="Print the shared crew directory")
    root.set_defaults(function=lambda args, crew_root: print(crew_root))

    send = commands.add_parser("send", help="Atomically append an immutable inbox message")
    send.add_argument("--from", dest="sender", required=True)
    send.add_argument("--to", dest="recipient", required=True)
    send.add_argument("--type", dest="kind", choices=MESSAGE_TYPES, required=True)
    send.add_argument("--task", required=True)
    send.add_argument("--subject", default="")
    add_body_arguments(send)
    send.set_defaults(function=command_send)

    inbox = commands.add_parser("inbox", help="List one role's pending messages")
    inbox.add_argument("role")
    inbox.set_defaults(function=command_inbox)

    read = commands.add_parser("read", help="Read one message")
    read.add_argument("role")
    read.add_argument("message")
    read.add_argument("--archive", action="store_true")
    read.set_defaults(function=command_read)

    archive = commands.add_parser("archive", help="Archive a processed message")
    archive.add_argument("role")
    archive.add_argument("message")
    archive.set_defaults(function=command_archive)

    ack = commands.add_parser("ack", help="Acknowledge and archive one received message")
    ack.add_argument("role")
    ack.add_argument("message")
    add_body_arguments(ack)
    ack.set_defaults(function=command_ack)

    status = commands.add_parser("status", help="Show one or all actor-owned status files")
    status.add_argument("role", nargs="?")
    status.set_defaults(function=command_status)

    set_status = commands.add_parser("set-status", help="Replace the caller's status file")
    set_status.add_argument("role")
    set_status.add_argument("--state", choices=STATES, required=True)
    set_status.add_argument("--task", default="none")
    set_status.add_argument("--branch", default="none")
    set_status.add_argument("--ownership", default="unassigned")
    set_status.add_argument("--compile-permit", choices=("idle", "held"), default="idle")
    set_status.add_argument("--summary", default="")
    set_status.set_defaults(function=command_set_status)

    queue = commands.add_parser("queue", help="Show the leader-owned ready queue")
    queue.set_defaults(function=command_queue)

    set_queue = commands.add_parser("set-queue", help="Replace the leader-owned ready queue")
    set_queue.add_argument("--by", dest="role", required=True)
    add_body_arguments(set_queue)
    set_queue.set_defaults(function=command_set_queue)

    doctor = commands.add_parser("doctor", help="Check mailbox and permanent worktree wiring")
    doctor.set_defaults(function=command_doctor)
    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()
    try:
        root = (args.root.resolve() if args.root else default_crew_root())
        args.function(args, root)
    except (CrewError, OSError) as error:
        print(f"crew: {error}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
