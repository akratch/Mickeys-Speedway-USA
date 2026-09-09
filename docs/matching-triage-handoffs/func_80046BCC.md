<!-- plateau-handoff:func_80046BCC:start -->
### `func_80046BCC` plateau handoff

- source: `src/main/diCpu.c`
- score: 31 differing words
- frame: 0x40
- relocations: 3
- first mismatch: +0x2C
- summary: 106/106 words, the two hoisted constants now in the target's registers; the residual is one caller-saved live-range split.

#### 2026-09-09: the ninth callee-saved web is the working copy

41 differing words to 31 at 106/106 words, frame `0x40`, all three relocation
sites exact.

The previous pass folded the m2c draft's `var_s0` into `var_s2` to close a +4
size mismatch. That was right about the size and wrong about the register
file: the fold frees a callee-saved register, so the candidate hoists a third
loop-invariant constant (`0x78`) into `s8`, where the target materialises it
inline with `li at` and hoists only `0xA` and `0x30`. The copy is a real
source variable, not the allocator's artefact: `var_s0` carries the character
through the range tests while `var_s2` keeps the value the next iteration's
`temp_s6` reads. Reinstating it puts `s7 = 0xA` and `s8 = 0x30` back in the
target's registers and removes ten differing words.

Remaining: one live-range split. The target computes the masked character
straight into its callee-saved carrier and splits a copy into `s0` for the
range tests; the candidate computes it into a caller-saved temporary, runs
every range test out of that temporary, and copies into the saved carrier.
That one decision also exchanges `var_s2` with `var_s3` and `var_v0` with its
own temporary, which is most of the 31. The instruction count, the frame, the
branch structure and the relocation sites all agree.

Measured flat or worse against it in this pass: both orders of the
`var_s2 = 0` / `var_s3 = 0` initialisers (33); all legal orders of the three
loop-head statements (31, 32, 32); `u8 var_v0`; the loop test written as
`while ((var_v0 = *var_s4) != 0)`; the range tests spelled entirely on either
variable; reversed equality operands at two sites; the copy written after the
case block (41), inside each arm (41), and as a plain working copy with one
write-back (40); and three declaration orders. Resume on why the mask lands in
a caller-saved temporary here and directly in the saved carrier there.

<!-- plateau-handoff:func_80046BCC:end -->
