<!-- plateau-handoff:overlay17AdvanceChain:start -->
### `overlay17AdvanceChain` plateau handoff

- source: `src/overlays/o017/overlay17AdvanceChain.c`
- score: 96/147 words
- frame: 0x70
- relocations: 1
- first mismatch: +0x18
- summary: Removing the named newBuffer web is byte-flat; exact 147-word/frame/relocation geometry still leaves 51 pre-call copy-schedule differences.

Fresh configured evidence on 2026-09-04 re-proves the owned overlay-17
`+0x668..+0x8B4` range at 147 instructions, frame `0x70`, and 96/147
positionally exact words. The sole runtime `R_MIPS_26` record at function
offset `+0xF4` resolves to the local overlay entry at `+0x0`; target and
candidate offset, type, and identity all agree.

Workbench 0.8.0 reports a candidate pool lane one web longer than retail and
routes the first divergence through its drop-a-declared-local lever. The one
authorized structural reshape removed `newBuffer` and expressed the stored
and indexed value as the same `(u8)(oldBuffer ^ 1)` computation. IDO commoned
the repeated expression back into the existing web: `.text` SHA-1 remained
`f8d1c89a853e`, with 51 raw/positional differences, 33 opcode differences,
16 alignment gaps, and the same first mismatch at `+0x18`.

The retained source is restored. This closes the unnamed-new-buffer route;
reopen only with producer evidence for the interleaved source/destination
offset schedule, not another declaration removal or repeated xor spelling.
<!-- plateau-handoff:overlay17AdvanceChain:end -->
