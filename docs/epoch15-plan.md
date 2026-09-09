# Epoch 15 plan of attack: open the blocks nobody has a file for

Date: 2026-09-01. Baseline `campaign/unchain` plus the stale-ledger maintenance
commit that precedes this plan. Every derived figure below was remeasured from
this tree; the commands are recorded with the figures instead of carrying
forward an earlier campaign message.

## State at the opening

`gmake progress` reports 1,057/1,460 resident functions matched (72.40%) and
384,916/944,348 whole-program text bytes resolved (40.76%). It also reports 84
verified original-assembly functions. The post-maintenance queue command
`python3 tools/ready_queue.py --base HEAD --format maintenance` scans 397
ranked `NON_MATCHING` rows: ready=1, already-integrated/exhausted=396, and
stale-ledger=0. The queue is therefore plateau-locked under ADR 0011 apart
from its one ready row; ledger repair no longer blocks assignment.

The opportunity outside that queue is raw splat `asm` with no C translation
unit. Counting `glabel` records in the configured resident subsegments gives
152 compiler-generated functions and 16 odd-single-FP hand-assembly
functions, 168 total. The compiler-generated ranges own 57,708 bytes (about
57.7 KB). The count and byte receipts are:

```sh
compiler='asm/2340.s asm/30E0.s asm/4F40.s asm/34E60.s asm/35024.s asm/354C8.s asm/37680.s asm/37D50.s asm/3B1A0.s asm/4F4E0.s asm/505E0.s asm/506D0.s asm/58570.s asm/5885C.s asm/58C10.s asm/6F3E0.s'
oddfp='asm/main/trackasm.s asm/main/shadows_fp.s asm/main/weather_snow_asm.s asm/59BF0.s asm/59DB0.s'
rg --no-filename '^glabel ' $=compiler | wc -l
rg --no-filename '^glabel ' $=oddfp | wc -l
print $((0x30BC-0x2340 + 0x3100-0x30E0 + 0xC950-0x4F40 + 0x35008-0x34E60 + 0x354A0-0x35024 + 0x37650-0x354C8 + 0x37D50-0x37680 + 0x39350-0x37D50 + 0x3B480-0x3B1A0 + 0x4FC30-0x4F4E0 + 0x506D0-0x505E0 + 0x50820-0x506D0 + 0x58810-0x58570 + 0x58C10-0x5885C + 0x58E50-0x58C10 + 0x6F420-0x6F3E0))
```

The compiler-generated cohort is:

| Raw resident range | Functions | Lineage or role |
|---|---:|---|
| `0x4F40`-`0xC950` | 65 | JFG `objects.c` lineage |
| `0x34E60`, `0x35024`, `0x354C8` | 24 | JFG `textures.c` |
| `0x37680`, `0x37D50`, `0x3B1A0` | 24 | `gameVi` / menu |
| `0x2340`, `0x30E0` | 12 | JFG `audiomgr.c` |
| `0x58570`, `0x5885C`, `0x58C10` | 16 | libultra flash |
| `0x4F4E0`, `0x505E0`, `0x506D0` | 10 | SDK leaves, including `osCreateThread` |
| `0x6F3E0` | 1 | isolated resident gap |

The 16 excluded odd-FP functions are the configured `trackasm`, `shadows_fp`,
`weather_snow_asm`, `0x59BF0`, and `0x59DB0` subsegments. With the 84 verified
assembly functions from `gmake progress`, the matchable resident ceiling is
1,460 - 84 - 16 = 1,360.

The overlay YAML has 10 executable raw-ASM blocks totaling 44,756 bytes. Two
additional eight-byte labels, `overlay_002_tail` and `overlay_071_tail`, are
tail/alignment fragments; including them yields 44,772 bytes across 12
non-`padding` ASM labels. This distinction comes from the following
`mickey.us.yaml` census and avoids conflating executable work blocks with the
two tiny tails:

```sh
.venv/bin/python -c 'import re; from pathlib import Path; lines=Path("mickey.us.yaml").read_text().splitlines(); p=re.compile(r"^\s+- \[(0x[0-9A-Fa-f]+), (asm|c|bin)(?:, ([^]]+))?\]"); r=[(int(m.group(1),16),m.group(2),m.group(3) or "") for line in lines if (m:=p.match(line))]; x=[(b-a,n) for (a,t,n),(b,_,_) in zip(r,r[1:]) if a>=0x184C3E0 and t=="asm" and "padding" not in n and b-a>8]; print(len(x),sum(s for s,_ in x))'
```

Finally, a lexical guard census of `src/main/*.c` finds four bare resident
`GLOBAL_ASM` functions: `func_800517E0`, `func_80053868`, `func_8003D4FC`, and
`func_8001DD70`. The measurement command is the same stack-aware scan used to
distinguish a pragma inside `#ifdef NON_MATCHING` from an unconditional one:

```sh
.venv/bin/python -c 'import re; from pathlib import Path; rows=[]
for p in Path("src/main").glob("*.c"):
 stack=[]
 for n,line in enumerate(p.read_text().splitlines(),1):
  s=line.strip()
  if re.match(r"#if(n?def)?\b",s): stack.append("NON_MATCHING" in s)
  elif s.startswith("#endif") and stack: stack.pop()
  m=re.search(r"#pragma GLOBAL_ASM\(\"[^\"]*/([^/\"]+)\.s\"\)",s)
  if m and not any(stack): rows.append((p,n,m.group(1)))
print(len(rows)); [print(*r) for r in rows]'
```

## Engines and routing

Codex TU lanes own the raw compiler blocks. Each lane takes one TU, smallest
first, and stops at an exact match or a compiling frame-exact guarded
candidate; it never spends its budget on allocation-only passes. These lanes
use `gpt-5.6-luna` at maximum reasoning, four concurrent slots, and 300-minute
deadlines.

One continuous `tools/permute_sweep.sh` pass follows: resident candidates
first, then annotated overlay scratch ordered by `promotion_trial` class. ADR
0016 makes this fidelity-proven batch sweep standing authorization over every
guarded candidate, including a current plateau. Fable/Opus is reserved for
the sweep's frame-exact residuals of at most eight relocation-masked words
when the workbench names a source lever, plus tool faults and match-bearing
merge conflicts; each expensive-model target gets one attempt.

The occupied-machine cap is four TU lanes plus one sweep pass, with integration
serial and niced and total load held at or below 12.

## Waves

**Wave 1:** `tu-sdk-leaves`, `tu-textures`, `tu-audiomgr`, and
`tu-objects-a`.

**Wave 2:** `tu-objects-b`, `tu-objects-c`, `tu-menu-asm`, and overlay
raw-ASM shape lanes ordered by `promotion_trial`.

**Wave 3:** the four bare resident `GLOBAL_ASM` functions, ROM-size ownership
carves, and reshape lanes only where `promotion_trial` still ranks them
closest. Workbench backlog item 3 is funded only if the fresh stuck list
exceeds about 20 frame-exact functions.

## Exit criteria

- Every compiler-generated raw resident block has a C TU, with every function
  either matched or retained as a committed frame-exact candidate.
- Resident matching reaches at least 1,120 functions.
- The standing sweep has searched every guarded candidate once against a
  current ranking.
- Documentation, scoreboard, atlas, and public mirror are current after the
  ordinary private integration and release gates.
