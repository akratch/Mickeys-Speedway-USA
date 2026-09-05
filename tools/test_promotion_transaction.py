#!/usr/bin/env python3
"""Exercise promotion failure boundaries with real Git and synthetic build gates."""
from __future__ import annotations

import contextlib
import os
import signal
import subprocess
import sys
import tempfile
import threading
import time
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import permute_batch as batch
import promotion_transaction as transaction
import promotion_proof as proof


class Fixture:
    def __init__(self):
        self.tmp = tempfile.TemporaryDirectory(prefix="promotion-transaction-")
        self.root = Path(self.tmp.name)
        self.stack = contextlib.ExitStack()
        self.env = dict(os.environ, GIT_CONFIG_GLOBAL=os.devnull, GIT_CONFIG_NOSYSTEM="1")
        self.stack.enter_context(patch.dict(os.environ, self.env))
        self.git("init", "-q")
        self.git("config", "user.name", "Fixture")
        self.git("config", "user.email", "fixture@example.invalid")
        self.source = self.write("src/fixture.c", """#ifdef NON_MATCHING
int fixture(void) { return 1; }
#else
#pragma GLOBAL_ASM("fixture.s")
#endif
""" + "\n/* independent context */\n" * 15 + "int independent = 3;\n")
        for name in ("config/overlays.us.json", "mickey.us.yaml", "overlay_undefined_syms.us.txt",
                     "config/overlay-donors.us.json", "README.md", "docs/matching-triage-handoffs/fixture.md"):
            self.write(name, "original " + name + "\n")
        self.write("unrelated.txt", "original unrelated\n")
        self.git("add", ".")
        self.git("commit", "-qm", "fixture baseline")
        self.head = self.git("rev-parse", "HEAD")
        self.original = {p.relative_to(self.root): p.read_bytes() for p in self.root.rglob("*")
                         if p.is_file() and ".git" not in p.parts}
        self.winner = self.write("build/winner/source.c", "int fixture(void) { return 2; }\n")
        self.item = batch.QueueItem("fixture", self.source, overlay=1)
        for name, value in (("ROOT", self.root), ("ATLAS_PATH", self.root / "config/overlays.us.json"),
                            ("HANDOFF_DIR", self.root / "docs/matching-triage-handoffs"),
                            ("BUILD_PERMUTER", self.root / "build/permuter"), ("PYTHON", Path(sys.executable))):
            self.stack.enter_context(patch.object(batch, name, value))
        batch.CANCEL_EVENT.clear()
        self.calls = []
        self.real_capture = batch.bounded_capture
        self.after = None
        self.fail_return = None
        self.proof_report = {
            "schema": proof.REPORT_SCHEMA, "requested_symbol": "fixture",
            "candidate_symbol": "fixture", "linked_symbol": "fixture",
            "resolution_mode": "post_promotion", "preflight": {"status": "complete"},
            "workbench": {"comparison_mode": "rom", "differing_words": 0,
                          "target_words": 32, "candidate_words": 32,
                          "first_mismatch": None, "verdict": "exact",
                          "target_frame": 0, "candidate_frame": 0},
            "relocation_comparison": {"target_record_count": 3, "candidate_record_count": 3,
                                      "offset_type_exact": True,
                                      "effective_identity_alignment_count": 3,
                                      "effective_identity_exact": True,
                                      "identity_proof_mode": "static"},
        }
        self.stack.enter_context(patch.object(batch, "bounded_capture", side_effect=self.command))

    def __enter__(self):
        return self

    def __exit__(self, *_):
        batch.CANCEL_EVENT.clear()
        self.stack.close()
        self.tmp.cleanup()

    def git(self, *args):
        return subprocess.check_output(["git", *args], cwd=self.root, env=self.env,
                                       text=True, stderr=subprocess.PIPE).strip()

    def write(self, name, text):
        path = self.root / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)
        return path

    def command(self, args, deadline, **kwargs):
        self.calls.append(tuple(args))
        if args[0] == "git":
            result = self.real_capture(args, deadline, **kwargs)
        else:
            batch.remaining_timeout(deadline)
            if "overlay-atlas-write" in args:
                self.write("config/overlays.us.json", "generated atlas\n")
                self.write("mickey.us.yaml", "generated yaml\n")
            elif "overlay-syms" in args:
                self.write("overlay_undefined_syms.us.txt", "generated symbols\n")
            elif "tools/refresh_atlas_digest.py" in args:
                self.write("config/overlay-donors.us.json", "generated donor digest\n")
            elif "scoreboard" in args:
                self.write("README.md", "generated scoreboard\n")
            result = subprocess.CompletedProcess(args, 0, "OK fixture\n", "")
            if "tools/promotion_proof.py" in args:
                try:
                    proof.validate_report("fixture", self.proof_report)
                except proof.ProofError as error:
                    result = subprocess.CompletedProcess(args, 1, str(error), "")
        if self.after:
            self.after(len(self.calls), args, deadline)
        if self.fail_return == len(self.calls):
            result = subprocess.CompletedProcess(args, 7, "synthetic command failure\n", "")
        return result

    def promote(self, *, commit=True, seconds=15):
        return batch.promote(self.item, self.winner, 1, time.monotonic() + seconds, commit=commit)

    def assert_restored(self, case):
        case.assertEqual(self.git("rev-parse", "HEAD"), self.head)
        for path, content in self.original.items():
            case.assertEqual((self.root / path).read_bytes(), content, str(path))
        case.assertEqual(self.git("diff", "--cached", "--name-only"), "")
        case.assertTrue(self.winner.exists())


class PromotionTests(unittest.TestCase):
    def test_apply_without_commit_is_verified_and_unstaged(self):
        with Fixture() as fixture:
            ok, error = fixture.promote(commit=False)
            self.assertTrue(ok, error)
            self.assertEqual(fixture.git("rev-parse", "HEAD"), fixture.head)
            self.assertEqual(fixture.git("diff", "--cached", "--name-only"), "")
            self.assertIn("return 2", fixture.source.read_text())

    def test_success_commits_exact_paths_and_preserves_unrelated_staging(self):
        with Fixture() as fixture:
            fixture.write("unrelated.txt", "staged independent change\n")
            fixture.git("add", "unrelated.txt")
            result, error = fixture.promote()
            self.assertTrue(result, error)
            self.assertNotEqual(fixture.git("rev-parse", "HEAD"), fixture.head)
            self.assertEqual(fixture.git("show", "HEAD:unrelated.txt"), "original unrelated")
            self.assertEqual(fixture.git("diff", "--cached", "--name-only"), "unrelated.txt")
            self.assertNotIn("NON_MATCHING", fixture.source.read_text())
            self.assertIn("generated symbols", fixture.git("show", "HEAD:overlay_undefined_syms.us.txt"))
            self.assertFalse((fixture.root / "docs/matching-triage-handoffs/fixture.md").exists())

    def test_exception_after_every_command_restores_files_and_ref(self):
        with Fixture() as reference:
            ok, error = reference.promote()
            self.assertTrue(ok, error)
            count = len(reference.calls)
        for boundary in range(1, count + 1):
            with self.subTest(boundary=boundary), Fixture() as fixture:
                def fail(number, args, deadline):
                    if number == boundary:
                        raise subprocess.TimeoutExpired(args, 0)
                fixture.after = fail
                ok, error = fixture.promote()
                self.assertFalse(ok)
                fixture.assert_restored(self)
                self.assertIn("TimeoutExpired", error)

    def test_cancellation_after_every_command_restores_files_and_ref(self):
        with Fixture() as reference:
            ok, error = reference.promote()
            self.assertTrue(ok, error)
            count = len(reference.calls)
        for boundary in range(1, count + 1):
            with self.subTest(boundary=boundary), Fixture() as fixture:
                fixture.after = lambda number, args, deadline: batch.CANCEL_EVENT.set() if number == boundary else None
                ok, error = fixture.promote()
                self.assertFalse(ok)
                fixture.assert_restored(self)
                self.assertIn("cancelled", error)

    def test_nonzero_exit_from_each_generator_and_proof_restores(self):
        with Fixture() as reference:
            ok, error = reference.promote()
            self.assertTrue(ok, error)
            boundaries = [i for i, args in enumerate(reference.calls, 1) if args[0] != "git"]
        for boundary in boundaries:
            with self.subTest(boundary=boundary), Fixture() as fixture:
                fixture.fail_return = boundary
                ok, error = fixture.promote()
                self.assertFalse(ok)
                fixture.assert_restored(self)
                self.assertIn("synthetic command failure", error)

    def test_exception_after_each_direct_file_mutation_restores(self):
        original = transaction.FileJournal.write
        for failure in (1, 2):
            with self.subTest(mutation=failure), Fixture() as fixture:
                count = 0
                def write(journal, path, data):
                    nonlocal count
                    original(journal, path, data)
                    count += 1
                    if count == failure:
                        raise KeyboardInterrupt()
                with patch.object(transaction.FileJournal, "write", write):
                    ok, error = fixture.promote()
                self.assertFalse(ok)
                fixture.assert_restored(self)

    def test_precommit_hook_failure_restores(self):
        with Fixture() as fixture:
            fixture.write(".git/hooks/pre-commit", "#!/bin/sh\nexit 1\n").chmod(0o755)
            ok, error = fixture.promote()
            self.assertFalse(ok)
            fixture.assert_restored(self)

    def test_failed_commit_preserves_unrelated_staged_and_worktree_edits(self):
        with Fixture() as fixture:
            fixture.write("unrelated.txt", "independent staged edit\n")
            fixture.git("add", "unrelated.txt")
            fixture.write("unrelated.txt", "further independent unstaged edit\n")
            fixture.write(".git/hooks/pre-commit", "#!/bin/sh\nexit 1\n").chmod(0o755)
            ok, error = fixture.promote()
            self.assertFalse(ok)
            self.assertEqual(fixture.git("show", ":unrelated.txt"), "independent staged edit")
            self.assertEqual((fixture.root / "unrelated.txt").read_text(), "further independent unstaged edit\n")
            self.assertEqual(fixture.source.read_bytes(), fixture.original[Path("src/fixture.c")])

    def test_preexisting_promotion_path_changes_refuse_commit_and_remain(self):
        with Fixture() as fixture:
            fixture.source.write_text(fixture.source.read_text().replace("independent = 3", "independent = 9"))
            before = fixture.source.read_bytes()
            ok, error = fixture.promote()
            self.assertFalse(ok)
            self.assertIn("already differ", error)
            self.assertEqual(fixture.source.read_bytes(), before)

    def test_unrelated_commit_during_proof_is_preserved_while_our_files_restore(self):
        with Fixture() as fixture:
            external_head = None
            def change(number, args, deadline):
                nonlocal external_head
                if "verify" in args:
                    fixture.write("unrelated.txt", "independent committed edit\n")
                    fixture.git("add", "unrelated.txt")
                    fixture.git("commit", "-qm", "independent commit")
                    external_head = fixture.git("rev-parse", "HEAD")
            fixture.after = change
            ok, error = fixture.promote()
            self.assertFalse(ok)
            self.assertIn("unrelated concurrent commit preserved", error)
            self.assertEqual(fixture.git("rev-parse", "HEAD"), external_head)
            for path, content in fixture.original.items():
                if str(path) != "unrelated.txt":
                    self.assertEqual((fixture.root / path).read_bytes(), content)

    def test_foreign_commit_after_private_tree_capture_is_not_reverted(self):
        with Fixture() as fixture:
            foreign = None
            def change(number, args, deadline):
                nonlocal foreign
                if args == ["git", "write-tree"]:
                    fixture.write("unrelated.txt", "foreign committed content\n")
                    fixture.git("add", "unrelated.txt")
                    fixture.git("commit", "-qm", "foreign publication")
                    foreign = fixture.git("rev-parse", "HEAD")
            fixture.after = change
            ok, error = fixture.promote()
            self.assertFalse(ok)
            self.assertEqual(fixture.git("rev-parse", "HEAD"), foreign)
            self.assertEqual(fixture.git("show", "HEAD:unrelated.txt"), "foreign committed content")
            self.assertEqual(fixture.source.read_bytes(), fixture.original[Path("src/fixture.c")])
            self.assertIn("refs/sweep-recovery/", error)

    def test_branch_switch_does_not_publish_to_either_branch(self):
        with Fixture() as fixture:
            original_ref = fixture.git("symbolic-ref", "HEAD")
            fixture.git("branch", "other", fixture.head)
            def change(number, args, deadline):
                if args == ["git", "write-tree"]:
                    fixture.git("symbolic-ref", "HEAD", "refs/heads/other")
            fixture.after = change
            ok, error = fixture.promote()
            self.assertFalse(ok)
            self.assertIn("branch changed", error)
            self.assertEqual(fixture.git("rev-parse", original_ref), fixture.head)
            self.assertEqual(fixture.git("rev-parse", "other"), fixture.head)
            fixture.assert_restored(self)

    def test_hook_private_index_mutation_is_recovery_only(self):
        with Fixture() as fixture:
            blob_path = fixture.write("build/hook-blob", "unproved hooked index content\n")
            blob = fixture.git("hash-object", "-w", str(blob_path))
            fixture.write(".git/hooks/pre-commit", "#!/bin/sh\ngit update-index --add --cacheinfo "
                          + f"100644,{blob},unrelated.txt\n").chmod(0o755)
            ok, error = fixture.promote()
            self.assertFalse(ok)
            self.assertIn("differs from the proved tree", error)
            self.assertIn("refs/sweep-recovery/", error)
            fixture.assert_restored(self)

    def test_owned_path_staged_before_index_lock_is_preserved(self):
        with Fixture() as fixture:
            blob_path = fixture.write("build/staged-blob", "independent staged source\n")
            blob = fixture.git("hash-object", "-w", str(blob_path))
            original = transaction.locked_index
            count = 0
            @contextlib.contextmanager
            def competing(index):
                nonlocal count
                count += 1
                if count == 1:
                    fixture.git("update-index", "--add", "--cacheinfo", f"100644,{blob},src/fixture.c")
                with original(index) as temporary:
                    yield temporary
            with patch.object(transaction, "locked_index", competing):
                ok, error = fixture.promote()
            self.assertFalse(ok)
            self.assertIn("concurrent promotion-path index edits preserved", error)
            self.assertEqual(fixture.git("rev-parse", "HEAD"), fixture.head)
            self.assertEqual(fixture.git("show", ":src/fixture.c"), "independent staged source")
            self.assertEqual(fixture.source.read_bytes(), fixture.original[Path("src/fixture.c")])

    def test_main_index_lock_blocks_writer_between_comparison_and_reset(self):
        with Fixture() as fixture:
            attempted = False
            def change(number, args, deadline):
                nonlocal attempted
                if args[:2] == ["git", "ls-files"] and (fixture.root / ".git/index.lock").exists():
                    attempted = True
                    with self.assertRaises(subprocess.CalledProcessError):
                        fixture.git("add", "src/fixture.c")
            fixture.after = change
            ok, error = fixture.promote()
            self.assertTrue(ok, error)
            self.assertTrue(attempted)
            self.assertFalse((fixture.root / ".git/index.lock").exists())

    def test_checkout_cannot_switch_branch_between_head_check_and_publication(self):
        with Fixture() as fixture:
            original_ref = fixture.git("symbolic-ref", "HEAD")
            fixture.git("branch", "other", fixture.head)
            attempted = False
            def change(number, args, deadline):
                nonlocal attempted
                if (args == ["git", "symbolic-ref", "HEAD"]
                        and (fixture.root / ".git/index.lock").exists()):
                    attempted = True
                    with self.assertRaises(subprocess.CalledProcessError):
                        fixture.git("checkout", "-q", "other")
                    self.assertEqual(fixture.git("symbolic-ref", "HEAD"), original_ref)
            fixture.after = change
            ok, error = fixture.promote()
            self.assertTrue(ok, error)
            self.assertTrue(attempted)
            self.assertEqual(fixture.git("symbolic-ref", "HEAD"), original_ref)
            self.assertEqual(fixture.git("rev-parse", "other"), fixture.head)
            self.assertEqual(fixture.git("diff", "--cached", "--name-only"), "")

    def test_checkout_after_locked_publication_does_not_roll_back_completed_commit(self):
        with Fixture() as fixture:
            original_ref = fixture.git("symbolic-ref", "HEAD")
            fixture.git("branch", "other", fixture.head)
            original = transaction.locked_index
            @contextlib.contextmanager
            def following_checkout(index):
                with original(index) as temporary:
                    yield temporary
                fixture.git("checkout", "-q", "other")
                batch.CANCEL_EVENT.set()
            with patch.object(transaction, "locked_index", following_checkout):
                ok, error = fixture.promote()
            self.assertTrue(ok, error)
            self.assertNotEqual(fixture.git("rev-parse", original_ref), fixture.head)
            self.assertEqual(fixture.git("symbolic-ref", "HEAD"), "refs/heads/other")
            self.assertEqual(fixture.git("rev-parse", "HEAD"), fixture.head)
            self.assertEqual(fixture.git("diff", "--cached", "--name-only"), "")
            self.assertEqual(fixture.source.read_bytes(), fixture.original[Path("src/fixture.c")])

    def test_checkout_is_blocked_during_ref_and_index_recovery(self):
        with Fixture() as fixture:
            fixture.git("branch", "other", fixture.head)
            original = transaction.locked_index
            locks = 0
            @contextlib.contextmanager
            def guarded_recovery(index):
                nonlocal locks
                with original(index) as temporary:
                    locks += 1
                    if locks == 2:
                        with self.assertRaises(subprocess.CalledProcessError):
                            fixture.git("checkout", "-q", "other")
                    yield temporary
            def fail(number, args, deadline):
                if args[:2] == ["git", "reset"]:
                    raise RuntimeError("failure before index publication")
            fixture.after = fail
            with patch.object(transaction, "locked_index", guarded_recovery):
                ok, error = fixture.promote()
            self.assertFalse(ok)
            self.assertEqual(locks, 2)
            fixture.assert_restored(self)

    def test_existing_index_lock_is_preserved_and_owned_files_restore(self):
        with Fixture() as fixture:
            lock = fixture.write(".git/index.lock", "independent writer lock\n")
            ok, error = fixture.promote()
            self.assertFalse(ok)
            self.assertEqual(lock.read_text(), "independent writer lock\n")
            fixture.assert_restored(self)

    def test_configured_hooks_path_is_not_bypassed_by_detached_context(self):
        with Fixture() as fixture:
            fixture.git("config", "core.hooksPath", ".custom-hooks")
            fixture.write(".custom-hooks/pre-commit", "#!/bin/sh\nexit 9\n").chmod(0o755)
            ok, error = fixture.promote()
            self.assertFalse(ok)
            fixture.assert_restored(self)

    def test_worktree_and_conditional_commit_policy_fail_closed(self):
        for scope in ("worktree", "onbranch", "gitdir"):
            with self.subTest(scope=scope), Fixture() as fixture:
                if scope == "worktree":
                    fixture.git("config", "extensions.worktreeConfig", "true")
                    fixture.git("config", "--worktree", "commit.gpgSign", "true")
                else:
                    policy = fixture.write(".git/origin-policy", "[commit]\n\tgpgSign = true\n")
                    condition = ("onbranch:" + fixture.git("symbolic-ref", "--short", "HEAD")
                                 if scope == "onbranch" else "gitdir:" + str((fixture.root / ".git").resolve()))
                    fixture.git("config", f"includeIf.{condition}.path", str(policy))
                self.assertEqual(fixture.git("config", "--get", "commit.gpgSign"), "true")
                ok, error = fixture.promote()
                self.assertFalse(ok)
                self.assertIn("configuration differ", error)
                self.assertFalse(any(args[0] == "git" and "commit" in args for args in fixture.calls))
                fixture.assert_restored(self)

    def test_full_proof_rejects_relocation_and_missing_ownership_surfaces(self):
        mutations = (
            ("identity", lambda row: row["relocation_comparison"].update(effective_identity_exact=False)),
            ("count", lambda row: row["relocation_comparison"].update(candidate_record_count=2)),
            ("type-offset", lambda row: row["relocation_comparison"].update(offset_type_exact=False)),
            ("ownership", lambda row: row["preflight"].update(status="partial")),
        )
        for overlay in (None, 1):
            for name, mutate in mutations:
                with self.subTest(overlay=overlay, surface=name), Fixture() as fixture:
                    fixture.item.overlay = overlay
                    mutate(fixture.proof_report)
                    ok, error = fixture.promote()
                    self.assertFalse(ok)
                    self.assertIn("promotion_proof.py", error)
                    self.assertFalse(any(args[0] == "git" and "commit" in args for args in fixture.calls))
                    fixture.assert_restored(self)

    def test_full_proof_command_runs_after_verify_without_duplicate_canonical_build(self):
        with Fixture() as fixture:
            ok, error = fixture.promote()
            self.assertTrue(ok, error)
            commands = fixture.calls
            target = (str(batch.PYTHON), "tools/promotion_proof.py", "fixture", "--json")
            self.assertIn(target, commands)
            self.assertLess(next(i for i, args in enumerate(commands) if "verify" in args), commands.index(target))
            self.assertFalse(any(args[0] == "tools/wb_compare.sh" for args in commands))

    def test_public_run_prepared_apply_commit_path(self):
        with Fixture() as fixture:
            scratch = fixture.root / "build/scratch"
            fixture.write("build/scratch/base.c", fixture.winner.read_text())
            result = batch.RunResult("fixture", "src/fixture.c", 1, False)
            with patch.object(batch, "run_permuter", return_value=(0, 0, False, False)):
                batch.run_prepared(fixture.item, scratch, scratch.parent, result, 1, 1, 1,
                                   True, [], 0, 0, True, 0, time.monotonic() + 15)
            self.assertTrue(result.promoted, result.promote_error)
            self.assertTrue(result.zero_found)
            self.assertIsNone(result.error)

    def test_run_prepared_does_not_credit_failed_apply(self):
        with Fixture() as fixture:
            scratch = fixture.root / "build/scratch"
            fixture.write("build/scratch/base.c", fixture.winner.read_text())
            def fail(number, args, deadline):
                if "verify" in args:
                    raise RuntimeError("proof failed")
            fixture.after = fail
            result = batch.RunResult("fixture", "src/fixture.c", 1, False)
            with patch.object(batch, "run_permuter", return_value=(0, 0, False, False)):
                batch.run_prepared(fixture.item, scratch, scratch.parent, result, 1, 1, 1,
                                   True, [], 0, 0, True, 0, time.monotonic() + 15)
            self.assertFalse(result.promoted)
            self.assertIn("proof failed", result.promote_error)
            fixture.assert_restored(self)

    def test_real_postcommit_timeout_retains_recovery_and_restores_head(self):
        with Fixture() as fixture:
            fixture.write(".git/hooks/post-commit", "#!/bin/sh\nsleep 20\n").chmod(0o755)
            start = time.monotonic()
            ok, error = fixture.promote(seconds=1)
            self.assertFalse(ok)
            self.assertLess(time.monotonic() - start, 4)
            fixture.assert_restored(self)
            self.assertIn("refs/sweep-recovery/", error)
            self.assertTrue(fixture.git("for-each-ref", "--format=%(refname)", "refs/sweep-recovery/"))

    def test_independent_source_edit_is_merged_during_rollback(self):
        with Fixture() as fixture:
            def change(number, args, deadline):
                if "verify" in args:
                    fixture.source.write_text(fixture.source.read_text().replace("independent = 3", "independent = 4"))
                    raise RuntimeError("synthetic proof failure")
            fixture.after = change
            ok, error = fixture.promote()
            self.assertFalse(ok)
            expected = fixture.original[Path("src/fixture.c")].decode().replace("independent = 3", "independent = 4")
            self.assertEqual(fixture.source.read_text(), expected, error)
            self.assertEqual(fixture.git("rev-parse", "HEAD"), fixture.head)

    def test_overlapping_source_edit_is_preserved_and_flagged(self):
        with Fixture() as fixture:
            def change(number, args, deadline):
                if "verify" in args:
                    fixture.source.write_text(fixture.source.read_text().replace("return 2", "return 77"))
                    raise RuntimeError("synthetic proof failure")
            fixture.after = change
            ok, error = fixture.promote()
            self.assertFalse(ok)
            self.assertIn("return 77", fixture.source.read_text())
            self.assertIn("manual rollback", error)

    def test_expired_deadline_does_not_mutate_source(self):
        with Fixture() as fixture:
            ok, error = fixture.promote(seconds=-1)
            self.assertFalse(ok)
            fixture.assert_restored(self)

    def test_cancellation_interrupts_promotion_lock_wait(self):
        with Fixture() as fixture:
            batch.PROMOTE_LOCK.acquire()
            timer = threading.Timer(0.05, batch.CANCEL_EVENT.set)
            timer.start()
            start = time.monotonic()
            try:
                ok, error = fixture.promote(seconds=15)
            finally:
                batch.PROMOTE_LOCK.release()
                timer.join()
            self.assertFalse(ok)
            self.assertIn("cancelled", error)
            self.assertLess(time.monotonic() - start, 1)
            fixture.assert_restored(self)

    def test_signal_handler_cancels_cli_and_restores_previous_handler(self):
        with Fixture() as fixture:
            previous = signal.getsignal(signal.SIGTERM)
            def cancel(_argv):
                os.kill(os.getpid(), signal.SIGTERM)
                return 0
            with patch.object(batch, "run_batch", side_effect=cancel):
                self.assertEqual(batch.main([]), 130)
            self.assertEqual(signal.getsignal(signal.SIGTERM), previous)

    def test_cancellation_interrupts_headroom_wait(self):
        with Fixture():
            timer = threading.Timer(0.05, batch.CANCEL_EVENT.set)
            timer.start()
            start = time.monotonic()
            try:
                with patch.object(batch.os, "getloadavg", return_value=(100, 100, 100)):
                    with self.assertRaisesRegex(RuntimeError, "cancelled"):
                        batch.wait_for_headroom(1, batch_deadline=time.monotonic() + 15)
            finally:
                timer.join()
            self.assertLess(time.monotonic() - start, 1)


if __name__ == "__main__":
    unittest.main()
