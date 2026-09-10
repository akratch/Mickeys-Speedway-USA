#!/usr/bin/env bash
# Run the final link, and on failure say what the failure usually means.
#
#   tools/link_diagnose.sh <ld> <ldflags...> -o <out>
#
# A fresh lane worktree cannot link until `gmake overlay-syms` has run: an
# overlay is linked at 0xF0000000 and cannot `jal` a 0x8000xxxx entry point
# directly, so it must reach it through the generated *Reloc surface, and
# tools/reloc_surface.py writes those names into the compiled overlay objects
# only when overlay-syms runs. Until then the link reports undefined resident
# references and R_MIPS_26 overflows, which read like a defect in someone's
# source and are not.
#
# Four lanes in one campaign lost time to this. It is documented in CLAUDE.md
# and it was still costing people, because nobody reads a build-setup note
# while staring at a linker error. So the error says it now.
#
# The success path is untouched: this passes the linker's own output through
# and exits with its status.
set -uo pipefail

log=$(mktemp -t link-diagnose)
trap 'rm -f "$log"' EXIT

"$@" 2>&1 | tee "$log"
status=${PIPESTATUS[0]}
[ "$status" -eq 0 ] && exit 0

# Only advise when the failure carries this signature; a genuine link error
# should not be buried under a guess about build state.
if grep -qE 'R_MIPS_26|relocation truncated|undefined reference' "$log"; then
    cat >&2 <<'MSG'

link failed with undefined resident references or R_MIPS_26 overflows.

This is almost always build state rather than source. Overlays reach resident
entry points through the generated *Reloc surface, and tools/reloc_surface.py
writes those names into the compiled overlay objects only when overlay-syms
runs. A fresh worktree has never had it run; `gmake extract` and any edit to
symbol_addrs discard it again.

    gmake overlay-syms && gmake

If that fixes it, nothing was wrong with the source. See "Promoting an overlay
function" in CLAUDE.md.
MSG
fi
exit "$status"
