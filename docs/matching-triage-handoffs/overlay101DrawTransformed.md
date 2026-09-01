<!-- plateau-handoff:overlay101DrawTransformed:start -->
### `overlay101DrawTransformed` plateau handoff

- source: `src/overlays/o101/overlay101DrawTransformed.c`
- score: 104/166 words
- frame: 0x90
- relocations: 7
- first mismatch: +0x64
- summary: Fresh exact-size V0 retains 62 raw differences; unresolved call proxy shifts raw first to +0x64, while the first nonrelocation mismatch remains +0x7C.

- geometry: Target and configured C are both `0x298`/664 bytes/166 words with frame `0x90`; the owned Overlay 101 range is `+0x29A4..+0x2C3C`, ROM `0x18DE1C4..0x18DE45C`, immediately followed by `overlay101BuildFrame`.
- relocation proof: Target runtime and candidate static surfaces each contain seven `R_MIPS_26` records, and all seven offsets/types align at `+0x64`, `+0x19C`, `+0x1B8`, `+0x1DC`, `+0x208`, `+0x22C`, and `+0x27C`. All candidate proxy identities remain unresolved; the runtime table uniquely authenticates three resident targets and the Overlay 101 `+0x2118` and repeated `+0x1F80` targets.
- diagnosis: Workbench reports structure mismatch with eleven opcode mismatches, 51 register differences, and two alignment gaps. The seven unresolved call proxies prevent identity-exact promotion but do not change the exact code extent.
- stale-record result: Exact geometry, the 62-word deficit, frame, and first nonrelocation mismatch `+0x7C` reproduce. Fresh raw comparison begins at the unresolved proxy call at `+0x64`.
- next action: Preserve the fallback until exact candidate proxy witnesses or a genuinely new source-authentic command-schedule/temp-FIFO mechanism exists. Prior command-pointer, dimension-width, rotation-lifetime, flag, and permutation families remain closed.
<!-- plateau-handoff:overlay101DrawTransformed:end -->
