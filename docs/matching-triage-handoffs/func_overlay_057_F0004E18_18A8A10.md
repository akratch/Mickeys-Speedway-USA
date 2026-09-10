<!-- plateau-handoff:func_overlay_057_F0004E18_18A8A10:start -->
### `func_overlay_057_F0004E18_18A8A10` plateau handoff

- source: `src/overlays/o057/func_overlay_057_F0004E18_18A8A10.c`
- score: 272/1208 words
- frame: 0x140
- relocations: 373
- first mismatch: +0x34
- summary: 494 falls to 272 on three edits -- both path-list walks respelled as while loops over *list with no named index or re-read local and a (u8) cast rather than a mask, which is a delta-0 pair worth 36 words where each half alone moves the size, and row = 0x51 moved after func_8004B0A4 so the constant leaves the guard branch delay slot, worth 186; what is left is ONE surplus instruction, the s6 address web the candidate keeps for gO57MiddleChoices across the tail where the target re-materialises, which shifts every word past about +0xf90 and hides a prefix residual of only 57 masked words.
<!-- plateau-handoff:func_overlay_057_F0004E18_18A8A10:end -->
