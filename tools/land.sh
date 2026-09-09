#!/usr/bin/env bash
# Land integrated campaign work on master, the private canonical branch.
#
#   tools/land.sh
#
# Pushes campaign/unchain, merges it into master, re-verifies the ROM from
# the merge result, and pushes master. Run it after every integration batch;
# leaving master behind means someone merges it by hand later.
#
# Why the ROM is re-verified AFTER the merge and not before: the merge can
# combine two lanes' generated artifacts (the ranking, the atlas, the
# authorizations) in ways neither side produced alone, and `gmake verify` on
# campaign/unchain does not cover the merge commit's own tree.
#
# Never force-pushes. Never touches the `upstream` remote (that is Rare's
# public JFG repository, not ours) or the public mirror, whose release
# process is separate and documented in CLAUDE.md.
set -euo pipefail
cd "$(git rev-parse --show-toplevel)"

start=$(git rev-parse --abbrev-ref HEAD)
if [ -n "$(git status --porcelain --untracked-files=no)" ]; then
    echo "tools/land.sh: tracked changes present; commit them first" >&2
    exit 1
fi
restore() { git checkout -q "$start" 2>/dev/null || true; }
trap restore EXIT

echo "== push campaign/unchain"
git push origin campaign/unchain

echo "== merge campaign/unchain into master"
git checkout -q master
git fetch -q origin master
git merge -q --ff-only origin/master
git merge --no-edit campaign/unchain

echo "== verify the merge result"
gmake verify

echo "== push master"
git push origin master
echo "== landed: $(git log --oneline -1)"
