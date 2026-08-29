#!/usr/bin/env bash
# Create an isolated worktree ("lane") for one worker.
#
#   tools/new_lane.sh <name> [--no-extract] [--no-cache] [base-branch]
#
# Creates ../mickey-lane-<name> on branch lane/<name> from base-branch
# (default campaign/unchain), shares the untracked toolchain, baserom, venv and
# vendored tool checkouts with this repository by symlink. When a verified
# commit-keyed bootstrap exists below Git's common directory, it copy-on-write
# clones the ignored split/build prerequisites into this lane; otherwise it
# runs the splat extract. Each lane still owns its build/ and asm/. Prints the
# lane path.
set -euo pipefail
name=${1:?lane name}; shift
extract=1; cache=1; base=campaign/unchain
for a in "$@"; do
  case "$a" in
    --no-extract) extract=0 ;;
    --no-cache) cache=0 ;;
    *) base=$a ;;
  esac
done
# Always anchor lane creation in the primary checkout. When this helper is
# invoked from an existing linked worktree, --show-toplevel names that lane and
# its .git is a file, so "$root/.git/modules" cannot be the shared submodule
# store. The common directory is stable from every worktree.
common=$(git rev-parse --path-format=absolute --git-common-dir)
if [ "$(basename "$common")" != .git ]; then
  echo "expected a non-bare repository with a .git common directory: $common" >&2
  exit 2
fi
root=$(dirname "$common")
dest=$(dirname "$root")/mickey-lane-$name
if [ -e "$dest" ]; then echo "lane exists: $dest" >&2; exit 2; fi
git -C "$root" worktree add -q -b "lane/$name" "$dest" "$base"
# Creating several full worktrees can make macOS Spotlight index every copied
# source/build path at once.  Mark the lane before extraction and compilation;
# other platforms harmlessly ignore this git-ignored empty file.
: > "$dest/.metadata_never_index"
for p in baseroms tools/ido tools/binutils .venv tools/objdiff; do
  [ -e "$root/$p" ] && ln -s "$root/$p" "$dest/$p"
done
# Submodules: clone from this repository's own module store (no network),
# so the lane's git status stays clean. A symlink here makes git complain
# that it "expected submodule path not to be a symbolic link".
for p in tools/asm-processor tools/asm-differ tools/m2c; do
  if ! git -C "$dest" -c protocol.file.allow=always \
      -c "submodule.$p.url=$common/modules/$p" \
      submodule update --init --quiet "$p"; then
    echo "submodule init failed for $p; refusing a dirty symlink fallback" >&2
    exit 1
  fi
done
# The permuter checkout is outside the repository; tools/permute.sh expects
# tools/permuter to point at it (git-ignored, machine-specific).
[ -e "$root/tools/permuter" ] && ln -s "$(readlink "$root/tools/permuter" || echo "$root/tools/permuter")" "$dest/tools/permuter"
restored=0
if [ "$cache" = 1 ] && [ "$extract" = 1 ]; then
  set +e
  (cd "$dest" && python3 tools/lane_cache.py restore --quiet)
  cache_status=$?
  set -e
  case "$cache_status" in
    0) restored=1 ;;
    3) ;; # no exact-commit cache: use the ordinary extraction path below
    *) echo "verified lane-cache restore failed" >&2; exit "$cache_status" ;;
  esac
fi
if [ "$extract" = 1 ] && [ "$restored" = 0 ]; then
  (cd "$dest" && gmake extract >"$dest/.lane-extract.log" 2>&1) || {
    echo "extract failed, see $dest/.lane-extract.log" >&2; exit 1; }
fi
echo "$dest"
