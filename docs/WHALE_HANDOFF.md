# Whale handoff — 2026-09-16 (wv-v), at 11 words, unforced

`func_overlay_058_F000138C_18B0574`, 14,456 bytes, the largest unmatched
function in the tree. Seven bounded lanes have taken it **187 → 11 masked
words at delta zero**, unforced. This file is the resumption point; the
standing plan is `docs/WHALE.md` and the lane reports it links.

## State

    masked                11 / 3,614 words   (99.7% of words byte-exact)
    raw                  188
    delta                  0
    frame              0x138
    relocations        1,253
    buckets      3603 exact / 11 naming / 0 immediate / 0 structural
    displacement tax       0
    first mismatch    +0x12E4, the &D_o058_5EA0 fragment's head piece (W)

Verified with `tools/score_symbol.py` and `tools/align_symbol.py` on the
tree source. The C remains guarded `NON_MATCHING`; nothing is promoted.
Source on `lane/wv-v`; the report is
[whale-index-carriers.md](whale-index-carriers.md) and the cells are under
`cells-wv-v/`.

Lane `wv-v` (2026-09-16) closed case 13 and the `+0x0` cascade together
by changing *which symbol* the loop-index resets are written on. The
seven folded resets are dead defs that uopt colours as one web per
symbol; on a symbol of their own they take `a0` (nothing forbids it and
they cannot be decided later), so they are written on locals that are
dead where the loops run and already coloured harmlessly elsewhere:
`textY` (`s4`) for cases 3/13 and `letter1` (`s1`) for cases 1/2. The two
groups cannot share a carrier because the `D_o058_5EF8` address web spans
the entry loop and both draw loops while `textY` is case 1's y.

Lane `wv-u` (2026-09-15) itemised the case-12 identity and derived the
case-13 roles ([whale-split-order.md](whale-split-order.md)); lane `wv-t`
built the split instrument ([whale-split-growth.md](whale-split-growth.md)).
Both readings stand.

## What remains: 11 aligned rows, all naming

1. **`&D_o058_5EA0`'s fragment W in case 12** (9 rows, +0x1280..+0x1500):
   `a2` where the target has `t0`. On the 11 body the identity reads as
   wv-u itemised it: the `&D_o058_5E9C` piece seeded at 183 splits at idx
   127 (3.60) and accepts the title loop's call block 185 at margin 0
   (`left 12`, `numintf 24`); `w40` (the `n*2` temp, 3.0) takes `v1` at
   136; W's remainder splits at 178 (2.26) and rejects 185 by one (`left
   11`, `numintf 23`). Both need 12 colours left at 185; they differ by
   the `v1` decided between them.
2. **`saves + index*32` operand order in cases 9/10** (2 rows). Not the
   carrier (deleting `saves` costs 10-18 rows) and not the expression
   (wv-r's fourteen spellings).

## The next cycle, named by wv-v

**Put `w40`'s decision after W's split.** With any carrier web at 183 the
`&D_o058_5E9C` piece needs 17 colours at its seed and W needs 16, and the
only colour that separates them is the `v1` w40 (3.0, idx 136) takes
between the piece's split (127, 3.60) and W's (178, 2.26); with it free
for W, W accepts 185 and colours `t0` with the target's extent, and the
piece is unchanged. The neighbour captures under `cells-wv-v/` name every
member. Three things are measured closed on the 11 body: moving w40 off
`v1` (w251 takes it instead; cycle 7); a carrier decided after both, even
one live at 191 (the piece keeps 13 colours through 185, rejects 191 by
one and takes 202: +4, wv-t's `k` again; cycle 8); and pushing w40
*before* the piece (wv-u: the piece loses its 185 fold and swallows 202).
What is left is either `save(w40) < 2.26` -- impossible by occurrence
count, since `nocs = f(reference blocks + live-in blocks)` keeps a
two-block temp at 6/2 whatever is added, and only a live-in span the
target also has could lower it -- or the `&D_o058_5EA0` remainder above
3.0 at its 183 seed (95/42 now, because the seed walk carves case 13's
229/234/243/245/275 pieces, 90 of its 185, before case 12's). That second
handle is a seed-order or weight question on the 5EA0 lineage, and the
aliasing route to it (`(&D_o058_5E9C)[1]`) changes which register a load
uses wherever the two lineages hold different pieces, so it is not
zero-width where it would count. Everything wv-u closed for this lever
still holds. If a lane takes this, read `cells-wv-v/allocator-cell-c6a-detail-*.log`
with `intfdiff.py` first and price the seed order before any cell.

The two slot rows are below the expression: wv-r's fourteen and wv-v's
seven spellings, including the explicitly reversed sum and an index
through a dead local, are one object. Whatever orders `saves` first in the
target is not the source of the sum.

## The cycle wv-u named (done)

**Colour the seven-reset web harmlessly without a force.** Done by wv-v:
the web's colour follows its symbol, so the resets are written on `textY`
and `letter1`; see [whale-index-carriers.md](whale-index-carriers.md).

## The cycle wv-t named (done)

**A web living in blocks 183/184/185/190 that is coloured `v1` before the
`&D_o058_5E9C` remainder splits at save 3.60.** wv-u found the flip is
real (two head probes take w40 to 4.0 and the piece rejects 202 with
`left 12`) and that no zero-width occurrence exists to produce it: probes
insert blocks that the target's interleaved block 183 forbids, aliasing
through `&D_o058_5E98` splits the `D_o058_5E9C` value web or poisons alias
analysis, and bare expression statements are dropped by cfe.

## The cycle wv-s named (done)

The per-web block-set dump and the growth records, built by wv-t
(`webblocks`, `seed`, `grow`, `growv`, `livbb`; `lineage_census.py --web`);
see [whale-split-growth.md](whale-split-growth.md).

## The governing principle, and why this session moved

Six earlier passes held at 187 by asking which colour a value wanted. That
is the wrong question.

**uopt colours a symbol as ONE web across the whole function, so which cases
a variable spans fixes the decision order.** Decision order is strictly
descending `save` among coloured webs — all 46 inversions are split
fragments — so every remaining swap is a *ratio pair* you change by changing
the arithmetic, not by asking for a register.

Three of the four wins follow directly from it:

- `textY` spans cases 1/2/8/9/10 and is decided early into s4. The target's
  case-2 rank difference carries a *late* web's colours, so `columnX`
  (cases 9/12/13) is the carrier that lands in s1. **−10 words.**
- `opponent` and `columnX` exchange roles in cases 12/13, both keeping their
  colours: the target's column X *is* its s0 variable and its inner index
  *is* its s1 variable. The roles were assigned to the wrong variables.
  **−22 words.**
- A declared carrier hides the web the target needs (L160); delete the
  declaration and let strength reduction generate the value. **−3 words**,
  and six other functions matched on this mechanism this week.

## Axes that are closed, with receipts

Do not re-dispatch against any of these.

| axis | finding |
|---|---|
| `p2` forces | zero `phase=p2` records on this procedure |
| joint `wA+wB=cN` forces | `uopt` refuses the grammar, exit 55 |
| the colour axis, first order | landscape at 48: **zero winners at delta zero** |
| the colour axis, higher orders | third-order at 90 and second-order at 85 both found zero winners; 90 is the single-force same-kind floor on the 157 body |
| `portraitX` placement ratio | closed from every placement of either def; the pair stays 15.5 vs 11.67 |
| entry-loop index carrier | 7 carriers byte-identical — not an interference question |
| switch discriminant copy | the reload is aliasing-forced and the target has it too |
| compiler flags | settled at `-O2 -mips2 -32` on the nop/branch-likely census |
| case-12 capture constraint | 7 attempt families, 27 source cells |
| L160 at the case-3 cursor | already compiler-generated; forcing its register costs 4 bytes through lost transition-address sharing |
| the case-12 reset's placement | closed at 18: any dead carrier lands it in the delay slot; the residue is fragment splits, not the reset |
| interference tokens as a lever | zero-width in the zero-store block only; a load elsewhere, and a declared token costs nine rows unread |
| declared pointer cursors for case 12 | fresh, dead-role or shared: exact loop head, paid through `&D_o058_5E9C` or `rowBase` |
| case-13 coordinate spellings with `opponent` | 17 head/row-preheader spellings, guarded reset, nine tokens: the head piece stays shed |
| `saves` carrier deletion (cases 9/10) | 28-36; the operand order is below the records |

| a dead carrier web at 183 | the itemised identity: the two pieces share every window interferer but W's own remainder, colours differ by `v1`/`a0`/`a1` timing only; no such body passes both tests |
| zero-width occurrences | `if` probes insert two blocks each and every web live across them gains `nocs`; probes after a reset are not folded; bare expression statements are dropped by cfe |
| aliasing `D_o058_5E9C` through `&D_o058_5E98` | case 2/3 loops: 3.20 and 39 rows or a -16 fold; the head read: splits the `a0` value web (+4); the row-loop negate: alias analysis poisoned |
| pointer walks in cases 1/2 | +24..+56 and the frame moves; the frame census is identical on both sides |
| the case-13 coordinate as any web but `opponent` | a real def counts an occurrence, so no coordinate beats the cursor's 31/2; portraitX can never be `s0` (`w26` holds it at blocks 9 and 85) |
| the case-13 row loop on `i` | exact locally (36 at delta 0) but splits the `D_o058_5EE0[portraitX]` SR temp shared with case 3: 20 rows there |
| a reset outside its preheader | before/inside a head call or above the if/else join: the index stays live into the loop, +12..+36 (wv-v cycle 1) |
| a dead-def web pushed later | `nocs = f(reference blocks)`, so n dead defs give n/(((n-2)>>2)+2) >= 2.33 and the web is always decided before `w222`, `w9` and the `a2` piece; only its symbol decides its colour |
| one carrier for all seven resets | the `D_o058_5EF8` address web spans the entry loop and both draw loops (splitting it moves the entry cursor `s3` -> `s2`), and `textY` is live in case 1's draw loop: two carriers are required |
| `s0` locals as the cases-1/2 carrier | `letter0`, `portraitIndex`, `countdownX` find `s0` held at 9/85, take `s1`, and flip their own case: 15 |
| freeing `v1` for W by moving w40 | w251 (2.33) takes `v1` instead; W's growth byte-identical (wv-v cycle 7) |
| a late case-12 carrier, also live at 191 | W t0 but the 5E9C piece keeps 13 colours, rejects 191 by one and takes 202: +4 (wv-v cycle 8, wv-t's k) |
| deleting the unused `s32 portraitX;` | 284 at delta 0: an unused declaration's symbol number orders later web numbers and their tie-breaks |
| the slot-address sum's spelling | wv-r's 14 and wv-v's 7 (pointer arithmetic, `(u32)`, byte arithmetic, reversed order, dead-local index) are one object |

`p1:wN=s` split forces **are** honoured and `force_lattice.py` parses the
receipt, but every split measured so far costs width.

## Tooling built this session

- **`web_footprint.py --hold`** — the second-order landscape. Holds a force
  set applied while probing every other web, reading the probe plan from the
  *held* baseline's own trace, because held colours change which colours the
  other webs are offered. `--trace` is refused alongside it for that reason.
  Every landscape before this was first-order. It found 12 winners invisible
  first-order on its first run.
- **`lineage_census.py`** — the split axis. Use `bb`, never `line`: the
  member `line` is a constant 8 on this procedure and `webdetail.line` is −1
  for 330 of 395.
- **`draw_census.py`** — the ugen freelist/emission trace as a per-line
  census. A draw is `ALLOC_GP_RESULT`/`ALLOC_FP_RESULT` only.
- **A landscape freshness guard** — every report stamps the ranking's
  `source_context_sha256`, and `--report` exits nonzero with `STALE:` when
  the tree no longer matches. Five consecutive lanes once measured colours
  against a source that had moved underneath them; that is now an error
  message rather than a judgement call.

## Banked resources

`.git/whale-resources/`, outside every worktree, never tracked. Complete
instrument sets at **18, 48, 147, 157, 160, 169, 187** (footprints, landscape,
lattice, lineage, draws, residual, allocator), plus the held landscapes
`157hold5`, `157hold7`, `147hold7`, wv-r's cell tables under `cells-wv-r/`,
and wv-t's growth-profile traces: `allocator18-growth.log`,
`allocator48-growth.log`, the `allocator18-cell-*.log`/`cell18-*.c` pairs,
three `allocator18-detail-w*.log` neighbour captures, `uoptlist18.txt`, and
`cells-wv-t/` (every wv-t cell with its source and the scratch harness).
Lane wv-v adds `cells-wv-v/`: every cell, result and trace, the 16 / 15 / 11
objects, and the readers `readsym.py`, `early.py`, `mkcarrier.py`. Lane wv-u adds `cells-wv-u/`: every cell's source, result and trace, the
reader scripts (`readW.py`, `intfdiff.py`, `peels.py`, `colourdiff.py`,
`case13.py`), the neighbour captures on the pointer and opponent bodies, the
case-13-exact 36 object, and the forced 11 objects `forced11-w22a3.o` /
`forced11-w22s4.o` with their unforced twin `c15-unforced120.o`. **Re-running a banked measurement is wasted budget**, and the
freshness guard will say so if a landscape has gone stale.

## Two repository faults fixed, worth remembering

- **`tools/merge_lane.sh` piped its own clean-room gate into `tail -1`** —
  the exact footgun `CLAUDE.md` documents, living in the integration tool.
  A failing scan reported `tail`'s status, always 0, so it read as a pass
  and the findings scrolled away, leaving one line of remediation advice
  with no finding above it. Now captures output, branches on the real
  status, and prints the whole scan on failure.
- **`nm_ranking.py --write-doc` is documentation mode and does not compile.**
  It validates the existing JSON and rewrites the Markdown from it, so
  running it against a stale row is a no-op that looks like a success. The
  measuring form is the default mode, then `--write-doc` for the document.

## Open housekeeping

- The per-pass shard `docs/matching-triage-handoffs/func_overlay_058_F000138C_18B0574.md`
  crossed the 262,144-byte clean-room `oversize` limit and now carries a
  `CONTENT_EXEMPTIONS` entry. The exemption is narrow — that path, that rule
  — and the word-table, hex-run and mnemonic-density detectors still apply to
  the file, but it grows every pass and should be split, with superseded
  prose archived to a sibling, before it doubles.
- `origin` reports that it has **moved to
  `git@github.com:akratch/Mickeys-Speedway-USA-Decomp.git`**. Pushes still
  succeed by redirect, but `CLAUDE.md` names the old URL and the remote
  should be updated before the redirect stops being honoured.

## The standing arithmetic

The whale is worth **14,456 bytes**, and credits **zero** until it fully
matches — a partial match banks nothing. At 11 words the function is 99.7%
matched by word, every remaining row is naming, and two named clusters --
W's nine and the two slot rows -- are all that stand between here and the
largest single byte gain available in the tree.
