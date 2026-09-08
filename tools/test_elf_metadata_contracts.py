#!/usr/bin/env python3
"""Synthetic ELF tests for branch metadata and digest-guarded data removal."""
import hashlib
import os
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import unittest

from reloc_surface import Elf


TOOLS = Path(__file__).resolve().parent


def fixture(path, *, anchor=0, displacement=1, opcode=4, rodata_flags=2):
    names = ["", ".text", ".rodata", ".rel.text", ".symtab", ".strtab", ".shstrtab"]
    name_blob = b"\0"
    name_offsets = [0]
    for name in names[1:]:
        name_offsets.append(len(name_blob))
        name_blob += name.encode() + b"\0"
    strings = b"\0branch_site\0callee\0"
    symbols = bytes(16)
    symbols += struct.pack(">IIIBBH", 1, anchor, 0, 0, 0, 1)
    symbols += struct.pack(">IIIBBH", 13, 0, 0, 16, 0, 0)
    text = struct.pack(">III", (opcode << 26) | (1 << 21) | displacement, 3 << 26, 0)
    payloads = [b"", text, b"keepdrop", struct.pack(">II", 4, (2 << 8) | 4),
                symbols, strings, name_blob]
    data = bytearray(52)
    data[:16] = b"\x7fELF\x01\x02\x01" + bytes(9)
    struct.pack_into(">HHIIIIIHHHHHH", data, 16, 1, 8, 1, 0, 0, 0, 0, 52, 0, 0, 40, 7, 6)
    headers = []
    for index, payload in enumerate(payloads):
        while len(data) % 4:
            data.append(0)
        offset = len(data)
        data.extend(payload)
        kind = [0, 1, 1, 9, 2, 3, 3][index]
        flags = 6 if index == 1 else rodata_flags if index == 2 else 0
        link = 4 if index == 3 else 5 if index == 4 else 0
        info = 1 if index == 3 else 2 if index == 4 else 0
        entsize = 8 if index == 3 else 16 if index == 4 else 0
        headers.append((name_offsets[index], kind, flags, 0, offset, len(payload), link, info, 4, entsize))
    while len(data) % 4:
        data.append(0)
    struct.pack_into(">I", data, 32, len(data))
    for header in headers:
        data.extend(struct.pack(">10I", *header))
    path.write_bytes(data)


class MetadataContracts(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.path = Path(self.temp.name) / "synthetic.o"
        fixture(self.path)

    def run_tool(self, tool, *args):
        env = dict(os.environ)
        env.pop("PROMOTION_TRIAL", None)
        return subprocess.run([sys.executable, str(TOOLS / tool), str(self.path), *args],
                              env=env, capture_output=True, text=True)

    def add_branch(self, digest=None, spec="0:PC16:branch_site:1"):
        if digest is None:
            digest = hashlib.sha256(Elf(self.path).section_bytes(".text")).hexdigest()
        return self.run_tool("add_elf_relocations.py", ".text", "12", digest, spec)

    def test_branch_preserves_bytes_symbols_and_existing_relocation(self):
        before = Elf(self.path)
        result = self.add_branch()
        self.assertEqual(result.returncode, 0, result.stderr)
        after = Elf(self.path)
        self.assertEqual(before.symbols(), after.symbols())
        for name in (".text", ".rodata", ".strtab", ".shstrtab"):
            self.assertEqual(before.section_bytes(name), after.section_bytes(name))
        self.assertEqual(after.relocations(), [(".text", 0, 10, 1), *before.relocations()])
        # S == P means the PC16 link formula leaves the stored field intact.
        branch_site = after.symbols()[1][1]
        field = struct.unpack_from(">I", after.section_bytes(".text"))[0] & 0xFFFF
        self.assertEqual((branch_site + field * 4 - 0) // 4, field)

    def test_branch_rejects_wrong_anchor_opcode_destination_addend_or_hash(self):
        for kwargs, spec, digest in [
            ({"anchor": 4}, "0:PC16:branch_site:1", None),
            ({"opcode": 8}, "0:PC16:branch_site:1", None),
            ({"displacement": 8}, "0:PC16:branch_site:8", None),
            ({}, "0:PC16:branch_site:2", None),
            ({}, "0:PC16:branch_site:1", "0" * 64),
        ]:
            with self.subTest(kwargs=kwargs, spec=spec, digest=digest):
                fixture(self.path, **kwargs)
                before = self.path.read_bytes()
                self.assertNotEqual(self.add_branch(digest, spec).returncode, 0)
                self.assertEqual(self.path.read_bytes(), before)

    def test_branch_rejects_duplicate(self):
        self.assertEqual(self.add_branch().returncode, 0)
        before = self.path.read_bytes()
        self.assertNotEqual(self.add_branch().returncode, 0)
        self.assertEqual(self.path.read_bytes(), before)

    def test_digest_trim_keeps_prefix_and_instruction_bytes(self):
        before = Elf(self.path)
        digest = hashlib.sha256(b"drop").hexdigest()
        result = self.run_tool("trim_elf_section.py", ".rodata", "4", "sha256:" + digest)
        self.assertEqual(result.returncode, 0, result.stderr)
        after = Elf(self.path)
        self.assertEqual(after.section_bytes(".rodata"), b"keep")
        self.assertEqual(before.section_bytes(".text"), after.section_bytes(".text"))
        self.assertEqual(before.relocations(), after.relocations())

    def test_digest_trim_rejects_mismatch_malformed_and_executable_sections(self):
        for section, digest, flags in [
            (".rodata", "0" * 64, 2), (".rodata", "bad", 2),
            (".rodata", hashlib.sha256(b"drop").hexdigest(), 6),
            (".text", "0" * 64, 2),
        ]:
            with self.subTest(section=section, digest=digest, flags=flags):
                fixture(self.path, rodata_flags=flags)
                before = self.path.read_bytes()
                self.assertNotEqual(self.run_tool("trim_elf_section.py", section, "4", "sha256:" + digest).returncode, 0)
                self.assertEqual(self.path.read_bytes(), before)

    def test_default_trim_still_rejects_nonzero_data(self):
        before = self.path.read_bytes()
        self.assertNotEqual(self.run_tool("trim_elf_section.py", ".rodata", "4").returncode, 0)
        self.assertEqual(self.path.read_bytes(), before)


if __name__ == "__main__":
    unittest.main()
