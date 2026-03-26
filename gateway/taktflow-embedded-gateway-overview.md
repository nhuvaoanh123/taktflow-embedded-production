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

## Lessons Learned Rule

Every gateway topic in this document that undergoes HITL review discussion MUST have its own lessons-learned file in [`gateway/lessons-learned/`](lessons-learned/). One file per gateway topic. File naming: `GW-<topic>.md`.

# Gateway — Raspberry Pi Edge Gateway

Python services: CAN listener, cloud publisher, ML inference, fault injector, SAP QM mock.

Phase 11 deliverable.

<!-- HITL-LOCK START:COMMENT-BLOCK-GW-SEC1 -->
**HITL Review (An Dao) — Reviewed: 2026-02-27:** The gateway README is extremely sparse — it lists the five Python services (CAN listener, cloud publisher, ML inference, fault injector, SAP QM mock) but provides no architecture diagram, service interaction description, interface contracts, or deployment instructions. For a Phase 11 deliverable, this needs significant expansion: at minimum, a service architecture overview, a table of entry points/ports/protocols per service, configuration requirements, and references to the gateway test and model directories. The existing `gateway/` folder has `sap_qm_mock/`, `tests/`, and `models/` subdirectories but none are linked or described here.
<!-- HITL-LOCK END:COMMENT-BLOCK-GW-SEC1 -->

