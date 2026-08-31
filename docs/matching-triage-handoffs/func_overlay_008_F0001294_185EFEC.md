<!-- plateau-handoff:func_overlay_008_F0001294_185EFEC:start -->
### `func_overlay_008_F0001294_185EFEC` plateau handoff

- source: `src/overlays/o008/overlay_008.c`
- score: 1030 differing words
- frame: 0x180
- relocations: 137
- first mismatch: +0x0
- summary: Typed 1256-word direct reconstruction is three words short; authenticated ABI/type fixes remove 193 differing words, but 0xD0 of excess automatic-home frame remains.

Mechanism-driven reopen evidence on base `4b4109b583ca0a37427e0f38f3fa0fbb14474220`:

- The authenticated overlay-8 text range is `+0x1294..+0x2640` (5,036 bytes, 1,259 words). Its only in-tree caller is the same overlay at `+0x6A4`; the call and preceding C establish the three-argument `void (O8P0058Owner *, O8P0058State *, f32)` ABI.
- The new `NON_MATCHING` body is a typed reconstruction of the complete control-flow draft, including the per-update loop, motion/angle integration, resource lifetime, selector flags, and color propagation. It compiles with the TU's configured `-O2 -mips2 -Wab,-r4300_mul` policy; the ordinary build continues to select the authenticated assembly fallback.
- All 137 shipped runtime records resolve to 63 concrete identities. The 15 calls are `overlay8GetIndexed`, local `func_overlay_008_F0001000_185ED58`, three `controlSetRumble`, `func_800299E8`, three `func_800031E8`, three `func_80002FE0`, `func_8002A204`, `mathDiffAngle`, and `func_800031C0`; the local `+0x1000` helper and all rumble calls take three arguments, not the former four-argument proxy forms.
- The former `D_0` source alias conflated three target identities: the motion scalar at reserved module `4095:+0x458C4`, impact gate at `4093:+0x31A4`, and final color gate at `4093:+0x31AC`. They now have separate typed roles. The 40 `D_F8..D_194` fields remain distinct lexical identities but are authenticated as one local target block at `+0x74A8..+0x7544`; expressing them through one C base is rejected because IDO commoned the address and removed 33 target instructions.
- Configured V0 now emits 1,256 words in a `0x180` frame versus the target's 1,259 words and `0xB0` frame. Workbench reports 1,030 relocation-masked differences, 1,036 raw differences, first mismatch `+0x0`, and `structure-mismatch`. The exact-size `temp_v0+0x14`-as-f32 diagnostic emits 1,259 words with 1,128 differences, improving the prior exact-size 1,201-word residual but not the overall best.
- The strict gains were: authenticated callees/arity/return types (`1264 -> 1261` words, `0x1B8 -> 0x1B0`, `1223 -> 1211` differences); typed entry parameters (`0x1B0 -> 0x1A8`); direct resource fields (`0x1A8 -> 0x190`); and changing the capped `state+0x192` carrier from `u8` to `s32` (`1211 -> 1030` differences, 798 opcode mismatches). Removing four inert m2c homes reached the retained `0x180` frame without changing code bytes.
- The full 119-combination flag lattice found no exact result. Removing the TU-specific assembler scheduling flag reached exact size but still left 1,201 differences from `+0x0`; that flag is shared by the translation unit and is not a source-authentic promotion lever.
- Pinned DKR v77 `src/racer.c` supplies a permitted semantic skeleton for the top-speed, spin/drift, steering, sound, and particle phases. It confirms the absolute-speed cap/subtract/floor curve, drift multiplier 68 versus 58, reverse steering sign, signed spin state, rumble, and `+/-0x2EE` yaw clamp; JFG and exact-object donor scans remain negative.
- Resume from the retained 1,030-difference form by removing automatic homes without replacing explicit single-load carriers with repeated field reads: a 13-copy direct-access batch reached a `0x158` frame and one-word size delta but regressed to 1,125 differences. The target has nine non-save homes at `+0x44/+0x60/+0x68/+0x78/+0x84/+0x88/+0x8C/+0x94/+0x9C`; candidate still has `0xD0` excess frame. Do not run flags or generic permutation until the home set is structurally close.
<!-- plateau-handoff:func_overlay_008_F0001294_185EFEC:end -->
