<!-- plateau-handoff:overlay17DrawStrip:start -->
### `overlay17DrawStrip` plateau handoff

- source: `src/overlays/o017/overlay17DrawStrip.c`
- score: 103/119 words
- frame: 0x40
- relocations: 1
- first mismatch: +0x0
- summary: Reusing pair for the final sync improves 18 to 16 differences in both the object and linked overlay, with no out-of-range differences. The remaining blocker is the 0x38 target frame versus 0x40 candidate frame; packet-local consolidation recovers the frame but destabilizes allocation.

#### 2026-09-10 lane `c2-reopen`: frame is not per-declaration here

Baseline reproduces: 476 bytes, 16 relocation-masked words, zero size delta,
frame 0x40 against the target's 0x38.

Two levers the new laws open were measured and both fail:

- The induction-exit-test law, applied to the scan loop's `while (remaining--)`
  as an explicit `!=` against the wrapped sentinel, leaves the frame at 0x40
  and adds four bytes.
- Inlining the `segment` constant, which removes a declaration outright, also
  leaves the frame at 0x40 and adds twelve bytes, because the constant is then
  materialized twice.

The second result is the useful one: on this function the frame is **not** a
per-declaration reservation, so the "recover the frame by consolidating packet
locals" route named in the summary is the only one that has ever moved it, and
its allocation cost is real rather than incidental.

Verdict: **the closure stands.**
<!-- plateau-handoff:overlay17DrawStrip:end -->
