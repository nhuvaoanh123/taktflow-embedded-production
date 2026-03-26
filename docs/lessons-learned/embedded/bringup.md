# Embedded / HIL Bring-up — Lessons Learned

## 2026-03-25 — HIL test runner static analysis pass

### Com TX cycle rate bug
**Context**: CVC heartbeat (0x010) and Vehicle_State (0x100) were both firing at 10ms
instead of their configured periods (50ms/100ms). Bus load was 29% (target <8%).
**Mistake**: `Com_MainFunction_Tx` had no per-PDU cycle counter. All PDUs with
`com_tx_pending == TRUE` fired every 10ms regardless of `CycleTimeMs`.
**Fix**: Added `static uint16 com_tx_cycle_cnt[COM_MAX_PDUS]`. Periodic PDUs gate
on `(cnt * 10ms) >= CycleTimeMs`. Event-triggered PDUs (CycleTimeMs==0) still fire
on `com_tx_pending`.
**Principle**: Test COM cycle rates with a CAN sniffer before declaring a build
working. A PDU broadcasting 10x too fast is not immediately obvious from logs.

### SC heartbeat confirmation latch (HIL vs production)
**Context**: SC reported CVC=TIMEOUT even though CVC was alive and beating.
**Mistake**: `SC_HB_TIMEOUT_TICKS = 10` (100ms) was calibrated for production
(coordinated power-on). On the HIL bench, individual flash-reset cycles mean the
SCM is already monitoring when an ECU restarts. A single missed heartbeat during
reflash caused `hb_confirmed` to latch permanently until power cycle.
**Fix**: Created `Sc_Cfg_Platform.h` with `PLATFORM_HIL` overrides: 500ms timeout,
100ms confirm window, 10s startup grace.
**Principle**: Safety controller timing constants must be tuned per deployment
environment. Always have a platform config override mechanism — don't hardcode.

### E2E CRC polynomial mismatch in HIL runner
**Context**: `can_helpers.py` used `_CRC8_POLY = 0x07` (CRC-8/ATM) and `init=0x00`,
`XOR-out=0x00`. Firmware uses SAE-J1850: poly=0x1D, init=0xFF, XOR-out=0xFF.
All E2E verdicts (HIL-040/041/042) would have permanently failed.
**Fix**: `_CRC8_POLY = 0x1D`, `crc8()` uses init=0xFF, XOR-out=0xFF.
**Principle**: When porting CRC from C to Python, verify ALL parameters: poly, init,
XOR-out, reflection. The polynomial alone is not sufficient to define the algorithm.

### DTC broadcast frame parser: wrong byte layout
**Context**: Runner's `_verdict_dtc_broadcast` read `code = msg.data[0] | (msg.data[1] << 8)`
(16-bit little-endian) and `source = msg.data[2]`. Firmware packs a 24-bit big-endian
DTC at bytes 0-2 and ECU source ID at byte 4.
**Fix**: `code = (msg.data[0] << 16) | (msg.data[1] << 8) | msg.data[2]`,
`source = msg.data[4]`.
**Principle**: Always read the C source frame packing before writing a Python parser.
`code = data[0] | data[1] << 8` is little-endian uint16 — not a 24-bit DTC.

### YAML key mismatches silently ignored (signal min, expected_state, timeout_ms)
**Context**: Several YAML scenario files used `min:` instead of `min_value:`,
`expected_state:` instead of `expected:`, `timeout_ms:` instead of `within_ms:`.
Python `dict.get("min_value", 0)` returns 0 when key is absent — test passes vacuously.
**Fix**: Made runner accept both forms as aliases. Also fixed YAML files to use
canonical keys. `hil_032` had wrong DTC code (0xE202 vs 0xD101 from firmware).
**Principle**: YAML-driven test runners must validate required keys are present and
warn loudly when unknown keys are found. Silent fallback to defaults masks authoring
errors indefinitely.

### MQTT payload double-encoding
**Context**: YAML authors wrote `payload: '{"fault":"overcurrent","current_ma":28000}'`
(a JSON string). Runner passed this string to `inject_mqtt_fault` which called
`json.dumps(payload)` — encoding the string again: `'"{\\"fault\\":\\"overcurrent\\"}"'`.
Plant-sim received a JSON string, not a JSON object, and silently ignored the fault.
**Fix**: Runner parses string payloads via `json.loads()` before passing to
`inject_mqtt_fault`.
**Principle**: When a YAML value is intended to be structured data, use YAML dict
syntax, not a JSON string. When accepting either, validate and normalize early.

### MQTT API key mismatch: "fault" vs "type"
**Context**: All 5 fault injection YAMLs sent `{"fault":"overcurrent"}` etc. Plant-sim
`simulator.py` reads `cmd.get("type","")` — key "fault" is silently ignored. Every
fault injection test would have sent the command and received no response. Also:
`reset_mqtt_faults()` in `can_helpers.py` sent `{"fault":"reset"}` — resets never executed.
Additionally, hil_033 used `voltage_mv`/`soc_pct` field names; plant-sim reads `mV`/`soc`.
And hil_034 used type `"overtemp"` instead of the correct handler name `"inject_temp"`.
**Fix**: All payloads aligned to `"type"` key. Field names verified against
`gateway/fault_inject/plant_inject.py` (authoritative API surface).
**Principle**: Before writing test YAML files, read the fault injection handler source —
not just the topic name. The canonical field names are in the handler, not in any doc.
`plant_inject.py` is the single source of truth for plant-sim MQTT API shape.

### DTC code mismatch: firmware DEM path vs. plant-sim direct generation
**Context**: hil_032 was "fixed" to use `dtc_code: 0xD101` (FZC DEM `FZC_DTC_BRAKE_FAULT`)
based on reading `firmware/fzc/src/main.c`. But plant-sim generates `DTC_BRAKE_FAULT =
0x00E202` directly in `_send_dtc()`, bypassing the FZC DEM path (which is not yet wired
for brake fault in firmware). So the correct expected DTC on the bus is 0xE202.
**Fix**: Reverted hil_032 to `dtc_code: 0xE202`.
**Principle**: For fault injection DTCs, trace both paths: firmware DEM and plant-sim
direct generation. Where the FZC/RZC DEM path is not yet wired, plant-sim generates
the DTC itself. Check `simulator.py` `_check_and_send_dtcs()` comments.
