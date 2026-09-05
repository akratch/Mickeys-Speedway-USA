<!-- plateau-handoff:overlay17DrawStrip:start -->
### `overlay17DrawStrip` plateau handoff

- source: `src/overlays/o017/overlay17DrawStrip.c`
- score: 103/119 words
- frame: 0x40
- relocations: 1
- first mismatch: +0x0
- summary: Reusing pair for the final sync improves 18 to 16 differences in both the object and linked overlay, with no out-of-range differences. The remaining blocker is the 0x38 target frame versus 0x40 candidate frame; packet-local consolidation recovers the frame but destabilizes allocation.
<!-- plateau-handoff:overlay17DrawStrip:end -->
