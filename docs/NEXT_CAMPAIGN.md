# Next campaign: overlay 101, and the one-decision set

Scoped 2026-09-11 from `config/nonmatching-ranking.us.json`,
`tools/align_symbol.py` and `tools/frame_census.py`. Every number here is
measured; re-derive rather than trust them if the tree has moved.

Queue at the time of writing: **344 functions, 429,056 bytes**.

## The headline: overlay 101 is a campaign, not a cohort

**23 queued functions, 40,160 bytes — 9.4% of everything remaining — and every
single one is the last queued function in its own translation unit.** Every
match there completes a TU.

It also holds three clusters of identical-size siblings, and the transfer
mechanism is proven: a lane diagnosed one function of a triplet, applied the
result to the twins by line range with no per-function tuning, measured each
separately, and both came out *two words better* than the one it was diagnosed
on.

| cluster | bytes | masked each | state |
|---|---|---|---|
| 4 × 2,100 B | 8,400 | 311 | worked; instruction multiset equals the target's, opcode for opcode; frame ladder identical at all nine offsets |
| 3 × 1,520 B | 4,560 | 211 / 211 / 213 | worked; structural rows down to ~41 |
| 3 × 832 B (`BuildPresentationA/B/C`) | 2,496 | 157 / 161 / 163 | **untouched** — the obvious next lane |

The 832 B trio is the best first move in the whole queue: three near-identical
functions nobody has opened, in a family whose levers are already documented in
the neighbouring shards.

**Levers that are established on this family** (and the one that is not):

- **L59 fold** — this family emits each per-element assignment group *reversed*;
  folding a group onto one physical line retires the `lineno` key and the raw
  ready-list order supplies the reversal. Worth 26 words on the triplet, 5 on
  the quadruplet, 27 on the fifth sibling. Check for macro-built rows first:
  a multi-line macro expansion carries the invocation's line, so no fold applies
  to it.
- **The call-spanning web** — the gate on the quadruplet. A pointer web that
  never crosses a call decides `bestcost=0` and takes v0; using the pointer
  after the call makes it span one, it takes s0, and a −4 size delta closes.
  456 → 326 in one step.
- **Counter partitioning is refuted on five of this family** and paid on three
  others. Check whether the counters are already separated before trying it.

## Tier 1: five functions, one decision each

5,796 bytes behind 41 words, and the aligner says each has exactly **one**
cause — no displacement tax, no mixed buckets:

| symbol | bytes | words | cause |
|---|---|---|---|
| `levelInit` | 2,064 | 6 | all register naming |
| `func_8003F154` | 1,188 | 13 | all register naming |
| `func_overlay_009_F00010B4_186772C` | 1,128 | 6 | all register naming |
| `func_overlay_071_F0000870_18CA390` | 728 | 11 | 9 naming, 2 structural |
| `func_overlay_022_F0000000_1878108` | 688 | 5 | **all immediate-only** |

That last one is the single most tractable function in the queue: five words,
every one an immediate or displacement difference, which `frame_census.py`
names directly and L119/L121 solve rather than sweep.

The three all-naming functions are p1-only (they contain calls), so their axis
is L100's save ratio — and note L114's bound is a *floor*: a colour another web
has forbidden is never offered by a force sweep, so free the interferer before
concluding a lever is out of reach.

## Tier 2: the whale, at 733

`func_overlay_058_F000138C_18B0574`, 14,456 bytes, 733 masked, delta 0. Four
lanes took it 3,215 → 733. Its partition is in the shard; the largest regions
are case 9 (119 words), case 8 (69) and case 12 (40). Its prologue is
force-declined with a `forbidden` mask — genuine interference, not preference —
so leave it. All 428 of its decisions are p1.

## Do not spend a lane on these

- **`overlay57UpdateModeState`** — 1,416 bytes at 5 masked words, which puts it
  second in the whole tree by bytes-per-word. **It is proven unmatchable at a
  floor of 2.** A trace replay reproduced 244 of 244 multi-candidate selections
  with zero mispredictions, and the target needs the first node of one dependent
  pair and the second of the other — opposite demands on a single per-pair key,
  with the whole key space covered using `#line` to reach positions no legal
  statement order can produce. Any ranking sorted by ratio will surface it; it
  is not a candidate.

## How to run it

Read `docs/LANE_BRIEF.md` — the standing brief — and dispatch with targets and
measured numbers only. Three habits that were learned expensively:

1. **Re-run a measurement before quoting it into a brief.** Two claims travelled
   handoff → brief → lane unverified in one session and both were wrong; one was
   a frame offset neither object touches, the other a debug bitset misread as an
   idle compiler pass. With the census and aligner that check is one command.
2. **Read a gate's exit status, not the tail of its output.** Filtering until it
   looks clean is how a red gate gets committed, which happened twice.
3. **Ask the call test before naming L106.** A procedure that issues a call is
   p1-only, so definition position, declaration order and statement order decide
   nothing. It was named as the lever for two lanes that had no such axis.
