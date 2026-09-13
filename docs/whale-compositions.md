# Whale witness composition matrix: lane wv-g, 2026-09-13

**Retained source: 187 masked / 358 raw differing words, 14,456 owned bytes,
delta zero, frame 0x138, first masked mismatch +0x50, 1,253 text relocations.**
No new executable bytes are matched. The best new target-width composition is
C19 + F at **615 masked / 772 raw**, with 349 aligned differing paired rows;
it is worse than the retained body's 155. The guarded source body is unchanged.

This pass measures **59 new source compositions and 10 repeated or nested
controls**, after a fresh baseline. The 70 local cells contain 62 distinct
source texts: baseline, two recovered single witnesses reproduced by nested
unions, and 59 new compositions. Source differences are not all object
differences; several overlaps compile identically, which is part of the result.

### Source overlap in the recovered witnesses

The four banked packets under Git-common `matching-evidence/` supply the
sources and original measurements; no single-site witness is re-derived.
The inherited nearest-oracle evidence remains JFG frontKeyboard; no donor
source or terminology is adopted.

| Key | Banked source | Original masked / delta bytes |
| --- | --- | --- |
| B | wv-b attempt 13, cached display-list address outside case 12 | 3534 / -280 |
| B9 | wv-b attempt 9, cached address in every case | 3552 / -280 |
| C19 | wv-c attempt 19, three late case-9 address conversions | 615 / 0 |
| C23 | wv-c attempt 23, the same three plus the final conversion | 1813 / +4 |
| D | wv-d attempt 11, C23 plus unsigned equality-to-four load | 1446 / 0 |
| F | wv-f attempt 10, existing menu pointer cursor | 194 / 0 |
| F8 | wv-f attempt 8, existing decrement pointer cursor | 1335 / 0 |

C19 is a **source subset** of C23, and C23 is a source subset of D. Their
unions do not create independent interventions. B overlaps every C conversion
site. The first matrix preserves both spellings by casting B's cached address;
full content-section, relocation and symbol fidelity proves B + C19 and
B + C23 identical to the banked B object. The C conversion mechanism disappears
on the cached operand. This is absorption, not cancellation of separate costs.

The alternate `(literal)` rows keep C's original literal-address conversion at
each shared site and retain B everywhere else. These are the other explicit
resolution of that source conflict. All other unions combine exact recovered
edits, apply identical replacements once, and reject incompatible replacements.
The private generator verifies that each single reconstruction reproduces its
banked source, apart from normalized EOF plateau prose.

T below is exactly D's one-line change relative to C23. It is composed with
C19 or F/F8 to test the repair without silently inheriting all of C23. **T is
never compiled alone.** No compiler flags, forced colours, new single-site
spellings, exhausted case-12 families or case-3 cursor controls are tried.

### What composes, what is absorbed, and what remains coupled

**There is partial compensation.** On C19, C23 and D, adding F removes five
naming rows and adds one paired structural row, confined to the menu windows.
The six earlier transition naming regressions and one later naming regression
that F introduced on 187 do not recur. Each combination is four paired rows
better than its corresponding witness, with identical stack traffic, width,
relocation count and draw order relative to that witness. Address interference
also falls by one. This is a measured change in F's collateral footprint.

It is still not an admissible overall gain: F adds one candidate-only and one
target-only word, retaining the menu load-order gap. Positional scores stay
615, 1813 and 1446 respectively. Counting gap rows as well as paired residual
reduces the apparent four-row gain to two. None approaches 187.

**The strongest pressure witness does not compose into target width.** B's
remaining literal-address lineage retains five members, one naturally coloured
web at degree 39 and no splits throughout its combinations. This is the
banked replacement lineage (table 938/chain 0), not the absent original
29-split family (table 1162/chain 0). B9's corresponding literal materialization
has one member and no allocator web. Those states are reported separately;
absence of the original family is never called zero pressure.

Cached-address C unions leave B's 280-byte deficit unchanged. Keeping C's
literal operands recovers 12 bytes with C19 or 20 with C23, leaving deficits
of 268 or 260 bytes. Adding D's repair removes another four bytes. Both B
forms have 36 stack homes and changed traffic throughout. Degree 39 survives,
but its width and traffic costs do not settle against the other witnesses.

**The two natural-colour witnesses do not rescue each other.** F + F8 is
1341 masked at delta zero, with 1322 paired residual rows, two gap pairs and
718 draws. On C19/C23/D, F8's broad naming and draw-order costs remain. F's
small paired-row benefit sometimes survives inside that larger regression;
it does not make the combined source better than the retained baseline.

**The isolated D increment has a consistent width/draw payment.** Every new
T combination is four bytes short, keeps the 35-home traffic signature, and
has one fewer draw than its corresponding source without T. It provides no
cheaper target-width repair when composed with C19 or the two cursor witnesses.

### Reading the matrices

Scores are relocation-masked positional differing words. Paired rows come from
`residual_map.py`; candidate-only/target-only gap rows are separate. All frames
are 0x138. `same` means the baseline's complete 35-home signature: offsets,
widths, loads, stores and address-takes, not just the number of homes.
The workbench reports 435 touched rows on that baseline. Every non-B cell
preserves that signature. The private matrix also records raw score, first
mismatch, relocation count, complete lineage census and exact draw-order
comparison. Equal draw counts alone are not used as evidence of equal order.

Degree ranges in non-B rows are the original address family's descendants;
that family remains entirely split. B and B9 use the separately identified
literal family described above. Source line insertion in B changes physical
line labels; total counts and complete draw order remain the comparable axes.

### Five-witness pairs, cheapest first

| Composition | Masked | Delta bytes | Paired rows | Gaps C/T | Address degree | Draws/emits | Stack |
| --- | --- | --- | --- | --- | --- | --- | --- |
| C19 + F | 615 | +0 | 349 | 10/10 | 120–142 | 720/5029 | same |
| D + F | 1446 | +0 | 1011 | 15/15 | 97–118 | 719/5054 | same |
| C19 + D | 1446 | +0 | 1015 | 14/14 | 98–119 | 719/5061 | same |
| C23 + F | 1813 | +4 | 751 | 14/13 | 100–118 | 720/5049 | same |
| C19 + C23 | 1813 | +4 | 755 | 13/12 | 101–119 | 720/5056 | same |
| C23 + D | 1446 | +0 | 1015 | 14/14 | 98–119 | 719/5061 | same |
| B + F | 3533 | -280 | 1775 | 11/81 | 39, coloured | 718/5050 | 36 homes; changed |
| B + C19 | 3534 | -280 | 1773 | 11/81 | 39, coloured | 718/5057 | 36 homes; changed |
| B + D | 3538 | -284 | 1775 | 11/82 | 39, coloured | 717/5062 | 36 homes; changed |
| B + C23 | 3534 | -280 | 1773 | 11/81 | 39, coloured | 718/5057 | 36 homes; changed |

C19 + C23 is exactly C23; C19 + D and C23 + D are exactly D. These are
reproduction controls and do not increment the source-attempt counter.

### Five-witness triples

| Composition | Masked | Delta bytes | Paired rows | Gaps C/T | Address degree | Draws/emits | Stack |
| --- | --- | --- | --- | --- | --- | --- | --- |
| B + C19 + C23 | 3534 | -280 | 1773 | 11/81 | 39, coloured | 718/5057 | 36 homes; changed |
| B + C19 + D | 3538 | -284 | 1775 | 11/82 | 39, coloured | 717/5062 | 36 homes; changed |
| B + C19 + F | 3533 | -280 | 1775 | 11/81 | 39, coloured | 718/5050 | 36 homes; changed |
| B + C23 + D | 3538 | -284 | 1775 | 11/82 | 39, coloured | 717/5062 | 36 homes; changed |
| B + C23 + F | 3533 | -280 | 1775 | 11/81 | 39, coloured | 718/5050 | 36 homes; changed |
| B + D + F | 3537 | -284 | 1777 | 11/82 | 39, coloured | 717/5055 | 36 homes; changed |
| C19 + C23 + D | 1446 | +0 | 1015 | 14/14 | 98–119 | 719/5061 | same |
| C19 + C23 + F | 1813 | +4 | 751 | 14/13 | 100–118 | 720/5049 | same |
| C19 + D + F | 1446 | +0 | 1011 | 15/15 | 97–118 | 719/5054 | same |
| C23 + D + F | 1446 | +0 | 1011 | 15/15 | 97–118 | 719/5054 | same |

Only the three B-with-F triples add new source texts in this table. The other
seven reproduce previously measured source unions and retain the attempt count.
Their object equalities and all four axes are checked, rather than inferred
from equal scalar scores.

### Alternate B/F witnesses, literal overlap resolutions, and factored D repair

These are attempts 11–59 in order. Duplicate source texts from the expanded
pair/triple enumeration are skipped. The final row combines four recovered
components to close the factored-repair control.

| Composition | Masked | Delta bytes | Paired rows | Gaps C/T | Address degree | Draws/emits | Stack |
| --- | --- | --- | --- | --- | --- | --- | --- |
| B + C19 (literal) | 3537 | -268 | 1805 | 18/85 | 39, coloured | 718/5021 | 36 homes; changed |
| B + C23 (literal) | 3540 | -260 | 1864 | 22/87 | 39, coloured | 718/5041 | 36 homes; changed |
| B + D (literal) | 3536 | -264 | 1853 | 23/89 | 39, coloured | 717/5046 | 36 homes; changed |
| B + F8 | 3533 | -280 | 1963 | 13/83 | 39, coloured | 716/5056 | 36 homes; changed |
| B9 + C19 | 3552 | -280 | 2061 | 11/81 | no web | 719/5059 | 36 homes; changed |
| B9 + C19 (literal) | 3555 | -268 | 2103 | 18/85 | no web | 719/5023 | 36 homes; changed |
| B9 + C23 | 3552 | -280 | 2061 | 11/81 | no web | 719/5059 | 36 homes; changed |
| B9 + C23 (literal) | 3557 | -260 | 2152 | 22/87 | no web | 719/5043 | 36 homes; changed |
| B9 + D | 3555 | -284 | 2110 | 11/82 | no web | 718/5064 | 36 homes; changed |
| B9 + D (literal) | 3552 | -264 | 2182 | 23/89 | no web | 718/5048 | 36 homes; changed |
| B9 + F | 3551 | -280 | 2063 | 11/81 | no web | 719/5052 | 36 homes; changed |
| B9 + F8 | 3551 | -280 | 1932 | 13/83 | no web | 717/5058 | 36 homes; changed |
| C19 + F8 | 1645 | +0 | 1450 | 9/9 | 121–143 | 718/5035 | same |
| C23 + F8 | 2419 | +4 | 1727 | 13/12 | 101–119 | 718/5055 | same |
| D + F8 | 1958 | +0 | 1508 | 14/14 | 98–119 | 717/5060 | same |
| F + F8 | 1341 | +0 | 1322 | 2/2 | 130–139 | 718/5064 | same |
| B + C19 + F (literal) | 3536 | -268 | 1804 | 18/85 | 39, coloured | 718/5014 | 36 homes; changed |
| B + C19 + F8 | 3533 | -280 | 1963 | 13/83 | 39, coloured | 716/5056 | 36 homes; changed |
| B + C19 + F8 (literal) | 3536 | -268 | 2009 | 20/87 | 39, coloured | 716/5020 | 36 homes; changed |
| B + C23 + F (literal) | 3539 | -260 | 1863 | 22/87 | 39, coloured | 718/5034 | 36 homes; changed |
| B + C23 + F8 | 3533 | -280 | 1963 | 13/83 | 39, coloured | 716/5056 | 36 homes; changed |
| B + C23 + F8 (literal) | 3537 | -260 | 2048 | 24/89 | 39, coloured | 716/5040 | 36 homes; changed |
| B + D + F (literal) | 3535 | -264 | 1852 | 23/89 | 39, coloured | 717/5039 | 36 homes; changed |
| B + D + F8 | 3536 | -284 | 2005 | 13/84 | 39, coloured | 715/5061 | 36 homes; changed |
| B + D + F8 (literal) | 3535 | -264 | 2078 | 25/91 | 39, coloured | 715/5045 | 36 homes; changed |
| B + F + F8 | 3533 | -280 | 1967 | 13/83 | 39, coloured | 716/5049 | 36 homes; changed |
| B9 + C19 + F | 3551 | -280 | 2063 | 11/81 | no web | 719/5052 | 36 homes; changed |
| B9 + C19 + F (literal) | 3554 | -268 | 2102 | 18/85 | no web | 719/5016 | 36 homes; changed |
| B9 + C19 + F8 | 3551 | -280 | 1932 | 13/83 | no web | 717/5058 | 36 homes; changed |
| B9 + C19 + F8 (literal) | 3554 | -268 | 1965 | 20/87 | no web | 717/5022 | 36 homes; changed |
| B9 + C23 + F | 3551 | -280 | 2063 | 11/81 | no web | 719/5052 | 36 homes; changed |
| B9 + C23 + F (literal) | 3556 | -260 | 2151 | 22/87 | no web | 719/5036 | 36 homes; changed |
| B9 + C23 + F8 | 3551 | -280 | 1932 | 13/83 | no web | 717/5058 | 36 homes; changed |
| B9 + C23 + F8 (literal) | 3557 | -260 | 2018 | 24/89 | no web | 717/5042 | 36 homes; changed |
| B9 + D + F | 3554 | -284 | 2112 | 11/82 | no web | 718/5057 | 36 homes; changed |
| B9 + D + F (literal) | 3551 | -264 | 2181 | 23/89 | no web | 718/5041 | 36 homes; changed |
| B9 + D + F8 | 3555 | -284 | 2005 | 13/84 | no web | 716/5063 | 36 homes; changed |
| B9 + D + F8 (literal) | 3553 | -264 | 2078 | 25/91 | no web | 716/5047 | 36 homes; changed |
| B9 + F + F8 | 3551 | -280 | 1936 | 13/83 | no web | 717/5051 | 36 homes; changed |
| C19 + F + F8 | 1646 | +0 | 1449 | 10/10 | 120–142 | 718/5028 | same |
| C23 + F + F8 | 2420 | +4 | 1726 | 14/13 | 100–118 | 718/5048 | same |
| D + F + F8 | 1959 | +0 | 1507 | 15/15 | 97–118 | 717/5053 | same |
| T + F | 1884 | -4 | 1066 | 3/4 | 130–139 | 719/5070 | same |
| T + F8 | 2458 | -4 | 1566 | 2/3 | 131–140 | 717/5076 | same |
| T + F + F8 | 2464 | -4 | 1570 | 3/4 | 130–139 | 717/5069 | same |
| C19 + T | 1846 | -4 | 1134 | 10/11 | 120–143 | 719/5041 | same |
| C19 + T + F | 1846 | -4 | 1130 | 11/12 | 119–142 | 719/5034 | same |
| C19 + T + F8 | 2421 | -4 | 1649 | 10/11 | 120–143 | 717/5040 | same |
| C19 + T + F + F8 | 2422 | -4 | 1648 | 11/12 | 119–142 | 717/5033 | same |

### Proof, stopping and preservation

The assignment gate returns `base-only` at assignment base
`dad3a9bac364ab3ff7fe1473cb3428ba7a4da97e`.
The configured full-TU candidate and private baseline pass content-section,
relocation and symbol fidelity. The baseline's actual preprocessed compiler
input passes self-context comparison. Every new cell preserves the non-function
context, maps its named Ucode stream and complete procedure index to ordinal
zero, and passes stock/indexed/detailed-trace fidelity. Trace-local web numbers
are not imported from a changed body. The supplied 187 landscape is not rerun,
and no stale landscape is used to explain a new colour residual.

Fresh preflight reproduces 1,253 runtime/static records on each side, 1,236
offset/type agreements, 671 stable identity agreements and 395 unresolved
candidate identities. Exact relocation identity is not claimed. The ordinary
canonical fresh-lane link required `gmake overlay-syms`; regeneration produced
no tracked alias changes and preflight then passed. The summary's raw-workbench
first offset is not substituted for the masked comparator's +0x50.

The finite banked-witness composition matrix is complete. Attempts 57–59 then
repeat the already measured F/F8 costs on C19 + T: all retain delta -4, none
beats 187, and the earlier pressure, menu-gap and draw-order tradeoffs remain.
They establish no new compensating mechanism; this is the final three-attempt
stall evidence. This is not a claim that all C source or whole-shape rewrites
are exhausted. No attempt is stopped merely to meet a count.

The retained 187 candidate stays behind its existing `NON_MATCHING` guard.
`tools/finalize_plateau.py` banks the source's EOF metadata and a link from
the per-function shard to this report.
Closing gates run through `tools/gates.sh`: verify, cleanroom, check-docs and
check-scoreboard. ROM verification proves the assembly fallback; no diagnostic
composition is ROM-exact or promoted. The final handoff carries gate statuses
and the commit hash.

Private packet: `matching-evidence/wv-g-compositions-20260913.tar.gz` under
Git's common directory, with a SHA-256 receipt. It retains the sources,
composition rules, per-attempt hypotheses, stock/indexed/traced objects,
compiler inputs, source-context checks, procedure receipts, all four censuses,
aligned comparisons and the matrix. Original packets remain separately banked;
this packet includes the selected recovered artifacts and references all four.
The local evidence directory is `build/wv-g/`; none of it is tracked.

Next concrete action: use C19 + F or D + F only as a controlled example of
absorbed menu naming collateral, with the remaining menu gap stated explicitly.
A further matching assignment needs a new mechanism that preserves that local
gain while removing the much larger address or draw-order residual. Repeating
these banked unions, treating their costs as disjoint from their lane labels,
or declaring the function unmatchable is not supported by this matrix.
