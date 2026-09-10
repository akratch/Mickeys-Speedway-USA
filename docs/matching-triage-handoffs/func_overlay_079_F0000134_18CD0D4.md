<!-- plateau-handoff:func_overlay_079_F0000134_18CD0D4:start -->
### `func_overlay_079_F0000134_18CD0D4` plateau handoff

- source: `src/overlays/o079/func_overlay_079_F0000134_18CD0D4.c`
- score: 198/882 words
- frame: 0xB8
- relocations: 88
- first mismatch: +0x7C
- summary: integer ring closed. Two composed edits, each a regression alone, take 288 to 229: spelling the race-state bit test as (flags << 0xD) & 0x80000000U == 0 burns the ugen ring temp the target burns between +0x54 and +0x58 (328 alone), and routing state->target->state through the already-declared spawned gives that pointer the target's pool colour instead of a ring temp (374 alone). Splitting dx = state->targetX - object->x into dx = state->targetX; dx -= object->x at the mode-0 dot product then closes 31 more. The remaining 198 are 167 fp rows: the fp scratch ring is still one web short from +0x368, and roughly 81 of them are the +0xB80 tail that cannot be bought back at 882 instructions
<!-- plateau-handoff:func_overlay_079_F0000134_18CD0D4:end -->
