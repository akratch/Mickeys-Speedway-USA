<!-- plateau-handoff:func_overlay_057_F0000000_18A3BF8:start -->
### `func_overlay_057_F0000000_18A3BF8` plateau handoff

- source: `src/overlays/o057/func_overlay_057_F0000000_18A3BF8.c`
- score: 282/597 words
- frame: 0x90
- relocations: 244
- first mismatch: +0x8
- summary: One shared stack packet for both spawn calls and a rolled 32-entry stride loop take 345 to 282; frame is still 0x18 over the target's 0x78. 2026-09-12, lane p11-mid: the shared packet is a FALSE ECONOMY and the comment above the union in this file states the opposite of what the target's stack census says. tools/frame_census.py reads 24 target homes against our 17, and the target's are two disjoint packet blocks -- a 20-byte one at +0x54 to +0x67 and a 13-byte one at +0x3C to +0x48 -- where we have one 24-byte union at +0x78. Splitting the union back into an O57SpawnPacket and an O57FinalSpawnPacket reproduces BOTH blocks with their internal structure matching slot for slot: our upper block then spans 0x9F to 0x88 against the target's 0x6B to 0x54, the same 0x17, and our lower block maps 0x84 0x83 0x82 0x7E 0x7C 0x7A 0x78 onto the target's 0x48 0x47 0x46 0x42 0x40 0x3E 0x3C at a constant 0x3C, with exactly one target store at +0x44 unaccounted. It also moves the size delta from -20 to -12, two instructions closer to the target's 597. It costs score -- 282 to 335, and the same either declaration order -- because every home below it then shifts, so it is only worth taking together with the frame fix it exposes. That fix is now localised: with the split the frame is 0xA0 against 0x78 and the ENTIRE excess is below the packets. Ours runs 0x78 down to 0x2C, sixty bytes of block-scoped register locals; the target's runs 0x3C down to 0x2C, sixteen. We declare sixteen pointer and index locals across seven inner blocks -- three separate `i`, two separate `spawned` -- and the target's homes leave room for about six. Merging them to function scope is the next edit, and under L115 the reuse is what the target is already doing.

Measured this lane:

- base 282 masked, 313 raw, size delta -20, frame 0x90, 17 homes against 24.
- the union split, initial declared first: 335 masked, 365 raw, delta -12,
  frame 0xA0, 23 homes against 24.
- the union split, final declared first: identical numbers, so the two packets'
  relative declaration order is inert at this stage.

Read the two ladders side by side with tools/frame_census.py before touching
anything here: the 0x18 frame excess and the 98 structural words are one
question, and the packet count is the half of it that the old comment gets
backwards.
<!-- plateau-handoff:func_overlay_057_F0000000_18A3BF8:end -->
