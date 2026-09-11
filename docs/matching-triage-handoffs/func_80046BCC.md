<!-- plateau-handoff:func_80046BCC:start -->
### `func_80046BCC` plateau handoff

- source: `src/main/diCpu.c`
- score: 16/106 words
- frame: 0x40
- relocations: 3
- first mismatch: +0x2C
- summary: Register-only at 106/106; the three-variable family is closed at 192 forms, all 83-91, and the 16 are four windows of per-iteration consumption

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

#### 2026-09-12 (lane `lane/p7-res2`): the three-variable family is closed at 192 forms

Still 16, register-only at 106/106 words with the frame, the branch structure
and all three relocation sites exact. The 2026-09-11 note closes on "separating
them again brings web 32 straight back: all four three-variable forms measure
31, 33, 89 and 91". Those four were re-tested as a lattice rather than four
points, and the closure holds with a great deal more force than it was written
with.

**192 three-variable forms measured, every one between 83 and 91.** The lattice
crosses eight declaration positions for the reinstated working copy, three
spellings of the copy itself (a plain copy of the masked character, a recomputed
mask of the loaded character, and an or-with-zero barrier), both operands for the
first range bound, both assignment orders inside the conversion body, and an L97
`if (1)` region around each arm's body present and absent. The distribution is
eight cells at 83, eight at 84, forty at 86, eight at 87, forty-eight at 88,
forty at 90 and forty at 91 -- no cell anywhere near the two-variable 16, and
none below 83.

The cause is visible in every one of them and is the same cause the earlier note
named: giving the working copy its own name gives the character mask a second
destination, uopt commons it into a temporary, the temporary takes the first
caller-saved colour and the loaded character is pushed one place down the pool.
The or-with-zero barrier does not prevent it, which is L135 doing exactly what
it says -- uopt folds the identity operation before the web builder, so the
probe is not a probe.

**So the decision variable is unchanged and now has a much wider negative under
it**: the loaded character's live range has to stop before the working copy's
begins without a second IR name existing, and no declaration, copy spelling,
bound operand, assignment order or region opener in this family does that.
Resume on something that splits one symbol's range rather than on adding a
symbol -- L131 is explicit that differing *spellings* at the def sites, not
differing locals, are what split a range.

Two smaller facts for whoever resumes. The remaining 16 words are not one
window: `register_census` reads 68% global coherence over five source registers
with four windows opening at +0x78, +0xAC and +0x164, so this is per-iteration
consumption and each window is its own question. And two of the sixteen are pure
comparison operand order against the two loop-hoisted constants -- the candidate
puts the character first and the target the constant first at both sites -- which
is a smaller and separable question from the twelve-word pool rotation.
<!-- plateau-handoff:func_80046BCC:end -->
