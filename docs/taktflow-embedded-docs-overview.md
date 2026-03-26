## Human-in-the-Loop (HITL) Comment Lock

`HITL` means human-reviewer-owned comment content.

**Marker standard (code-friendly):**
- Markdown: `<!-- HITL-LOCK START:<id> -->` ... `<!-- HITL-LOCK END:<id> -->`
- C/C++/Java/JS/TS: `// HITL-LOCK START:<id>` ... `// HITL-LOCK END:<id>`
- Python/Shell/YAML/TOML: `# HITL-LOCK START:<id>` ... `# HITL-LOCK END:<id>`

**Rules:**
- AI must never edit, reformat, move, or delete text inside any `HITL-LOCK` block.
- Append-only: AI may add new comments/changes only; prior HITL comments stay unchanged.
- If a locked comment needs revision, add a new note outside the lock or ask the human reviewer to unlock it.

# Docs Structure

Top-level documentation map for the project.

## Main Index

- `docs/INDEX.md` - master registry and process coverage

## Content Areas

- `docs/aspice/` - ASPICE work products by process area
- `docs/safety/` - ISO 26262 safety lifecycle artifacts
- `docs/plans/` - strategic master plan and pointers
- `docs/research/` - research links, standards notes, references
- `docs/reference/` - playbooks and lessons learned
- `docs/deliverable-artifact/` - ALM artifact catalog and creation guidance

## Quick Start

1. Start at `docs/INDEX.md`.
2. Use `docs/aspice/plans/apps-web-overview.md` for execution plans.
3. Use `docs/research/link-log.md` for source traceability.


