# Strategy to 60%

Run `tools/triage.py` before every wave; it recomputes the arithmetic, the
bands, the clusters and the assignability split in one command with no
compiles. It takes about four minutes and buffers its output, so redirect it
to a file rather than waiting on a terminal. The numbers here are from
2026-09-12 at 57.26%.

## The arithmetic

    resolved 540,772 / 944,344 = 57.26%
    60%      566,606 bytes
    GAP      25,834 bytes -- 10% of the 249,020 still queued

**The cheapest set covering that gap is 13 functions and 438 masked words**,
ordered by words per byte, worst member 14 words on 460 bytes. That is not a
grind. One evening in this state closed five functions and 7,880 bytes, so the
gap is roughly three such sessions -- *if* the work is pointed at the right
pool, which is the whole subject of this document.

## What changed on 2026-09-12, and why it changes the scoping

**A function's entire register-colour axis is now minutes of compiles.** A
forced IDO compile costs about 0.6s, so `tools/web_footprint.py --every-colour
--cross-kind` measures every legal `(web, colour)` pair a procedure has. On the
14,456-byte function that was 1,875 compiles in ten minutes. Before this, every
lane *sampled* that axis by hand and reported the sample as a floor; one
lattice nominated 5 of 139 coloured webs and called 185 a floor that an
exhaustive sweep then walked past.

Three consequences for how a wave is planned:

1. **The colour axis is no longer an open-ended search.** Run it first, and
   what remains is exactly one question: a source spelling, or an instruction.
2. **A new class of target exists: the proved zero.** When a force or pair
   scores 0 masked words at delta 0, the compiler has stated which registers it
   must choose and only the spelling is missing. Two such functions were
   converted to matches the day the class was created.
3. **Selection is a packing, not a sort** (L159). A force that scores better
   alone can make the set worse when its blast radius duplicates another's.
   `web_footprint.py --report` computes the packing; do not hand-pick from the
   winners list.

The route from a proved zero to a match is **L160**: a *declared* carrier can
hide the web the target needs, so delete the declaration and let IDO generate
the value. Indexed access instead of a walking pointer makes strength
reduction create the cursor and leaves the array-base web alive to take the
wanted colour; removing a named intermediate also changes which of two tied
webs comes first, which is what decides a tie. It matched three functions in
one edit each.

## Where the leverage actually is: the assignment gate

This is the finding that should shape the sprint. Of 303 queued functions:

| state / reason | fns | bytes | what it needs |
|---|---:|---:|---|
| `base-only` / authorized-reopen | 175 | 227,708 | nothing -- dispatch |
| `already-integrated/exhausted` / **current-plateau** | 82 | **97,152** | **a granted reopen, with a stated reason** |
| `active` / lane-owned | 27 | 34,056 | in flight |
| `already-integrated/exhausted` / reopen-authorization-stale | 3 | 18,160 | `authorize_reopen.py --refresh-stale` |
| `stale-ledger` / prose-needs-remeasurement | 8 | 2,780 | re-measure, then re-bank |
| `already-integrated/exhausted` / matched-or-promoted | 6 | 3,712 | nothing -- done |

**97,152 bytes are locked behind `current-plateau`, 3.8x the entire gap.** Those
plateaus were banked *before* the exhaustive landscape, the packing and L160
existed. A lane that could only sample the colour axis and stopped is not the
same lane as one that can close it in a minute, and that difference is a
legitimate, stateable reason to reopen — which is exactly what the
authorization file is for. Three of the five matches on 2026-09-12 came from
re-opening functions in precisely this state.

It is *not* a licence to reopen everything. The bar stays: a reason that names
what changed since the plateau was banked. "The colour axis can now be closed
exhaustively and this shard's landscape samples N of M coloured webs" is such a
reason. "It felt close" is not.

## The sprint: four waves

### Wave 0 — unlock (coordinator, no lanes, about forty minutes)

1. `authorize_reopen.py --refresh-stale`, commit, `--verify`. Recovers the 3
   stale pins (18,160 B).
2. Grant reopen on the `current-plateau` functions whose shard carries **no
   exhaustive landscape** — `--symbols <list> --reason "..."` naming the new
   capability — then commit, then `--verify`. Start with the best ratios:

       2648 B  masked=9    func_overlay_086_F0000474_18D22AC
        236 B  masked=9    func_80006448
        416 B  masked=24   overlay58DrawLargePointQuad   <- cluster pair
        416 B  masked=24   overlay58DrawPointQuad        <- cluster pair
       1216 B  masked=88   overlay58FinalizePackedStatus
        908 B  masked=63   overlay19BuildSpatialMasks

3. Re-run `triage.py` so the wave is planned against the unlocked pool.

**The pins arm only from the committed file**, and only while the source and
handoff commits match. So: grant, commit, verify, *then* dispatch — and never
edit a handoff between the verify and the dispatch.

### Wave 1 — the cheapest route (3 lanes, last-mile shape)

The dispatchable route today is 29 functions / 25,888 bytes / 1,049 words, and
it improves once Wave 0 lands. Batch 8-10 functions per lane, cheapest first,
with a hard "bank and move on after 25 minutes" rule. Measured productivity:
last-mile batches are the most productive lane class in this campaign, and on
2026-09-12 they returned five matches across nine lanes.

Every brief carries the same four-step method: aligned bucket split first
(`residual_map.py`), then the exhaustive landscape, then the packing, then
L160. Nothing else.

### Wave 2 — cluster leads (2 lanes)

9 groups, 19 functions, 18,932 bytes — **73% of the gap** — at 2.8x word
leverage: working every sibling is 3,343 words, working one lead per group is
1,196. The best leads are in the low bands:

       3 x  820 B  main    lead 17 words   func_80006534
       2 x  588 B  main    lead  3 words   func_80010900
       3 x  424 B  main    lead 10 words   func_8005A7A0
       2 x  636 B  main    lead 47 words   joyRead
       2 x  416 B  o058    lead 24 words   overlay58DrawLargePointQuad

Give one lane the whole cluster, never one sibling each: `dispatch_check.py`
now refuses a plan that splits a translation unit, but siblings can also live
in separate files and still share a diagnosis. Apply the lead's diagnosis by
line range to its siblings without per-function tuning; that has twice landed a
sibling better than the function it came from.

### Wave 3 — the whale and the structural class

`func_overlay_058_F000138C_18B0574` is 14,456 bytes, **56% of the gap on its
own**, currently at 217 masked / delta 0. Its colour axis is closed and proved
(all 1,875 single forces; the optimal packing returns the measured 185), so
there is nothing left to sweep. What remains is two insertion/deletion pairs:

    target-only +0xD5C   -> candidate-only +0xDDC    span 0x80  (32 words)
    target-only +0x1260  -> candidate-only +0x12F8   span 0x98  (38 words)

Closing the two smaller pairs took it 227 -> 217 and structural rows 30 -> 25.
Attack the instructions, not the colours.

**The wider structural class.** 158 of 303 queued functions — 239,512 bytes,
62% of queue bytes — carry a nonzero size delta: we emit a different number of
instructions than the target. A colour force normally cannot fix that (though
it occasionally can, by coalescing a move away: 8 of 923 probes changed size on
the whale). For any such function, read the candidate-only/target-only offsets
from `residual_map.py` *first* and treat the instruction as the question. Do
not open with a colour sweep, and do not rank its windows by the positional
count — between an insertion pair every word mismatches by position while
aligning perfectly, and on the whale that inflated 22 aligned rows into 81
positional words (L155).

This class is where the campaign's next method gap is. It is too big to leave
at "run a sweep and hope".

## Model assignment, measured

`gpt-6-astra` on the hardest single question in the wave — the whale's
instruction pairs, a proved-zero conversion, or the best-ratio batch. It
returned four of the five matches on 2026-09-12. `gpt-5.6-luna` and
`gpt-5.6-terra` on breadth: batches of 5-10 near-misses, cluster siblings, and
the "price the axis, do not chase the match" jobs, where they produced complete
landscapes and bounded negatives quickly.

Keep total load under about 12; five to seven concurrent lanes is the practical
ceiling on this machine, and two OOM kills earlier in the campaign came from
ignoring it.

## Footguns

Every one of these has cost this campaign real work. The first five cost it on
2026-09-12.

**Never pipe a gate.** `gmake check-docs | tail; echo $?` reports *tail's*
status. Use `tools/gates.sh`, and do not pipe that either. It has put commits
on top of a red gate three times, including once after the tool that prevents
it existed.

**Renew reopen pins AFTER the last handoff edit, not before.** A pin arms only
while its source and handoff commits match. Renewing and then committing a
handoff section moves the handoff commit and re-invalidates the pin the
renewal just wrote. Order: merge → last handoff edit → renew → commit → verify
→ dispatch.

**Regenerate every generated file after a merge; never `--refresh-stale` to
repair one.** A clean auto-merge of `config/nonmatching-ranking.us.json` parsed
fine and was internally inconsistent — header `resolved: 307` over 306 rows,
because the merge kept the incoming count while dropping a retired row.
`nm_ranking.py --check-doc` refused it. A refresh re-reads the same broken
header; a full regeneration fixes it.

**The conflict resolver's keep-both fallback is safe on prose and unsafe on
metadata.** It duplicated a single-valued `summary:` key inside a C file's
EOF handoff block. After any keep-both resolution, scan for *duplicated keys*,
not just for conflict markers.

**A promotion's first build is expected to fail.** `gmake extract` discards the
reloc-surface renames, so the first link reports undefined resident references
and prints its own remedy. Do not chain it with `&&`: run build, then
`overlay-syms`, then build again, then verify.

**Never `git merge --abort` while another merge is in progress** — it reverts
the resolution you are part-way through. Finish or stash first.

**Never run two `gates.sh` at once.** They race on `build/gates/*.log` and
neither finishes.

**One owner per translation unit.** `dispatch_check.py` now refuses a plan that
splits one, after three overlay 8 functions with distinct names turned out to
share `overlay_008.c`.

**A force is a diagnostic, never a shippable edit.** `CDX_FORCE` says which
colour the target wants; the deliverable is the source form that produces it
unaided.

**Do not re-run a colour sweep on a function whose shard already reports one**
unless you changed the source — in which case the old landscape is void and you
must re-measure.

**Re-run any specific measurement before quoting it into a brief.** Three
claims travelled report → brief → lane unverified in one session and all three
were wrong.

## Tooling to build next, in order of payback

1. **A forced-floor census.** For every queued function: the exhaustive
   landscape, the packing, and the predicted floor. That turns "which function
   should I work?" from a judgement into a lookup, and it is the natural
   successor to `triage.py`. A scratch prototype ran at roughly a minute per
   small function. Promote it with tests and have it write a tracked summary
   (never the objects, which are ROM-derived evidence).
2. **Cache the assignment classifier.** `lane_status.AssignmentContext.build`
   dominates `triage.py` (four minutes), `dispatch_check.py`, and
   `authorize_reopen --refresh-stale` (about ten minutes per land). Every
   integration pays it two or three times. A keyed cache invalidated by the
   branch tip would pay for itself within a day.
3. **An insertion-pair reader.** The structural class is 62% of queue bytes and
   the only instrument pointed at it is `residual_map.py`'s offset list. A tool
   that says *which IR construct* emits the extra word, the way
   `web_footprint.py` says which web owns a colour, is the next real
   multiplier.

## Do not spend a lane on this

`overlay57UpdateModeState` — 1,416 bytes at 5 words, near the top of any
ratio-sorted list, and **proven unmatchable at a floor of 2**. A trace replay
reproduced 244 of 244 multi-candidate selections with zero mispredictions.
`config/unassignable-symbols.us.json` carries the bar and the proof;
`tools/triage.py` drops barred symbols from every route, cluster, band and
total and prints an `EXCLUDED` line so the removal stays visible. Add a symbol
there only with evidence that *no legal source reaches the target* — a
plateau, however stubborn, is not that, and belongs in the handoff.

Two large functions now have their colour axis *priced*, which is its own kind
of "do not spend": colour reaches 12 of 851 words on
`func_overlay_054_F00005AC_189F24C` and 153 of 1,337 on
`func_overlay_052_F000063C_189ACAC`. Their residuals are structural. Do not
send another lane to sweep them.
