<!-- plateau-handoff:overlay31CreatePool:start -->
### `overlay31CreatePool` plateau handoff

- source: `src/overlays/o031/overlay31CreatePool.c`
- score: 11/50 words
- frame: 0x38
- relocations: 4
- first mismatch: +0x28
- summary: allocation-mismatch: typed bound repairs record/index colours but loses bound/multiply geometry; indexed carriers retain v0 interference.

#### 2026-09-11, lane `f9-small`: unused homed locals are inert on this frame

The 8-byte frame gap does not come from an unused local of any type: `void *`,
`f32`, `f64`, `volatile s32`, `volatile void *`, `s32[1..3]`, `u8[4]`,
`u8[8]`, a two-word struct, an address-taken `s32` (via `state = &tmp`), and
two pointers together all measure byte-identical (13 masked). An initialised
`volatile` costs instructions. So L99's "unused f32/pointer costs frame" does
not hold here, and the quantum is not a declaration. The other 11 words are
the record pointer and counter shifted `$a0/$a1 -> $a1/$a2`; a `count` copy
local regresses to +12 bytes. Decision variable: whichever web takes `$a0`
across the loop in the target (nothing in the loop reads `$a0`, so it is a
web with no instruction, as in `func_8003A754`'s v0) -- and it may be the
same thing that owns the frame quantum.
#### 2026-09-12, lane `p9-tight`: the frame is declaration order, and the rest is one colour

**The 8-byte frame gap is the declaration order.** Moving `s32 i; s32 j;` to
the head of the list gives the target's `0x38` exactly, with the same four-slot
ladder. That is adopted: 13 masked words to **11**, size delta 0, the aligner's
immediate-only bucket 2 to 0 and byte-exact 37 to 39. The previous note was
right that no *unused* local of any type supplies the quantum, and wrong to
conclude the quantum is not a declaration -- it is the order of the used ones.
Only `i` and `j` together do it; `i` alone and `j` alone both score 15 at
frame 0x30, and swapping them to `j, i` keeps 0x38 at 11.

**The remaining 11 words are one allocator decision, and it is priced.** An
instrumented uopt whose object was `cmp`-verified byte-identical to the
configured build records seven p1 decisions, in decision order:

- `state` save 305 -> c1 `v0`
- `j` save 155 -> c2 `v1`
- the literal 15 save 50 -> c7 `t0`, `forbidden0=0x7e000000` (c1-c6: it spans
  the four-argument config call)
- `record` save 40.5 -> c3 `a0`, `forbidden0=0x61000000` (c1, c2, c7 -- all
  taken, nothing denied)
- `i` save 15.5 -> c4 `a1`
- `count` save 4.33 -> c14 `s0`, `forbidden0=0x7f000000`
- `records` save 0.67 -> c8 `t1`, `forbidden0=0x7f020000`

`CDX_FORCE=p1:w9=c4,p1:w12=c5` with `CDX_PROC` set is **accepted** (the records
read `forced=4` and `forced=5`, not `-2`) and the forced object scores **0
masked words at delta 0** against this declaration order. So the function is
exactly one colour from a match.

**Decision variable: what puts c3 in `record`'s `forbidden` mask.** Three
things it is not, each measured:

- not the save ratio -- c3 is on `record`'s offer list at every save reached,
  and raising or lowering the ratio only reorders the decisions;
- not a call denial (L142) -- neither call loads `a0` alone. The alloc call
  loads a0 and a1, the config call loads a0-a3, and the literal-15 web shows
  what that looks like: `forbidden0=0x7e000000`, c1-c6 together. A web denied
  a0 by the config call would also be denied a1, and the target's `record`
  *is* a1;
- not an extra web taking c3 that the source can create, because such a web
  would need save above 40.5, must not span either call, and must emit no
  instruction -- the target's stream touches `$a0` nowhere between the two
  calls.

What is left is interference with a pre-coloured `a0`, and the only
pre-coloured `a0` in the function is the incoming `count`, whose range ends at
the size computation four instructions in, on both sides.

Flat at 11 on this base, so the next lane need not repeat them: all 32
combinations of `if (1)` regions at five sites (around the allocation, the
outer loop, the header writes, the inner loop, the config call); empty trailing
compares on `record`, `i`, `count`, `records` and `record != 0`; L109 probes on
`i` and `count` in the outer loop; a `header` pointer aliasing `record->state`;
a `kind` local supplying the config call's four zero arguments; an indexed
form; a trailing `record = records` reset; and `state` hoisted out of the outer
loop (that one costs 4 bytes).

**One idiom reaches 9 and must not be adopted.** The empty trailing compare
that matched `func_8003A754` (`if (record != records) { }`) gives 9 here, but
by the wrong mechanism: the records show it puts `0x40000000` in `state`'s
`forbidden` mask, so `state` and `j` each shift up a colour as well and the
inner loop goes wrong in two new places. It moves `record` and `i` correctly
and everything else incorrectly.

#### 2026-09-12, lane `p23-lastmile4`: force combination independently rechecked

The fresh base is 200 bytes, 50 words, delta 0, frame 0x38, four relocations,
and 11 aligned naming rows. The 59-probe every-colour footprint has three
winners: w9=c4 scores 3, w12=c5 scores 8, and w22=c3 scores 10. The first two
are rivals over the same window, so their combination was measured rather than
assumed.

With an instrumented-object fidelity pass, the accepted pair w9=c4 plus
w12=c5 scores zero masked words at delta 0. Their signed window effects are
minus eight and minus three, and the pair is exactly additive. The named source
question remains what makes the record web’s a0 unavailable while preserving
the target frame and call geometry; the diagnostic force itself is not a
shippable source change.

#### 2026-09-12, lane `p24-forcedzero`: source translation plateau

The retained guarded body is unchanged: 200 bytes, 50/50 words, frame 0x38,
12 raw and 11 relocation-masked differences, first masked mismatch +0x28.
Alignment is 39 exact, 11 naming, zero immediate, zero structural, with no
candidate-only or target-only words. No new matching credit is claimed.
A fresh Ucode-name receipt maps this TU to procedure 0 and passes text, data,
rodata, relocation and symbol fidelity. No colour sweep or force was repeated.

The final configured preflight calls this `allocation-mismatch`. Candidate
and target have four relocations with all offsets and types aligned; three
static identities resolve, while the allocation call at +0x1C remains
unresolved by the static resolver. That identity diagnostic precedes the
first instruction mismatch and must not be reported as an instruction error.
The canonical assembly fallback remains active.

**What moved, and what did not.**

- Deleting the state carrier lets IDO generate the identical inner pointer;
  it does not move the record web off c3. Changing the record carrier to an
  integer address, or the allocation-size literal to signed, also leaves
  the aligned residual unchanged.
- The indexed-source recipe that matched the angle scan does not transfer
  directly. Deriving record inside the outer loop can reproduce the target
  extent, but state then starts with `forbidden0=0x40000000`: v0 is already
  unavailable. State moves to c2 and the inner counter to c3, so getting the
  record and outer counter onto their wanted colours spends the inner-loop
  agreement. Explicit state carriers and a separate array-base alias do not
  remove that interference; other index forms also add instructions.
- Splitting the inner bound as `15U` is a real type-identity effect. The
  loop-bound web can now take c3, followed by record at c4 and the outer
  counter at c5; state and the inner counter keep c1 and c2. Thus all eleven
  original record/index naming rows are repaired by ordinary source. But
  the bound itself should remain in t0, and the now-independent final
  multiplier is strength-reduced rather than sharing the loop constant's
  materialization. The result is eight bytes short and 22 masked differences,
  with 33 aligned exact, eight naming, one immediate and six structural rows,
  plus two missing target words. The returned allocation also moves to the
  newly free t0. Merely giving the final multiplier an unsigned literal
  does not produce this change; it is byte-flat at eleven on the baseline.

**Attempt receipts.** Fifteen source candidates were measured. Global attempt
numbers include the thirteen angle-scan candidates. Every candidate retains
source, object, compiler log, scalar score, first mismatch, allocator trace
and `residual_map.py --object A --against B --window 0x40` output in ignored
lane evidence. The compact rows below give masked positional differences,
size delta, aligned exact/naming/immediate/structural counts, extra
candidate/target words, and first masked mismatch; aligned windows, not the
positional count alone, decided adoption.

- Attempt 1, unsigned multiplier literal: 11 masked, delta +0; aligned 39/11/0/0; extra 0/0; first +0x28.
- Attempt 2, delete state carrier and use a natural inner index: 11 masked, delta +0; aligned 39/11/0/0; extra 0/0; first +0x28.
- Attempt 3, delete both walking-pointer carriers: 46 masked, delta +20; aligned 30/15/0/5; extra 5/0; first +0x24.
- Attempt 5, reuse the count parameter as the outer counter: 49 masked, delta +4; aligned 24/14/2/9; extra 2/1; first +0x0.
- Attempt 10, signed allocation-size literal: 11 masked, delta +0; aligned 39/11/0/0; extra 0/0; first +0x28.
- Attempt 12, natural nested for loops without pointer carriers: 42 masked, delta +8; aligned 31/12/3/4; extra 2/0; first +0x28.
- Attempt 13, derive record from the outer index inside a natural for loop: 12 masked, delta +0; aligned 38/10/0/2; extra 0/0; first +0x28.
- Attempt 14, integer rather than pointer record carrier: 11 masked, delta +0; aligned 39/11/0/0; extra 0/0; first +0x28.
- Attempt 15, read count through its address at allocation: 42 masked, delta +8; aligned 19/12/2/15; extra 4/2; first +0x0.
- Attempt 23, retain a separate array-base alias and index it: 46 masked, delta +20; aligned 30/15/0/5; extra 5/0; first +0x24.
- Attempt 24, indexed array-base alias with explicit state carrier: 41 masked, delta +8; aligned 31/10/4/5; extra 2/0; first +0x28.
- Attempt 25, unsigned inner-bound literal: 22 masked, delta -8; aligned 33/8/1/6; extra 0/2; first +0x24.
- Attempt 26, unsigned integer spelling of indexed record address: 40 masked, delta -8; aligned 16/17/2/12; extra 1/3; first +0x28.
- Attempt 27, unsigned natural inner for loop: 22 masked, delta -8; aligned 33/8/1/6; extra 0/2; first +0x24.
- Attempt 28, signed integer spelling of indexed record address: 40 masked, delta -8; aligned 16/17/2/12; extra 1/3; first +0x28.

**Stop condition.** Attempt 25 was the last new causal result: it demonstrated
the bound/multiplier type coupling and its eight-byte cost. Attempts 26, 27
and 28 produced no better admissible residual and no new allocator mechanism:
the two integer-address forms retain the known v0 interference and are
identical to each other; the natural unsigned inner loop reproduces attempt
25's result. This is the ADR 0018 three-attempt stall, not a wall-clock expiry
or a claim that the function is unmatchable.

**Next action.** Reopen only with a source mechanism that leaves the existing
state/inner-counter assignments and shared bound/multiplier geometry intact
while reserving c3 from record, or with evidence that separates that literal
coupling at zero instruction cost. Do not repeat the index/carrier forms or
literal spellings listed here. The successful angle-scan recipe is committed
separately; its extra array-base web wins a low-priority tie, whereas this
record web has save 40.5 and is decided much earlier.
<!-- plateau-handoff:overlay31CreatePool:end -->
