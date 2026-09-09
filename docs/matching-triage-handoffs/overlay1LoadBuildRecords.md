<!-- plateau-handoff:overlay1LoadBuildRecords:start -->
### `overlay1LoadBuildRecords` plateau handoff

- source: `src/overlays/o001/overlay_001_head.c`
- score: 470 differing words
- frame: 0xD8
- relocations: 114
- first mismatch: +0x34
- summary: Exact frame; 23 words short. Local-data ownership creates 114 candidate versus 32 target relocations; consolidate the owner before further source matching.

- identity: Overlay 1 text `+0x10C8..+0x19B8`, ROM `0x184D4A8..0x184DD98`, 2,288 executable bytes with no credited padding
- ABI and flags: `void overlay1LoadBuildRecords(void)`, configured `-O2 -mips2 -32`; candidate and target frames are both `0xD8`
- V0: 549 candidate versus 572 target words, 470 relocation-masked and 491 raw differences, 471 aligned differences, 114 candidate versus 32 target relocations
- donor: the nearest permitted masked-shape row is JFG `squadsInitAIArrays` at 0.0845; all other returned rows are below 0.044, so no credible donor body exists
- retained evidence: target offset `+0x6C8` loads packed offset `+0xC` unsigned-halfword, proving `link` is `u16`; representing metric iteration as a 0x10-byte sliding cursor with fields at `+0x14` reproduces the target's `+0x70` cursor bases and reduces aligned differences from 471 to 462
- attempts: four coherent source mechanisms were compiled; declaration order and reverse clear-pointer forms regressed and were rejected, while duplicate-symbol spelling was byte-flat and reverted
- blocker: target local-data accesses largely carry no runtime relocation, while the split candidate uses many `Reloc` calls and separately relocated extern aliases; workbench reports 145 relocation-metadata and 153 relocation-target mismatches
- next lever: consolidate this function with the owning Overlay 1 data/callees so the target's local identities are representable, then remeasure geometry before revisiting loops or allocation
<!-- plateau-handoff:overlay1LoadBuildRecords:end -->
