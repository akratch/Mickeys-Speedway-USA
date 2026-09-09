<!-- plateau-handoff:func_80045BBC:start -->
### `func_80045BBC` plateau handoff

- source: `src/main/diCpu.c`
- score: 58/60 words
- frame: 0x30
- relocations: 24
- first mismatch: +0xBC
- summary: One ugen ring position: the stacked pointer argument wants t6 and gets t4, two slots early, and no source form found spends the two slots.

- 2026-09-09 pass, first structured record for this symbol. The candidate is
  60 of 60 instructions with the exact 0x30 frame, and exactly two words
  differ: the temp that materialises the fixed output-buffer pointer for the
  fifth, stack-passed argument of the pack write, and the store that spends
  it. The target takes that temp two ring positions later than the candidate.
  Everything else in the function, including the whole three-store header and
  both block copies, is word-identical.
- The six relocation identities the target has and the candidate does not are
  not a defect and are not part of the residual. The three header destinations
  are spelled as literal addresses here and as named symbols in the target;
  both forms link to the same words, which is why the comparator classes them
  as relocation layout. The previous lane measured that switching to the named
  symbols costs an instruction, so the literal form stays.
- Eliminated this pass, each measured and each flat at two words: a second
  size local so the rounded and unrounded values are distinct source
  variables; a named local for the buffer pointer, assigned before the call; a
  local for the header base; two dead zero-initialised locals, which is the
  documented way to reserve a colour and reserves nothing here; the rounding
  written mask-then-add against add-then-mask (the latter costs an
  instruction); the guard written as an explicit comparison against zero;
  three spellings of the buffer constant, including deriving it from the
  header address; and seven physical line groupings, from the call on one line
  to one argument per line, plus joining the header stores and joining the
  size computation to its guard. Physical line grouping, which moved a
  schedule on fx.c, is completely flat on this function.
- The reserve-a-colour lever therefore has a boundary worth recording: on this
  TU a plain zero initialiser of an otherwise unused local is eliminated before
  ring numbering, so it does not advance the ring.
- Next lever: the previous lane's ugen trace already localised this to emitted
  ordinal 60 wanting to be 62. The two missing ordinals are not buyable from
  statement shape at this site, so the next thing to test is whether they are
  spent earlier in the function -- most plausibly in the argument setup of one
  of the three block copies, or in the header stores if their destinations are
  a struct through one base rather than three independent objects.
<!-- plateau-handoff:func_80045BBC:end -->
