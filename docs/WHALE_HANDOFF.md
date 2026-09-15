# Whale handoff — 2026-09-15, at 18 words

`func_overlay_058_F000138C_18B0574`, 14,456 bytes, the largest unmatched
function in the tree. Five bounded lanes have taken it **187 → 18 masked
words at delta zero**. This file is the resumption point; the standing plan
is `docs/WHALE.md` and the lane reports it links.

## State

    masked                18 / 3,614 words   (99.5% of words byte-exact)
    raw                  195
    delta                  0
    frame              0x138
    relocations        1,253
    buckets      3596 exact / 18 naming / 0 immediate / 0 structural
    displacement tax       0  (the gap pair is closed)
    first mismatch    +0x12E4, the &D_o058_5EA0 fragment's head piece

Verified with `tools/score_symbol.py` and `tools/align_symbol.py`. The C
remains guarded `NON_MATCHING`; nothing is promoted. Source unchanged since
`lane/wv-s`; the instrument and its report are on `lane/wv-t`.

Lane `wv-t` (2026-09-15) held at 18 and built the instrument this file asked
for; read [whale-split-growth.md](whale-split-growth.md) before anything
else. The split is now a readable growth test (`lineage_census.py --web`),
and its verdicts explain all three fragments -- with an identity that rules
out every dead-carrier body for case 12. Its ledger and cycle 13 are in the
report; the section below is wv-s's reading and stays correct as far as it
goes.

## What remains: 18 aligned rows, all naming, three address-constant fragments

1. **`&D_o058_5EA0`'s fragment W in case 12** (9 rows, +0x1200/+0x1400):
   `a2` where the target has `t0`. W's extent is the target's; `a2` is
   *offered* because W's `numintf` is 25 on this body against 22 in base,
   and `c5` beats `c7` at equal cost. Forcing `t0` costs +64 (it is a ring
   temporary here). The three extra interferers are the dead carrier's web
   and two pieces of a neighbouring lineage; the records count them and do
   not name them.
2. **The case-13 first-loop pair** (7 rows, +0x1600). `opponent` as the
   coordinate lands both colours (the cursor takes `s1` on its own) and
   costs exactly one word: the `&D_o058_5E9C` fragment sheds its loop-1
   head piece. 27 cells of spelling, placement and tokens do not move it.
3. **`saves + index*32` operand order in cases 9/10** (2 rows). Not the
   carrier (deleting `saves` costs 10-18 rows) and not the expression
   (wv-r's fourteen spellings).

## The next cycle, named by wv-t

**A web living in blocks 183/184/185/190 that is coloured `v1` before the
`&D_o058_5E9C` remainder splits at save 3.60** (trace line ~21041 on the 18
body). That is the only lever the growth identity leaves: it takes the
dropped 183-piece's `left` at its 202 step from 13 to 12 without touching
W's, because W loses `v1` to `w40` anyway. Candidates: raise `w40`'s own save
above 3.6 (it is `n*2` at blocks 13 and 183, two weight-1 occurrences), or a
carrier whose loop-weighted occurrence survives dead-code elimination without
living across a call. Read `left_after` at the 202 step of the piece seeded
at 183 -- it must be 12 -- before believing a score.

The next paragraph is wv-s's cycle; it is done, and its readings are above.

## The cycle wv-s named (done)

**Extend the instrumented uopt to dump each web's block set** (the
`webdetail` rows carry `bb=-1`). Both fragment questions become readings:
which two webs beside `textY` interfere with W on the 18 body, and which
carve detaches `&D_o058_5E9C`'s head piece when the coordinate is
`opponent`. Each is then one zero-width edit. Without the instrument, the
source-side cycle is a `p1:wN=s` split force on W to read whether the
remainder's `numintf` drops below 24.

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
`cells-wv-t/` (every wv-t cell with its source and the scratch harness). **Re-running a banked measurement is wasted budget**, and the
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
matches — a partial match banks nothing. At 48 words that calculus has
inverted relative to the last pause: the function is 98.7% matched by word,
the colour axis is closed, and three named clusters of 16 aligned rows are
all that stand between here and the largest single byte gain available in
the tree.
