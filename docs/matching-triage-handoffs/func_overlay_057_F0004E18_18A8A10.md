<!-- plateau-handoff:func_overlay_057_F0004E18_18A8A10:start -->
### `func_overlay_057_F0004E18_18A8A10` plateau handoff

- source: `src/overlays/o057/func_overlay_057_F0004E18_18A8A10.c`
- score: 231/1208 words
- frame: 0x140
- relocations: 373
- first mismatch: +0x100
- summary: 494 falls to 231 on four edits -- both path-list walks respelled as while loops over *list with no named index or re-read local and a (u8) cast rather than a mask, which is a delta-0 pair worth 36 words where each half alone moves the size, and row = 0x51 moved after func_8004B0A4 so the constant leaves the guard branch delay slot, worth 186; and the choice loop bounded on &sourceState[4] rather than &gO57MiddleChoices[4], which closes the surplus instruction -- the loop bound, not the tail reads, is what anchored the callee-saved address web -- taking the size delta to 0 and the first structural difference from +0x34 to +0xBC4; what is left is 109 register-naming and 96 structural words, led by a displaced homed block that no local array dimension moves.
<!-- plateau-handoff:func_overlay_057_F0004E18_18A8A10:end -->
