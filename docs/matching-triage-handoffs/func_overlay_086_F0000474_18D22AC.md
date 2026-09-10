<!-- plateau-handoff:func_overlay_086_F0000474_18D22AC:start -->
### `func_overlay_086_F0000474_18D22AC` plateau handoff

- source: `src/overlays/o086/func_overlay_086_F0000474_18D22AC.c`
- score: 29 differing words
- frame: 0xA8
- relocations: 38
- first mismatch: +0xA8
- summary: 51 fell to 29 because the lever named as "one interfering caller-saved web removed" is a web SHORTENED, at no width. The two +0x48 dereference sites shared one cfe temporary, giving uopt a single six-reference web (save 30) that spans both switch arms and is therefore live across a call -- and a web live across a call has v0 struck from its candidate list entirely, which the traced p1cost rows show and no CDX_FORCE can override, so the target's v0 was never a colouring choice. Carrying the case-0 site in the already-declared temp_a0 splits that temporary: the case-2/3 site becomes its own one-block web, stops crossing a call, regains v0 and takes it, and the ten-row state-byte family falls into v1 behind it (51 to 30, delta 0, frame unmoved, no new declaration, L44). Writing the 0xC command word through the pointer before the advance is the target's own sh 0(s5) shape and closes one more. The 29 left are 7 words of temp_a0's own web, still spanning the head block's +0x40 reads and the f690 results and so still barred from v0 -- the only free head-block carrier, spA0, costs 579 words and 4 bytes -- and 22 words of the command pointer, whose a1 is held by a web that needs it (forcing it off costs 211 words and 4 bytes) and outranks it 6.67 to 2.67.
<!-- plateau-handoff:func_overlay_086_F0000474_18D22AC:end -->
