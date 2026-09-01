<!-- plateau-handoff:overlay19BuildAdjacency:start -->
### `overlay19BuildAdjacency` plateau handoff

- source: `src/overlays/o019/overlay19BuildAdjacency.c`
- score: 82/123 words
- frame: 0x80
- relocations: 1
- first mismatch: +0x7C
- summary: Fresh V0 reproduces exact geometry and 41 register-only differences; the sole +0x12C call relocation is exact to local +0xC1C.

- geometry: Target and configured C are both `0x1EC`/492 bytes/123 words with frame `0x80`; the owned Overlay 19 range is `+0xA30..+0xC1C`, ROM `0x1875C88..0x1875E74`, immediately followed by `overlay19FindAdjacent`.
- relocation proof: Target and candidate each contain one `R_MIPS_26` at `+0x12C`. Runtime identity proof resolves both to the same local Overlay 19 owner at `+0xC1C`; count, offset, type, identity, and addend are exact, and preflight is complete.
- diagnosis: Workbench reports allocation mismatch with zero opcode differences and zero alignment gaps; all 41 residual words are register allocation differences.
- stale-record result: The retained 492-byte geometry, 41-word deficit, frame, and first mismatch `+0x7C` reproduce unchanged.
- next action: Preserve the fallback pending a genuinely new source-authentic temporary-web mechanism. The previously attempted edge-loop locals, statement ordering, register hints, signedness, flags, and permutation remain closed.
<!-- plateau-handoff:overlay19BuildAdjacency:end -->
