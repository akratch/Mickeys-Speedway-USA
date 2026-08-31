<!-- plateau-handoff:func_8004B1DC:start -->
### `func_8004B1DC` plateau handoff

- source: `src/main/font.c`
- score: 465 differing words
- frame: 0x80
- relocations: 42
- first mismatch: +0x54
- summary: GBI macros reduce the size deficit to eight words; the remaining setup and glyph command webs are structural/register allocation
- assignment base: `8b29adc5`
- ownership: resident ROM `0x4BDDC..0x4C68C`, VRAM `0x8004B1DC..0x8004BA8C`; 2,224 bytes / 556 words, with no target padding before `func_8004BA8C`
- ABI/callers: `void (Gfx **, DialogueBoxBackground *, char *, s32)`; direct C callers are `func_8004B13C` and `func_8004C200`
- configured build: full `src/main/font.c` TU under `-O2 -mips2 -32`; the earlier 119-row flag lattice already retained this row as best
- baseline: 528/556 words, 509 positional/raw differences, exact `0x80` frame, first structural divergence `+0x54`, and 48 candidate versus 42 target relocations
- retained result: 548/556 words, 465 positional differences (466 raw), exact `0x80` frame, 348 opcode mismatches, 190 alignment gaps, and 67 relocation-symbol mismatches
- relocation evidence: the target has 42 text relocations and the candidate has 48; configured preflight and checkpoint-summary generation fail closed because the target's `D_800D64F2` HI16/LO16 pair lacks a stable runtime identity, although the linked map fixes that symbol at `0x800D64F2`
- donor evidence: the bounded five-reference skeleton oracle ranks JFG assembly-backed `func_80070518` first at 0.3231 (2,364 bytes); DKR `render_text_string` is second at 0.0642 (2,044 bytes) and remains the disclosed source-organization starting point, not byte authority
- attempts: ten coherent forms covered the missing glyph bound, standalone window-zero y2 identity, current-text load timing, active-font address lifetime, GBI colour/sync macros, scissor/fill isolation, the texture macro, the glyph-bound combination, and block-scoped custom-command pointers
- rejected plateaus: the most structurally compact alternate reached 554/556 words and 60 relocation mismatches but regressed to 474 positional differences; the target-supported glyph guard plus that texture form overshot by one word and regressed to 478
- tooling: no permuter batch was run because the retained residual remains broadly structural rather than a close allocator/scheduler finish; every attempt and object is retained under ignored `build/wb/` state
- next action: preserve the macro-authentic colour/sync/fill spelling and manual scissor/texture spelling, then explain the `+0x54` setup address-carrier web before revisiting the custom glyph-command pointer chronology
<!-- plateau-handoff:func_8004B1DC:end -->
