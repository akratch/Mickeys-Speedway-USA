<!-- plateau-handoff:overlay57UpdateModeState:start -->
### `overlay57UpdateModeState` plateau handoff

- source: `src/overlays/o057/overlay57UpdateModeState.c`
- score: 21/354 words
- frame: 0x30
- relocations: 59
- first mismatch: +0x108
- summary: The 43-word ring rotation and the 10-word savedEligible reload colour are closed by reading the global back in the byte store (u8)gO57ModeChoice4F8 instead of the local choice, which makes ugen number a ring temp for a load it then forwards from the store one line above, so the pop costs zero instructions and 74 falls to 21 at delta 0; what is left is 16 words of a globalcolor v0/a0 swap between entries and the dead post-decrement copy, 3 words of a store uopt sinks unless savedEligible is memory-resident which costs a frame cell, and 2 words of address order.
<!-- plateau-handoff:overlay57UpdateModeState:end -->
