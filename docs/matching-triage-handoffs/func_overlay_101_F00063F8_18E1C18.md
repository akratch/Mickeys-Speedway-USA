<!-- plateau-handoff:func_overlay_101_F00063F8_18E1C18:start -->
### `func_overlay_101_F00063F8_18E1C18` plateau handoff

- source: `src/overlays/o101/func_overlay_101_F00063F8_18E1C18.c`
- score: 167/380 words
- frame: 0x38
- relocations: 49
- first mismatch: +0x8C
- summary: 213 masked words; structure fell 130 to 41 and insertions 25 to 7. Residual is ugen ring phase behind the slot local.

Measured 2026-09-11, lane `lane/o11-triplet`, on the four-function overlay-101
builder family. Numbers below are `tools/align_symbol.py`, whose positional
figure agrees with `tools/score_symbol.py` by construction.

Before: 296 masked words, aligned 162 byte-exact, 108 register naming, 5
immediate only, 130 really different, with 25 insertion and 25 deletion words.
After: 213 masked, 178 byte-exact, 167 register naming, 1 immediate only, 41
really different, 7 insertions and 7 deletions. First mismatch moved from
+0x2C to +0x8C; the whole prologue through +0x88 is now byte-exact.

Two levers, both source shape, neither costing an instruction:

- L59. Every per-element assignment group is written as ONE physical line. as1
  minimises `(start_time, -aftercycles, -latency, addr, lineno, list position)`.
  With a group's stores on separate lines `lineno` is the deciding key and
  emits them in source order; the ROM emits each such group reversed. Folding
  the group retires the key and the raw list order supplies the reversal. On
  the first group alone this flipped three stores into place; applied to all
  five element groups it took 296 to 270 masked and 130 to 108 structural.
  The four text rows are already one line each, because a multi-line macro
  expansion carries the invocation's line, so no fold applies there.
- L100 and L115, the counter partition. One shared counter local gives the
  callee-saved home to the node counter and leaves the panel-order counter in a
  caller-saved register, and the ugen ring then rotates for the rest of the
  function. Splitting it so that `index` carries the panel-order counter D_1C4
  and the text-row counter D_1D0, while `slot` carries the two node counters,
  reproduces the ROM's home and its save placement exactly. 270 to 239 for the
  split, then 239 to 213 for moving the text rows onto `index`.

Measured inert, all at exactly 213 masked: declaration order of the two counter
locals either way, `register` on either, `u32` instead of `s32`, declaring
`slot` last, and every statement order tried inside the text-row macro. This is
the call test holding: the procedure issues calls, so it is p1-only and
definition position, declaration order and statement order decide nothing.

Measured regressive, each against 213: separate counter locals per block reads
324 at a size delta of 8; splitting the counter at the call boundary reads 250;
dropping `slot` for a direct counter read reads 331 at a size delta of 12 for
both node blocks and 363 at 12 for one; reading the counter global directly for
the pre-call half only reads 215 but with 51 structural words against 41 and 11
insertion sites against 7; moving the opacity store to the end of the text row
costs three instructions.

Next lever, with the decision variable named. The first divergence in the whole
function is at +0x8C, and it is `slot`'s register: the ROM splits `slot` across
two scratch registers, one before each call and one after, while the candidate
gives it a single dedicated caller-saved register. Everything downstream is the
ring phase that follows from that one choice, which is why the naming bucket is
167 words and is almost entirely ring-to-ring: by L114 those rows are ring
phase, not colour, and no save edit or force reaches them. The axis is what
makes ugen treat a named local as scratch rather than as a dedicated register.
`uopt -Wo,-zdbug:2` reports `colorcand` empty for this function, so globalcolor
colours nothing here and L100's ratio is not the lever that decides it; the
stock listing then dies in `wrapper_ecvt` inside the static-recomp libc before
printing any save, so the instrumented toolchain is the instrument this needs.
The remaining 41 structural words are the text rows' tail, where the ROM emits
the text store before the chain store and the opacity store last; statement
order inside the macro is inert against it, so that residual needs the as1
dependence graph (`cc -Wa,-R`), not a respelling.

<!-- plateau-handoff:func_overlay_101_F00063F8_18E1C18:end -->
