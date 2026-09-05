#!/usr/bin/env bash
# Integrate one lane branch into the current branch (campaign/unchain).
#
#   tools/merge_lane.sh <lane-name>
#
# Reads the lane's committed tip without touching its worktree, merges it,
# resolves generated files that conflict,
# conflict by regenerating them (README scoreboard block, overlay atlas), then
# re-runs verify/check-docs/overlay-atlas/check-scoreboard here. Exits non-zero
# and leaves the merge in progress if anything else conflicts or a gate fails.
set -euo pipefail
name=${1:?lane name}
root=$(git rev-parse --show-toplevel)
cd "$root"
branch=lane/$name
tip=$(git rev-parse --verify "$branch^{commit}")
build_jobs=${MICKEY_BUILD_JOBS:-6}
build_nice=${MICKEY_BUILD_NICE:-15}
case "$build_jobs" in ''|*[!0-9]*|0) echo "invalid MICKEY_BUILD_JOBS: $build_jobs" >&2; exit 2 ;; esac
case "$build_nice" in ''|*[!0-9]*) echo "invalid MICKEY_BUILD_NICE: $build_nice" >&2; exit 2 ;; esac
low_gmake() { nice -n "$build_nice" gmake -j"$build_jobs" "$@"; }
.venv/bin/python tools/merge_transaction.py clean
tools/cleanroom_check.sh --range "HEAD..$tip" 2>&1 | tail -1
echo "== merge $branch"
# --no-commit: the merge is committed only after every gate below passes.
if ! git merge --no-commit --no-ff "$tip" >/dev/null 2>&1; then
  conflicts=$(git diff --name-only --diff-filter=U)
  for f in $conflicts; do
    case "$f" in
      README.md|config/overlays.us.json|config/overlay-donors.us.json|config/postprocess-audit.us.json) git checkout --theirs "$f" && git add "$f" ;;
      docs/modules.md|docs/overlays.md) .venv/bin/python tools/resolve_modules_split.py || { echo "unresolved conflict: $f" >&2; exit 1; } ;;
      mickey.us.yaml|docs/resident.md|*.c|*.h) .venv/bin/python tools/resolve_comment_hunks.py "$f" && git add "$f" || echo "deferring $f to tools/resolve_lane_conflicts.py" ;;
      *) echo "deferring $f to tools/resolve_lane_conflicts.py" ;;
    esac
  done
fi
# Whatever the per-file rules above left unresolved goes through the
# hunk-level/three-way resolver (README theirs, Makefile keep-both, lane-owned
# new files theirs, ledgers theirs-per-hunk when ours is a subset, everything
# else three-way against the lane's merge-base). It never takes a shared
# file whole.
if git diff --name-only --diff-filter=U | grep -q .; then
  .venv/bin/python tools/resolve_lane_conflicts.py "$tip" || { echo "unresolved conflicts remain; merge left in progress" >&2; exit 1; }
fi
if ! git rev-parse --verify MERGE_HEAD >/dev/null 2>&1; then
  git merge-base --is-ancestor "$tip" HEAD || { echo "merge did not start" >&2; exit 1; }
  echo "$branch is already integrated"
  exit 0
fi
if git grep -q '^<<<<<<< ' -- . ':!*.md'; then echo "conflict markers left in tracked files:" >&2; git grep -l '^<<<<<<< ' -- . >&2; exit 1; fi
.venv/bin/python tools/merge_transaction.py begin
echo "== integration gates"
gmake overlay-atlas-write >/dev/null 2>&1 || true
.venv/bin/python tools/refresh_atlas_digest.py >/dev/null
gmake extract 2>&1 | tail -1
gmake overlay-atlas-write >/dev/null 2>&1 || true
.venv/bin/python tools/refresh_atlas_digest.py >/dev/null
.venv/bin/python tools/fix_stale_externs.py | tail -1
.venv/bin/python tools/check_match_regression.py HEAD || { echo "a function matched at HEAD carries GLOBAL_ASM again; merge left uncommitted (resolve hunks, never whole files)" >&2; exit 1; }
.venv/bin/python tools/check_duplicate_bodies.py || { echo "a function has both a C body and a GLOBAL_ASM pragma after the merge; drop the stale pragma line" >&2; exit 1; }
gmake -s check-nonmatching-builds || { echo "a candidate-bearing TU no longer compiles with -DNON_MATCHING (its candidates would drop out of the permuter sweep); merge left uncommitted" >&2; exit 1; }
# Fresh extraction and build: stale objects and stale asm/ have masked real failures twice.
gmake distclean >/dev/null 2>&1; gmake extract 2>&1 | tail -1
low_gmake >/dev/null 2>&1 || true   # warm-up: the first parallel build after a re-split can race
gmake overlay-syms 2>&1 | tail -1   # a merge that changes overlay relocation surfaces needs the generated symbol block before the link
low_gmake >/dev/null 2>&1 || true
out=$(tools/with_verify_lock.sh nice -n "$build_nice" gmake -j"$build_jobs" verify 2>&1 | tail -1); echo "$out"
case "$out" in OK*) ;; *) echo "verify FAILED after merging $branch; merge left uncommitted (git merge --abort to drop it)" >&2; low_gmake 2>&1 | grep -iE 'error|undefined ref|defined twice' | head -5 >&2; exit 1 ;; esac
gmake scoreboard 2>&1 | tail -1
gmake overlay-atlas 2>&1 | tail -1
.venv/bin/python tools/fix_jumptable_claim.py >/dev/null 2>&1 || true
gmake check-docs 2>&1 | tail -1
gmake check-overlay-syms 2>&1 | tail -1
gmake check-scoreboard 2>&1 | tail -1
gmake cleanroom 2>&1 | tail -1
.venv/bin/python tools/merge_transaction.py stage
git commit -q -m "Merge $branch into $(git rev-parse --abbrev-ref HEAD)

Gates at merge time: verify byte-identical, check-docs, overlay-atlas,
overlay symbols checked, scoreboard regenerated."
.venv/bin/python tools/merge_transaction.py clean
git log --oneline -1
