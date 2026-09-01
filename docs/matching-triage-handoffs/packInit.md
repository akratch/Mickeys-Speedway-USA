<!-- plateau-handoff:packInit:start -->
### `packInit` plateau handoff

- source: `src/main/saves.c`
- score: 81/115 words
- frame: 0x60
- relocations: 20
- first mismatch: +0xA0
- summary: Fresh exact-size V0 preserves 34 differences; 19/20 relocations are exact, with D_800D21C8 LO16 delayed from target +0xA0 to candidate +0xAC.

- geometry: Fresh configured C and target are both `0x1CC`/460 bytes/115 words with frame `0x60`; owned VRAM is `0x8002D300..0x8002D4CC`, ROM is `0x2DF00..0x2E0CC`, no padding intervenes, and `packIsPresent` begins at the end boundary.
- relocation proof: Target and candidate each have 20 fully resolved static identities: eight `R_MIPS_26` calls and six HI16/LO16 pairs. Nineteen sites match offset, type, and identity. The sole site drift is the `D_800D21C8` LO16 from target `+0xA0` to candidate `+0xAC`; its HI16 remains exact at `+0x9C`, and no identity is ambiguous.
- stale-record result: The retained 460-byte, 34-difference, first-`+0xA0` record reproduces unchanged at 81/115 positional words.
- next action: Preserve the fallback until a new source-authentic mechanism explains the delayed `D_800D21C8` low-half materialization and associated register rotation; flags, prior source families, and permutation remain closed.
- finalizer note: `finalize_plateau.py` refused because this shared TU contains the unrelated legacy inline `func_8002CF6C` handoff. The target marker was appended at EOF without moving measured source lines, and `plateau_handoff_audit.py --write` generated this fixed shard.
<!-- plateau-handoff:packInit:end -->
