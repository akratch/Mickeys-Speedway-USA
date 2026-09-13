<!-- plateau-handoff:overlay19BuildSpatialMasks:start -->
### `overlay19BuildSpatialMasks` plateau handoff

- source: `src/overlays/o019/overlay19BuildSpatialMasks.c`
- score: 63/227 words
- frame: 0x80
- relocations: 0
- first mismatch: +0x58
- summary: Point deletion moves draws and regresses; partial-offset naming adds spill costs, and deleting its vertex pointer is draw/byte-flat.

- geometry: Target and configured C are both `0x38C`/908 bytes/227 words with frame `0x80`; the owned Overlay 19 range is `+0xF58..+0x12E4`, ROM `0x18761B0..0x187653C`, followed by separately owned 12-byte assembly padding.
- relocation proof: Target runtime and candidate static surfaces both contain zero relocation records; count, type, offset, and identity surfaces are therefore vacuously exact, and preflight is complete.
- diagnosis: Fresh workbench comparison has 63 raw/normalized differences, two opcode mismatches, 61 register differences, and twenty shift-tolerant alignment gaps.
- history: The body remains the original guarded reconstruction from `752cefadbde9a2ffb9128690055650d33e6edcdc`; this target-named evidence refresh changes no executable source.
- authorized maintenance reproof: A second forced configured V0 from authorization tip `ce9f8cd6b8581e4ec3f3014c88ec2a6e66a8d310` reproduces every metric and confirms the separate `+0x12E4..+0x12F0` padding boundary.
- next action: Preserve the fallback pending a genuinely new natural pointer/item-index coloring or independently proved source structure. Prior declaration, lifetime, load-order, alias, loop-variable, flag, and permutation families remain closed.

#### 2026-09-13, lane `j1`: point and scaled-offset draw controls

Fresh baseline reproduces 227 words, delta zero, frame 0x80, masked 63,
first +0x58: 164 aligned exact, 61 naming, zero immediate and two structural
rows without gaps. Procedure 0 has 63 draws and 417 emissions. Both relocation
surfaces are empty. Existing pointer/arithmetic, lifetime and order evidence
was read; no colour sweep was repeated.

Deleting the explicit point carrier and indexing group->points at the selector
read moves two draws off line 32 and adds three on line 33. Draws become 64,
emissions 416; the complete sequence changes. Geometry and frame stay fixed,
but aligned naming rises to 113 and structural to six, with 119 masked words.
The point-carrier route moves the intended lines and is worse in both windows.

Separately, naming the unsigned selector-plus-base offset before its scale
removes two draws from line 33 while adding three elsewhere. This has 64
draws, 422 emissions, frame 0x88 and three extra words. It scores 218;
alignment is 78 exact, 124 naming, eight immediate and twenty structural
rows including the three additions. Deleting the resulting vertex pointer
and spelling the three field reads through that offset removes one emission
record on line 33, but changes no draw count or order and is object-byte-flat.
Thus pointer deletion does not undo the named-offset candidate's spill cost.

The guarded original remains best. Stop early under ADR 0018: the newly
identified carrier deletion and partial-offset routes have measured local
movement and costs; prior lifetime/order controls cover the remaining stated
levers. This is not general source impossibility. Private source/object pairs,
stock/traced full-TU identity, censuses and aligned deltas are retained.
Commands: assignment gate, configured compilation, draw_census.py before/after,
residual_map.py --object --against, finalize_plateau.py and tools/gates.sh.
The verified ROM uses the fallback; no new matching bytes are credited.
<!-- plateau-handoff:overlay19BuildSpatialMasks:end -->
