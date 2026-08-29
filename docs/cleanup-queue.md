# Cleanup queue: matched functions awaiting a more natural C form

Policy (user, 2026-08-27): when a match is only reachable via a non-idiomatic /
"looks fake" C form (a permuter-forced spelling — `do{}while(0)` boundaries,
`^ 0`, `if (1)`, `[x = 0]` index side-effects, redundant temps for register
pressure), **commit the working match now** with an in-source comment noting the
form is inert, and record it here. Return later to find a cleaner C that produces
the same bytes. These are byte-identical matches with **no fakes and no
ROM-derived content** — only the *spelling* is unnatural.

This exception records already-proved exact C; it never authorizes adding an
inert guard or operation to improve a non-exact candidate.

A match here is DONE for scoreboard purposes; the cleanup is a quality follow-up,
not a correctness gap.

| Function | TU | Non-natural form | Notes for the cleaner pass |
|---|---|---|---|
| `func_80046AA8` | diCpu | `do{}while(0)` around the setup block | The boundary forces IDO to close the setup as its own basic block before the blit loop. A cleaner equivalent (a real early computation dependency?) may exist. |
| `func_800320F0` | runlink | four inert allocation aids: one `overlayCount` block and three `if (1) {}` blocks | The blocks preserve the exact long-lived-register allocation found by the bounded permuter. Seek a natural declaration/lifetime spelling with the same 101 words and 21 relocations. |
| `runlinkUnloadOverlay` | runlink | `patchOperation ^ 0` comparison | The algebraically inert xor retains exact comparison operand allocation. Seek a direct comparison with the same bytes and relocation surface. |
| `func_8002C94C` | saves | `if (1)` around entry initialization | The block preserves the exact callee-saved slot/counter tie-break. Seek a natural declaration or loop-scope spelling retaining exact bytes and relocations. |
| `piRomLoadCompressed` | pi | empty `if (1)` before the ROM copy | The empty boundary preserves the exact tail-call parameter lifetime. Seek a natural gzip/output-pointer scope retaining exact bytes and relocations. |
| `func_80003760` | audio manager | canceling uninitialized pointer increment/decrement plus one-iteration `do/while` | The legacy operations preserve the exact 25-word temp/FIFO allocation. Replace them with defined, natural initialization and grouping while retaining frame, relocations, and linked bytes. |
| `overlay40AddEntry` | overlay 40 entry allocator | repeated masks of known-zero `new_var` added to `green` | The algebraically zero expression preserves the exact entry/scales register allocation. Seek a natural declaration/lifetime spelling retaining 33 words and four relocations. |
| `func_overlay_014_F00009F4_18702CC` | overlay 14 asset loader | inert `if (1) {}` around the relocation loop | The boundary preserves the exact pool allocation found by the annotated-target permuter; seek a natural loop/lifetime spelling. |
| `overlay1UpdateValueCache` | overlay 1 value cache | algebraically zero `remaining * 0` return | The live counter read preserves IDO's exact caller-saved web. Seek a natural zero return with the same 120 words and two local relocation pairs. |
| `overlay59DrawFrame` | overlay 59 frame renderer | empty duplicated `!index` condition | The inert condition preserves the exact entry/owner register order. Seek a natural declaration or lifetime spelling with the same 76 words and relocation surface. |
| `func_overlay_012_F00000C4_186D344` | overlay 12 release path | empty `!i` condition after fixed-count clear | The inert condition preserves exact allocation in the 60-word release routine. Seek a natural loop/epilogue spelling with the same bytes and relocation surface. |
| `overlay12Initialize` | overlay 12 resource initializer | empty `!remaining` condition after fixed-count clear | The inert condition preserves exact allocation in the 49-word initializer. Seek a natural loop/epilogue spelling with the same bytes and relocation surface. |
| `overlay3FindClosestObject` | overlay 3 closest-object search | empty `&pad`, `!cursor`, and `if (1)` blocks | The three inert blocks preserve the exact 77-word allocation/control-flow form. Seek natural declarations/scopes with the same frame and relocations. |
| `func_overlay_057_F0000954_18A454C` | overlay 57 interface update | empty read of `O57_config0954.field08` | The inert read preserves the exact register web in the 435-word body. Seek a natural use/lifetime spelling with the same bytes and relocations. |
| `overlay82Update` | overlay 82 update | empty `currentValues` read | The inert read preserves IDO's exact web priority in the 291-word body. Seek a natural declaration/lifetime spelling with the same bytes and relocations. |
| `func_800557F8` | anim collision callback | two empty `if (1)` blocks | The inert blocks preserve the target `v0` allocation. Seek natural call/result lifetimes with the same bytes and relocations. |
| `func_80055970` | anim collision callback | empty `if (1)` block | The inert block preserves the callback-result allocation. Seek a natural call/result lifetime with the same bytes and relocations. |
| `func_80055B24` | anim collision callback | empty `if (1)` block | The inert block preserves the post-callback allocation. Seek a natural typed-call/result lifetime with the same bytes and relocations. |
| `diRcpDmaOffsets` | diRcp debug decoder | empty `dList` read | The inert read preserves exact debug-call argument allocation. Seek a natural formatting expression with the same bytes and relocations. |
| `diRcpMoveWd` | diRcp debug decoder | duplicated empty `command` read | The inert read preserves exact switch/formatting allocation. Seek a natural command lifetime with the same bytes and relocations. |
| `partObjFreeTriggers` | particle trigger cleanup | empty `offset` read | The inert read preserves exact loop allocation. Seek a natural pointer/index spelling with the same bytes and relocations. |
| `__scHandleRSP` | scheduler RSP handler | empty task-flags condition | The inert condition preserves exact scheduler allocation. Seek a natural state/branch spelling with the same bytes and relocations. |
| `func_800148E0` | track fog changer | empty `if (1)` block | The inert block preserves exact fog-distance allocation. Seek a natural scope/association spelling with the same bytes and relocations. |
| `overlay8ScaleOutputs` | overlay 8 scale outputs | empty `!index` condition | The inert condition preserves exact selector/index allocation. Seek a natural cast/lifetime spelling with the same bytes and relocations. |
| `overlay27Activate` | overlay 27 state transition | duplicated empty state-null condition | The inert condition preserves exact saved-object allocation. Seek a natural assignment/lifetime spelling with the same bytes and relocations. |
| `overlay7FillValues` | overlay 7 value-table reset | empty duplicated `!value` condition and dummy comma-expression operand | Both inert forms preserve the exact 11-word allocation. Seek a natural spelling retaining 11 words and the two LOCAL HI16/LO16 records. |
| `overlay1UpdateCountdown` | overlay 1 countdown update | impossible empty pointer/mask condition plus packed statement line | Retained C is 22/22 after relocation masking with seven records. First remove only the condition; independently unpack the three real statements, retaining exact bytes. |
| `overlay80UpdateContact` | overlay 80 contact updater | redundant `new_var`, `new_var2`, and `new_var3` aliases | The aliases preserve IDO's shipped 180-word allocation and `0x80` frame. Seek natural declaration/lifetime spelling retaining all 20 relocation records. |

Add rows as ugly-but-verified matches land. When revisiting: reproduce the match
with idiomatic C, `gmake verify`, then delete the row + the in-source comment.
