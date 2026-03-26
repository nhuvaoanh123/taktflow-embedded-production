---
document_id: RESEARCH-LINK-LOG
title: "Research Link Log"
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


# Research Link Log

Use this as the single source of truth for all research references.

## Source Register

| ID | Title | URL | Type | Accessed (YYYY-MM-DD) | Owner | Relevance | Used In |
|----|-------|-----|------|------------------------|-------|-----------|---------|
| SRC-001 | Automotive SPICE | https://vda-qmc.de/en/automotive-spice/ | Standard Portal | 2026-02-21 | TBD | ASPICE process/work product framing | docs/deliverable-artifact/apps-web-overview.md |
| SRC-002 | Automotive SPICE PAM 4.0 | https://www.vda-qmc.de/fileadmin/redakteur/Publikationen/Automotive_SPICE/AutomotiveSPICE_PAM_4.0_ENG.pdf | Standard PDF | 2026-02-21 | TBD | Process outcomes and base practices reference | docs/deliverable-artifact/apps-web-overview.md |
| SRC-003 | ISO 26262 Overview | https://www.iso.org/publication/PUB200262.html | Standard Overview | 2026-02-21 | TBD | Functional safety lifecycle framing | docs/deliverable-artifact/apps-web-overview.md |
| SRC-004 | ISO/SAE 21434 | https://www.iso.org/standard/70918.html | Standard Page | 2026-02-21 | TBD | Cybersecurity engineering framework | docs/deliverable-artifact/apps-web-overview.md |
| SRC-005 | UNECE WP.29 Addenda | https://unece.org/transport/vehicle-regulations-wp29/standards/addenda-1958-agreement-regulations-141-160 | Regulation Portal | 2026-02-21 | TBD | R155/R156 regulation references | docs/deliverable-artifact/apps-web-overview.md |

## Decision Trace (Optional but Recommended)

| Decision ID | Decision Summary | Related Source IDs | Impacted Docs |
|-------------|------------------|--------------------|---------------|
| DEC-001 | Use ASPICE process-area structure under docs/aspice/plans | SRC-001, SRC-002 | docs/aspice/plans/apps-web-overview.md |


