#!/usr/bin/env python3
"""Compare prepared C context without authorizing declaration adoption.

Inputs must be the initial actual compiler input and a saved candidate, not
raw canonical C versus preprocessed C. Only the requested function body may
change. This is a conservative syntactic contract, not a C equivalence proof.
Reports and source inputs are private, ignored runtime evidence.
"""
from __future__ import annotations

import argparse
import difflib
import hashlib
import json
from pathlib import Path
import re
import sys

SCHEMA = "mickey-candidate-context-v1"
MAX_SOURCE_BYTES = 4 * 1024 * 1024
MAX_NODES = 100_000
MAX_DEPTH = 128
MAX_DECLARATIONS = 4096
MAX_CHANGES = 32
MAX_SNIPPET = 512
SYMBOL = re.compile(r"[A-Za-z_][A-Za-z0-9_]*\Z")
LOCATION_MACROS = {"__LINE__", "__FILE__", "__BASE_FILE__", "__FILE_NAME__",
                   "__COUNTER__", "__DATE__", "__TIME__", "__TIMESTAMP__",
                   "__INCLUDE_LEVEL__"}
PRAGMA_OPERATORS = {"_Pragma", "__pragma"}
# Literals precede comments, so comment-looking text inside a string survives.
LEXICAL = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*', re.S)


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def identity() -> dict:
    """Fingerprint the comparator and actual parser, without import-time I/O."""
    result = {"schema": SCHEMA, "comparator_sha256": _sha(Path(__file__).read_bytes())}
    try:
        import pycparser
        package = Path(pycparser.__file__).parent
        files = [(p.relative_to(package).as_posix(), _sha(p.read_bytes()))
                 for p in sorted(package.rglob("*.py"))]
        result.update(parser="pycparser", parser_version=pycparser.__version__,
                      parser_sha256=_sha(json.dumps(files, separators=(",", ":")).encode()))
    except (ImportError, OSError, AttributeError) as error:
        result.update(parser="unavailable", error=type(error).__name__)
    return result


class ContextError(ValueError):
    pass


def _strip_comments(text: str) -> str:
    def replace(match: re.Match) -> str:
        token = match.group()
        if token.startswith(("/*", "//")):
            return " " + "\n" * token.count("\n")
        return token
    return LEXICAL.sub(replace, text)


def _surface(source: bytes, symbol: str) -> tuple[list[dict], list[str]]:
    if len(source) > MAX_SOURCE_BYTES:
        raise ContextError("prepared source exceeds comparison byte limit")
    import pycparser
    from pycparser import c_ast, c_generator
    text = source.decode("utf-8")
    # Translation phase 1/2 precedes comment recognition. Trigraph behavior
    # depends on compiler mode; refuse it rather than apply the wrong dialect.
    if re.search(r"\?\?[=/'()!<>-]", text):
        raise ContextError("trigraph-bearing prepared input requires preprocessing")
    text = text.replace("\r\n", "\n").replace("\r", "\n")
    text = re.sub(r"\\\n", "", text)
    text = _strip_comments(text)
    without_literals = LEXICAL.sub(lambda match: " " if match.group().startswith(('"', "'"))
                                  else match.group(), text)
    identifiers = set(re.findall(r"[A-Za-z_][A-Za-z0-9_]*", without_literals))
    if LOCATION_MACROS.intersection(identifiers):
        raise ContextError("location/time-dependent macros must be expanded in prepared input")
    if PRAGMA_OPERATORS.intersection(identifiers):
        raise ContextError("pragma operators require independent context review")
    # A fresh parser per input also prevents typedef state leaking between inputs.
    ast = pycparser.CParser().parse(text, filename="<prepared>")
    if len(ast.ext) > MAX_DECLARATIONS:
        raise ContextError("prepared context exceeds declaration limit")
    targets = [node for node in ast.ext
               if isinstance(node, c_ast.FuncDef) and node.decl.name == symbol]
    if len(targets) != 1:
        raise ContextError("prepared source must define the requested function exactly once")
    target = targets[0]
    pending = [target.body]
    body_nodes = 0
    while pending:
        node = pending.pop()
        body_nodes += 1
        if body_nodes > MAX_NODES:
            raise ContextError("prepared body exceeds comparison structure limit")
        if isinstance(node, c_ast.Pragma):
            raise ContextError("target-body pragma cannot safely be excluded from context")
        pending.extend(child for _name, child in node.children())
    # Exclude only the body: signature and old-style parameter declarations
    # remain compiler input, even though the body is allowed to vary.
    target.body = c_ast.Compound(block_items=[])
    count = body_nodes

    def normalize(node, depth=0):
        nonlocal count
        count += 1
        if depth > MAX_DEPTH or count > MAX_NODES:
            raise ContextError("prepared context exceeds comparison structure limit")
        return {"kind": type(node).__name__,
                "attributes": {name: getattr(node, name) for name in node.attr_names},
                "children": [(name, normalize(child, depth + 1))
                             for name, child in node.children()]}

    generator = c_generator.CGenerator()
    rows, snippets = [], []
    for node in ast.ext:
        name = getattr(node, "name", None)
        if isinstance(node, c_ast.FuncDef):
            name = node.decl.name
        normalized = normalize(node)
        rows.append({"kind": type(node).__name__, "name": name,
                     "sha256": _sha(json.dumps(normalized, sort_keys=True,
                                               separators=(",", ":")).encode())})
        snippets.append(generator.visit(node)[:MAX_SNIPPET])
    return rows, snippets


def compare_context(baseline: bytes, winner: bytes, symbol: str) -> dict:
    """Return unchanged/changed/unverifiable; never silently assume equivalence.

    AST equality deliberately preserves declaration order, parameter names,
    qualifiers, pragmas and all non-target helper bodies. Conservative review
    flags are preferable to adopting an unproved context change. Coordinates
    (including preprocessor line markers), comments and formatting are ignored.
    """
    report = {"schema": SCHEMA, "status": "unverifiable", "symbol": symbol,
              "baseline_sha256": _sha(baseline), "winner_sha256": _sha(winner),
              "baseline_context_sha256": None, "winner_context_sha256": None,
              "changes": [], "truncated": False, "reason": None}
    if not isinstance(symbol, str) or not SYMBOL.fullmatch(symbol):
        report["reason"] = "invalid requested function name"
        return report
    try:
        old, old_text = _surface(baseline, symbol)
        new, new_text = _surface(winner, symbol)
        for field, value in (("baseline_context_sha256", old), ("winner_context_sha256", new)):
            report[field] = _sha(json.dumps(value, sort_keys=True, separators=(",", ":")).encode())
        report["status"] = "unchanged" if old == new else "changed"
        if old == new:
            return report
        matcher = difflib.SequenceMatcher(a=[row["sha256"] for row in old],
                                         b=[row["sha256"] for row in new], autojunk=True)
        for operation, a, b, c, d in matcher.get_opcodes():
            if operation == "equal":
                continue
            for i in range(max(b - a, d - c)):
                if len(report["changes"]) >= MAX_CHANGES:
                    report["truncated"] = True
                    break
                before = {**old[a + i], "text": old_text[a + i]} if a + i < b else None
                after = {**new[c + i], "text": new_text[c + i]} if c + i < d else None
                report["changes"].append({"operation": operation, "before": before, "after": after})
        report["reason"] = "prepared context changed; independent ABI/source review required"
    except Exception as error:
        # Parser diagnostics can echo large source lines; keep the scalar
        # error bounded and leave original inputs in their evidence files.
        report["status"] = "unverifiable"
        report["reason"] = (f"{type(error).__name__}: {error}")[:512]
    return report


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("baseline", type=Path)
    parser.add_argument("winner", type=Path)
    parser.add_argument("--symbol", required=True)
    args = parser.parse_args(argv)
    from sweep_receipts import owned_bytes
    try:
        report = compare_context(
            owned_bytes(args.baseline.parent, args.baseline.name, limit=MAX_SOURCE_BYTES),
            owned_bytes(args.winner.parent, args.winner.name, limit=MAX_SOURCE_BYTES), args.symbol)
    except (OSError, ValueError) as error:
        parser.error(str(error))
    print(json.dumps(report, indent=2, sort_keys=True))
    return {"unchanged": 0, "changed": 1, "unverifiable": 2}[report["status"]]


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
