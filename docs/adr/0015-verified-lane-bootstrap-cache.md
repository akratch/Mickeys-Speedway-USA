# 0015. Verified lane bootstrap cache

Status: Accepted
Date: 2026-08-30

## Context

Short-lived matching lanes repeatedly split the same ROM-derived `asm/` and
`assets/` inputs and then cold-build the same object graph. Those outputs are
ignored and cannot be committed, while sharing them as writable symlinks would
break ADR 0004's build isolation. Reading a sibling lane to seed another would
also violate ADR 0011's ownership boundary.

The integration commit already receives a clean ROM-exact proof. That proof can
publish an immutable snapshot below Git's common directory, where it is neither
tracked nor owned by a sibling worktree. APFS can clone the snapshot into a new
lane without duplicating its physical blocks; later writes remain lane-local.

## Decision

- `tools/lane_cache.py publish` accepts only a tracked-clean worktree, runs
  `gmake verify`, checks the rebuilt ROM digest, and writes an atomic cache keyed
  by the exact `HEAD` commit below Git's common directory.
- The cache contains only the allowlisted ignored split and verified build
  prerequisites. Its manifest records the schema, commit, ROM digest, proof
  command, and paths. It contains no source and is never tracked.
- `tools/new_lane.sh` restores only a cache whose commit and ROM identity match
  the new lane exactly. Restore refuses existing destinations, missing files,
  unknown paths, tracked dirt, or a stale Make graph.
- Restoration copy-on-write clones files on APFS and copies them normally on
  other filesystems. No directory is shared writable between lanes.
- When no exact-commit cache exists, lane creation retains its ordinary splat
  extraction behavior. `--no-cache` explicitly selects that path, while
  `--no-extract` continues to create a source-only lane without restoring.

## Consequences

- A newly assigned lane can begin from the last verified integration build
  without repeating extraction or a full unchanged build.
- Every source edit still invalidates and rebuilds its normal Make targets;
  `verify` remains required for every promotion and integration.
- The cache does not weaken clean-room rules or match evidence. It is ignored,
  ROM-derived workstation state and may be discarded at any time.
- A proof worktree should publish after a successful integration gate so the
  next wave has a warm exact-commit snapshot.
