<!-- plateau-handoff:overlay17DrawStrip:start -->
### `overlay17DrawStrip` plateau handoff

- source: `src/overlays/o017/overlay17DrawStrip.c`
- score: 105/119 words
- frame: 0x38
- relocations: 1
- first mismatch: +0x68
- summary: The frame blocker is closed and was declaration ORDER, not packet-local lifetimes: moving textured ahead of vertices and flush recovers 0x38 with no other home moving, 16 to 14 masked words. All 40,320 declaration orders and all 6,720 prologue statement orders were measured; 14 is the floor of both. What remains is one three-row block emitted three rows early plus seven register webs over start, previous and the packet cursor.

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

#### 2026-09-10 lane `c6-close`: the frame is declaration ORDER, and it is closed

The 0x38/0x40 frame is recovered, and the route is neither packet-local
consolidation nor any lifetime change: it is the order of the eight
declarations. Moving `textured` ahead of `vertices` and `flush` gives frame
`0x38`, 119 instructions, the `t5` spill still at sp+0x28, every other home
unmoved -- **16 -> 14 masked words**.

Why the previous two probes read as refutations, both of which are real
measurements pointing the wrong way:

- Removing one declaration (inlining `segment`) left the frame at `0x40`
  because the non-save block is `align8(4N)`: N=8 and N=7 both give 32 bytes
  and only N=6 gives 24. One removal could never move it and two would have
  been needed, which is why that probe also cost twelve bytes.
- Adding declarations to probe the ladder is inert on this body -- an unused
  local is eliminated before the frame is sized, so a dummy `s32` changes
  nothing. Both a ninth and a tenth dummy leave frame `0x40` and 119
  instructions. The ladder here is over surviving locals in *order*, not count.

Exhaustively measured on this function: all 40,320 declaration orders (floor
14; every 14-scoring order carries frame `0x38`), and all 6,720 legal orders of
the eight prologue statements with the winning declaration order held (byte-flat
at 14, so statement order is spent).

**Remaining, named.** One three-row block and seven register webs, with the
temp ring and the shared lane already exact at 33/33 and 19/19:

- structural: the candidate emits `move t2,zero` / `move a1,v0` / the count
  test three rows before the target does, which emits `move a3,zero` first.
- register: t3->a3 (2 sites), a0->v1 (3), a1->a2 (3), v1->t3, t3->t1, a0->v0,
  t1->t3, over `start`, `previous` and the packet cursor. The pool lane first
  diverges at slot 7 and the divergence is an a0/a3 permutation, so the count
  of coloured webs is right and only their order is wrong.
<!-- plateau-handoff:overlay17DrawStrip:end -->
