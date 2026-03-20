# Plan: Fix UDS Routing in Codegen Pipeline

## Context

ThreadX + full FZC BSW runs (Steps 1-9 PASS). CAN heartbeat works. But UDS TesterPresent on 0x7E1 gets no response on 0x7E9 because the codegen pipeline doesn't generate CanTp/Dcm routing.

## Problem (top-down)

```
0x7E1 → CanIf ✅ → PduR ❌(routes to Com, not CanTp) → CanTp ❌(not configured) → Dcm ❌(TxPduId=0xFF) → no response
```

## Root Cause

The `arxmlgen` codegen:
- Only generates `PDUR_DEST_COM` routing (hardcoded in template)
- Has no CanTp config template
- Has no Dcm TxPduId generation
- Sidecar YAML has no UDS/diagnostic metadata

## Fix Steps (each step must pass regression)

### Step 1: Add UDS metadata to sidecar YAML
- File: `model/ecu_sidecar.yaml`
- Add per-ECU: `uds_rx_pdu_name`, `uds_tx_pdu_name`, `uds_rx_can_id`, `uds_tx_can_id`
- Verify: `python -m tools.arxmlgen --config project.yaml --dry-run` still works

### Step 2: Load UDS metadata in arxmlgen Python model
- File: `tools/arxmlgen/__init__.py` or model loader
- Read `uds_rx_pdu_name` and `uds_tx_pdu_name` from sidecar into ECU model
- Verify: `--verbose` output shows UDS PDU names for each ECU

### Step 3: Fix PduR template for CanTp routing
- File: `tools/arxmlgen/templates/pdur/PduR_Cfg.c.j2`
- If `pdu.name == ecu.uds_rx_pdu_name` → `PDUR_DEST_CANTP`
- Regenerate: `python -m tools.arxmlgen --config project.yaml --ecu fzc --generator pdur`
- Verify: `PduR_Cfg_Fzc.c` has `PDUR_DEST_CANTP` for 0x7E1
- Regression: bare-metal FZC builds and CAN frames still appear

### Step 4: Add CanTp config template
- Create: `tools/arxmlgen/templates/cantp/CanTp_Cfg.c.j2`
- Generate: `CanTp_Cfg_Fzc.c` with RX/TX PDU IDs from sidecar
- Config: rxPduId = FZC_COM_RX_UDS_PHYS_REQ_FZC, txPduId = FZC_COM_TX_UDS_RESP_FZC
- Register generator in arxmlgen
- Verify: file generated, compiles

### Step 5: Fix Dcm TxPduId
- File: `firmware/ecu/fzc/cfg/Dcm_Cfg_Fzc.c` (hand-written)
- Change: `.TxPduId = FZC_COM_TX_UDS_RESP_FZC`
- Or: make TxPduId a sidecar-driven value with a Dcm template
- Verify: Dcm can construct response with correct PDU ID

### Step 6: Wire CanTp init in experiment
- File: `experiments/threadx-can/Src/app_threadx.c`
- Add: `CanTp_Init(&fzc_cantp_config)` with generated config
- Verify: builds

### Step 7: Build and flash ThreadX experiment
- `make clean && make && make flash`
- Verify: no HardFault, heartbeat still on 0x011

### Step 8: Test UDS — 10 TesterPresent
- Send 10× `cansend can0 7E1#023E000000000000`
- Expect 10× `7E9#027E00CCCCCCCCCC`
- Each request must get a response

### Step 9: Test other UDS services
- DiagSessionControl (0x10 01) → positive response
- ReadDID F190 (ECU ID) → "FZC1"
- ReadDID F195 (SW version) → "0.7.0"

### Step 10: Regenerate ALL ECUs
- `python -m tools.arxmlgen --config project.yaml`
- Verify: CVC, RZC, BCM, ICU, TCU all get correct PduR routing
- Regression: bare-metal builds for all ECUs pass

## Verification after each step

```bash
# Bare-metal regression
make -f firmware/platform/stm32/Makefile.stm32 TARGET=fzc clean
make -f firmware/platform/stm32/Makefile.stm32 TARGET=fzc

# ThreadX experiment
cd experiments/threadx-can && make clean && make

# Flash and CAN check
make flash
candump can0 -t A  # heartbeat on 0x011

# UDS test (Step 8)
for i in $(seq 1 10); do cansend can0 7E1#023E000000000000; sleep 1; done
# Expect 10 responses on 0x7E9
```

## Status: NOT STARTED
