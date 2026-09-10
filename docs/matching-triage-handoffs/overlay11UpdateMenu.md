<!-- plateau-handoff:overlay11UpdateMenu:start -->
### `overlay11UpdateMenu` plateau handoff

- source: `src/overlays/o011/overlay11UpdateMenu.c`
- score: 299/301 words
- frame: 0x48
- relocations: 102
- first mismatch: +0x138
- summary: The indexed form reaches frame 0x48 with every target stack home and fixes the +0x138 spill pair; the residual moves to the preheader because uopt emits an induction pointer's initialiser after every user statement.


Current diagnosis, 2026-09-10, lane `astra-o11`, assignment base
`701478e37ab623dc870605e630cf7b12be2539a9`.
This remeasurement supersedes the earlier universal claims that the fact
requires an induction pointer, that every indexed form adds compiler temps,
and that the residual is unreachable from C. No C unreachability proof is
claimed here.

The retained guarded C scores **299/301 relocation-masked words**: 1,204
executable bytes, zero size delta, two masked differences at `+0x138` and
`+0x140`, 16 raw differences (14 relocation artifacts), frame `0x48`.
Ownership remains Overlay 11 text `+0x1398..+0x184C`, ROM
`0x1869BE0..0x186A094`. No padding or new matching credit is claimed.

### Register and scope measurements

The argument load in the handle loop uses **$3 (v1)** as its address base;
its result goes to $4 (a0). The index is in $2 (v0). The two spill homes are
frame offsets 68 for the index and 36 for the handle. These facts were read
from the actual configured TU's compiler listing, not inferred from a score.

- Earlier accesses to D_1BC: $6 against $sp, later $2 against $sp: Both facts are explicitly closed before the handle loop.
- Handle loop and its two stores: None for $3 against $sp: The pointer is initialized from D_1CC and walked; no directive covers its argument load or spill pair.
- Later menuInput accesses: $3 against $sp: Three scopes, all after the loop, where $3 holds D_menuBase + 0x1C4. Each has a matching close.

The indexed `D_0[D_1C4]` accesses emit symbolic memory operands, with no
explicit `.noalias` directive of their own in this listing. Their presence
therefore does not establish a fact for the handle pointer.

The proposed `menuInput = (s32 *)&D_menuBase[0x1C4]` rewrite has **identical
full text and identical relocation tuples** to the original decay-plus-offset
form. Its five `.noalias` and five `.alias` directives have the same registers
and scope locations. The original form already emits the menuInput fact in
this TU. Thus hypothesis (a) is true only in the sense that the fact already
exists elsewhere in the body. Hypothesis (b), a different physical register,
is false for menuInput: it also uses $3. The blocker is **a different value
lifetime and a later directive position**, not the numerical register name.
Facts for a later materialized address do not cover the earlier handle load.

### Faithful replay and controlled attempts

Recovered the recipe with `nm_ranking.configured_compile_commands`, retained
its game-code flags and NON_MATCHING define, and resolved every include path
against the original compile directory. The unchanged TU was copied into
ignored scratch before running `cc -S`; no listing was written into `src/`.
A direct scratch compile and the configured asm-processor compile have
identical full text and relocation tuples. Replaying the listing through
`as0` and `as1`, using the compiler-path flags and excluding the assembly-input
`-pic0`/`-noglobal` additions, reproduces both exactly. The replay baseline
reproduces the configured 16 raw / 2 masked score before any perturbation.

- 1, assembly diagnostic: Open the $3/$sp fact at the load and keep it through both stores: permit the crossing.: 0 masked differences, 14 raw relocation artifacts, zero size delta, 102 relocation records.
- 2, assembly diagnostic: Name $2 instead: leave the load base unproved.: 2 masked differences, first +0x138, zero size delta.
- 3, assembly diagnostic: Open the $3 fact but close it before the second store: remove permission at the decision point.: 2 masked differences, first +0x138, zero size delta.
- 4, C remeasurement: Index the later menuInput address: leave the handle lifetime's fact unchanged.: Byte- and relocation-identical to baseline; 2 masked differences, frame 0x48.

The assembly-only zero is a diagnostic, never a C match or a promotion.
No instruction was edited in a compiled object. The remaining raw relocation
fields were not treated as exact linked bytes.

Both original and indexed menuInput forms were also compiled with `cc -g3`.
Parsing the ECOFF `.mdebug` HDRR/FDR/SYMR records gives the same eleven named
local entries and offsets, with no compiler temporary local entries. Both
have frame `0x48`. The debug objects contain 1,208 function bytes, versus
1,204 in the configured build, so their table is secondary allocation
evidence; the debug code is not used as matching evidence. The zero-cost
menuInput rewrite disproves a blanket indexed-syntax temp penalty. It does
not supply a zero-cost fact for the handle value.

Stopping evidence: the assignment's ordered diagnostic question is resolved.
Three targeted phase controls and one C remeasurement identify the missing
input precisely. Only one C form was tested here, and it left that input
unchanged. This is not a spelling sweep or an exhaustion argument. Earlier
indexed-handle regressions are not promoted into a universal lower bound.
The precise remaining C lever is a source-authentic form that emits the
$3/$sp fact **for handle at the spill pair**, at the existing 0x48 frame and
with no compiler temp cells. A named-array fact for another value or a later
scope does not meet that requirement.

The assignment gate reports stale reopen pins on this base. This session
followed the owner's explicit diagnostic assignment; it did not change
`config/lane-reopen-authorizations.us.json` or assign additional targets.
The C body, guard, fallback, symbols and atlas ownership remain unchanged.

Evidence is preserved in ignored `build/wb/o11-alias/`: baseline C and objects,
configured command and flags, listing and phase commands, `fidelity.json`,
`controls.json`, per-attempt sources/listings/objects/scores/hypotheses,
`.mdebug` tables, and validation logs. Compiler listings, objects and raw
evidence are not tracked. Reproduction commands are the retained `setup.py`,
`measure.py`, and `controls.py`; the source score uses
`python3 tools/score_symbol.py overlay11UpdateMenu`.

Fresh `function_preflight.py --json` authenticates all **102/102** relocation
offsets, types, stable identities and effective identities, with zero
unresolved candidate identities. The canonical linked assembly fallback's
owned 1,204-byte range equals the same baserom range byte for byte.
`gmake verify` passes with SHA1
`507341c0a40ca3e9a7cee969b396ee53facfb548`; `gmake cleanroom` and
`gmake check-docs` pass. The fresh lane needed `gmake overlay-syms` after the
expected resident-link failure. `tools/finalize_plateau.py` refreshed the
source marker and this shard and passed its gates. A final configured source
score still reports 2 masked differences at the original extent.

Files changed for this handoff: only this shard and the source's diagnostic
comment/plateau marker. The candidate body is unchanged. The separate generic
compiler-learning correction records the scope lesson without target-specific
addresses or attempt scores. Next action is the zero-temp handle-lifetime
producer described above, if new causal evidence establishes one; this lane
hands off a diagnosis, not a C match.


#### 2026-09-10, lane `c4-o11`: the frame objection falls, the residual moves

Baseline reproduces: 1,204 bytes, zero size delta, **2** relocation-masked
words at `+0x138` and `+0x140`, 16 raw, frame `0x48`. Every number below is
from a `cc -S` listing replayed through `as0` and `as1` with the compiler-path
flags (no `-pic0`/`-noglobal`), round trip confirmed byte-exact first, plus
`cc -g3` `.mdebug` for every frame claim and one configured-path object for
the fixed pair.

**Why the pair inverts, from the scheduler trace.** `cc -Wa,-R` prints the
dependence graph and every selection. Without the disambiguation fact the
argument load carries *both* spill stores as dependent successors; scheduling
the load releases the two of them together, they enter the ready list in
**reverse emission order**, and the later-emitted store wins the tie. With the
fact both stores have no predecessor, sit in the initial ready list in emission
order, and the scheduler keeps that order, which is the target. The residual is
a ready-list release-order effect. Liveness, statement placement and line
grouping are not the axis, and neither is the tie-break's line-number tail.

**A second sufficient ugen input, and its closure.** All six orderings of the
argument load and the two spill stores were replayed. Exactly one non-baseline
ordering reaches the target -- emitting the handle spill *before* the index
spill -- and its object is **identical** to the fact-bearing control, so the
prior lane's diagnostic zero is reachable two ways. It is then closed: across
391 compiled translation units, **136 of 136** clean caller-save spill groups
are emitted in ascending physical register number, and the target's own bytes
put the index in `v0` and the handle in `v1`. ugen cannot emit the handle
spill first with that assignment. Sharing a base was already falsified; this
closes the other ugen-level route, leaving the disambiguation fact as the only
input that can produce the target's order.

**The temp-cell exclusion is false for this function.** The indexed producer
does cost three pooled temporaries here, but the frame is
`align8(28 + declared block + 4 * pooled temporaries)`, verified on five
independent censuses, and the pooled pointer always takes pool cell **1**.
Cutting the declared block from 44 to 28 bytes -- inline the object local into
its single field read, fold `selection` and `action` onto `index`, inline the
`value` ternary, keep one pointer local -- puts the pooled pointer at
`36(sp)`, `index` at `68(sp)`, `status` at `48(sp)` and `finish` at `44(sp)`,
frame `0x48`, **215 instructions**, matching the baseline instruction for
instruction and the target's eight distinct stack offsets exactly. The
configured object built from that body carries the target's store at `+0x138`
and its delay-slot store at `+0x140`. **That pair is fixed.** The standing
"every indexed spelling adds three or four temp cells, so the frame excludes
it" reading is a property of one census, not of the construct.

**The new barrier, a different mechanism.** The residual moves to `+0x10C` and
`+0x110`, in the loop preheader. uopt appends a strength-reduced induction
pointer's preheader initialisation **after every user preheader statement**;
the target orders the pointer's address materialisation *before* the index
initialiser, which only a user assignment produces. Measured across five loop
shapes (statement-order swap, `for`, `while`, increment at the top of the body,
increment inside the exit test) and three placements of the pointer
initialiser: the induction initialisation is last in all eight. The assembler
then keeps that order, because among ready nodes the lower emission index wins.

**So the two halves are complementary and their intersection is empty.** The
fact requires an array reference with a variable index; a variable index
requires a strength-reduced induction pointer; that pointer's initialiser is
emitted last, which costs the preheader pair. Dropping the index restores the
preheader and loses the fact, which costs the spill pair. Both bodies score
**299/301** at zero size delta and frame `0x48`, so the retained body is
unchanged.

**Bounding forms, all measured, none of which needs repeating.** Six constant
index forms fold to a plain dereference and emit no fact: subscript zero on the
walking pointer, a cast subscript, a pointer-to-one-element-array with its
element subscript, a one-element array struct member, address-of element zero
as the initialiser, and address-of element one as the increment. Six
pointer-arithmetic spellings of the indexed load all fold to the same
base-plus-four with a minus-four displacement; only routing the offset through
a pointer variable restores the zero displacement the target has, and that is
the shape that is one mechanism from a match. A loop-invariant subscript
collapses to a symbolic memory operand with no register base and no fact.

**Next action.** A producer for the disambiguation fact that does not create a
strength-reduced induction pointer, or an input that makes the induction
initialisation precede a user preheader statement. Nothing else in the spelling
space is open; do not re-run the lattice.

<!-- plateau-handoff:overlay11UpdateMenu:end -->
