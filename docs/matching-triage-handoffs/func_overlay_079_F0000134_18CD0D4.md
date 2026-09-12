<!-- plateau-handoff:func_overlay_079_F0000134_18CD0D4:start -->
### `func_overlay_079_F0000134_18CD0D4` plateau handoff

- source: `src/overlays/o079/func_overlay_079_F0000134_18CD0D4.c`
- score: 198/882 words
- frame: 0xB8
- relocations: 88
- first mismatch: +0x7C
- summary: integer ring closed. Two composed edits, each a regression alone, take 288 to 229: spelling the race-state bit test as (flags << 0xD) & 0x80000000U == 0 burns the ugen ring temp the target burns between +0x54 and +0x58 (328 alone), and routing state->target->state through the already-declared spawned gives that pointer the target's pool colour instead of a ring temp (374 alone). Splitting dx = state->targetX - object->x into dx = state->targetX; dx -= object->x at the mode-0 dot product then closes 31 more, to 198. 2026-09-12, lane p11-mid, three measured corrections to what is written above, all read off the instrumented toolchain rather than inferred. FIRST, the fp scratch ring is FIVE registers, not ten, and its cyclic draw order is f4 f6 f8 f10 f18 (ugen ids 36 38 40 42 50) taken FIFO; the complementary five f0 f2 f12 f14 f16 are globalcolor's float colours 24 to 28. The cycle recorded above as f4 to f18 to f8 to f10 to f6 is not the free list's order. SECOND, there are TWO separate fp facts, not one: at +0x368 both sides use a POOL colour and differ only in which (f16 against f2), while the ring phase itself opens at +0x494, where we take f18 and the target takes f4 -- adjacent positions in that five-cycle -- and holds one step for the rest of the function. So the axis is one fp DRAW, countable with DKWB_UGEN_SCHED plus DKWB_UGEN_TRACE, and no fp colour lattice reaches it. THIRD, globalcolor's float table here is saturated and the denial is a forbidden mask, not a ratio: every float web above save 4.25 carries forbidden0 0xb0, 0xd8, 0xf0 or 0xf8 and takes decision=split at bestcolor=30, so raising any of their saves cannot buy a colour, exactly as L101 says. The 0x40 against 0x3C spilltemp home is confirmed separate and still open.

Added to the flat list this lane, on the same base:

- all 18 adjacent swaps of the twenty declarations, scored 198 to 206, with
  `u04` and `u15` tying the incumbent at 198. None of them moves the differing
  spilltemp home: the frame census still reads +0x40 for us against +0x3C for
  the target on every one of them, so declaration ORDER is not what places it,
  which is a different axis from the "adding any local grows the frame" note
  already recorded and it is now covered too.
- ten pad-local probes, an unused `f32` and an unused `void *` each at five
  declaration positions, 245 and 246 with the frame growing 0xB8 to 0xC0 in
  every case. The local block really is quantized in eight-byte steps.

The instrument recipe, for the next lane: set IDO_DIR to the instrumented
toolchain, DKWB_UGEN_SCHED=1 and DKWB_UGEN_TRACE=1, and read the
`DKWB-FREELIST ALLOC_FP_RESULT` rows for this procedure; each is stamped with
its source line, so the draw index at any source statement is a direct
readout. `CDX_PROC=0 CDX_LOG=1 CDX_OUT=<path>` gives the colouring records for
the same procedure, where float webs print `reg=?` and colours 24 to 30.
<!-- plateau-handoff:func_overlay_079_F0000134_18CD0D4:end -->
