#!/usr/bin/env bash
# Launch one non-interactive Codex CLI worker in its own lane worktree.
#
#   tools/codex_lane.sh <name> <prompt-file> [--minutes N] [--no-extract]
#
# Creates the lane with tools/new_lane.sh, then runs `codex exec` detached
# inside it with the prompt file on stdin. Progress goes to
# <lane>/.codex-run.log, the final message to <lane>/.codex-last.md, and the
# exit status to <lane>/.codex-status when the run ends. All runner files are
# ignored by git. The worker commits on branch lane/<name>; nothing here
# touches this repository's own worktree. The soft deadline is shown to the
# worker; GNU timeout interrupts it after a five-minute handoff grace period.
set -euo pipefail
name=${1:?lane name}; prompt=${2:?prompt file}; shift 2
minutes=${CODEX_MINUTES:-180}
lane_args=()
while [ "$#" -gt 0 ]; do
  case "$1" in
    --minutes)
      [ "$#" -ge 2 ] || { echo "--minutes needs a value" >&2; exit 2; }
      minutes=$2; shift 2 ;;
    --minutes=*) minutes=${1#*=}; shift ;;
    *) lane_args+=("$1"); shift ;;
  esac
done
case "$minutes" in
  ''|*[!0-9]*) echo "minutes must be a positive integer" >&2; exit 2 ;;
esac
[ "$minutes" -gt 5 ] || { echo "minutes must be greater than 5" >&2; exit 2; }
[ -r "$prompt" ] || { echo "no readable prompt file: $prompt" >&2; exit 2; }
prompt_dir=$(cd "$(dirname "$prompt")" && pwd)
prompt="$prompt_dir/$(basename "$prompt")"
timeout_bin=$(command -v timeout || command -v gtimeout || true)
[ -n "$timeout_bin" ] || { echo "GNU timeout/gtimeout is required" >&2; exit 2; }

lane=$(tools/new_lane.sh "$name" "${lane_args[@]}")
model=${CODEX_MODEL:-}
args=(exec --dangerously-bypass-approvals-and-sandbox -C "$lane" -o "$lane/.codex-last.md")
[ -n "$model" ] && args+=(-m "$model")
effort=${CODEX_EFFORT:-}
[ -n "$effort" ] && args+=(-c "model_reasoning_effort=\"$effort\"")
budget_seconds=$((minutes * 60))
soft_deadline=$(( $(date +%s) + budget_seconds ))
hard_grace_seconds=300
hard_deadline=$((soft_deadline + hard_grace_seconds))
(
  cd "$lane"
  # Detach into a new session so the lane survives whatever launched it
  # (an interactive tool call's process group is killed when that tool exits).
  python3 - "$prompt" "$lane" "$budget_seconds" "$soft_deadline" \
    "$hard_deadline" "$timeout_bin" "${args[@]}" <<'PY' > "$lane/.codex-pid"
import os, subprocess, sys
prompt, lane, budget, soft, hard, timeout_bin, *args = sys.argv[1:]
effective = os.path.join(lane, '.codex-effective-prompt.md')
preamble = f'''Campaign task budget (ADR 0011):
- Soft deadline: Unix {soft} ({budget} seconds from launch).
- Hard runner stop: Unix {hard} (five-minute handoff grace).
- Reserve the end of the soft budget to commit an exact result or the best
  meaningful plateau. Do not start a compile, permuter, or campaign call whose
  own cap cannot fit in the remaining time. Preserve work; never reset it away.

'''
with open(prompt, encoding='utf-8') as source, open(effective, 'w', encoding='utf-8') as output:
    output.write(preamble)
    output.write(source.read())
env = os.environ.copy()
env['MICKEY_TASK_BUDGET_SECONDS'] = budget
env['MICKEY_TASK_DEADLINE_UNIX'] = soft
env['MICKEY_TASK_HARD_DEADLINE_UNIX'] = hard
hard_seconds = str(max(1, int(hard) - int(__import__('time').time())))
runner = [timeout_bin, '--signal=INT', '--kill-after=60', hard_seconds, 'codex', *args, '-']
cmd = '"$@" < "$0"; rc=$?; printf "%s\\n" "$rc" > .codex-status; exit "$rc"'
p = subprocess.Popen(['sh', '-c', cmd, effective, *runner], cwd=lane, env=env,
                     start_new_session=True,
                     stdin=subprocess.DEVNULL, stdout=open(os.path.join(lane, '.codex-run.log'), 'w'),
                     stderr=subprocess.STDOUT)
print(p.pid)
PY
)
echo "$lane (soft deadline ${minutes}m; hard grace 5m)"
