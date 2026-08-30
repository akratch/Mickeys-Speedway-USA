#!/usr/bin/env bash
# Launch one non-interactive Codex CLI worker in its own lane worktree.
#
#   tools/codex_lane.sh <name> <prompt-file> [--minutes N] [--target SYMBOL] [--no-extract]
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
target=$name
resume=0
lane_args=()
while [ "$#" -gt 0 ]; do
  case "$1" in
    --minutes)
      [ "$#" -ge 2 ] || { echo "--minutes needs a value" >&2; exit 2; }
      minutes=$2; shift 2 ;;
    --minutes=*) minutes=${1#*=}; shift ;;
    --target)
      [ "$#" -ge 2 ] || { echo "--target needs a value" >&2; exit 2; }
      target=$2; shift 2 ;;
    --target=*) target=${1#*=}; shift ;;
    --resume) resume=1; shift ;;
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

if [ "$resume" = 1 ] && [ -d "../mickey-lane-$name" ]; then
  # --resume: relaunch a worker in an existing lane (after a crash or a
  # restart); the lane keeps its branch, commits and build state.
  lane=$(cd "../mickey-lane-$name" && pwd)
  rm -f "$lane/.codex-status"
else
  lane=$(tools/new_lane.sh "$name" "${lane_args[@]}")
fi
model=${CODEX_MODEL:-}
args=(exec --dangerously-bypass-approvals-and-sandbox -C "$lane" -o "$lane/.codex-last.md")
[ -n "$model" ] && args+=(-m "$model")
effort=${CODEX_EFFORT:-}
[ -n "$effort" ] && args+=(-c "model_reasoning_effort=\"$effort\"")
budget_seconds=$((minutes * 60))
soft_deadline=$(( $(date +%s) + budget_seconds ))
hard_grace_seconds=300
hard_deadline=$((soft_deadline + hard_grace_seconds))
base_commit=$(git -C "$lane" rev-parse HEAD)
common_dir=$(git -C "$lane" rev-parse --path-format=absolute --git-common-dir)
heartbeat_file="$common_dir/codex-crew/heartbeats/$name.json"
heartbeat_args=(heartbeat --worker "$name" --deadline-unix "$soft_deadline"
  --progress "runner launched" --state active)
if [ ! -f "$heartbeat_file" ]; then
  heartbeat_args+=(--target "$target" --base "$base_commit" --attempt-count 0
    --best-score "not measured" --mismatch-class unclassified)
fi
(cd "$lane" && python3 tools/crew.py "${heartbeat_args[@]}" >/dev/null)
export MICKEY_HEARTBEAT_WORKER="$name"
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
- After the baseline, each material attempt, and before a long bounded call,
  refresh the shared checkpoint. Keep attempt count monotonic; report the best
  score and current mismatch class, not merely that you are still working:
    python3 tools/crew.py checkpoint --worker {os.environ['MICKEY_HEARTBEAT_WORKER']} \\
      --progress "attempt 3: changed declaration order" --attempt-count 3 \\
      --best-score "187/217 words differ" --mismatch-class frame-allocation
- If you can estimate handoff time, add --eta-unix UNIX. The assignment's soft
  deadline is already recorded and available as $MICKEY_TASK_DEADLINE_UNIX.
- The field contract and fail-closed poll behavior are in
  docs/worker-checkpoints.md.
- If stopping at a plateau, guard the candidate and use
  tools/finalize_plateau.py before the handoff.

'''
with open(prompt, encoding='utf-8') as source, open(effective, 'w', encoding='utf-8') as output:
    output.write(preamble)
    output.write(source.read())
env = os.environ.copy()
env['MICKEY_TASK_BUDGET_SECONDS'] = budget
env['MICKEY_TASK_DEADLINE_UNIX'] = soft
env['MICKEY_TASK_HARD_DEADLINE_UNIX'] = hard
env['MICKEY_HEARTBEAT_WORKER'] = os.environ['MICKEY_HEARTBEAT_WORKER']
hard_seconds = str(max(1, int(hard) - int(__import__('time').time())))
runner = [timeout_bin, '--signal=INT', '--kill-after=60', hard_seconds, 'codex', *args, '-']
cmd = ('"$@" < "$0"; rc=$?; printf "%s\\n" "$rc" > .codex-status; '
       'if [ "$rc" -eq 0 ]; then hb_state=complete; else hb_state=failed; fi; '
       'python3 tools/crew.py heartbeat --worker "$MICKEY_HEARTBEAT_WORKER" '
       '--progress "runner exited with status $rc" --state "$hb_state" >/dev/null 2>&1 || true; '
       'exit "$rc"')
p = subprocess.Popen(['sh', '-c', cmd, effective, *runner], cwd=lane, env=env,
                     start_new_session=True,
                     stdin=subprocess.DEVNULL, stdout=open(os.path.join(lane, '.codex-run.log'), 'w'),
                     stderr=subprocess.STDOUT)
print(p.pid)
PY
)
echo "$lane (soft deadline ${minutes}m; hard grace 5m)"
echo "poll: python3 tools/crew.py heartbeat-status --worker $name --json --check"
