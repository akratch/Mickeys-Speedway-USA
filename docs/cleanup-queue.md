# Cleanup queue: matched functions awaiting a more natural C form

Policy (user, 2026-08-27): when a match is only reachable via a non-idiomatic /
"looks fake" C form (a permuter-forced spelling — `do{}while(0)` boundaries,
`^ 0`, `if (1)`, `[x = 0]` index side-effects, redundant temps for register
pressure), **commit the working match now** with an in-source comment noting the
form is inert, and record it here. Return later to find a cleaner C that produces
the same bytes. These are byte-identical matches with **no fakes and no
ROM-derived content** — only the *spelling* is unnatural.

A match here is DONE for scoreboard purposes; the cleanup is a quality follow-up,
not a correctness gap.

| Function | TU | Non-natural form | Notes for the cleaner pass |
|---|---|---|---|
| `func_80046AA8` | diCpu | `do{}while(0)` around the setup block | The boundary forces IDO to close the setup as its own basic block before the blit loop. A cleaner equivalent (a real early computation dependency?) may exist. |
| `func_800320F0` | runlink | four inert allocation aids: one `overlayCount` block and three `if (1) {}` blocks | The blocks preserve the exact long-lived-register allocation found by the bounded permuter. Seek a natural declaration/lifetime spelling with the same 101 words and 21 relocations. |
| `func_overlay_014_F00009F4_18702CC` | overlay 14 asset loader | inert `if (1) {}` around the relocation loop | The boundary preserves the exact pool allocation found by the annotated-target permuter; seek a natural loop/lifetime spelling. |
| `overlay1UpdateValueCache` | overlay 1 value cache | algebraically zero `remaining * 0` return | The live counter read preserves IDO's exact caller-saved web. Seek a natural zero return with the same 120 words and two local relocation pairs. |
| `overlay59DrawFrame` | overlay 59 frame renderer | empty duplicated `!index` condition | The inert condition preserves the exact entry/owner register order. Seek a natural declaration or lifetime spelling with the same 76 words and relocation surface. |
| `func_overlay_012_F00000C4_186D344` | overlay 12 release path | empty `!i` condition after fixed-count clear | The inert condition preserves exact allocation in the 60-word release routine. Seek a natural loop/epilogue spelling with the same bytes and relocation surface. |
| `overlay12Initialize` | overlay 12 resource initializer | empty `!remaining` condition after fixed-count clear | The inert condition preserves exact allocation in the 49-word initializer. Seek a natural loop/epilogue spelling with the same bytes and relocation surface. |
| `overlay3FindClosestObject` | overlay 3 closest-object search | empty `&pad`, `!cursor`, and `if (1)` blocks | The three inert blocks preserve the exact 77-word allocation/control-flow form. Seek natural declarations/scopes with the same frame and relocations. |
| `func_overlay_057_F0000954_18A454C` | overlay 57 interface update | empty read of `O57_config0954.field08` | The inert read preserves the exact register web in the 435-word body. Seek a natural use/lifetime spelling with the same bytes and relocations. |
| `overlay82Update` | overlay 82 update | empty `currentValues` read | The inert read preserves IDO's exact web priority in the 291-word body. Seek a natural declaration/lifetime spelling with the same bytes and relocations. |

Add rows as ugly-but-verified matches land. When revisiting: reproduce the match
with idiomatic C, `gmake verify`, then delete the row + the in-source comment.
