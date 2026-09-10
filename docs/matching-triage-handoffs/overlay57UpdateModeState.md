<!-- plateau-handoff:overlay57UpdateModeState:start -->
### `overlay57UpdateModeState` plateau handoff

- source: `src/overlays/o057/overlay57UpdateModeState.c`
- score: 21/354 words
- frame: 0x30
- relocations: 59
- first mismatch: +0x108
- summary: 74 fell to 21 by reading the global back in the byte store (u8)gO57ModeChoice4F8 instead of the local choice, so ugen numbers a ring temp for a load it forwards from the store above it and the pop costs zero instructions; the 21 that remain are 16 words of a globalcolor swap between entries and the dead post-decrement copy, 3 words of a store uopt sinks, and 2 words of address order, and this pass corrects the 3: savedEligible is already memory-resident at sp+0x28 in the candidate so it is partial-dead-store sinking rather than a missing home, and the frame cost of forcing residency is a declaration-position artifact -- volatile or a one-element array declared BETWEEN timer and eligible keeps frame 0x30 and home 0x28 and lands the store, paying instead 18 words of re-colouring in the timer region because eligible loses its long live range and is coloured before the address web.
<!-- plateau-handoff:overlay57UpdateModeState:end -->
