<!-- plateau-handoff:func_overlay_029_F000042C_187D6DC:start -->
### `func_overlay_029_F000042C_187D6DC` plateau handoff

- source: `src/overlays/o029/overlay29InitializeObject.c`
- score: 87/102 words
- frame: 0x58
- relocations: 8
- first mismatch: +0x14
- summary: direct early-source placement regressed to 80/102; the structural producer-order trial is exhausted
- measured shape: target and retained C are both 102 words with frame 0x58; 15 raw and relocation-masked words differ, with mixed structure, schedule, and register effects beginning at the first source-object load.
- relocation evidence: all 8 candidate and target sites agree in function-relative offset, type, and stable runtime identity. Three old resident callee spellings omitted the resident base adjustment; the retained candidate now calls `mathOneFloatRPY`, `trackMakePolylist`, and `func_80010900`.
- bounded attempts: seven prior probes covered two source/contact assignment placements, canonical callee identities, exact-sibling vector ordering, aggregate copying, a split source-object producer, and pointer volatility. A fresh direct early-source placement retained 102 words and frame 0x58 but regressed from 87/102 to 80/102 with 18 opcode differences. Other regressions ranged from 18 to 93 differing words; aggregate copying exceeded the 0x198 owner by 8 bytes. The prior plateau also exhausted broader typed/load/declaration variants and a bounded permuter run.
- donor evidence: Mickey's exact Overlay 26 initializer corroborates the transform, polylist, and collision-call sequence but its natural grouped-vector ordering regresses this target to 18 differences. The five-reference skeleton search found no JFG candidate in the top ten; the best cross-project score was only 0.076 (Conker), so there is no donor claim.
- JFG utility: no public-ledger-grade JFG function was uncovered. The corrected calls only corroborate JFG's existing `src/hasm/ido/math_util.s::mathOneFloatRPY` and `ver/symbols/symbol_addrs.us.txt::trackMakePolylist` in the track/collision setup pipeline; this Mickey body remains non-matching and has no identified JFG insertion point.
- next lever: retain the volatile field and inspect CFE/ugen statement boundaries for a producer shape that issues `init->object` and its contact load before the position stores without extending the source pointer's lifetime or adding a stack home.

#### Re-open under law L90 (2026-09-10, lane/c3-reopen2)

- **L90 does not apply to this function.** It contains no loop and no
  induction variable, so there is no exit test for uopt to rewrite. The
  candidate pool's L90 tag here is a date comparison, not an applicability
  test: the closure predates the law, so the tool offered it. Recording that
  distinction is the result.
- lever set the closure actually searched: two source/contact assignment
  placements, canonical callee identities, exact-sibling vector ordering,
  aggregate copying, a split source-object producer, pointer volatility, a
  direct early-source placement, and a bounded permuter run.
- re-tested anyway, because the closure's own next-lever note asked for a
  producer shape that issues the source object and its contact load before the
  position stores: seven fresh forms, all regressions. Assigning the source
  first scores 22, assigning it second scores 22, splitting the volatile
  object read into its own local scores 64 to 65 across three declaration
  positions, and reusing that local as the call argument costs 4 bytes and
  scores 101. The retained late assignment is the optimum at 15.
- the deciding variable, named: the web-creation order of the source-contact
  web relative to the two saved-register copies of the object and init
  parameters. The target opens the source-contact web immediately after the
  object-contact web -- pool slot 1 -- and copies the object parameter to a
  saved register before the init parameter. The candidate opens the
  source-contact web at slot 4 and copies init first, which is what swaps the
  two saved-register colours and displaces the two zero stores. The whole
  15-word residual lives in that one entry block; everything from the first
  call onward already agrees.
- **verdict: the closure was correct and remains correct.** Resume only with
  evidence about uopt's web ordering in the entry block, not with another
  statement permutation.
<!-- plateau-handoff:func_overlay_029_F000042C_187D6DC:end -->
