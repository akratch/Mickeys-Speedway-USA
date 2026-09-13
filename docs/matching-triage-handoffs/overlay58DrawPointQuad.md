<!-- plateau-handoff:overlay58DrawPointQuad:start -->
### `overlay58DrawPointQuad` plateau handoff

- source: `src/overlays/o058/overlay58DrawPointQuad.c`
- score: 24 differing words
- frame: 0x18
- relocations: 11
- first mismatch: +0x14
- summary: Physical intermediate and colour-store folding are byte-inert; generated display cursor adds six draws and twelve bytes.
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

<!-- plateau-handoff:overlay58DrawPointQuad:end -->
