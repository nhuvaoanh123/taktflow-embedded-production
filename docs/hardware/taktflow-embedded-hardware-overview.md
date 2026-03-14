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

Every hardware topic in this document that undergoes HITL review discussion MUST have its own lessons-learned file in [`hardware/lessons-learned/`](lessons-learned/). One file per hardware topic. File naming: `HW-<topic>.md`.

# Hardware Folder Structure

## Files

- `hardware/bom.md` — consolidated BOM, procurement tracker, and validation chronicle (merged from bom.md + bom-list.md + procurement-validation.md)
- `hardware/pin-mapping.md` — board and signal pin mappings for all 4 physical ECUs

<!-- HITL-LOCK START:COMMENT-BLOCK-HW-README-SEC1 -->
**HITL Review (An Dao) — Reviewed: 2026-02-27:** The file listing is clear and complete for the current project state. Each file has a distinct purpose (quick-buy, detailed BOM, pin mapping). Consider adding a reference to the hardware design document (HWDES) and hardware safety requirements (HSR) when those are created, as they are referenced in pin-mapping.md but not yet listed here.
<!-- HITL-LOCK END:COMMENT-BLOCK-HW-README-SEC1 -->

## Subfolders

- `hardware/schematics/` - schematics and related notes

<!-- HITL-LOCK START:COMMENT-BLOCK-HW-README-SEC2 -->
**HITL Review (An Dao) — Reviewed: 2026-02-27:** The schematics subfolder is referenced but its contents are not described. As the project progresses, consider documenting expected schematic files (e.g., per-ECU schematics, power distribution schematic, CAN bus wiring diagram) to set expectations for the folder's structure.
<!-- HITL-LOCK END:COMMENT-BLOCK-HW-README-SEC2 -->

## Recommended Workflow

1. Buy against `hardware/bom.md` (sections 5 and 7 have procurement priority and status tracker).
2. Track substitutions in `hardware/bom.md` section 8 (procurement validation).
3. Update `hardware/pin-mapping.md` if component variants change.

<!-- HITL-LOCK START:COMMENT-BLOCK-HW-README-SEC3 -->
**HITL Review (An Dao) — Reviewed: 2026-02-27:** The 3-step workflow is practical and clear. Step 2 (tracking substitutions) is important for configuration management per ASPICE SUP.8. Consider adding a step for verifying received components against the BOM (incoming inspection), which is relevant for ISO 26262 Part 7 (production and operation). The workflow correctly prioritizes bom-list.md for procurement and bom.md for tracking.
<!-- HITL-LOCK END:COMMENT-BLOCK-HW-README-SEC3 -->

