<!-- plateau-handoff:overlay29DrawGroups:start -->
### `overlay29DrawGroups` plateau handoff

- source: `src/overlays/o029/overlay29DrawGroups.c`
- score: 82/129 words
- frame: 0x58
- relocations: 3
- first mismatch: +0x4C
- summary: Naming the pre-branch E700 command is byte-flat; exact 129-word/frame/relocation geometry still leaves 47 pre-loop scheduling differences.

Fresh configured evidence on 2026-09-04 re-proves the owned overlay-29
`+0x14C8..+0x16CC` range at 129 instructions, frame `0x58`, 82/129
positionally exact words, and first mismatch `+0x4C`. The three runtime
`R_MIPS_26` records at `+0x138/+0x18C/+0x1A8` match the candidate offsets and
types and resolve to the authenticated resident callees.

Retail materializes the E700 and FB00 display-list commands before the enabled
branch and places `groupIndex = 3` in its delay slot. The candidate materializes
only E700 before the branch, hoists `groupIndex`, and delays FB00. The one
authorized structural reshape introduced a `pipeSyncCommand` local beside the
existing `fillCommand` initialization and used it for the first display-list
store. IDO folded that local before scheduling: `.text` remained byte-identical
at SHA-1 `031c8c9a3c86`, with 47 raw/positional differences, 16 opcode
differences, 10 alignment gaps, and 12 aligned structural rows.

The retained source is restored. Reopen only with source-line scheduler trace
evidence for the three pre-loop initializers; do not repeat constant locals or
ordinary statement reordering.
<!-- plateau-handoff:overlay29DrawGroups:end -->
