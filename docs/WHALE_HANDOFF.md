# Whale handoff — 2026-09-14, at 48 words

`func_overlay_058_F000138C_18B0574`, 14,456 bytes, the largest unmatched
function in the tree. This session took it **187 → 48 masked words at delta
zero** across four bounded lanes. This file is the resumption point; the
standing plan is `docs/WHALE.md` and the four lane reports it links.

## State

    masked                48 / 3,614 words   (98.7% of words byte-exact)
    raw                  221
    delta                  0
    frame              0x138
    relocations        1,253
    draws                720, original register sequence, 5,076 emissions
    buckets      3597 exact / 11 naming / 1 immediate / 8 structural
    first mismatch    the gap pair, candidate +0x12F8 / target +0x1260

Verified with `tools/score_symbol.py`, which uses the same comparator the
ranking uses. `config/nonmatching-ranking.us.json` reports `stale=0` over
298 of 298 rows. The C remains guarded `NON_MATCHING`; nothing is promoted.

Landed at `e05e53ee` on `master`. Tree clean, both branches pushed.

## What remains: 16 aligned rows in three named clusters

1. **The case-12 gap pair** (7 rows) — candidate `+0x12F8` against target
   `+0x1260`. Worth roughly 32 *positional* words under L155: a one-word
   insertion makes every following word mismatch positionally while aligning
   perfectly. Three lanes have now failed to reach it; lane wv-r's cycle 9
   closed it at a cost of 120 colour rows, so it is reachable and expensive.
2. **The case-13 first-loop s0/s1 pair** (7 rows).
3. **`saves + index*32` operand order in cases 9/10** (2 rows).

## The next cycle, named by wv-r

The case-13 first-loop pair, by the same principle that won 63 and 48. The
residual of `coord-countdownX` names `+0x17CC`/`+0x17E4` as target-only and
`+0x199C`/`+0x1A40` as candidate-only: the coordinate's increment leaves the
loop tail and the row loop gains two words whenever the coordinate is not
`portraitX`. The question is which live role `portraitX` plays that the
target's coordinate variable also plays — and giving that role's *neighbour*
to the variable rather than swapping the coordinate.

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
instrument sets at **48, 147, 157, 160, 169, 187** (footprints, landscape,
lattice, lineage, draws, residual, allocator), plus the held landscapes
`157hold5`, `157hold7`, `147hold7` and wv-r's cell tables under
`cells-wv-r/`. **Re-running a banked measurement is wasted budget**, and the
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
