<!-- plateau-handoff:func_overlay_057_F00060F8_18A9CF0:start -->
### `func_overlay_057_F00060F8_18A9CF0` plateau handoff

- source: `src/overlays/o057/func_overlay_057_F00060F8_18A9CF0.c`
- score: 256/441 words
- frame: 0x60
- relocations: 175
- first mismatch: +0x14
- summary: The +2 temp-ring offset is two loop-invariant webs in the menu-fill loop, the menu-entry stride and the controller-map base, coloured t0 and t1 here and s1 and s2 in the target, which removes both from the ring for the whole function; walking the entry pointer frees them and closes 50 of the 256 words but costs 12 instructions, so what is wanted is an indexed form that keeps the reload and multiply while leaving both webs off the ring.
<!-- plateau-handoff:func_overlay_057_F00060F8_18A9CF0:end -->
