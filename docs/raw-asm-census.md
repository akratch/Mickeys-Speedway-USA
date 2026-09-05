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

The gate requires the local baserom and is separate from source-only
`check-docs` and clean-room hooks. Synthetic census regressions run through
`gmake check-tooling`. Matching and ROM proof remain governed by ADR 0001 and
the carve-before-decomp workflow in ADR 0016.
