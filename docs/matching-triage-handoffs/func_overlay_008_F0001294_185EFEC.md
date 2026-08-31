<!-- plateau-handoff:func_overlay_008_F0001294_185EFEC:start -->
### `func_overlay_008_F0001294_185EFEC` plateau handoff

- source: `src/overlays/o008/overlay_008.c`
- score: 1223 differing words
- frame: 0x1B8
- relocations: 137
- first mismatch: +0x0
- summary: Typed 1264-word direct reconstruction is five words long; recover exact helper/global identities and reduce m2c-derived automatic homes before allocation work.

Fresh direct-reconstruction evidence on base `0ce733343a82eab623942137ddc9ec917cd15b00`:

- The authenticated overlay-8 text range is `+0x1294..+0x2640` (5,036 bytes, 1,259 words). Its only in-tree caller is the same overlay at `+0x6A4`; the call and preceding C establish the three-argument `void (O8P0058Owner *, O8P0058State *, f32)` ABI.
- The new `NON_MATCHING` body is a typed reconstruction of the complete control-flow draft, including the per-update loop, motion/angle integration, resource lifetime, selector flags, and color propagation. It compiles with the TU's configured `-O2 -mips2 -Wab,-r4300_mul` policy; the ordinary build continues to select the authenticated assembly fallback.
- Configured V0 emits 1,264 words in a `0x1B8` frame versus the target's 1,259 words and `0xB0` frame. Workbench reports 1,223 relocation-masked differences, 1,224 raw differences, first mismatch `+0x0`, and `structure-mismatch`; the frame has 264 extra non-save bytes from the conservative m2c-derived automatic homes.
- Target and candidate each contain 137 static relocations, but only 13 offset/type sites align at this broad plateau. Candidate identity resolution is incomplete at 136 records, so no relocation-identity or linked-match claim is made.
- The full 119-combination flag lattice found no exact result. Removing the TU-specific assembler scheduling flag reached exact size but still left 1,201 differences from `+0x0`; that flag is shared by the translation unit and is not a source-authentic promotion lever.
- The context-aware m2c pass was blocked by unrelated incomplete/flexible-array declarations in the generated whole-project context; a contextless draft was used only to recover control flow, then rewritten with Mickey-local overlay types, globals, and helper roles. No donor source was used, and the skeleton oracle could not run because no reference directories are configured in this lane.
- Resume by proving the remaining helper roles and overlay-local global identities from call/relocation evidence, then collapsing the conservative temporary/home set toward the target frame. Do not spend allocator or permutation work while the body remains 1,201-plus words away under every tested flag group.
<!-- plateau-handoff:func_overlay_008_F0001294_185EFEC:end -->
