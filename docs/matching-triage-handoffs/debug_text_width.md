<!-- plateau-handoff:debug_text_width:start -->
### `debug_text_width` plateau handoff

- source: `src/main/diprint.c`
- score: 4 differing words
- frame: 0x138
- relocations: 5
- first mismatch: +0x68
- summary: Seven words to four on an L97 region plus a named newline constant paid for by L112 buffer length; the residual is four range tests reading the raw byte where the target reads the copy, and the cause is uopt copy propagation stopping only at the statement that redefines the source.

#### 2026-09-10, lane `o7-mid`: the two-web form is buildable, and it is an exact v0/v1 transposition

Base reproduces at **7 relocation-masked words, size delta 0**, first mismatch
`+0x38`.

**All seven words are one fact.** The target holds the raw byte in `v1` (the
`+0x38` and `+0xEC` loads, the `+0x44` null test, the `+0x5C` newline test, the
`+0x64` space test), copies it into `v0` at the loop top, and uses `v0` from
the `+0x68` range test onward. The copy lands in the newline branch's delay
slot, which is why the target's branch is a plain `beq` and ours a `beql` that
pulls the latch load up instead. Fix the two webs and all seven close; there is
nothing else in the residual.

**The copy can be bought.** It survives when its *source* is redefined later
inside the loop and dies when it is not. Writing `charIndex` as the raw byte
and copying it into `pad` at the loop top keeps the copy — `charIndex` is
redefined by the three masked index assignments. Writing `pad` as the raw byte
and copying into `charIndex` is copy-propagated away and stays flat at seven,
which is what the 2026-09-10 `nm-mixed` note measured.

**The surviving form scores 20, category `register-only`, size delta 0** — and
every one of those 20 words is the v0/v1 transposition. The raw byte, the copy,
the index and the `+0xD4` address temp all swap, plus the `+0x5C` branch
operand order. So the residual has moved from *"the copy cannot be made to
survive"* to *"we cannot choose which of the two webs gets `v0`"*, and the
buffer-shift obstacle the earlier note names is gone: reusing `charIndex`
carries the second web with no eighth declaration.

Flat at 20 across, on the two-web base:

- all 24 declaration orders of the four scalars;
- all four positions of the 260-byte buffer within the declaration list;
- both role assignments — char+index in `charIndex` with the copy in `pad`, and
  the mirror. Names are inert here ([L26], [L20]);
- reversed newline and space comparison operand orders;
- an extra preheader copy intended to found the copy web first (uopt folds it);
- the guard test moved onto the copy variable.

Web founding is live-ucode first-occurrence order, and the raw byte's preheader
load always precedes the loop-top copy, so no source form reaches a lower web
number for the copy web.

Re-measured and flat at **seven** on the one-web base, so the next lane need not
repeat them: 36 points crossing six spellings of the two definition sites
(including `pad = *ch++` and reading through `ch` from `s[0]`) against three
loop shapes (`do`/`while`, `while`, `goto`); `u8 pad`; `u8` and `s16`
`fontTexture`; and `u8 charIndex` with the truncation written at the store —
[L85]'s renumbering spelling, which does not move the colour here.

**Next lever.** A CDX force sweep on the 20-word two-web form. If `v0` is on
the copy web's candidate list the residual is one force from a verdict; if it
is absent ([L101]'s silent-decline shape) then what is needed is a web-number
lever for caller-saved integer webs, which no law currently supplies.

**The tree keeps the one-web body**, because 7 < 20. The two-web form is a
diagnostic, not an improvement.
#### 2026-09-11, lane `p6-small`: seven to four, and the closure above was wrong about the two-web form

The 2026-09-10 note is correct that the two-web form is the structurally right
body and correct that it scores 20 as an exact v0/v1 transposition. What it
missed is that the transposition is not a property of the body: **opening one
L97 region anywhere in the function turns it the right way up.**

Measured on the two-web base, 128 points: both `if (1)` and `do { } while (0)`
spellings, at five nesting sites (around the guard read plus loop, around the
loop, around the loop body, around the newline test body, around the range
test body) and every combination of them. **Every point that opens at least one
region scores 5; the point that opens none scores 20.** The one-web body is
flat at 7 across the same 128 points. So the fact is the region's existence,
not its placement, and the closure's "no source form reaches a lower web number
for the copy web" was an inference from the wrong axis: the region does not
renumber the copy web, it splits a block, and per L115 that removes a
symbol-level interference the whole colouring was resting on. The same edit
matched `func_8004BA8C` in `src/main/font.c` on the same day.

Two further words then came off, leaving four:

- **the newline test's operand order.** ugen emitted `beq mark, current` where
  the target has `beq current, mark`. Eight literal spellings are inert
  (`!=` both ways, `!(==)` both ways, `10` for `'\n'`, and the test moved onto
  the copy), because cfe canonicalises `constant != variable` back to
  variable-first -- confirmed by reading the `cc -S` listing, which is
  identical for `charIndex != '\n'` and `'\n' != charIndex`. With the constant
  in a VARIABLE the source order survives and the target's order is emitted.
  `s32 newlineMark` does it; `u8`, `char`, `u32`, `s16` and `u16` do not.
- **`char s[256]` pays for that variable.** The extra scalar takes a frame home
  in all eight declaration positions and in all six types measured (56 points,
  every one 15 or 16 words on the frame alone, because the 0x138 frame becomes
  0x140 and every sp displacement moves). The buffer length is unobservable, so
  L112 solves for it: 253, 254, 255 and 256 all restore the exact 0x138 frame,
  and 257 through 260 do not. 256 is the natural choice.

**The residual is four words and one named cause.** The four range tests
(`slti at,x,33`, `,128`, `,64`, `,96`) read the raw byte where the target reads
the copy. Both sides' `cc -S` listings show this before allocation, so it is
not a colour: uopt copy-propagates `pad` back to `charIndex` at every use up to
but NOT including the statement that redefines `charIndex`. That is why exactly
the compares move and the three subtractions -- whose destination is
`charIndex` -- do not, even though they sit in the same basic block as the
first compare.

**Decision variable:** a definition of `pad` that ugen lowers to a bare `move`
and uopt does not treat as a propagatable copy.

Falsified at the four-word base, all measured:

- fifteen no-op copy expressions -- or-with-zero, xor-with-zero,
  and-with-minus-one, plus zero, times one, shift left zero, shift right zero,
  double negation, double complement, self-or, self-and, `(u8)` and
  `(s32)(u8)`: the ones cfe does not fold cost an instruction, the ones it
  folds are propagated;
- five copy placements: loop top, inside the newline test, inside the else,
  loop bottom, preheader. Only the loop top holds size delta 0; inside the
  newline test costs 4 bytes because the copy no longer reaches the branch
  delay slot;
- all 49 type pairings of `charIndex` and `pad`;
- all 120 declaration orders of the five scalars;
- the range tests reading the raw symbol rather than the copy, and the reverse
  (uopt makes the two spellings identical, both 4);
- the subtractions reading the raw symbol -- the copy then dies entirely and
  the score returns to 20, which is what keeps the copy alive;
- splitting the `&&`, and sixteen comparison spellings of each range test;
- naming the space constant instead of the newline constant.

<!-- plateau-handoff:debug_text_width:end -->
