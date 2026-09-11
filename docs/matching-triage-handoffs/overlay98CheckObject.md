<!-- plateau-handoff:overlay98CheckObject:start -->
### `overlay98CheckObject` plateau handoff

- source: `src/overlays/o098/overlay98CheckObject.c`
- score: 80/111 words
- frame: 0xA8
- relocations: 6
- first mismatch: +0x38
- summary: 31 differences after declaration-order repair; coherent s7/fp cycle remains

Fresh lane measurement (2026-09-11, lane/p7-mix): moving `s32 scratch[10]`
before the vector/local declarations is a strict configured-TU gain, 35 to 31
differing words, with exact stack-home ladder. The remaining 29 naming sites
retain one coherent `s7 <-> fp` cycle plus the float-bank shift. A bounded
20-minute permutation reached scratch score 165 only with an inert
`if (result) {}` guard; that diagnostic was rejected and no exact result was
promoted.
<!-- plateau-handoff:overlay98CheckObject:end -->
