# Process Playbook — Taktflow Embedded Production

## Planning Discipline
Plan → approve → implement. Update plan before next phase. Ask before coding.

## Phase-Based Execution
Numbered phases with DONE criteria. Status table: PENDING → IN PROGRESS → DONE.

## Security-First Mindset
Fail-closed. Generic errors. Validate at boundaries. Graceful degradation. No hardcoded secrets.

## DBC-First Workflow
1. Edit `gateway/taktflow.dbc` (add/modify CAN messages and signals)
2. Run `tools/arxml/dbc2arxml.py` to regenerate ARXML
3. Run `tools/arxml/codegen.py` to regenerate ECU configs
4. Build and test — configs are always consistent with DBC

## Vendor Independence
Prefer generic protocols over vendor SDKs. Wrap unavoidable SDKs in abstraction.

## TODO Markers
Greppable: `TODO:SCALE`, `TODO:POST-BETA`, `TODO:HARDWARE`, `TODO:SECURITY`, `TODO:ISO`, `TODO:TEST`.
