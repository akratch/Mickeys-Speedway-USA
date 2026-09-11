<!-- plateau-handoff:overlay34RemoveRecord:start -->
### `overlay34RemoveRecord` plateau handoff

- source: `src/overlays/o034/overlay34RemoveRecord.c`
- score: 12/44 words
- frame: 0x18
- relocations: 9
- first mismatch: +0x14
- summary: Making shadow the decrement carrier regressed to 43 words; retained p1 carrier allocation and relocation identity block closure.

#### 2026-09-11, lane `f9-small`: the copy-site lattice is flat at 12

225 points measured with the direct-cc harness, delta 0 or worse, none below
12: outer loop as guarded do/while, `while (remaining-- != 0)` and `for`;
`shadow = remaining` at every subset of {before the loop, loop top, found
path, shift-loop top}; shift loop as guarded do/while, `while (remaining--)`
and `for`; cursor advance as `slot++`, the m2c `slot++; slot++; slot--` and
`slot = slot + 1`. The target copies the counter into `shadow` at the loop
top AND again in the found path (two `move $a1,$v1` in a row), and the
post-decrement scratch and `shadow` share `$a1` there, while every spelling
here gives the scratch its own register (`$a0`). Decision variable: what
makes the post-decrement's old-value temp and `shadow` one web. Not tried:
`shadow` as the loop's own post-decremented counter with `remaining`
derived from it.
<!-- plateau-handoff:overlay34RemoveRecord:end -->
