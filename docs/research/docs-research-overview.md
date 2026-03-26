---
document_id: RESEARCH-INDEX
title: "Research Repository"
version: "0.1"
status: active
---

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


# Research Repository

Central place for all links, references, and working notes used during project research.

## Folder Structure

- `docs/research/link-log.md` - master register of all external references
- `docs/research/standards/` - standards notes and excerpts metadata
- `docs/research/articles/` - papers, blog/article summaries, vendor docs notes
- `docs/research/notes/` - internal research notes and comparisons
- `docs/research/archives/` - snapshots/exported references (if permitted)

## Rules

- Every new external source must be logged in `docs/research/link-log.md`.
- Each entry must include relevance and where it is used in the project.
- If a source influences a requirement or design decision, add the decision/reference link in the same entry.
- Do not store copyrighted full texts unless license/usage allows it.

## Minimum Entry Criteria

- URL
- Title
- Source type
- Date accessed
- Owner
- Relevance summary
- Mapped artifact(s) or document(s)

