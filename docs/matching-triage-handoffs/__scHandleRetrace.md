<!-- plateau-handoff:__scHandleRetrace:start -->
### `__scHandleRetrace` plateau handoff

- source: `src/main/sched.c`
- score: 84 differing words
- frame: 0xE8
- relocations: 114
- first mismatch: +0x3B4
- summary: Fresh V0 remains one word short with 84 differing target positions; relocation counts remain 114/109.

- geometry: The resident target is `0x664`/1,636 bytes/409 words at VRAM `0x80030AE8..0x8003114C`, ROM `0x316E8..0x31D4C`; configured C emits 1,632 bytes/408 words. Both frames are `0xE8`, there is no padding, and `__scHandleRSP` begins at the exact target end.
- ABI/flags: The configured candidate is `void __scHandleRetrace(OSSched *)` under resident game-code `-O2 -mips2 -32`; `sched.c.o` retains its canonical `0x38`-byte rodata trim postprocess.
- relocation proof: Target has 114 static records and candidate 109; 99 offsets/types and 97 stable/effective identities align. All 109 candidate identities resolve, but count, position, and identity surfaces remain nonexact.
- diagnosis: Workbench reports structure mismatch with a true `-1` instruction delta, 71 opcode differences, 12 register differences, and five alignment gaps. The 84 positional differences begin at `+0x3B4`.
- caller/donor evidence: `__scMain` directly calls the function for `VIDEO_MSG`. JFG `__scHandleRetrace` is the disclosed body donor and strongest skeleton hit at 0.3822 Jaccard; DKR/JFG `dummy_80079810` is a distant 0.1324 and supplies no replacement evidence.
- history: The guarded body last changed in `f71f25307fdc2d541b4edbdc9bde92057115a81f`; stale prose was refreshed in `b453619c2d37`. This exact-symbol evidence refresh changes no executable source.
- next action: Run only forced V0 and structured finalize maintenance. Diagnostic scheduling, source-line grouping, explicit narrowing, flags, permutation, and C-body mutation remain closed.
<!-- plateau-handoff:__scHandleRetrace:end -->
