# Natural source colours

## Lane wv-n, 2026-09-14

**Retained: 165 masked / 336 raw differing words, 14,456 bytes, delta zero,
frame 0x138, first mismatch +0x50.** The menu text cursor now takes s1
naturally. Its indexed Y coordinate takes s0 and the constant 160 takes s2;
all nine naming rows in the three menu windows disappear. No executable
bytes are promoted: this remains guarded NON_MATCHING C.

The baseline was 169 / 340, with aligned buckets **3476 exact, 128 naming,
one immediate, eight structural**. The retained source measures **3483 exact,
119 naming, one immediate, nine structural**. Paired residual rows fall from
137 to 129. The menu argument-load gap reopens, so the net positional gain is
four words. The original candidate +0x12F8 / target +0x1260 gap remains;
the additional menu pair is candidate +0x1CDC / target +0x1CE8. Every other
aligned window is unchanged. The complete 720-draw sequence, frame and 1,253
owned text relocations are preserved. No source in cases 3 or 12 changes.

### Reproduction before source work

The assignment gate returns base-only. The configured build, private-source
baseline and instrumented outputs pass section, relocation and symbol fidelity;
the actual baseline self-comparison is function-exact. Named Ucode captures
map every source attempt to procedure zero, with separate capture fidelity.
The oracle remains JFG frontKeyboard at 0.081; no donor material is adopted.

A local exhaustive landscape reproduces **all 1,898 supplied rows exactly**,
including the ten winners and the predicted five-force packing at 131.
The run is split across two bounded calls; every call-cap deferral is completed.
The earlier 187 and current 169 baselines also have identical 395 allocator
decision records. This does not imply source controls are invariant: all twelve
controls from the earlier report below are compiled again at 169.

The report freshness check reads the ranking's saved fingerprint, which differs
from the live file after EOF metadata updates. The private packet therefore
also pins the actual source bytes and retains the fresh complete measurements;
the report-only freshness check is not used as standalone source proof.

### What changed

The successful source is attempt 32. The menu retains its independent row
index, uses the existing text-pointer cell, and computes Y directly as
`0x50 + rowBase * 0x1E`. This removes the menu's named `textY` recurrence.
Both forms draw at 80, 110, 140 and 170, visit the same four text slots, and
make the same calls in order. No pointed-to value is loaded across a call.
This is an ordinary indexed-coordinate spelling, not an added inert probe.

The predecessor that assigned that expression back into `textY` retained the
720-draw order but cost eight bytes. Removing the named carrier lets strength
reduction generate the coordinate and eliminates that width cost. Thus L160
works here on the coordinate carrier; the old text-array cursor was already
compiler-generated.

A focused neighbor capture explains the earlier pointer witness's six naming
regressions. Its constant-seven web keeps saving 1/7, but loses interference
with the s2 scalar web belonging to `rowBase`; its interference count falls
from 17 to 16, and seven takes s2 instead of s5. Reintroducing an indexed
coordinate retains that scalar relationship, while removing `textY` lets the
menu's three values take their target colours. The retained source's seven
is back at s5 with the original 17-entry decision count. Raw neighbor records
remain private.

### Bounded controls

The earlier twelve controls, in their original order, now score:

    paired unsigned entry subscripts       169 / 340, delta 0
    visible index for entry accesses       3472 / 3483, delta +16
    shared byte-offset induction          3518 / 3522, delta +20
    explicit entry cursor                 3467 / 3475, delta -8
    count loaded then incremented         2259 / 2343, delta -8
    unsigned count with shared stride      268 / 440, delta 0
    conditional count expression           169 / 340, delta 0
    explicit decrement cursor             1317 / 1407, delta 0
    copy input in existing cursor          442 / 613, delta 0
    menu in existing cursor                176 / 347, delta 0
    menu in fresh local cursor             187 / 358, delta 0
    menu address in original scalar        191 / 362, delta 0

New entry controls cover a one-based index, an entry-only byte expression,
and a declared competing name cursor. None improves the baseline; the first
changes row classification without gaining exact rows, the second reproduces
the full baseline, and the third changes width and draws.

Count controls cover unsigned equality operands, reuse of the tested count on
the equal-four branch, and a boolean increment producer. The first is inert.
The second leaves the separate count at 124/6 behind the visible index at
1534/71. The third moves the count ahead, but loses twelve bytes and changes
draw order. It is not a count-colour repair at target width.

Decrement controls remove the explicit cursor's independent counter through
endpoint, pointer-difference and named-array forms. All change width. The
named-array form restores the relocation count but still adds twelve bytes
and changes draws. Copy-input controls use the existing path, saves and slot
pointer cells; all retain the wrong 0x130 frame and broad displacement costs.

Menu follow-ups cover pointer-difference and unsigned byte-difference array
indices, the seven constant's equality type, an indexed pointer at the loop
head or tail, an independent row-bound counter, and a row-derived coordinate.
The loop-head pointer restores seven's interference but returns the text
cursor to s0 and keeps the load-order gap. The bound and named-coordinate
forms both add eight bytes. Deleting the coordinate carrier supplies the
retained improvement; no failed control is adopted.

### Preservation

Every meaningful source, stock object, score, first mismatch, aligned delta,
draw census, allocator trace and fidelity receipt is retained in the private
wv-n packet. The function is guarded and banked through
`tools/finalize_plateau.py`. Gate verdicts and commits accompany the handoff;
canonical ROM verification proves the assembly fallback, not this nonexact C.
The next local question is the menu text load's alias/scheduling constraint.

Earlier context: [width pressure](whale-width-pressure.md),
[entry phase](whale-entry-phase.md), [compositions](whale-compositions.md),
and [gap emission](whale-gap-emission.md).

## Lane wv-f, 2026-09-13

**Retained source unchanged: 187 masked / 358 raw differing words, 14,456
bytes, delta zero, frame 0x138, first masked mismatch +0x50.** No executable
bytes are promoted. Twelve source attempts give no admissible improvement.
The decrement and menu pointers can take their nominated colours naturally,
but neither source witness preserves the rest of the baseline's agreement.
This is a source-form plateau, not an impossibility claim about the five forces.

### Baseline and the actual source questions

The ADR 0011 assignment check returns base-only. The supplied 187 landscape
passes its source-fingerprint check; no landscape or force lattice is rerun,
and no forced object is compiled. Configured, private-copy, supplied and fresh
instrumented objects pass section, relocation and symbol fidelity. The actual
stock object's self-comparison is function-exact. Named Ucode and complete
procedure-index captures map the baseline and every attempt to procedure zero;
each indexed and detailed output separately passes stock fidelity.

Four nominated addresses are already compiler-generated from array indices:
the copy input, advancing entries, decrement scan and menu text array. Thus
L160 does not supply a walking-pointer declaration to delete at those sites.
The count/stride question is a shared scalar lifetime. Source controls must
change that representation or its interference without undoing the benefits
of the existing indexed source.

All attempts preserve physical source lines. The case-3 and case-12 split
region is untouched. The nearest-oracle evidence remains the inherited JFG
frontKeyboard comparison; no donor code or names are adopted.

### Source attempts

Scores below are masked/raw differing words; delta is executable bytes.
Each row has retained source, stock/indexed/detailed objects, commands,
allocator records, first mismatch and aligned per-window comparison.

- Attempt 1: Unsigned entry subscripts in both cases 1 and 2: **187/358**, delta 0.
- Attempt 2: Only entry subscripts use visible i; other arrays retain portraitX: **3475/3486**, delta +16.
- Attempt 3: Shared byte-offset induction for entry, name and points arrays: **3517/3521**, delta +20.
- Attempt 4: Explicit entry pointer in the existing cursor cell: **3465/3473**, delta -8.
- Attempt 5: Case-13 count loaded once, then conditionally incremented: **2265/2349**, delta -8.
- Attempt 6: Unsigned case-local count with shared signed case-12/13 stride: **281/453**, delta 0.
- Attempt 7: Count initialized by a conditional expression: **187/358**, delta 0.
- Attempt 8: Explicit decrement pointer in the existing cursor cell: **1335/1425**, delta 0.
- Attempt 9: Copy input shares the existing later call-spanning cursor: **460/631**, delta 0.
- Attempt 10: Menu text uses the existing explicit cursor: **194/365**, delta 0.
- Attempt 11: Menu text uses a fresh block-local char-pointer cursor: **205/376**, delta 0.
- Attempt 12: Menu address carried in the original rowBase cell: **209/380**, delta 0.

Every frame is 0x138 except attempt 9, which shrinks it to 0x130 and first
mismatches at +0x0. All other first masked mismatches remain +0x50. Owned
text relocations are 1,253 except attempts 3 and 4 (1,251) and attempt 8
(1,255). These are text-surface counts; whole-object relocation totals also
include the jump table and are not substituted for them.

**Entry address, nominated w75=c16.** Attempt 1 changes the generated cursor's
datatype from signed to unsigned but leaves its 152/16 saving, 47 interference
entries and s0 selection unchanged. Its entire content/relocation/symbol
surface reproduces the baseline. Unlike the earlier single-case unsigned
control, both consuming cases change together. Attempts 2 and 3 remove the
old generated-address identity but introduce the listed width costs. Attempt
4 creates a shared symbol-class cursor at 183/18, selected to s1, and loses
eight bytes and two relocations. None supplies the requested isolated s2 repair.

**Count/stride, nominated w379=c20.** Attempt 5 raises the count-bearing
lifetime from 235/10 to 237/10 and leaves it at s5, ahead of the stride's
111/5 at s6; it also loses eight bytes. Attempt 6 preserves width but repeats
the known split-ranking defect: shared stride 222/9 takes s5, visible index
1534/71 takes s6, and unsigned count 124/6 takes s7. Unsigned type does not
put the count ahead of that index. Attempt 7's generated conditional join
normalizes back to the full baseline object surface and allocator decisions.
The previously exhausted signed carrier grid is not repeated.

**Decrement cursor, nominated w225=c14.** Attempt 8 does reach s0 naturally:
the existing cursor's shared symbol-class lifetime is 72/3. But the local
+0x780 window keeps seven naming rows and gains ten structural rows; the full
aligned residual worsens by 1,163 rows. Its extra two text relocations and
broad regressions reject it despite exact width and frame. This is a natural
colour witness, not a seven-word source improvement.

**Copy input, nominated w27=c17.** Attempt 9 differs from wv-e's block-local
copyIn control: the pointer shares the existing cursor's later lifetime.
That symbol-class lifetime has 62/2 saving and takes s1, not the requested s3.
The frame shrink introduces widespread displacement differences. The local
window changes from ten naming rows to nine naming plus two structural rows;
this is not an entry repair. No FIFO-cycle hypothesis is used.

**Menu cursor, nominated w498=c15.** Attempt 10 reaches s1 naturally, with
the shared cursor at 62/4. The constant 160 then takes s0. It removes four
paired residual rows across the nominated windows, but introduces six naming
rows in earlier transition windows, one later naming row, and the old
candidate +0x1CDC / target +0x1CE8 load-order pair. The seven constant's saving
remains 1/7 while its selection changes from s5 to s2. The net paired-window
delta is +3, and the positional score rises to 194. This reproduces the
pointer-versus-named-array alias tradeoff already proved by p23-whale-shape.

Attempt 11 separates the pointer identity and adds local-home displacement
costs without removing those six naming rows or the gap. Attempt 12 keeps the
original scalar identity using the target's 32-bit pointer representation;
it also retains the gap and increases naming costs. The latter is a private
address-carrier control, not an adopted source representation.

### Stopping, preservation and next action

The last three menu-pointer forms repeat the already recorded alias and
allocation tradeoff: none improves the retained aligned residual, resolves
that tradeoff or establishes a new repair mechanism. This is the ADR 0018
stall evidence. The other controls are bounded negative evidence for their
stated forms; they do not prove all C spellings or all five colours unreachable.

Do not repeat the explicit menu-pointer inversion, its fresh-local or scalar
address-carrier variants, or treat a naturally correct colour as the forced
single's isolated gain. A future packet needs a named-array source hypothesis
that changes the relevant allocation while preserving the stack-disjoint load
fact and avoiding the six earlier constant naming regressions. The decrement
witness similarly needs the original loop shape and relocation surface before
its s0 colour can count as progress.

Fresh preflight retains 1,253 runtime/static records per side, 1,236 offset/type
agreements and 671 stable identities; 395 candidate identities remain
unresolved. Exact runtime relocation identity is not claimed. The canonical
fresh-lane link is repaired with overlay-syms and a full-core-count build,
without a tracked alias change. Canonical ROM verification uses the assembly
fallback, not the nonexact C.

Private evidence is preserved as
`matching-evidence/wv-f-source-colours-20260913.tar.gz` under Git's common
directory; the live packet is `build/wv-f/`. tools/finalize_plateau.py updates
the guarded source's EOF metadata and its symbol shard. Closing gates run
through tools/gates.sh with their true statuses. No merge, rebase or push is
performed. The final handoff supplies the commit and gate verdicts.
