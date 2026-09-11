# Strategy to 60%

Run `tools/triage.py` before every wave; it recomputes everything below in one
command with no compiles. The numbers here are from 2026-09-11 at 52.80%.

## The arithmetic

    resolved 498,608 / 944,344 = 52.80%
    60%      566,606 bytes
    GAP      67,998 bytes -- 16% of the 427,704 still queued

**The cheapest set covering that gap is 60 functions and 3,059 masked words**,
ordered by words per byte. The worst function in it carries 149 words on 2,100
bytes. This is not a grind: it is about five times what one good session
closes, and the work is concentrated.

## The strategy, in one line

**Work cluster leads in the low bands.** Two multipliers compound:

**Clusters — 2.9x.** 21 groups of identical-size siblings in the same overlay
hold 50 functions and 46,152 bytes, which is **68% of the entire gap**. Working
every sibling separately is 7,088 words; working one lead per group is 2,457.
Three transfers this week applied a diagnosis by line range with no per-function
tuning and twice landed siblings *better* than the function it came from. The
method, with its two cautions, is in the workbench field guide under *Work a
cluster's lead, not its members*.

**Bands — the low two are two thirds of the gap for a tenth of the words.**

| band | functions | bytes | words | behaviour |
|---|---|---|---|---|
| 0–20 | 40 | 17,592 | 439 | closes often |
| 21–60 | 53 | 27,716 | 2,011 | one or two decisions |
| 61–150 | 94 | 73,012 | 9,850 | a region or two |
| 151–400 | 101 | 132,740 | 24,559 | several regions |
| 401+ | 54 | 176,644 | 35,295 | reduces, rarely closes |

The first two bands are 45,308 bytes for 2,450 words. Add the cluster
multiplier where they overlap and the gap is covered.

**So the priority order is:** a low-band function that is *also* a cluster lead;
then the rest of the low band; then cluster leads in the middle bands; and the
401+ band only for what a lane can partition and hand on. Do not chase the
largest residuals — 54 functions hold 35,295 words there and historically they
reduce without closing.

## Where the tooling still costs more than it should

Three instruments are in the tree: `align_symbol.py` (cause split),
`frame_census.py` (stack slots), `triage.py` (wave scoping). One is not, and it
is now the most-needed:

**The ugen free-list trace.** L127 established that a peephole-deleted no-op
still consumes a ring temp, which makes whole-function ring phase a source-level
fix at zero byte cost — and ring phase is a large share of what is left. The
instrument is the instrumented ugen at `~/Desktop/dev/ido-instrumented`, outside
the tree, and three lanes have now needed it. Bringing a free-list reader
in-tree is the next tool to build.

## How to run a wave without wasting it

1. `tools/triage.py`, then `tools/align_symbol.py` on the shortlist. Pick by
   cause, not by residual size.
2. Dispatch against `docs/LANE_BRIEF.md`; the message carries targets, measured
   numbers, and what **not** to redo. Briefs that restate the standing rules
   drift — the L106 premise was stated wrongly to two lanes that way.
3. **Re-run any specific measurement before quoting it into a brief.** Three
   claims travelled report → brief → lane unverified in one session and all
   three were wrong: a frame offset neither object touched, a debug bitset
   misread as an idle pass, and an axis that did not exist. Each cost a lane
   part of its budget. With the census and aligner the check is one command.
4. Batch merges: regenerate the ranking once per wave, not once per lane.
5. Gate commits on **exit status**, and on the edit having applied. A filtered
   gate output hid a red `check-docs` twice; a half-applied edit script produced
   a commit message describing work that had not happened.

## Do not spend a lane on this

`overlay57UpdateModeState` — 1,416 bytes at 5 words, second in the tree by
bytes-per-word, and **proven unmatchable at a floor of 2**. A trace replay
reproduced 244 of 244 multi-candidate selections with zero mispredictions, with
the key space covered using `#line` to reach positions no legal statement order
produces. Any ratio-sorted list will surface it. It is not a candidate.
