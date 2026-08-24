#!/usr/bin/env bash
# Launch one non-interactive Codex CLI worker in its own lane worktree.
#
#   tools/codex_lane.sh <name> <prompt-file> [--no-extract]
#
# Creates the lane with tools/new_lane.sh, then runs `codex exec` detached
# inside it with the prompt file on stdin. Progress goes to
# <lane>/.codex-run.log, the final message to <lane>/.codex-last.md, and the
# exit status to <lane>/.codex-status when the run ends. All three are
# ignored by git. The worker commits on branch lane/<name>; nothing here
# touches this repository's own worktree.
set -euo pipefail
name=${1:?lane name}; prompt=${2:?prompt file}; shift 2
lane=$(tools/new_lane.sh "$name" "$@")
model=${CODEX_MODEL:-}
args=(exec --dangerously-bypass-approvals-and-sandbox -C "$lane" -o "$lane/.codex-last.md")
[ -n "$model" ] && args+=(-m "$model")
(
  cd "$lane"
  nohup sh -c 'codex "$@" - < "$0"; echo "$?" > .codex-status' "$prompt" "${args[@]}" \
    > "$lane/.codex-run.log" 2>&1 &
  echo $! > "$lane/.codex-pid"
)
echo "$lane"
