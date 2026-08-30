<!-- plateau-handoff:overlay19BuildSpatialMasks:start -->
### `overlay19BuildSpatialMasks` plateau handoff

- source: `src/overlays/o019/overlay19BuildSpatialMasks.c`
- score: 63 differing words (164/227 exact positional words)
- frame: 0x80
- relocations: 0
- first mismatch: +0x58
- summary: Exact-sized configured C retains the target frame but differs in 61 register words and two opcode-order sites. All 119 flag configurations were nonexact; ten source-faithful declaration, lifetime, load-order, alias, and loop-variable forms produced no legal gain. A four-minute, two-worker permutation batch improved its internal score only by replacing the advancing Z-bin lower bound with a constant bound, which changes behavior and was rejected. JFG's two strongest similarity hits are assembly-only; DKR `func_8002C954` is useful structural guidance for the X/Z spatial-mask loops, while Mickey additionally batches spans and emits Y masks. Resume only with a new natural pointer/item-index coloring mechanism or independently proved original-source structure.
<!-- plateau-handoff:overlay19BuildSpatialMasks:end -->
