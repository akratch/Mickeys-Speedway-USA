<!-- plateau-handoff:func_overlay_027_F0000064_187BA3C:start -->
### `func_overlay_027_F0000064_187BA3C` plateau handoff

- source: `src/overlays/o027/overlay_027.c`
- score: 48 differing words
- frame: 0x60
- relocations: 22
- first mismatch: +0x94
- summary: Stack-home and two value-web regions closed (62 to 48); the 40-word remainder is one fp web the candidate colours f12 and the target f16.
#### Region partition (measured at 62, re-measured at 48)

At 62 the 368-word residual partitioned into four independent decisions, not
one. Counts are relocation-masked differing words:

- 3 words -- stack home of the update-rate copy: which frame slot the
  retained copy of `updateRate` gets (+0x18, +0x47C, +0x480). **closed**
- 11 words -- post-loop fade and pulse-state webs: which colour the fade read
  and the pulse-state read take. **closed**
- 4 words -- in-loop value webs: the case-3 intensity read sharing a web with
  the other cases. **closed**
- 40 words -- persistent fp web: the register the long-lived scale value gets,
  f12 in the candidate and f16 in the target. open
- 4 words -- instruction placement: two adjacent-pair orderings, one around
  the default-case scale load and one in a call's argument setup. Two of the
  four fell out with the fade web; open.

Three closures, all delta 0 and confirmed on both the positional count and the
schedule-tolerant multiset distance:

1. Declaring the update-rate copy **first** in the local list moves its frame
   slot up by 8 and closes all three stack-home words (62 -> 59, multiset
   49). This is the L99 declaration-order axis, and only the first position
   works: positions 1-8 measured 62, 62, 64, 64, 64, 66, 66, 66.
2. Caching `state->fade` into the existing `value` local across the post-loop
   null test and the 0xFF test closes 7 (59 -> 52, multiset 44). The two reads
   then share one web and that web takes v0, which is what the target does.
3. Using the already-declared `phase` instead of `value` for the case-3
   intensity read closes 4 more (52 -> 48, multiset 41).

#### The open 40: one fp colour, and what it is not

Every one of the 40 is downstream of a single choice. The candidate gives the
long-lived scale value f12 (an fp argument register, 9 uses); the target gives
it f16 and uses f12 only once, for the one outgoing float argument. The 1.0f
constant is f14 in both, and both re-materialise the scale value after every
call, so neither web crosses a call and neither needs a spill. With the scale
value on f12 the candidate's ring temp pool is {f4,f6,f8,f10,f16,f18}; the
target's is {f4,f6,f8,f10,f18}. That one-element difference rotates the ring
at every arithmetic site in the switch, which is where the other 39 come from.

Measured flat on this axis (no change to the fp assignment at all):

- 18 declaration-order forms for the two fp locals (positions 0-8 each).
- A 64-cell lattice over six commutativity/statement-order axes: the order of
  the three coordinate copies, both scale multiply operand orders, the
  scale-target multiply order, the two fade-float multiply orders, and folding
  the pre-loop scale assignment onto the enclosing `if` line (L59). All 64
  cells measured 59-60.
- Loop form (`do`/`for`/`while`), a `register` qualifier, an extra local
  carrying the call's float argument, and a copy-through second local.

What does move it, and why it cannot be adopted as it stands: making the scale
web live across the call -- either by assigning it before the call in case 0,
or by giving case 0 its own scale local -- moves it off f12 immediately and
brings the whole ring into agreement (multiset distance 49 -> 37, the lowest
measured). Both forms cost a spill pair around the call, +8 bytes, so the size
delta stops being 0. The next attempt should look for a spelling that denies
f12 without extending the web's lifetime across the call.

#### Cross-function note

`func_overlay_079_F0000134_18CD0D4` has the same signature: its candidate uses
f12 eleven times more than its target and f16 nine times fewer. The two
functions plausibly share one mechanism, so a solution here transfers.
#### Further forms measured flat at 48

- Switch case order: 23 permutations that keep `default` last. All scored 75
  or worse; the source order is already the target's.
- The eight remaining integer words are the default arm's fade and intensity
  reads. Caching the intensity read into the existing `value` local is exactly
  flat (48, multiset 41); into `phase` it costs a size delta; splitting the
  fade subtraction, moving the cache below the test, and caching the case-3
  test operand all measured 49 to 94. Adding a dedicated local regresses by
  eleven words because it moves every frame slot, the same frame sensitivity
  overlay 79 shows.
<!-- plateau-handoff:func_overlay_027_F0000064_187BA3C:end -->
