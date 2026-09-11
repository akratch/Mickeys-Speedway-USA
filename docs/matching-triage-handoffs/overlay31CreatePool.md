<!-- plateau-handoff:overlay31CreatePool:start -->
### `overlay31CreatePool` plateau handoff

- source: `src/overlays/o031/overlay31CreatePool.c`
- score: 37/50 words
- frame: 0x30
- relocations: 4
- first mismatch: +0x0
- summary: Naming the final slot count is byte-identical; CFE erases it before frame layout. Baseline stays 37/50 with target frame 0x38 vs candidate 0x30.

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
<!-- plateau-handoff:overlay31CreatePool:end -->
