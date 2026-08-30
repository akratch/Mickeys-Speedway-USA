#!/usr/bin/env python3
"""Focused tests for deterministic public-release reconciliation."""

from __future__ import annotations

import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from contextlib import redirect_stdout
import io


TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))

import public_release as pr  # noqa: E402


class GitRepoCase(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory(prefix="public-release-")
        self.repo = Path(self.temporary.name)
        self.git("init", "-q", "-b", "master")
        self.git("config", "user.email", "release@example.invalid")
        self.git("config", "user.name", "Release Test")
        self.git("remote", "add", "public", "https://example.invalid/project.git")
        (self.repo / "README.md").write_text("public project\n")
        (self.repo / "code.c").write_text("int example;\n")
        self.commit("Initial public tree")
        self.git("update-ref", "refs/remotes/public/master", "HEAD")

    def tearDown(self) -> None:
        self.temporary.cleanup()

    def git(self, *args: str) -> str:
        return subprocess.run(
            ["git", *args],
            cwd=self.repo,
            check=True,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        ).stdout

    def commit(self, message: str) -> str:
        self.git("add", "-A")
        self.git("commit", "-q", "-m", message)
        return self.git("rev-parse", "HEAD").strip()

    def track_remote_tip(self) -> None:
        self.git("update-ref", "refs/remotes/public/master", "HEAD")

    def configure_upstream(self) -> None:
        self.git("config", "branch.master.remote", "public")
        self.git("config", "branch.master.merge", "refs/heads/master")


class ReleaseContextTests(GitRepoCase):
    def test_named_fast_forward_context_passes(self) -> None:
        (self.repo / "README.md").write_text("safe outgoing update\n")
        self.commit("Safe update")
        ctx = pr._release_context(
            self.repo, "master", "public", require_clean=True
        )
        self.assertEqual(ctx.branch, "master")
        self.assertEqual(ctx.remote, "public")
        self.assertEqual(len(ctx.outgoing_commits), 1)
        self.assertFalse(pr._scan_release(ctx, include_worktree=False))
        commands = pr._reconciliation_commands(
            ctx, write_derived=False, check_reference_builds=False
        )
        self.assertEqual(commands[0][0], "outgoing-cleanroom")
        self.assertIn("refs/remotes/public/master..HEAD", commands[0])

    def test_wrong_branch_fails_closed(self) -> None:
        self.git("switch", "-q", "-c", "topic")
        with self.assertRaisesRegex(pr.PublicReleaseError, "current branch"):
            pr._release_context(self.repo, "master", "public", require_clean=True)

    def test_remote_credential_is_rejected_before_url_is_reported(self) -> None:
        credential = "ghp_" + "A" * 30
        self.git(
            "remote",
            "set-url",
            "public",
            f"https://{credential}@example.invalid/project.git",
        )
        with self.assertRaisesRegex(pr.PublicReleaseError, "remote URL"):
            pr._release_context(self.repo, "master", "public", require_clean=True)

    def test_transient_outgoing_text_is_scanned_after_later_removal(self) -> None:
        marker = "campaign/" + "unchain"
        (self.repo / "README.md").write_text(f"temporary {marker}\n")
        self.commit("Temporary text")
        (self.repo / "README.md").write_text("clean final tree\n")
        self.commit("Remove temporary text")
        ctx = pr._release_context(
            self.repo, "master", "public", require_clean=True
        )
        findings = pr._scan_release(ctx, include_worktree=False)
        self.assertTrue(any("README.md" in row for row in findings))

    def test_operator_only_path_is_rejected(self) -> None:
        hidden = self.repo / ("." + "codex")
        hidden.mkdir()
        (hidden / "note.txt").write_text("ordinary prose\n")
        self.commit("Add misplaced operator note")
        ctx = pr._release_context(
            self.repo, "master", "public", require_clean=True
        )
        findings = pr._scan_release(ctx, include_worktree=False)
        self.assertTrue(any("forbidden tracked path" in row for row in findings))

    def test_uninitialized_gitlink_is_not_a_missing_tracked_file(self) -> None:
        commit = self.git("rev-parse", "HEAD").strip()
        self.git(
            "update-index",
            "--add",
            "--cacheinfo",
            f"160000,{commit},vendor/tool",
        )
        ctx = pr._release_context(
            self.repo, "master", "public", require_clean=False
        )
        findings = pr._scan_release(ctx, include_worktree=True)
        self.assertFalse(any("vendor/tool" in row for row in findings))

    def test_missing_regular_tracked_file_still_fails(self) -> None:
        (self.repo / "code.c").unlink()
        ctx = pr._release_context(
            self.repo, "master", "public", require_clean=False
        )
        findings = pr._scan_release(ctx, include_worktree=True)
        self.assertIn("worktree: tracked path is missing: code.c", findings)


class DeltaTests(unittest.TestCase):
    def test_scoreboard_metric_deltas_are_exact(self) -> None:
        def scoreboard(functions: int, resident: int, overlay: int) -> str:
            return f"""before
{pr.SCOREBOARD_BEGIN}
## Progress
```
functions      {functions} / 20
.text bytes    {resident} / 1000
verified asm   40 / 1000
overlay C      {overlay} / 800
whole resolved {resident + overlay + 40} / 1800
named          15 / 20
symbols        200
```
```
decompiled              {resident + overlay} / 1800
GLOBAL_ASM remaining    300 / 1800
NON_MATCHING            200 / 1800
```
{pr.SCOREBOARD_END}
after
"""

        lines = pr._metric_delta_lines(scoreboard(10, 400, 200), scoreboard(11, 428, 232))
        self.assertIn("metric functions: 10 -> 11 (+1)", lines)
        self.assertIn("metric resident C bytes: 400 -> 428 (+28)", lines)
        self.assertIn("metric overlay C bytes: 200 -> 232 (+32)", lines)
        self.assertIn("metric whole resolved bytes: 640 -> 700 (+60)", lines)

    def test_overlay_promotions_and_retractions_reconcile_to_total(self) -> None:
        def row(start: int, end: int, *, exact: bool, source: str) -> dict:
            return {
                "offset": hex(start),
                "end_offset": hex(end),
                "size": hex(end - start),
                "type": "c",
                "matched": True,
                "nonmatching": not exact,
                "source": source,
            }

        def atlas(rows: list[dict], matched: int) -> dict:
            return {
                "schema_version": 1,
                "totals": {"matched_overlay_c_bytes": matched},
                "modules": [{"overlay": 7, "text_ownership": rows}],
            }

        old = atlas(
            [
                row(0x0, 0x20, exact=True, source="oldExact"),
                row(0x20, 0x40, exact=False, source="oldCandidate"),
            ],
            32,
        )
        new = atlas(
            [
                row(0x0, 0x20, exact=False, source="oldExact"),
                row(0x20, 0x40, exact=True, source="oldCandidate"),
            ],
            32,
        )
        delta = pr._exact_range_delta(old, new)
        self.assertEqual(
            [(row["offset"], row["end_offset"], row["size"]) for row in delta["retractions"]],
            [(0x0, 0x20, 32)],
        )
        self.assertEqual(delta["promotions"][0]["offset"], 0x20)
        self.assertEqual(delta["totals"]["net_exact_c_bytes"], 0)

    def test_overlay_delta_fails_if_totals_do_not_reconcile(self) -> None:
        empty = {
            "schema_version": 1,
            "totals": {"matched_overlay_c_bytes": 0},
            "modules": [],
        }
        stale = json.loads(json.dumps(empty))
        stale["totals"]["matched_overlay_c_bytes"] = 4
        with self.assertRaisesRegex(pr.PublicReleaseError, "atlas declares 4"):
            pr._exact_range_delta(empty, stale)


class CommandPlanTests(unittest.TestCase):
    def test_default_plan_is_read_only_and_no_plan_can_publish(self) -> None:
        dry_targets = [part for row in pr.READ_ONLY_GENERATORS for part in row]
        write_targets = [part for row in pr.WRITE_GENERATORS for part in row]
        self.assertIn("overlay-atlas", dry_targets)
        self.assertIn("overlay-atlas-write", write_targets)
        for forbidden in ("push", "merge", "fetch", "cp", "rsync"):
            self.assertNotIn(forbidden, dry_targets)
            self.assertNotIn(forbidden, write_targets)

    def test_reference_build_preflight_is_explicit_and_runs_first(self) -> None:
        parser = pr._parser()
        default = parser.parse_args(["--remote", "public", "--branch", "master"])
        opted_in = parser.parse_args(
            [
                "--remote",
                "public",
                "--branch",
                "master",
                "--check-reference-builds",
            ]
        )
        self.assertFalse(default.check_reference_builds)
        self.assertTrue(opted_in.check_reference_builds)

        ctx = pr.ReleaseContext(
            repo=Path("."),
            branch="master",
            remote="public",
            base_ref="refs/remotes/public/master",
            base_oid="0" * 40,
            head_oid="1" * 40,
            fetch_url="https://example.invalid/project.git",
            push_url="https://example.invalid/project.git",
            outgoing_commits=("1" * 40,),
        )
        default_plan = pr._reconciliation_commands(
            ctx, write_derived=True, check_reference_builds=False
        )
        preflight_plan = pr._reconciliation_commands(
            ctx, write_derived=True, check_reference_builds=True
        )
        self.assertNotIn("verify_reference_builds.sh", str(default_plan))
        self.assertEqual(
            preflight_plan[0],
            ("reference-builds", "bash", "tools/verify_reference_builds.sh"),
        )
        self.assertEqual(preflight_plan[1:], default_plan)

    def test_new_public_files_do_not_trigger_their_own_text_scan(self) -> None:
        root = TOOLS.parent
        for relative in (
            "Makefile",
            "docs/tools.md",
            "tools/public_release.py",
            "tools/test_public_release.py",
        ):
            findings = pr._scan_payload(relative, (root / relative).read_bytes())
            self.assertFalse(findings, f"{relative}: {findings}")

    def test_tracked_local_toolchain_links_are_forbidden_paths(self) -> None:
        self.assertTrue(pr._forbidden_path("tools/" + "ido"))
        self.assertTrue(pr._forbidden_path("tools/" + "binutils"))


class DerivedPathTests(GitRepoCase):
    def test_write_mode_allowlist_rejects_unexpected_tracked_change(self) -> None:
        (self.repo / "README.md").write_text("generated scoreboard\n")
        self.assertFalse(pr._unexpected_derived_changes(self.repo))
        (self.repo / "code.c").write_text("int changed;\n")
        self.assertEqual(pr._unexpected_derived_changes(self.repo), ["code.c"])


class ManifestTests(GitRepoCase):
    def setUp(self) -> None:
        super().setUp()
        self.configure_upstream()
        (self.repo / ".gitignore").write_text("build/\n")
        self.commit("Ignore generated release metadata")
        self.track_remote_tip()
        self.base = self.git("rev-parse", "HEAD").strip()

    def manifest(self) -> dict:
        self.track_remote_tip()
        return pr._manifest_from_current_source(
            self.repo, "master", "public", f"{self.base}..HEAD"
        )

    @staticmethod
    def approve(manifest: dict) -> dict:
        approval = pr._approval_template(manifest)
        for unit in approval["units"]:
            for row in unit["paths"]:
                row["decision"] = "approve"
        return approval

    def test_function_sized_commits_remain_separate_deterministic_units(self) -> None:
        (self.repo / "src").mkdir()
        (self.repo / "src" / "one.c").write_text("int one(void) { return 1; }\n")
        (self.repo / "symbol_addrs.us.txt").write_text("one = 1;\n")
        first = self.commit("Match one")
        (self.repo / "src" / "two.c").write_text("int two(void) { return 2; }\n")
        (self.repo / "symbol_addrs.us.txt").write_text("one = 1;\ntwo = 2;\n")
        second = self.commit("Match two")

        manifest = self.manifest()
        repeated = self.manifest()
        self.assertEqual(pr._canonical_json(manifest), pr._canonical_json(repeated))
        self.assertEqual(
            [unit["private_commit"] for unit in manifest["units"]],
            [first, second],
        )
        self.assertEqual([unit["unit"] for unit in manifest["units"]], [1, 2])
        self.assertEqual(
            [row["path"] for row in manifest["units"][0]["changes"]],
            ["src/one.c", "symbol_addrs.us.txt"],
        )
        self.assertTrue(
            all(
                row["classification"] == "public-safe"
                for unit in manifest["units"]
                for row in unit["changes"]
            )
        )
        self.assertIn("gmake verify", manifest["required_final_gates"])
        self.assertFalse(manifest["operations"]["pushes"])

    def test_rename_and_delete_are_review_required_and_keep_preimages(self) -> None:
        (self.repo / "docs").mkdir()
        (self.repo / "docs" / "old.md").write_text("stable documentation payload\n")
        (self.repo / "docs" / "remove.md").write_text("remove this later\n")
        self.commit("Add release documents")
        self.base = self.git("rev-parse", "HEAD").strip()
        self.git("mv", "docs/old.md", "docs/new.md")
        (self.repo / "docs" / "remove.md").unlink()
        self.commit("Reorganize release documents")

        manifest = self.manifest()
        rows = manifest["units"][0]["changes"]
        rename = next(row for row in rows if row["status"].startswith("R"))
        deletion = next(row for row in rows if row["status"] == "D")
        self.assertEqual(rename["old_path"], "docs/old.md")
        self.assertEqual(rename["path"], "docs/new.md")
        self.assertEqual(rename["old_blob"], rename["new_blob"])
        self.assertEqual(rename["classification"], "review-required")
        self.assertIsNotNone(deletion["old_blob"])
        self.assertIsNone(deletion["new_blob"])
        self.assertEqual(deletion["classification"], "review-required")

    def test_forbidden_path_fails_before_manifest_is_emitted(self) -> None:
        forbidden = self.repo / ("AG" + "ENTS.md")
        forbidden.write_text("operator material\n")
        self.commit("Add misplaced operating document")
        self.track_remote_tip()
        with self.assertRaisesRegex(pr.PublicReleaseError, "forbidden"):
            pr._manifest_from_current_source(
                self.repo, "master", "public", f"{self.base}..HEAD"
            )

    def test_symlink_and_gitlink_changes_fail_closed(self) -> None:
        os.symlink("code.c", self.repo / "code-link")
        self.commit("Add source link")
        self.track_remote_tip()
        with self.assertRaisesRegex(pr.PublicReleaseError, "symlink"):
            pr._manifest_from_current_source(
                self.repo, "master", "public", f"{self.base}..HEAD"
            )

        with tempfile.TemporaryDirectory(prefix="public-release-gitlink-") as raw:
            repo = Path(raw)
            subprocess.run(["git", "init", "-q", "-b", "master"], cwd=repo, check=True)
            for key, value in (
                ("user.email", "release@example.invalid"),
                ("user.name", "Release Test"),
            ):
                subprocess.run(["git", "config", key, value], cwd=repo, check=True)
            (repo / "README.md").write_text("start\n")
            subprocess.run(["git", "add", "README.md"], cwd=repo, check=True)
            subprocess.run(["git", "commit", "-q", "-m", "Initial"], cwd=repo, check=True)
            base = subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=repo, text=True).strip()
            subprocess.run(
                ["git", "update-index", "--add", "--cacheinfo", f"160000,{base},vendor/tool"],
                cwd=repo,
                check=True,
            )
            subprocess.run(["git", "commit", "-q", "-m", "Add tool"], cwd=repo, check=True)
            tip = subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=repo, text=True).strip()
            with self.assertRaisesRegex(pr.PublicReleaseError, "submodule"):
                pr._raw_commit_changes(repo, tip)

    def test_stale_blob_identity_and_pending_decision_are_rejected(self) -> None:
        (self.repo / "code.c").write_text("int example = 2;\n")
        self.commit("Update example")
        manifest = self.manifest()
        pending = pr._approval_template(manifest)
        with self.assertRaisesRegex(pr.PublicReleaseError, "not explicitly approved"):
            pr._validate_approval(manifest, pending)

        approval = self.approve(manifest)
        approval["units"][0]["paths"][0]["new_blob"] = "f" * 40
        with self.assertRaisesRegex(pr.PublicReleaseError, "identity has drifted"):
            pr._validate_approval(manifest, approval)

    def test_secret_and_source_workflow_messages_are_rejected(self) -> None:
        marker = "campaign/" + "unchain"
        (self.repo / "code.c").write_text("int example = 2;\n")
        self.commit(f"Move from {marker}")
        self.track_remote_tip()
        with self.assertRaisesRegex(pr.PublicReleaseError, "message failed"):
            pr._manifest_from_current_source(
                self.repo, "master", "public", f"{self.base}..HEAD"
            )

    def test_secret_changed_text_is_rejected_without_echoing_it(self) -> None:
        token = "ghp_" + "A" * 32
        (self.repo / "code.c").write_text(f'const char *value = "{token}";\n')
        self.commit("Update example configuration")
        self.track_remote_tip()
        with self.assertRaisesRegex(pr.PublicReleaseError, "changed text failed") as caught:
            pr._manifest_from_current_source(
                self.repo, "master", "public", f"{self.base}..HEAD"
            )
        self.assertNotIn(token, str(caught.exception))

    def test_absolute_workstation_path_and_automated_trailer_are_rejected(self) -> None:
        workstation_path = "/" + "Users/example/work/source.c"
        (self.repo / "code.c").write_text(f'const char *path = "{workstation_path}";\n')
        self.commit("Record source location")
        self.track_remote_tip()
        with self.assertRaisesRegex(pr.PublicReleaseError, "changed text failed"):
            pr._manifest_from_current_source(
                self.repo, "master", "public", f"{self.base}..HEAD"
            )

        with tempfile.TemporaryDirectory(prefix="public-release-message-") as raw:
            repo = Path(raw)
            subprocess.run(["git", "init", "-q", "-b", "master"], cwd=repo, check=True)
            subprocess.run(
                ["git", "config", "user.email", "release@example.invalid"], cwd=repo, check=True
            )
            subprocess.run(
                ["git", "config", "user.name", "Release Test"], cwd=repo, check=True
            )
            (repo / "code.c").write_text("int value;\n")
            subprocess.run(["git", "add", "code.c"], cwd=repo, check=True)
            trailer = "Co-authored-by: " + "Codex <agent@example.invalid>"
            subprocess.run(
                ["git", "commit", "-q", "-m", "Update value", "-m", trailer],
                cwd=repo,
                check=True,
            )
            commit = subprocess.check_output(
                ["git", "rev-parse", "HEAD"], cwd=repo, text=True
            ).strip()
            with self.assertRaisesRegex(pr.PublicReleaseError, "message failed"):
                pr._message_subject(repo, commit)

    def test_source_tip_drift_and_ambiguous_range_fail_closed(self) -> None:
        (self.repo / "code.c").write_text("int example = 2;\n")
        self.commit("Update example")
        with self.assertRaisesRegex(pr.PublicReleaseError, "remote-tracking ref differ"):
            pr._manifest_from_current_source(
                self.repo, "master", "public", f"{self.base}..HEAD"
            )
        self.track_remote_tip()
        with self.assertRaisesRegex(pr.PublicReleaseError, "exactly BASE"):
            pr._manifest_from_current_source(
                self.repo, "master", "public", f"{self.base}...HEAD"
            )

    def test_multiple_remote_destinations_fail_closed(self) -> None:
        (self.repo / "code.c").write_text("int example = 4;\n")
        self.commit("Update example")
        self.track_remote_tip()
        self.git(
            "config",
            "--add",
            "remote.public.url",
            "https://example.invalid/second-project.git",
        )
        with self.assertRaisesRegex(pr.PublicReleaseError, "exactly one fetch"):
            pr._manifest_from_current_source(
                self.repo, "master", "public", f"{self.base}..HEAD"
            )

    def test_cli_emits_ignored_manifest_and_validates_reviewed_approval(self) -> None:
        (self.repo / "code.c").write_text("int example = 3;\n")
        self.commit("Match example")
        self.track_remote_tip()
        arguments = [
            "--repo",
            str(self.repo),
            "--remote",
            "public",
            "--branch",
            "master",
            "--manifest-range",
            f"{self.base}..HEAD",
        ]
        with redirect_stdout(io.StringIO()):
            self.assertEqual(pr.main(arguments), 0)
        manifest_path = self.repo / "build/public-release/manifest.json"
        approval_path = self.repo / "build/public-release/manifest.approval.json"
        first_bytes = manifest_path.read_bytes()
        with redirect_stdout(io.StringIO()):
            self.assertEqual(pr.main(arguments), 0)
        self.assertEqual(manifest_path.read_bytes(), first_bytes)

        approval = json.loads(approval_path.read_text())
        for unit in approval["units"]:
            for row in unit["paths"]:
                row["decision"] = "approve"
        approval_path.write_text(json.dumps(approval, indent=2, sort_keys=True) + "\n")
        with redirect_stdout(io.StringIO()):
            self.assertEqual(
                pr.main(arguments + ["--approval", "build/public-release/manifest.approval.json"]),
                0,
            )
        serialized = manifest_path.read_text()
        self.assertNotIn(str(self.repo), serialized)


class DivergentExportTests(GitRepoCase):
    def test_unrelated_public_history_validates_by_units_paths_and_blobs(self) -> None:
        self.configure_upstream()
        source_base = self.git("rev-parse", "HEAD").strip()
        (self.repo / "code.c").write_text("int example = 7;\n")
        self.commit("Match example")
        (self.repo / "docs").mkdir()
        (self.repo / "docs" / "release.md").write_text("ordinary public notes\n")
        self.commit("Document example")
        self.track_remote_tip()
        manifest = pr._manifest_from_current_source(
            self.repo, "master", "public", f"{source_base}..HEAD"
        )
        approval = ManifestTests.approve(manifest)

        with tempfile.TemporaryDirectory(prefix="public-release-destination-") as raw:
            public = Path(raw)
            subprocess.run(["git", "init", "-q", "-b", "master"], cwd=public, check=True)
            subprocess.run(
                ["git", "config", "user.email", "public@example.invalid"], cwd=public, check=True
            )
            subprocess.run(
                ["git", "config", "user.name", "Public Test"], cwd=public, check=True
            )
            subprocess.run(
                ["git", "remote", "add", "public", "https://example.invalid/project.git"],
                cwd=public,
                check=True,
            )
            (public / "README.md").write_text("public project\n")
            (public / "code.c").write_text("int example;\n")
            subprocess.run(["git", "add", "-A"], cwd=public, check=True)
            subprocess.run(
                ["git", "commit", "-q", "-m", "Unrelated public root"], cwd=public, check=True
            )
            public_base = subprocess.check_output(
                ["git", "rev-parse", "HEAD"], cwd=public, text=True
            ).strip()
            subprocess.run(
                ["git", "update-ref", "refs/remotes/public/master", "HEAD"],
                cwd=public,
                check=True,
            )
            (public / "code.c").write_text("int example = 7;\n")
            subprocess.run(["git", "add", "-A"], cwd=public, check=True)
            subprocess.run(
                ["git", "commit", "-q", "-m", "Match example"], cwd=public, check=True
            )
            (public / "docs").mkdir()
            (public / "docs" / "release.md").write_text("ordinary public notes\n")
            subprocess.run(["git", "add", "-A"], cwd=public, check=True)
            subprocess.run(
                ["git", "commit", "-q", "-m", "Document example"], cwd=public, check=True
            )
            ctx = pr._release_context(public, "master", "public", require_clean=True)
            units, paths = pr._validate_export_units(ctx, manifest, approval)
            self.assertEqual((units, paths), (2, 2))
            self.assertNotEqual(source_base, public_base)
            self.assertNotEqual(
                manifest["units"][0]["private_commit"], ctx.outgoing_commits[0]
            )


if __name__ == "__main__":
    unittest.main()
