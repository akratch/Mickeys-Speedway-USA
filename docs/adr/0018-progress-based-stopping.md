# 0018. Progress-based stopping replaces the fixed attempt cap

Status: Accepted
Date: 2026-09-08

Supersedes ADR 0009's "hard cap of ~10 attempts per function" and the
sentence in ADR 0017 that retains it. Everything else in both ADRs stands:
frontier models with compiler feedback, similarity-first scheduling, causal
packets, and the plateau record itself are unchanged.

## Context

ADR 0009 set a hard cap of about ten attempts per function. The evidence
behind it was real: unbounded source mutation produced grinding, and a fixed
count is trivially auditable. But a count is a proxy for the thing actually
worth limiting, which is *attempting without learning anything*. Two runs in
the march3 wave show the proxy failing in both directions on the same day.

`func_8000590C` (2,876 bytes, 719 words, a structural size-mismatch
reconstruction) improved monotonically across its series: 692, then 619, then
538 differing words, with the extent and frame proved correct along the way.
Every one of those attempts eliminated an explanation and moved the residual.
The cap stopped it at ten while it was still gaining, and the plateau it filed
names a concrete untested lever, the earliest header-scheduling mismatch. The
cap did not prevent grinding here; it interrupted a working search.

`func_overlay_092_F0000068_18D5F88` ran the other way. Its committed handoff
already recorded the flag lattice exhausted, donor scans negative, and the
adjacent explicit-dereference mechanism flat in three isolated forms. Assigned
as an evidence gate, the worker tested the documented resumption bar, could not
meet it, made **zero** source attempts, and stopped at roughly a third of its
budget. Under an attempt cap that run had nine attempts of headroom it should
never have spent, and spending them would have re-derived a known-flat result.

Published practice elsewhere agrees that hard functions are not ten-attempt
work: comparable projects have finished individual functions only after
sustained frontier-model effort measured in days, not iterations.

## Decision

- **Stop on absence of new information, not on a count.** Continue while
  attempts keep producing new information: a better score, a newly proved
  identity, or an eliminated hypothesis. Stop when a bounded run of attempts
  produces none of those.
- The default stall threshold is **three consecutive attempts that neither
  improve the best measured residual nor eliminate a recorded hypothesis**.
  Record the stall explicitly; it is the stopping evidence.
- **A monotonically improving series is a reason to continue**, and needs no
  separate authorization while the wall-clock budget holds.
- **Stop early and say so** when the target's committed evidence already rules
  out the mechanism available, as in the evidence-gate case above. Zero
  attempts is a valid, and sometimes the correct, outcome. Verifying exhaustion
  is a result; re-deriving a known-flat one is waste.
- The binding resource is the **wall-clock task budget** of ADR 0011, not an
  iteration count. Budgets follow work class and may be enlarged for a target
  whose series is still improving.
- Every attempt still records its cause, predicted change and invariants
  (ADR 0017). An attempt that records none of these is not a disciplined
  attempt, and repeating one is grinding regardless of the count.
- Baseline measurements, tool failures and reruns of an unchanged
  configuration are not attempts, and never were.

## Consequences

- `AGENTS.md`'s matching workflow and causal-packet summary replace "stop at
  ~10 attempts" with the stall rule. Assignment packets state the stall
  threshold rather than an attempt budget.
- A plateau record now carries the stall evidence: the last attempt that
  improved anything, and what the following attempts failed to move. A plateau
  filed at a count, with the series still improving, is not a plateau.
- The heartbeat's `attempt_count` remains a monotonic activity signal for
  supervision. It is no longer a limit, and a coordinator must not treat a high
  count as a reason to intervene when `best_score` is still moving.
- This ADR does not weaken ADR 0001's exact-match standard, ADR 0017's
  evidence rules, lane ownership, or any acceptance proof. It changes when a
  worker stops, not what counts as a match.
