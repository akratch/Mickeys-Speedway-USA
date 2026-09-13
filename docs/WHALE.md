# Harpooning `func_overlay_058_F000138C_18B0574`

14,456 bytes, the largest unmatched function in the tree and **63% of the
remaining distance to 60%**. Nine passes have taken it 733 → 227 → 217 → 187,
and five consecutive passes have now held at 187. This document is the plan for
the next phase: what it actually consists of, every lever still available, and
the tooling that makes each lever readable.

`docs/matching-triage-handoffs/func_overlay_058_F000138C_18B0574.md` remains the
per-pass log and the source of truth for what has been tried. This is the map.

## Wave D: target width and reduced pressure coexist

[The wv-d source witness](whale-width-pressure.md) takes the previous attempt
23 to target width with unchanged stack traffic and address interference
98–119. It scores 1,446 masked words and changes the draw sequence; the guarded
187 body and its resource profiles below remain the best retained residual.
The complete alternative source and proof packet are preserved privately.
No new executable bytes are matched, and the original address family still
has no naturally coloured pair.

## What 187 is made of

Measured 2026-09-13 on the retained source, delta zero, frame `0x138`, first
mismatch `+0x50`, 1,253 relocations.

    positional masked        187
      aligned rows           155   = 134 naming + 1 immediate + 20 structural
      insertion shadow        32   from the ONE remaining gap pair

**Three of the four original insertion pairs are closed.** What remains is a
single pair: target-only `+0x1260` against candidate-only `+0x12F8`. It is worth
about 32 positional words and nothing else in the function depends on it.

Aligned rows by region (0x400 windows), naming first:

    +0x0000  36 rows  34 naming  2 structural   <- the largest single block
    +0x0800  21 rows  21 naming
    +0x1400  20 rows  18 naming  2 structural
    +0x1C00  13 rows  13 naming
    +0x0400  12 rows  12 naming
    +0x1000  14 rows   9 naming  1 imm  4 structural
    +0x2400   9 rows   7 naming  2 structural
    +0x3000   6 rows   2 naming  4 structural

The `+0x0000` block is a closed four-cycle over the save ring —
`s0 → s2 → s3 → s1 → s0` — with 34 naming rows behind it. One rotation explains
the biggest block in the function.

The ugen census is **720 draws and 5,072 emissions**, with two draws at the
coordinate line and zero at the capture line.

## The thing to undo first

**Every colour landscape on record was measured against a source that no longer
exists, and the last five whale briefs told the lane not to re-run it. That was
my error.**

- The complete 1,875-probe landscape was measured at **227**.
- A second complete landscape, 1,914 probes across 142 coloured webs, was
  measured at **207**, and its best disjoint packing measured **162** with six
  forces (w75→c16, w379→c20, w225→c14, w320→c18, w27→c17, w506→c15).
- The source is now **187**, and its colour axis has never been measured.

The standing rule is already correct — a landscape is void once the source
changes — and I contradicted it in five consecutive dispatch briefs on the
grounds that the axis was "closed and proved". It was closed and proved *for a
different function body*. A fresh landscape at 187 is the first action of the
next phase, not a formality: the 207 landscape found a 45-word forced gain, and
the source has moved twice since.

## The lever nobody has pulled

**The allocator makes 395 recorded decisions on this procedure. 141 are
`decision=color`. 254 are `decision=split`.** Every force this campaign has
ever applied — 1,875 probes, then 1,914, then every lattice — has addressed only
the 141. `CDX_FORCE=p1:wN=cM` sets a colour; it has nothing to say to a web the
allocator decided to split.

And the blocker is stated in split terms. The last four passes converged on:

> removing a capture range fixes the **first split**, but a **later split** then
> takes the restore; reuse has to be preserved across **both successive splits**
> while holding draws and target size.

The split decisions carry the pressure, too: their median `numintf` is 92
against 24 for coloured webs, and the most congested sit at `numintf=140` with
`regsleft=6`. The admission bound the passes keep hitting — "admits the row
entry at 24 interference entries and rejects restoration at 25", "admits
restoration at 26 against its 26-entry bound" — is this number, and it is
recorded per web in the `p1dec` rows that nothing currently reads.

**This is 64% of the allocator's decisions on the tree's biggest function, it is
where the blocker lives, and it has never been measured.**

## The instrument that already exists and is unused

The instrumented `uopt` exposes six knobs: `CDX_LOG`, `CDX_OUT`, `CDX_PROC`,
`CDX_DETAIL_WEB`, `CDX_FORCE`, and **`CDX_LINEAGE_TABLES`**. The campaign uses
five. On this function `CDX_LINEAGE_TABLES=all` emits:

    lineage_range    204 rows   proc event table chain type dtype sym exprtable exprchain
    lineage_member  1514 rows   ... plus bb, line, flags

`event` is a monotonic sequence number, so these rows are the **web creation
order**. L154 says a web's number follows its type first and its first use
second, and that numbering decides colouring order — so creation order is the upstream cause of the colour
questions the landscapes keep measuring downstream. Nothing in `tools/` parses
these records. One earlier handoff spotted the possibility and said a lineage
capture would let a lane "screen spellings on the trace instead of the score";
no lane has.

204 lineage ranges against 254 split decisions is not a coincidence worth
ignoring.

## First readings from the new instruments (2026-09-13)

`tools/lineage_census.py` now exists and its first run on this function says:

    395 webs, 395 decisions: 141 coloured, 254 SPLIT
    204 lineages, 35 of which became more than one web
    numintf median: coloured 24, split 92

    the five largest lineages, by webs they became:
      event  members  webs  split/coloured  first basic blocks
         37       70    29     29 / 0       15,19,21,27,43,44 (+64 more)
         45       53    28     26 / 2       17,19,21,27,43,44 (+47 more)
         39       47    21     14 / 7       15,17,57,58,91,93 (+31 more)
        212       43    20     13 / 7       78,108,110,111,114,129 (+29 more)
        223       40    20     14 / 6       79,91,95,97,99,105 (+26 more)

**One lineage became twenty-nine webs and every one of them was split.** The
top five account for 118 of the 395 webs. The most contended webs sit at
`numintf=140` with `regsleft=6`, all split, and they cluster: webs 1098, 1104,
1172, 1173, 1181, 1183, 1185, 1186 all carry identical pressure, which is the
signature of one live range fighting one region rather than eight separate
problems.

That is the shape of the thing. It was invisible before today.

## Levers, ranked by expected value

1. **Measure the split axis.** What the 254 splits are, where they fall, which
   two are the "successive splits" in the blocker, and what the interference
   bound is at each. Needs the lineage reader below. No source change required
   to get the first real picture.
2. **Fresh colour landscape at 187**, then the packing, then measure the packed
   set. The 207 run bought 45 words; assume nothing, measure it.
3. **Higher-order forces.** Every landscape to date is *single*-force. Two webs
   forced jointly, and any `p2` force at all, are explicitly outside all of
   them. The blocker being a *pair* of successive splits is exactly the shape a
   single-force landscape cannot see.
4. **The last gap pair**, `+0x1260` / `+0x12F8`, worth about 32 positional
   words. Read with `draw_census`: an extra instruction is an extra emission at
   a line.
5. **The `+0x0000` ring rotation**, 34 naming rows behind one four-cycle
   `s0 → s2 → s3 → s1 → s0`. The largest single block, and a rotation is the
   shape L127/L145 are about.
6. **The interference bound itself** — whether anything in source can lower
   `numintf` at the restoration site from 25 to 24.

## Tooling: all three are built (2026-09-13)

### 1. `tools/lineage_census.py` — built, 22 tests

Parses `CDX_LINEAGE_TABLES=all` and reports, per procedure:

- **creation order**: every lineage in `event` order with the basic blocks it
  touches, so "which of these two values is numbered first" is a lookup rather
  than an inference. This is the direct readout L154 has been asking for.
  **Positionally, use `bb` and not `line`**: the member `line` field reads 8 for
  all 1,514 rows on this procedure -- a constant -- and `webdetail`'s is -1 for
  330 of its 395. Anything attributing a lineage to a source line off these
  records is reading nothing; `draw_census` is where lines come from.
- **the split picture**: every `decision=split` web from `p1dec` joined to its
  lineage range and members — where it splits, into how many members, and at
  which lines.
- **the interference bound**: `numintf`, `regsleft`, and the decoded
  `forbidden`/`available` masks per web, so "admits at 24, rejects at 25" is a
  number you can look at rather than a symptom you infer.
- `--compare before.json after.json`, like `draw_census`, so a source edit is
  judged by whether it changed the creation order or the split structure.

This is the analogue of `web_footprint` for the phase upstream of colouring, and
it is the single highest-value tool left to build. It needs no new compiler
work — the records are already emitted and thrown away.

### 2. Higher-order forces in `tools/force_lattice.py` — built, 58 tests

The grammar now accepts a **joint force**, `p1:w27+w75=c17`: two or more webs
driven onto one colour in the same compile. Validation compares *webs* rather
than whole spec strings, so a web appearing in both a joint and a single force
is refused, and acceptance requires every web the force names -- a joint force
that lands on only one of its two webs is reported as not applied, because
reading its score as the pair's would attribute a single force's result to a
coupling that never happened. `p2` was already in the grammar and has still
never been exercised on this function.

### 3. A landscape freshness guard — built, in `web_footprint`

A landscape is void when the source changes, and nothing enforces that. `web_footprint` now stamps the ranking's `source_context_sha256` into every
report it writes, and `--report` prints a `STALE:` line and exits nonzero when
that hash no longer matches the tree. A report written before the stamp existed
is flagged too, because it cannot be vouched for. The mistake this document
opens with is now a line of output rather than a judgement call.

## Wave A is done: the blocker has a number (2026-09-13)

`lineage_census` read the split axis and it answers the question the prose
constraint could not.

**Colouring collapses as interference rises**, measured across all 395 webs:

    numintf   0- 40   149 webs   127 coloured   85%
    numintf  40- 70    66 webs    11 coloured   16%
    numintf  70-100    78 webs     2 coloured    2%
    numintf 100-130    72 webs     1 coloured    1%
    numintf 130+       30 webs     0 coloured    0%

**The two successive splits are identified**, and both span exactly the
coordinate and capture sites at source lines 632-665:

- **lineage event 37** -- 70 members became **29 webs, all 29 split**, at
  `numintf` 131-140, `regsleft` 6-7. Nothing in the 130+ band is ever coloured,
  so as the source stands those 29 webs have a structurally zero chance.
- **lineage event 45** -- 53 members became 28 webs, 26 split and **two
  coloured**: web 1063 at `numintf=7` and web 1065 at `numintf=35`, both in the
  band where 85% get a register.

That contrast is the finding. Lineage 45 keeps registers because two of its webs
are cheap; lineage 37 keeps none because all of its are expensive. The bound the
last six passes kept hitting -- "admits at 24, rejects at 25", "26 against 26" --
is this pressure, and it was recorded per web the whole time.

**So the objective is now a measurement rather than a prose constraint: reduce
`numintf` at lines 632-665 toward 40.** `lineage_census --compare` says whether
an edit moved it, which the score does not -- six passes moved the score by zero
while the axis went unread.

The lines themselves come from joining each lineage's basic blocks to the
emission records' `block`/`line` fields; `bb` is positional and the lineage
`line` field is a constant, so that join is the only way to put a lineage on a
source line.

## The campaign: resources are gathered, do not re-derive them

Everything below was measured on 2026-09-13 against the current 187 source and
is retained under Git's common directory at **`whale-resources/`** — outside the
worktree, never tracked, and small enough (7.8 MB) to keep indefinitely:

| file | what it is |
|---|---|
| `footprints187.json` | the complete 1,898-probe landscape at 187, with the packing |
| `landscape187.txt` | its rendered report: winners, rivals, the packed set |
| `lattice187.txt` | the measured six-force lattice, radii and every interaction |
| `lineage187.json` | the split census: 395 webs, 254 splits, 204 lineages |
| `draws187.json` | the ugen census: 720 draws, 5,072 emissions, per line |
| `residual187.txt` | the aligned residual map by address, with the register census |
| `allocator187.log` | the raw unforced trace, `CDX_DETAIL_WEB` + `CDX_LINEAGE_TABLES` |

A lane reads these. **Re-running any of them is wasted budget**, and re-running
the landscape is wasted twice over because it now carries a source fingerprint
and `web_footprint --report` will say so if it ever goes stale.

### The fresh landscape changed the picture

The re-run that this document opened by demanding has been done. At 187:

    1,898 probes across 141 webs, both save kinds
    10 forces beat the unforced 187 at delta 0
    best disjoint packing predicts 149 -- and the lattice MEASURES 149

    p1:w75=c16   172  (+15)      p1:w962=c17  183  (+4)   <- new web
    p1:w225=c20  177  (+10)      p1:w27=c17   184  (+3)
    p1:w379=c20  177  (+10)      p1:w498=c15  184  (+3)   <- new web
    p1:w225=c14  180  (+7)

**The forced diagnostic is 149, not 187**, and two of the productive webs — 962
and 498 — appear in no earlier landscape, because no earlier landscape was
measured against this body. The measured best set is five forces:

    w75=c16 + w379=c20 + w225=c14 + w27=c17 + w498=c15  ->  149

Radii, for nominating anything further:

    w75=c16    0x0180 0x0280 0x0300 0x0380 0x0580 0x0880 0x0980 0x0a00 0x0a80 0x0b80
    w379=c20   0x1280 0x1700 0x1780 0x1900
    w225=c14   0x0780
    w27=c17    0x0080
    w498=c15   0x1c00 0x1c80 0x1d00
    w962=c17   0x0180 0x0300 0x0480 0x0580 0x0600 0x0680 0x0c00 0x0c80 0x1e00 ...

`w225=c20` and `w379=c20` remain rivals on one radius. `w962` is antagonistic
with `w75` (+43, contending in `0x0180, 0x0300, 0x0580`), which is why the
packing takes `w498` instead.

### Wave structure, in order, one lane each

**Wave A — the split axis, no source change.** Read `lineage187.json` and the
`allocator187.log` it came from. Answer: which two of the 254 splits are the
"successive splits" in the blocker; what the interference bound is at each; and
whether the 29-web lineage at event 37 is the live range the whole thing turns
on. The deliverable is a map, not a match. Nobody has ever looked at this.

**Wave B — the coupled repair, using joint forces.** `force_lattice` now accepts
`p1:wA+wB=cN`. Take the two webs Wave A names and drive them onto one colour
together. 3,789 single-force probes could not see a coupled repair; this is the
first instrument that can. If a joint force reaches a materially better score
than 149, that names the source question exactly.

**Wave C — the last gap pair.** Target `+0x1260` against candidate `+0x12F8`,
about 32 of the 187 positional words, and the only one of the original four
still open. An extra instruction is an extra emission at a line, and
`draws187.json` is the before-profile: take an after-profile and `--compare`.

**Wave D — the `+0x0000` block.** 34 naming rows behind one closed four-cycle
`s0 -> s2 -> s3 -> s1 -> s0`, the largest single block in the function, and the
landscape's `w27=c17` only reaches `0x0080` of it. A ring rotation is the shape
L127 and L145 are about.

Waves A and B are sequential; C and D are independent of both and of each other,
so they can run concurrently with A.

### What a whale brief must carry, and what it must not

Carry: the paths above, the measured 149 and its five forces, the radii table,
and the wave this lane is running. Nothing else — the shard has the rest.

Must not: re-run a landscape, re-derive the residual map, re-try any of the
seven exhausted case-12 attempt families, or re-try L160 at the case-3 cursor.
Each is recorded with evidence, and a pass that repeats one has spent its budget
learning what the shard already says.

## What not to spend the next pass on

- **Another variation on the case-12 capture constraint.** Seven attempt
  families and 27 source cells are recorded against it: capture-carrier,
  first-loop join, saved-local access, capture-type split, late coordinate
  roles, restore-exit distribution, nested-sum identity. Each was measured and
  each added draws or opened a replacement gap. Read them before proposing
  anything adjacent.
- **L160 at the case-3 cursor.** Ruled out with evidence: that cursor is already
  compiler-generated, and forcing its target register costs four bytes through
  lost transition-address sharing.
- **Re-deriving the colour landscape at 227 or 207.** Both are complete and on
  record. The one to run is 187.

## The honest alternative

If the split axis is measured and the coupled repair is still not reachable,
the right outcome is an argued bar, not another pass. The standard is the one
`overlay57UpdateModeState` meets: enumerate the space, and show why nothing
outside the enumeration reaches the target. A plateau, however stubborn and
however large the function, is not that — and this function has 155 aligned
rows, so any such argument has to account for all of them, not just the
transition reuse. Nobody has been close to that argument yet, which is itself a
reason to think the axis above is worth measuring first.
