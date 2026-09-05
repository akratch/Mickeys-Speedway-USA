# Raw assembly ownership census

Run `gmake check-raw-asm` before populating a raw-assembly carve queue, or
`.venv/bin/python tools/raw_asm_census.py --json` for exact range identities.
The tool checks the baserom against the configured SHA1 and derives extents
from parsed splat subsegments, including dictionary-form and unnamed rows.
It never prints instruction text or ROM bytes.

The overlay gate fails for every raw range except explicitly named padding
or tails whose bytes are all zero and whose end is a text/data or module
boundary. A small nonzero tail remains work. A zero-filled unnamed range also
remains work: zero bytes alone do not establish padding ownership. Missing,
reversed, unaligned, or out-of-ROM boundaries fail rather than disappear.

Resident raw ranges are reported separately. The existing
`verified_asm.us.txt` ledger identifies handwritten subsegments; the remainder
are **unclassified**, not automatically compiler-generated or ready to carve.
Review the YAML evidence and exact function boundaries before assigning them.
Reported subsegment bytes include alignment, unlike the scoreboard's
function-owned executable bytes. Neither raw ownership nor a new C scaffold
earns matching credit.

An unclassified census row is a gap in this tool's ledger coverage, not
necessarily an unexplored decompilation opportunity. Consult the settled
odd-FP/handwritten exclusions in [modules.md](modules.md#62-odd-floating-point-registers-settled--those-files-stay-assembly),
the YAML ownership comments and the function evidence in
[resident.md](resident.md) before scheduling work. A reference object's exact
assembly fallback is not evidence that a compiler can emit that body. Likewise,
padding established by an adjacent function's boundary proof must not become
a new tiny-function assignment. Keep these distinctions explicit rather than
loosening the census gate or adding unproved matching credit to its ledger.

The gate requires the local baserom and is separate from source-only
`check-docs` and clean-room hooks. Synthetic census regressions run through
`gmake check-tooling`. Matching and ROM proof remain governed by ADR 0001 and
the carve-before-decomp workflow in ADR 0016.
