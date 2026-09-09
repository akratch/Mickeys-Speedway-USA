<!-- plateau-handoff:func_80046BCC:start -->
### `func_80046BCC` plateau handoff

- source: `src/main/diCpu.c`
- score: 31 differing words
- frame: 0x40
- relocations: 3
- first mismatch: +0x2C
- summary: the masked char is uopt web 32, coloured before every local and holding v0; forcing var_v0 back to v0 is declined, so the temp has to stop existing

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


#### 2026-09-09 (second pass): the live-range split is a compiler temporary

The previous pass asked "why does the mask land in a caller-saved temporary
here and directly in the saved carrier there". The instrumented uopt answers
it. The masked character is a web of its own -- phase-one web 32 -- and it is
coloured *first*, ahead of every declared local, so it takes v0; `var_v0` is
web 0 and gets v1 behind it. The target has no such web: its mask writes the
callee-saved carrier directly.

Two force probes bound the space:

- `CDX_FORCE=p1:w0=c1`, putting `var_v0` back in v0, is declined twice --
  web 32 already holds v0 and the two interfere. The register file cannot be
  recovered by moving `var_v0`; web 32 has to stop existing.
- Forcing web 32 into a callee-saved colour reaches the target's registers and
  wrecks the schedule instead (73 and 74 differing words for s2 and s3), and
  forcing its split path costs two instructions (108 words against 106).

Web 32 exists because `var_s2 = var_v0 & 0xFF;` is immediately followed by
`var_s0 = var_s2;` in the same basic block. The value has two destinations, so
uopt commons it into a temporary and copy-propagates both names onto it -- which
is also why every range test in the candidate reads the temporary rather than a
carrier, while the target tests the first bound on its saved carrier and the
second on the copy.

Newly eliminated this pass: nested `if`s in place of the two `&&` pairs (31,
byte-flat); `var_s0` spelled as a second `var_v0 & 0xFF` (31, flat); the copy
moved ahead of the pointer increment (32); the copy pushed inside each arm with
an `else` copy on the short path (103, and the frame moves); `var_s0 = var_s2 &
0xFF` (89); both range tests on `var_s2` with the copy after the case block
(41); `var_v0` masked at the load (86); and `temp_s6` taken from `var_s0` (56).

Resume by removing the mask's second destination, not by reordering it: the
target's source cannot be creating a second name for that value in the same
block. Everything else -- instruction count, frame, branch structure, the three
relocation sites and the two hoisted constants -- already agrees.

<!-- plateau-handoff:func_80046BCC:end -->
