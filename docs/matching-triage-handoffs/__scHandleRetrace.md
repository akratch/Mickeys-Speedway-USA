<!-- plateau-handoff:__scHandleRetrace:start -->
### `__scHandleRetrace` plateau handoff

- source: `src/main/sched.c`
- score: 84 differing words
- frame: 0xE8
- relocations: 114
- first mismatch: +0x3B4
- summary: Authorized V0 reproduces the one-word deficit and 84 differences; relocation counts remain 114/109.

- geometry: The resident target remains `0x664`/1,636 bytes/409 words at VRAM `0x80030AE8..0x8003114C`, ROM `0x316E8..0x31D4C`; configured C emits 1,632 bytes/408 words. Both frames are `0xE8`, there is no padding, and `__scHandleRSP` starts at the exact target end.
- ABI/flags: The configured candidate is `void __scHandleRetrace(OSSched *)` under resident game-code `-O2 -mips2 -32` with the canonical scheduler rodata trim.
- relocation proof: Target has 114 records and candidate 109; 99 offsets/types and 97 stable/effective identities align. All 109 candidate identities resolve, but the missing records track the unchanged one-word/late-schedule deficit rather than a new identity mechanism.
- diagnosis: Workbench reproduces the true `-1` instruction delta, 84 positional differences from `+0x3B4`, 71 opcode differences, 12 register differences, and five alignment gaps.
- caller/donor result: `__scMain` remains the direct `VIDEO_MSG` caller. The disclosed JFG donor remains strongest at 0.3822 skeleton Jaccard; DKR/JFG `dummy_80079810` remains 0.1324 and supplies no new source mechanism.
- maintenance result: No new caller, donor, or relocation mechanism appeared. The exact tracked scope therefore permits no C-body, flag, or permutation attempt.
- next action: Preserve the fallback until a new source-authentic mechanism explains the missing word and late retrace-counter/store schedule.
<!-- plateau-handoff:__scHandleRetrace:end -->
