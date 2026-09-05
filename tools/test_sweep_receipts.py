#!/usr/bin/env python3
"""Durable receipt identity, concurrency and real runner failure regressions."""
from __future__ import annotations

import contextlib
import copy
import dataclasses
import importlib.util
import json
import os
import shutil
import subprocess
import sys
import tempfile
import time
import tomllib
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import sweep_receipts as receipts

spec = importlib.util.spec_from_file_location("sweep_runner_test", Path(__file__).with_name("permute_batch.py"))
batch = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = batch
spec.loader.exec_module(batch)


class ReceiptTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(prefix="sweep-receipt-")
        self.root = Path(self.tmp.name)
        self.store = receipts.ReceiptStore(self.root / "store")
        self.inputs = {"schema": 1, "context": {"source": "fixture",
                       "identity": {"overlay": 1, "section": ".text", "offset": 16}},
                       "search": {"minutes": 1}}
        self.result = {"func": "fixture", "ok": True, "base_score": 20,
                       "best_score": 10, "zero_found": False, "promoted": False}

    def tearDown(self):
        self.tmp.cleanup()

    def record(self, result=None, inputs=None):
        inputs = inputs or self.inputs
        key = receipts.digest(inputs)
        with self.store.claim(key) as owned:
            self.assertTrue(owned)
            self.store.record(inputs, result or self.result)
        return key

    def test_completed_and_best_survive_new_store_instance(self):
        key = self.record()
        self.record({**self.result, "best_score": 15})
        store = receipts.ReceiptStore(self.store.root)
        self.assertEqual(store.completed(key)["result"]["best_score"], 15)
        self.assertEqual(json.loads((store.directory(key) / "best.json").read_text())["score"], 10)
        self.assertEqual(len(list((store.directory(key) / "attempts").glob("*.json"))), 2)

    def test_failure_interrupt_and_unpromoted_zero_do_not_complete(self):
        for change in ({"ok": False, "error": "compiler died"}, {"stopped_batch": True},
                       {"zero_found": True, "best_score": 0}, {"promote_error": "proof failed"},
                       {"commit_error": "hook failed"}, {"base_score": None}):
            inputs = copy.deepcopy(self.inputs)
            inputs["search"]["case"] = str(change)
            key = self.record({**self.result, **change}, inputs)
            self.assertIsNone(self.store.completed(key))
            self.assertEqual(len(list((self.store.directory(key) / "attempts").glob("*.json"))), 1)

    def test_changed_identity_and_settings_do_not_resume(self):
        self.record()
        for change in ("overlay", "section", "offset"):
            inputs = copy.deepcopy(self.inputs)
            inputs["context"]["identity"][change] = "different"
            self.assertIsNone(self.store.completed(receipts.digest(inputs)))
        inputs = copy.deepcopy(self.inputs)
        inputs["search"]["minutes"] = 2
        self.assertIsNone(self.store.completed(receipts.digest(inputs)))
        self.assertTrue(self.store.descending(inputs["context"]))
        inputs["context"]["source"] = "changed"
        self.assertFalse(self.store.descending(inputs["context"]))

    def test_corrupt_receipt_is_not_reused(self):
        key = self.record()
        path = self.store.directory(key) / "complete.json"
        path.write_text("{unfinished")
        self.assertIsNone(self.store.completed(key))

    def test_cross_process_claim_and_crash_release(self):
        key = receipts.digest(self.inputs)
        code = ("import sys,time; from pathlib import Path; "
                "from sweep_receipts import ReceiptStore; "
                "s=ReceiptStore(Path(sys.argv[1])); "
                "c=s.claim(sys.argv[2]); print(c.__enter__(),flush=True); time.sleep(20)")
        env = dict(os.environ, PYTHONPATH=str(Path(receipts.__file__).parent))
        proc = subprocess.Popen([sys.executable, "-c", code, str(self.store.root), key],
                                env=env, stdout=subprocess.PIPE, text=True)
        try:
            self.assertEqual(proc.stdout.readline().strip(), "True")
            with self.store.claim(key) as owned:
                self.assertFalse(owned)
        finally:
            proc.kill()
            proc.wait()
            proc.stdout.close()
        with self.store.claim(key) as owned:
            self.assertTrue(owned)

    def test_common_directory_shared_by_linked_worktree(self):
        repo = self.root / "repo"
        repo.mkdir()
        env = dict(os.environ, GIT_CONFIG_GLOBAL=os.devnull, GIT_CONFIG_NOSYSTEM="1")
        def git(*args):
            subprocess.run(["git", "-C", str(repo), *args], check=True, env=env,
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        git("init", "-q")
        git("-c", "user.name=Fixture", "-c", "user.email=fixture@example.invalid",
            "commit", "--allow-empty", "-qm", "fixture")
        lane = self.root / "lane"
        git("worktree", "add", "-qb", "lane", str(lane))
        self.assertEqual(receipts.ReceiptStore.for_repo(repo).root,
                         receipts.ReceiptStore.for_repo(lane).root)


class RecipeTests(unittest.TestCase):
    def test_full_argument_tail_reaches_importer_settings(self):
        source, obj = "src/fixture.c", "build/src/fixture.c.o"
        line = (".venv/bin/python tools/asm-processor/build.py tools/ido/cc -- "
                "tools/binutils/mips64-elf-as -32 -- -c -non_shared -G 0 "
                "-O2 -mips2 -DBUILD_VERSION=7 -DRAREDIFFS -I 'fixture include' "
                f"-Xphase,uopt,+ -Xphase,uopt,-O1 -o {obj} {source}")
        args = batch.compiler_arguments(line, source, obj)
        self.assertIn("-DBUILD_VERSION=7", args)
        self.assertIn("-DRAREDIFFS", args)
        self.assertIn("fixture include", args)
        self.assertIn("-Xphase,uopt,-O1", args)
        self.assertNotIn(source, args)
        self.assertNotIn(obj, args)
        recipe = batch.BuildRecipe(("-O2", "-mips2"), (), (), True, args)
        with tempfile.TemporaryDirectory(prefix="sweep-settings-") as tmp:
            settings = Path(tmp) / "settings.toml"
            batch.write_settings_toml(settings, recipe.flags, recipe=recipe)
            parsed = tomllib.loads(settings.read_text())
        self.assertEqual(batch.shlex.split(parsed["compiler_command"]),
                         ["tools/ido/cc", *args, "-DNON_MATCHING"])
        self.assertTrue(parsed["objdump_command"].startswith("tools/binutils/"))

    def test_unsupported_wrapper_and_shell_fail_closed(self):
        for line in ("ccache tools/ido/cc -c -O2 -mips2 -o out.o fixture.c",
                     "env MODE=test tools/ido/cc -c -O2 -mips2 -o out.o fixture.c",
                     "tools/ido/cc -c -O2 -mips2 -o out.o fixture.c && echo changed"):
            with self.assertRaises(ValueError):
                batch.compiler_arguments(line, "fixture.c", "out.o")

    def test_recipe_is_refreshed_after_same_path_flags_change(self):
        with tempfile.TemporaryDirectory(prefix="sweep-recipe-") as tmp:
            root = Path(tmp)
            source = root / "src/fixture.c"
            source.parent.mkdir()
            source.write_text("int fixture(void);\n")
            mtime = source.stat().st_mtime_ns
            def dry(value):
                return subprocess.CompletedProcess([], 0, stdout=(
                    "tools/ido/cc -c -O2 -mips2 " + value +
                    " -o build/src/fixture.c.o src/fixture.c\n"))
            with patch.object(batch, "ROOT", root), \
                 patch.object(batch, "bounded_capture", side_effect=[dry("-DVALUE=1"), dry("-DVALUE=2")]) as command:
                first = batch.build_recipe_for(source)
                second = batch.build_recipe_for(source)
            self.assertIn("-DVALUE=1", first.compiler_args)
            self.assertIn("-DVALUE=2", second.compiler_args)
            self.assertEqual(source.stat().st_mtime_ns, mtime)
            self.assertEqual(command.call_args.args[0][:4], ["gmake", "-n", "-W", "src/fixture.c"])
            self.assertTrue(command.call_args.kwargs["check"])

    def test_failed_dry_run_rejects_usable_compiler_line(self):
        with tempfile.TemporaryDirectory(prefix="failed-recipe-") as tmp:
            root = Path(tmp)
            source = root / "fixture.c"
            source.write_text("int fixture(void);\n")
            command = root / "gmake"
            command.write_text("#!/bin/sh\necho 'tools/ido/cc -c -O2 -mips2 -o build/fixture.c.o fixture.c'\nexit 2\n")
            command.chmod(0o755)
            with patch.object(batch, "ROOT", root), patch.dict(os.environ, {"PATH": str(root) + os.pathsep + os.environ["PATH"]}):
                with self.assertRaises(subprocess.CalledProcessError):
                    batch.build_recipe_for(source)


class ImportPreservationTests(unittest.TestCase):
    def test_preexisting_and_failed_imports_survive_retry(self):
        for fault in ("exit", "timeout", "cancel", "success"):
            with self.subTest(fault=fault), tempfile.TemporaryDirectory(prefix="import-preserve-") as tmp:
                root = Path(tmp)
                source = root / "fixture.c"
                source.write_text("int fixture(void);\n")
                stale = root / "nonmatchings/fixture"
                stale.mkdir(parents=True)
                (stale / "manual-best.c").write_text("manual candidate\n")
                output = root / "build/run"
                output.mkdir(parents=True)
                def importer(args, deadline):
                    stale.mkdir()
                    (stale / "candidate.c").write_text("new candidate\n")
                    if fault == "timeout":
                        raise subprocess.TimeoutExpired(args, 0)
                    if fault == "cancel":
                        raise KeyboardInterrupt()
                    return subprocess.CompletedProcess(args, 0 if fault == "success" else 7, "diagnostic")
                with patch.object(batch, "ROOT", root), patch.object(batch, "bounded_capture", side_effect=importer):
                    item = batch.QueueItem("fixture", source)
                    if fault == "success":
                        batch.run_import(item, output, output / "settings", output / "target")
                    else:
                        with self.assertRaises((RuntimeError, subprocess.TimeoutExpired, KeyboardInterrupt)):
                            batch.run_import(item, output, output / "settings", output / "target")
                self.assertEqual(next(output.glob("preexisting-import-*/manual-best.c")).read_text(), "manual candidate\n")
                pattern = "scratch/candidate.c" if fault == "success" else "failed-import-*/candidate.c"
                self.assertEqual(next(output.glob(pattern)).read_text(), "new candidate\n")
                self.assertFalse(stale.exists())


class RunnerTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(prefix="sweep-runner-")
        self.root = Path(self.tmp.name)
        self.stack = contextlib.ExitStack()
        self.store = receipts.ReceiptStore(self.root / "receipts")
        for name, value in (("ROOT", self.root), ("BUILD_PERMUTER", self.root / "build/permuter"),
                            ("PERMUTER_DIR", self.root / "permuter"),
                            ("PERMUTER_PY", self.root / "permuter/permuter.py"),
                            ("PYTHON", Path(sys.executable))):
            self.stack.enter_context(patch.object(batch, name, value))
        self.write("src/fixture.c", "int fixture(void) { return 1; }\n")
        self.write("tools/ido/cc", "compiler fixture\n")
        self.write("tools/binutils/as", "assembler fixture\n")
        self.write("permuter/permuter.py", "print('base score = 20', flush=True)\n")
        self.item = batch.QueueItem("fixture", self.root / "src/fixture.c")
        recipe = batch.BuildRecipe(("-O2", "-mips2"), (), (), True, ("-c", "-O2", "-mips2"))
        self.stack.enter_context(patch.object(batch, "build_recipe_for", return_value=recipe))
        self.stack.enter_context(patch.object(batch, "replicate_objcopy"))
        self.stack.enter_context(patch.object(batch, "prepare_target_asm", side_effect=self.target))
        self.stack.enter_context(patch.object(batch, "run_import", side_effect=self.importer))
        self.stack.enter_context(patch.object(batch, "annotate_overlay_scratch", return_value=0))

    def tearDown(self):
        self.stack.close()
        self.tmp.cleanup()

    def write(self, path, text):
        path = self.root / path
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)
        return path

    def target(self, item, out_dir):
        path = out_dir / "target.s"
        # Synthetic address metadata only, no target machine words.
        path.write_text("/* 100 80000100 fixture */\n")
        return path

    def importer(self, item, out_dir, settings, target, deadline):
        scratch = out_dir / "scratch"
        scratch.mkdir()
        (scratch / "base.c").write_text(item.c_file.read_text())
        (scratch / "compile.sh").write_text(f'#!/bin/sh\ncd {self.root}\ncompiler "$INPUT"\n')
        (scratch / "settings.toml").write_text('compiler_type = "ido"\n')
        shutil.copy(target, scratch / "target.s")
        return scratch

    def run_one(self, **kwargs):
        return batch.run_one(self.item, 1, 1, 1, False, [], load_threshold=0,
                             annotate_overlays=False, receipt_store=self.store, **kwargs)

    def test_real_search_then_resume_from_durable_receipt(self):
        first = self.run_one()
        self.assertTrue(first.ok, first.error)
        self.assertEqual(first.base_score, 20)
        with patch.object(batch, "run_permuter", side_effect=AssertionError("must skip search")):
            second = self.run_one(resume=True)
        self.assertTrue(second.resumed, second.error)
        self.assertEqual(first.receipt_key, second.receipt_key)
        self.assertTrue(Path(first.scratch_path).exists())

    def test_import_lock_wait_observes_total_deadline(self):
        batch._IMPORT_LOCK.acquire()
        start = time.monotonic()
        try:
            result = self.run_one(batch_deadline=time.monotonic() + 0.05)
        finally:
            batch._IMPORT_LOCK.release()
        self.assertFalse(result.ok)
        self.assertTrue(result.stopped_batch)
        self.assertLess(time.monotonic() - start, 1)

    def test_identical_inputs_share_key_across_lane_paths(self):
        first = self.run_one()
        original_root = self.root
        original_item = self.item
        lane = self.root / "other-lane"
        for name in ("src", "tools", "permuter"):
            shutil.copytree(self.root / name, lane / name)
        try:
            self.root = lane
            self.item = batch.QueueItem("fixture", lane / "src/fixture.c")
            with patch.object(batch, "ROOT", lane), \
                 patch.object(batch, "BUILD_PERMUTER", lane / "build/permuter"), \
                 patch.object(batch, "PERMUTER_DIR", lane / "permuter"), \
                 patch.object(batch, "PERMUTER_PY", lane / "permuter/permuter.py"):
                second = self.run_one(resume=True)
            self.assertTrue(second.resumed, second.error)
            self.assertEqual(first.receipt_key, second.receipt_key)
        finally:
            self.root = original_root
            self.item = original_item

    def test_changed_source_compiler_permuter_and_prepared_context_retry(self):
        previous = self.run_one()
        for path in ("src/fixture.c", "tools/ido/cc", "permuter/weights.toml"):
            target = self.root / path
            target.write_text((target.read_text() if target.exists() else "") + "\n")
            result = self.run_one(resume=True)
            self.assertTrue(result.ok, result.error)
            self.assertFalse(result.resumed)
            self.assertNotEqual(result.receipt_key, previous.receipt_key)
            previous = result
        original = self.importer
        def changed_header(*args):
            scratch = original(*args)
            (scratch / "base.c").write_text("int fixture(void) { return 2; }\n")
            return scratch
        with patch.object(batch, "run_import", side_effect=changed_header):
            result = self.run_one(resume=True)
        self.assertNotEqual(result.receipt_key, previous.receipt_key)

    def test_header_expanded_path_literals_do_not_alias_placeholder_text(self):
        original = self.importer
        literal = str(self.root)
        def expanded(*args):
            scratch = original(*args)
            (scratch / "base.c").write_text('const char *header_value = ' + json.dumps(literal) + ';\n')
            return scratch
        with patch.object(batch, "run_import", side_effect=expanded):
            first = self.run_one()
            literal = "<repo>"
            second = self.run_one(resume=True)
        self.assertFalse(second.resumed)
        self.assertNotEqual(first.receipt_key, second.receipt_key)

    def test_extension_failure_retains_primary_best_scalar_and_artifact(self):
        def primary(scratch, *args, **kwargs):
            best = scratch / "output-10-1"
            best.mkdir()
            (best / "score.txt").write_text("10\n")
            (best / "source.c").write_text("int fixture(void) { return 2; }\n")
            return (20, 60, False, False)
        count = 0
        def search(*args, **kwargs):
            nonlocal count
            count += 1
            if count == 1:
                return primary(*args, **kwargs)
            raise RuntimeError("extension failed")
        with patch.object(batch, "run_permuter", side_effect=search):
            result = self.run_one(extend_minutes=1)
        self.assertFalse(result.ok)
        self.assertEqual(result.best_score, 10)
        self.assertIsNone(self.store.completed(result.receipt_key))
        saved = json.loads((self.store.directory(result.receipt_key) / "best.json").read_text())
        self.assertEqual(saved["score"], 10)
        self.assertTrue((Path(result.scratch_path) / "output-10-1/source.c").is_file())

    def test_nonzero_process_exit_is_retryable_even_after_base_score(self):
        self.write("permuter/permuter.py", "print('base score = 20', flush=True)\nraise SystemExit(7)\n")
        for _ in range(2):
            result = self.run_one(resume=True)
            self.assertFalse(result.ok)
            self.assertIn("exited 7", result.error)
            self.assertFalse(result.resumed)
            self.assertIsNone(self.store.completed(result.receipt_key))

    def test_process_exit_124_is_failure_not_runner_cap(self):
        self.write("permuter/permuter.py", "print('base score = 20', flush=True)\nraise SystemExit(124)\n")
        result = self.run_one()
        self.assertFalse(result.ok)
        self.assertIn("exited 124", result.error)

    def test_failed_parent_does_not_leave_term_ignoring_worker(self):
        self.write("permuter/permuter.py", """import os, signal, time
read_fd, write_fd = os.pipe()
worker = os.fork()
if worker == 0:
    os.close(read_fd)
    signal.signal(signal.SIGTERM, signal.SIG_IGN)
    os.write(write_fd, b'1')
    time.sleep(20)
    os._exit(0)
os.close(write_fd)
os.read(read_fd, 1)
print('worker =', worker, flush=True)
print('base score = 20', flush=True)
raise SystemExit(7)
""")
        result = self.run_one()
        self.assertFalse(result.ok)
        log = (Path(result.scratch_path).parent / "permuter.log").read_text()
        worker = int(log.split("worker = ")[1].splitlines()[0])
        deadline = time.monotonic() + 2
        while True:
            state = subprocess.run(["ps", "-p", str(worker), "-o", "stat="],
                                   capture_output=True, text=True).stdout.strip()
            if not state or state.startswith("Z"):
                break
            if time.monotonic() >= deadline:
                self.fail(f"our worker {worker} survived cleanup: {state}")
            time.sleep(0.02)

    def test_zero_exit_without_base_score_is_failure(self):
        self.write("permuter/permuter.py", "print('no measurement')\n")
        result = self.run_one()
        self.assertFalse(result.ok)
        self.assertIn("no base score", result.error)

    def test_batch_cap_is_not_a_completed_receipt(self):
        self.write("permuter/permuter.py", "import time\nprint('base score = 20', flush=True)\ntime.sleep(20)\n")
        start = time.monotonic()
        result = self.run_one(batch_deadline=start + 0.2)
        self.assertLess(time.monotonic() - start, 3)
        self.assertTrue(result.stopped_batch)
        self.assertIsNone(self.store.completed(result.receipt_key))

    def test_failed_later_search_preserves_prior_best_artifact(self):
        first = self.run_one()
        with patch.object(batch, "run_permuter", side_effect=RuntimeError("synthetic failure")):
            second = self.run_one()
        self.assertFalse(second.ok)
        self.assertNotEqual(first.scratch_path, second.scratch_path)
        self.assertTrue(Path(first.scratch_path).exists())
        self.assertIsNotNone(self.store.completed(first.receipt_key))

    def test_search_settings_change_does_not_resume(self):
        first = self.run_one()
        second = batch.run_one(self.item, 2, 1, 1, False, [], load_threshold=0,
                               annotate_overlays=False, receipt_store=self.store, resume=True)
        self.assertTrue(second.ok, second.error)
        self.assertFalse(second.resumed)
        self.assertNotEqual(first.receipt_key, second.receipt_key)

    def test_deep_selection_uses_current_prepared_context(self):
        with patch.object(batch, "_best", return_value=(None, 10)):
            first = self.run_one()
        self.assertTrue(first.ok, first.error)
        second = self.run_one(deep=True)
        self.assertFalse(second.deep_skipped)
        self.assertTrue(second.ok, second.error)
        self.item.c_file.write_text("int fixture(void) { return 2; }\n")
        third = self.run_one(deep=True)
        self.assertTrue(third.deep_skipped)

    def test_source_change_during_search_does_not_complete(self):
        def changing_search(*args, **kwargs):
            self.item.c_file.write_text("int fixture(void) { return 3; }\n")
            return 20, 1, False, False
        with patch.object(batch, "run_permuter", side_effect=changing_search):
            result = self.run_one()
        self.assertFalse(result.ok)
        self.assertIn("source changed", result.error)
        self.assertIsNone(self.store.completed(result.receipt_key))

    def test_tools_change_during_search_does_not_complete(self):
        def changing_search(*args, **kwargs):
            self.write("tools/ido/cc", "changed compiler\n")
            return 20, 1, False, False
        with patch.object(batch, "run_permuter", side_effect=changing_search):
            result = self.run_one()
        self.assertFalse(result.ok)
        self.assertIn("tools changed", result.error)
        self.assertIsNone(self.store.completed(result.receipt_key))

    def test_batch_ignores_legacy_symbol_only_resume_and_returns_failure(self):
        self.write("permuter/permuter.py", "print('base score = 20', flush=True)\nraise SystemExit(7)\n")
        summary = self.write("build/permuter/summary.json", json.dumps({"results": [{
            "func": "fixture", "c_file": "src/fixture.c", "overlay": None,
            "ok": True, "base_score": 20}]}))
        with patch.object(batch, "discover_queue", return_value=[self.item]), \
             patch.object(batch, "SUMMARY_JSON", summary), \
             patch.object(batch, "SUMMARY_TXT", summary.with_suffix(".txt")), \
             patch.object(receipts.ReceiptStore, "for_repo", return_value=self.store):
            code = batch.run_batch(["--resume", "--no-integration-ref-filter", "--order", "queue",
                                    "--minutes", "1", "--load-threshold", "0"])
        self.assertEqual(code, 1)
        result = json.loads(summary.read_text())["results"][0]
        self.assertIn("exited 7", result["error"])
        self.assertFalse(result["resumed"])

    def test_resume_limit_counts_searches_after_receipt_skips(self):
        second_source = self.write("src/fixture2.c", "int fixture2(void) { return 2; }\n")
        second_item = batch.QueueItem("fixture2", second_source)
        for jobs in (1, 2):
            with self.subTest(jobs=jobs):
                self.store = receipts.ReceiptStore(self.root / f"receipts-{jobs}")
                first = self.run_one()
                self.assertTrue(first.ok, first.error)
                summary = self.root / f"build/permuter/summary-{jobs}.json"
                with patch.object(batch, "discover_queue", return_value=[self.item, second_item]), \
                     patch.object(batch, "SUMMARY_JSON", summary), \
                     patch.object(batch, "SUMMARY_TXT", summary.with_suffix(".txt")), \
                     patch.object(receipts.ReceiptStore, "for_repo", return_value=self.store):
                    code = batch.run_batch(["--resume", "--limit", "1", "--jobs", str(jobs),
                        "--no-integration-ref-filter", "--order", "queue", "--minutes", "1",
                        "--load-threshold", "0", "--flat-minutes", "0",
                        "--permuter-threads", "1", "--no-overlay-annotate"])
                self.assertEqual(code, 0)
                results = json.loads(summary.read_text())["results"]
                self.assertEqual(len(results), 2)
                self.assertTrue(results[0]["resumed"])
                self.assertFalse(results[1]["resumed"])
                self.assertEqual(results[1]["base_score"], 20)

    def test_preparation_timeout_stops_command(self):
        start = time.monotonic()
        with self.assertRaises(subprocess.TimeoutExpired):
            batch.bounded_capture([sys.executable, "-c", "import time; time.sleep(20)"],
                                   start + 0.1)
        self.assertLess(time.monotonic() - start, 3)


if __name__ == "__main__":
    unittest.main()
