<!-- plateau-handoff:overlay63UpdateEffects:start -->
### `overlay63UpdateEffects` plateau handoff

- source: `src/overlays/o063/overlay63UpdateEffects.c`
- score: 211/350 words
- frame: 0xF8
- relocations: 71
- first mismatch: +0x16C
- summary: Exact geometry/frame; preflight fails closed on ambiguous gO63RenderMatrixReloc identity. Fresh structured remeasurement only.

Fresh maintenance evidence on base
`72463bb1cadfceb4f883a21e2af73cb9087c6458`:

- the fallback and adjacent owners uniquely bound overlay 63 `.text`
  `+0x1D4..+0x74C`, ROM `0x18C2D5C..0x18C32D4`: `0x578` / 1,400
  executable bytes with no target padding. The ABI is `void (s32)`; export
  table entry 1429 and resident relocation 286 authenticate the inbound call;
- configured IDO 5.3 `-O2 -mips2 -32` emits an exact-size 350-word V0 with
  211 positional matches, 139 relocation-masked / 142 raw differences and
  first masked mismatch `+0x16C`. Target and candidate frames are both
  `0xF8`;
- workbench classifies 40 aligned structural, 12 schedule, 104 register, and
  three constant residuals, with 14 insertions and 14 deletions. This is a
  broad saved-register/address-materialization plateau, not a near-exact
  relocation-only result;
- the runtime target has 71 configured relocation records while the candidate
  exposes 72 static relocation sites. `function_preflight.py` and the
  structured workbench summary both fail closed because
  `gO63RenderMatrixReloc` has ambiguous runtime identity in the consolidated
  candidate owner. Scalar source-shape diagnostics remain usable, but exact
  relocation identity is not proved;
- the donor oracle is non-actionable: its best result is DKR
  `audioStopThread` at only `0.0615` masked four-gram similarity. No permitted
  source donor supports a replacement body;
- the prior flag sweep, narrowed particle/count lifetime, one-past-end particle
  pointer, and typed opacity-pointer scopes remain exhausted. This evidence
  pass did not alter the function body or repeat those forms.

Next lever: first resolve the consolidated runtime identity for
`gO63RenderMatrixReloc` without guessing across overlays. Until preflight is
complete, a one-shot source pass may only remeasure and refresh this structured
evidence; it must not claim exact relocations or mutate the body.
<!-- plateau-handoff:overlay63UpdateEffects:end -->
