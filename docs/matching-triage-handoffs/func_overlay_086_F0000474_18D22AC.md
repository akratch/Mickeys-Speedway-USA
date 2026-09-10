<!-- plateau-handoff:func_overlay_086_F0000474_18D22AC:start -->
### `func_overlay_086_F0000474_18D22AC` plateau handoff

- source: `src/overlays/o086/func_overlay_086_F0000474_18D22AC.c`
- score: 51 differing words
- frame: 0xA8
- relocations: 38
- first mismatch: +0x70
- summary: Exact geometry, frame and all 38 relocations. Carrying the epilogue's 256-scaled speed in the already-declared spA0 instead of temp_a0 removes one caller-saved pool web and closes 8 words. The remaining 51 are one uniform upward step of the caller-saved ladder: the target's v0 webs (the +0x3E0 pointer and the +0x48 dereference) read v1 and a0, its v1 state byte reads a1 and its a1 command pointer reads a2. Removing one more interfering caller-saved web decided before that family closes all four.
<!-- plateau-handoff:func_overlay_086_F0000474_18D22AC:end -->
