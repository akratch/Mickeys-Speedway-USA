<!-- plateau-handoff:overlay19BuildAdjacency:start -->
### `overlay19BuildAdjacency` plateau handoff

- source: `src/overlays/o019/overlay19BuildAdjacency.c`
- score: 41/123 words
- frame: 0x80
- relocations: 1
- first mismatch: +0x7C
- summary: Residual is one integer ring phase, +2 at row 31 with rows 0-30 byte-identical. The (s16)x & 0x7FFF phantom on frame.suppressed is worth THREE ring pops and the loop head wants two: spelling it adjacentItem & 0x7FFF puts slot 0 on the target t2 and extends prefix-exact 31 to 37, at 48 words because the region after row 36 then wants two pops nobody has found. Named invalidRecords and the comma-joined loop head are both load-bearing; inlining either costs 14 or more.
<!-- plateau-handoff:overlay19BuildAdjacency:end -->
