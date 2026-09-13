<!-- plateau-handoff:overlay27UpdateCoordinates:start -->
### `overlay27UpdateCoordinates` plateau handoff

- source: `src/overlays/o027/overlay_027.c`
- score: 19/65 words
- frame: frameless
- relocations: 10
- first mismatch: +0x0
- summary: Explicit proc-3 census confirms 29 draws; swapping record/countdown preheader initializers moved neither draws nor emissions and was restored.

#### 2026-09-13, lane g1: preheader and carrier draw controls

The assignment gate returns base-only. A retained Ucode capture and
allocator_trace_receipt.py map this symbol to procedure 3 of six. The configured
stock and traced full-TU text agree. Baseline: 260 bytes, zero size delta,
46 aligned exact words and 19 naming rows, first +0x0, frameless, ten relocations.
The old score header's 19 is the differing count, not the exact count.
The procedure spends 29 draws and 118 emission events. No colour sweep is rerun.

Four separately preserved source controls test the preheader and loop:

- Reversing the X assignment chain removes one draw on that line, while total
  emissions remain 118. It adds one executable word and regresses to 51 naming
  plus three structural rows, with a candidate-only offset at +0x38.
- Replacing the declared record cursor with an indexed array access preserves
  all 29 draws and their order, but adds 19 emission events. Size remains equal;
  aligned residual becomes 26 naming and 11 structural rows, with four offsets
  unique to each side. The generated-cursor form does not retain the loop shape.
- Unsigned countdown type changes no line's draws or emissions and produces
  the same 19-row object.
- Swapping the countdown and Y-offset carriers also leaves every draw and
  emission count and the draw order unchanged, but grows the aligned naming
  residual to 33. This reaches colouring without changing the draw schedule.

The original 19-row body is restored. None of these controls improves the best
residual or opens a source lever beyond the existing web/address-order blocker;
the latest unchanged-schedule controls and recorded grouped-store regression
support stopping rather than repeating a colour or order sweep. The next
question remains the countdown/address web creation and table-base completion
order, requiring a source form that preserves the existing loop. This is a
NON_MATCHING plateau and carries no new matching credit. Source snapshots,
objects, raw traces and aligned comparisons remain ignored under
build/g1/overlay27UpdateCoordinates. Commands: draw_census.py,
residual_map.py --object/--against, register_census.py, allocator_trace_receipt.py,
and finalize_plateau.py.

<!-- plateau-handoff:overlay27UpdateCoordinates:end -->
