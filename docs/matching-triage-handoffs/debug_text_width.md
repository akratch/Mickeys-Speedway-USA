<!-- plateau-handoff:debug_text_width:start -->
### `debug_text_width` plateau handoff

- source: `src/main/diprint.c`
- score: 59/66 words
- frame: 0x138
- relocations: 5
- first mismatch: +0x38
- summary: JFG donor-shaped source forms rebuilt flat at 59/66; next lever is an IDO UGEN scheduling or assembler selection trace.

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
<!-- plateau-handoff:debug_text_width:end -->
