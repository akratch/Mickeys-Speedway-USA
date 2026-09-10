#!/usr/bin/env bash
# Report whether a Codex lane is running, finished, or out of budget.
#
#   tools/codex_watch.sh <lane-name>
#
# Deliberately does NOT grep for the English words "exhausted" or "quota".
# This project uses "exhausted" as a term of art for a searched space, so those
# words appear throughout prompts, ADRs and handoff prose that the worker reads
# and echoes into its log. A naive grep reported 16 hits on a healthy run, all
# of them prose. A monitor that cries wolf is worse than none.
#
# It matches API-failure shapes instead: an underscored error token or a
# usage-limit phrase. A bare "429" is NOT enough on its own -- the first real
# run tripped on 4294967292, which is 0xFFFFFFFC in a debug locals dump -- so
# 429 needs digit boundaries AND an error word somewhere on the same line.
# Note "somewhere": a first attempt required the word to follow the number and
# so missed "HTTP status 429 returned by the API", which is the failure that
# actually matters. The two conditions are tested independently.
set -uo pipefail
name=${1:?lane name}
lane="$(git rev-parse --show-toplevel)/../mickey-lane-${name}"
log="$lane/.codex-run.log"
status="$lane/.codex-status"

[ -r "$log" ] || { echo "$name: no run log at $log"; exit 2; }

if [ -r "$status" ]; then
    code=$(cat "$status")
    echo "$name: FINISHED, exit=$code"
else
    echo "$name: running ($(wc -l < "$log" | tr -d ' ') log lines)"
fi

# Error-shaped, not prose-shaped.
# Textual tokens the API actually emits: these stand alone.
TOKENS='rate_limit[a-z_]*|quota_exceeded|insufficient_quota|usage limit reached|too many requests'
# A bare status code only counts with an error word anywhere on the line.
STATUS='(^|[^0-9])429([^0-9]|$)'
CONTEXT='status|error|http|limit'

hits=$( { grep -inE "$TOKENS" "$log"; grep -inE "$STATUS" "$log" | grep -iE "$CONTEXT"; } | sort -u )
if [ -n "$hits" ]; then
    echo "  BUDGET: $(printf '%s\n' "$hits" | wc -l | tr -d ' ') API-limit signal(s):"
    printf '%s\n' "$hits" | tail -3 | sed 's/^/    /'
else
    echo "  budget: no API-limit signal"
fi

# grep -c already prints 0 and exits 1 when there are no matches, so a
# `|| echo 0` fallback appends a SECOND zero and the arithmetic test then
# fails with "integer expected". Take grep's own output and swallow only
# its status.
last=$(grep -c 'tokens used' "$log" 2>/dev/null) || true
[ "${last:-0}" -gt 0 ] && echo "  token reports in log: $last"
exit 0
