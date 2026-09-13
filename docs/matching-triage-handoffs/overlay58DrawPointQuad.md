<!-- plateau-handoff:overlay58DrawPointQuad:start -->
### `overlay58DrawPointQuad` plateau handoff

- source: `src/overlays/o058/overlay58DrawPointQuad.c`
- score: 24 differing words
- frame: 0x18
- relocations: 11
- first mismatch: +0x14
- summary: 80/104 aligned exact at delta 0; 34-draw sequence retained. Carrier, generated cursor and physical-line controls cannot repair the address/store schedule.
#### 2026-09-13, lane f1: address carriers and colour-store line census

The configured baseline reproduces 416 bytes at delta zero, 80 exact and
24 naming aligned rows, first +0x14, no other buckets or gaps. The one-procedure
trace records 34 draws and 188 emission records. Every probe passes full-TU
stock/instrumented text identity. The 108-probe landscape and in-source
carrier/order closures were read, without repeating a colour sweep.

Deleting only the physical-address intermediate removes one emission record
at its former definition. All draw counts, their sequence, and object bytes
are unchanged. This declaration was not controlling the desired address web.

Deleting the display-list cursor and using the immediately preceding indexed
command changes the intended command lines: six additional draws, 40 total,
and two additional emission records. It adds 12 bytes and regresses the
aligned map from 24 naming to 53 naming and nine structural paired rows,
with four candidate-only and one target-only word. The source and object are
retained as a failed generated-cursor control, not adopted.

Folding the sixteen colour stores and their local cursor step onto one source
line moves 32 emission records onto the first store line, but changes no draw,
no draw order and no object byte. In this group, the physical-line tie is
inert even though the census correctly observes changed source attribution.

The guarded baseline is restored. Stop early under ADR 0018: the physical
intermediate and colour-store grouping do not move the residual, the generated
display cursor worsens geometry, and the prior vertex-cursor and order
receipts cover the other identified carrier routes. The open requirement is
still the display/address web population plus the remaining coordinate/store
schedule; these controls do not establish a general source impossibility.

Sources, stock and instrumented objects, census profiles and aligned deltas
remain ignored under build/f1/overlay58DrawPointQuad. Commands: configured
stock compile, draw_census.py profiles/comparison, residual_map.py --object
--against, finalize_plateau.py and tools/gates.sh. No new matching bytes.

#### 2026-09-13, lane h1: generated x-coordinate controls

The baseline reproduces 104 words, 416 bytes, frame 0x18, 24 raw/masked
naming differences, first +0x14, and no immediate, structural or gap rows.
The census records 34 draws and 188 emission records. Stock and instrumented
full-TU text agree. The candidate has 11 static relocations; the extracted
target has three, all three agreeing at their static tuple sites. This static
comparison does not resolve the overlay runtime relocation identities.

After reading the existing landscape and carrier/store-line controls, two
source probes test whether declaring x-coordinate intermediates suppresses
the generated value identities. Both preserve physical lines and store order.
Deleting xPlus reduces emission records to 187 without changing any draw or
the draw sequence. It leaves 24 naming rows but moves one out of the second
window into the final window. Equal scalar scores do not mean byte identity.
Deleting both xPlus and xMinus reduces records to 186, again leaving all draws
and their sequence fixed; aligned buckets regress to 73 exact, 29 naming,
zero immediate and two structural rows at the same size and frame.

Neither probe supplies the intended temporary draw-order change. Restore the
original guarded body. ADR 0018 early stop combines these negatives with the
existing physical-address, display-cursor, vertex-cursor and store-order
receipts; no broader source impossibility is claimed. No colour sweep was
repeated, and no byte credit is earned. Source/object pairs, profiles and
aligned per-window deltas remain ignored under build/h1/overlay58DrawPointQuad.
Commands: configured compilation, draw_census.py --save/--compare,
residual_map.py --object/--against, finalize_plateau.py and tools/gates.sh.

<!-- plateau-handoff:overlay58DrawPointQuad:end -->
