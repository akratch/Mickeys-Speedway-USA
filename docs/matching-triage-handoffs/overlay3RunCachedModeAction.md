<!-- plateau-handoff:overlay3RunCachedModeAction:start -->
### `overlay3RunCachedModeAction` plateau handoff

- source: `src/overlays/o003/overlay3RunCachedModeAction.c`
- score: 79/113 words
- frame: 0x58
- relocations: 15
- first mismatch: +0x14
- summary: Fresh V0 is exact-size with 34 raw differences; unresolved proxies move raw first to +0x14, while the first nonrelocation mismatch remains +0x84.

- geometry: Target and configured C are both `0x1C4`/452 bytes/113 words with frame `0x58`; the owned overlay range is `+0xB8..+0x27C`, ROM `0x1859DE8..0x1859FAC`, and the next owner starts exactly at `+0x27C`.
- relocation proof: Target runtime and candidate static surfaces each contain 15 records: eleven `R_MIPS_26` calls and two HI16/LO16 pairs. Thirteen sites align in offset and type. The encode-angle and path-check calls are candidate `+0xA4/+0xD4` versus target `+0xA0/+0xD0`; all 15 candidate proxy identities remain unresolved, while the runtime table uniquely authenticates the target resident and Overlay 1/2/3/36 identities.
- stale-record result: Exact geometry and the 34-word deficit reproduce. Fresh raw comparison starts at the unresolved proxy word `+0x14`; relocation-masked structural comparison retains the prior first mismatch `+0x84`.
- next action: Preserve the fallback until canonical proxy evidence resolves the candidate identities or a new source-authentic valid/mode handoff mechanism exists. Prior source forms, flags, and permutation remain closed.
<!-- plateau-handoff:overlay3RunCachedModeAction:end -->
