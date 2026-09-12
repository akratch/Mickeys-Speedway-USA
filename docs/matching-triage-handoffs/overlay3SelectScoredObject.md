<!-- plateau-handoff:overlay3SelectScoredObject:start -->
### `overlay3SelectScoredObject` plateau handoff

- source: `src/overlays/o003/overlay3SelectScoredObject.c`
- score: 21/118 words
- frame: 0x80
- relocations: 5
- first mismatch: +0x38
- summary: The 21 words are one block, the entry and cached path; everything from +0xA0 on is byte-exact. The ROM copies the helper's return register into $a1 before the guard, so the cached path keeps using the raw $v0 result while the loop path reads $a1, which frees $v0 for both reloads of count and lets the index come straight off it with addiu s3,v0,-1; this candidate leaves objects in $v0 for both paths, puts count in $a0 and pays an extra move s3,v1. Records, instrumented object cmp-identical to the configured one: objects is web 114, save 20, nocs 1, totalsave 20, bestcost 0, forbidden mask empty, so it takes c1 v0 unopposed; the two count reloads are webs 17 and 36 at save 1.5 and 1.0 and take c3 a0. Two forces, both recorded accepted, prove the ROM's shape is neither a colour nor a split of that one web: forcing w114 to c4 gives 78 at delta -4 because it removes the move s3,v1 without adding the copy, and forcing the split path gives 79 at delta +4. The ROM has BOTH the copy and count in v0, so uopt kept the call result as its own range and gave the loop use a caller-saved copy. A source-level second pointer local is not the way in: assigned after the call, at the top of the else, or used for the cached path instead, all cost 8 bytes of frame at 0x88 and score 30, and so does replacing the unused u16 timer with it. A two-step cursor and a pointer-plus-index cursor are byte-identical at 21. Decision variable: what makes uopt copy a helper return register into a caller-saved argument register while leaving an earlier-block use on the raw result. Confound to avoid: a 240-point declaration sweep read 27 to 28 everywhere, but it also re-packed the declarations one per line while the retained body packs several, so it measures L59 packing as well as order and is not a clean negative.
#### 2026-09-12, lane `p9-tight`: the shape is named and two forces price it

The residual reproduces at 21 masked words, size delta 0, frame 0x80, and it is
**one block**: the entry and cached path. Every instruction from `+0xA0` onward
is byte-exact, so the loop body and the scoring are finished.

**The shape, exactly.** The ROM copies the helper's return register into `$a1`
before the guard. The cached path then keeps using the raw `$v0` result, which
leaves `$v0` free for both reloads of `count` -- and the index falls straight
out of it as `addiu $s3, $v0, -1`, where this candidate needs
`addiu $v1, $a0, -1` plus a `move $s3, $v1`. The loop path reads the copy:
`addu $s5, $a1, $t0` against our `addu $s5, $t0, $v0`. So the ROM spends one
`move` at the top and saves one at the bottom, at the same 118 words.

**Records** (instrumented object `cmp`-identical to the configured one, sixteen
integer p1 decisions): `objects` is web 114 -- save 20, nocs 1, totalsave 20,
bestcost 0, `forbidden0` empty -- so it takes c1 `$v0` unopposed, fourth in
decision order. The two `count` reloads are webs 17 and 36 at save 1.5 and 1.0
and take c3 `$a0`.

**Two forces, both recorded accepted, prove the ROM's shape is neither a colour
nor a split of that one web:**

- `p1:w114=c4` (objects to `$a1`) scores **78 at delta -4**. It removes the
  `move $s3, $v1` without adding the copy, because with the web coloured `$a1`
  from the start no copy is needed.
- `p1:w114=s` (the split path) scores **79 at delta +4**.

The ROM has *both* the copy and `count` in `$v0`, which means uopt kept the
call result as its own range and handed the loop use a caller-saved copy.
That is a range decision, not a colour one.

**A source-level second pointer local is not the way in.** `base = objects`
assigned after the call, at the top of the `else`, or used for the cached path
instead all cost 8 bytes of frame (0x88) and score 30 -- and so does replacing
the unused `u16 timer` with it, so the frame is not paying for `timer` and the
pointer run cannot absorb one more declaration. A two-step
`cursor = objects; cursor = &cursor[index];` and a `cursor = objects + index;`
spelling are byte-identical to the retained form at 21.

**Decision variable:** what makes uopt copy a helper's return register into a
caller-saved argument register while leaving an earlier-block use on the raw
result. L105 is the nearest law and it points the other way -- uses in the
call's own block get the raw register and later blocks get the *callee-saved*
copy; here the copy is caller-saved and the raw use is in a later block than
the copy.

**A confound the next lane should avoid.** A 240-point declaration sweep over
the five `s32` and two `Overlay3Object **` locals read 27 to 28 everywhere,
which looks like a clean negative on declaration order and is not: it also
re-packed the declarations one-per-kind onto their own lines, while the
retained body packs several per line. That sweep measures L59 line packing as
well as L143 order. Re-run it with the packing held fixed before believing it.

#### 2026-09-12, lane p23-lastmile3: exhaustive colour floor

Fresh residual map: 110 byte-exact, 5 naming, 1 immediate, 1 structural, with
one candidate-only and one target-only offset. The every-colour footprint
sampled 104 probes over 17 coloured webs; no accepted force beat 21, so the
winner list is empty and the lattice floor is 21. The named source question
remains the caller-saved copy of the helper return before the cached-path
guard, not a colour or split of the objects web.

<!-- plateau-handoff:overlay3SelectScoredObject:end -->
