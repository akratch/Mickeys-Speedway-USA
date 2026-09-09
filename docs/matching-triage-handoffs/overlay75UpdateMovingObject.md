<!-- plateau-handoff:overlay75UpdateMovingObject:start -->
### `overlay75UpdateMovingObject` plateau handoff

- source: `src/overlays/o075/overlay75UpdateMovingObject.c`
- score: 285/304 words
- frame: 0x58
- relocations: 20
- first mismatch: +0x54
- summary: Baseline reproved with zero source attempts; residual confined to one head window where the target reads the state field twice. Next lever is the repeated field read at the test, not the initializers.

#### 2026-09-09 lane `lm-bigsingles`: baseline reproved, residual re-localised

The 285/304 baseline reproduces exactly on this lane's assignment base, with
the exact extent and no size delta. No source attempt was spent: the target was
screened last in a four-target lane and the budget went to the three units
where a mechanism-level lever was found. Recording what the re-measurement
does establish, so the next worker starts from evidence rather than a rerun.

All nineteen differing words fall in one window near the head of the function,
between the slot-flag test and the first floating-point setup; the rest of the
function is word-identical. Inside that window the target reads the state's
active field **twice** -- once in a branch-likely delay slot on the path that
skips the flag update, and once again after it -- while the candidate reads it
once, later, after the three retained-initialiser stores. The candidate's three
initialiser stores are emitted before the test; the target's equivalents are
emitted after the branch on that field, with the negative-one constant in the
branch delay slot.

That shape is the assembler's likely-branch delay-slot fill acting on a load
that is already in the join block, which means the ordering question is where
the three initialisers sit relative to the active-field test, not how the test
itself is written. The committed handoff already records that after-exit forms
of the initialisers add a branch-likely or a cache load; combine that with the
duplicated read and the next lever is the repeated-field-read lever applied to
the *test* rather than to the initialisers -- spell the active field as the
field access again at the test site so the join block owns the load, and keep
the initialisers where they are.

`docs/ido-learnings.md` gained a phase-replay method during this lane that
applies here directly: the candidate's own phase output can be re-assembled
byte-faithfully, so the delay-slot decision can be searched at the assembler
level first, and only the winning input translated back into C.
<!-- plateau-handoff:overlay75UpdateMovingObject:end -->
