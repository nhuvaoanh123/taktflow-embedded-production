# Lessons Learned — Listener-First Pattern for Protocol Testing

**Project:** Taktflow Embedded — Zonal Vehicle Platform
**Period:** 2026-04-01
**Scope:** Missed CAN/UART/UDS responses due to send-before-listen race conditions
**Result:** 3 scripts fixed, cross-project rule added

---

## 1. Root Cause: Send-Before-Listen Race Condition

Multiple test and debug scripts sent requests/stimuli before the listener was ready to receive responses. Since protocol responses (CAN heartbeats, UDS positive responses, UART boot messages) arrive within milliseconds, any delay between send and listen results in dropped frames.

## 2. Affected Scripts

| Script | Pattern | Impact |
|--------|---------|--------|
| `scripts/debug/reset_4ecus.py` | Reset 4 ECUs sequentially, then open CAN listener | Early heartbeats from fast-booting ECUs missed |
| `test/integration/layer4/test_cvc_single_ecu.py` | Start CVC process, sleep 1.5s, then read bus | Startup frames dropped during sleep window |
| `test/hil/test_hil_uds.py` Hop 15 | Raw `uds_send()` + `uds_recv()` without flush | Stale frames from prior hops confused response |

## 3. Fixes Applied

### reset_4ecus.py — Background listener thread
Started CAN capture in a background thread **before** sending reset pulses:
```python
listener = threading.Thread(target=_capture_thread, daemon=True)
listener.start()
time.sleep(0.3)  # Let serial port open
for ecu in ecus:
    pulse_uart_reset(ecu)
listener.join()
```

### test_cvc_single_ecu.py — Flush after startup sleep
Bus was already opened before subprocess (correct), but stale startup frames accumulated during sleep. Added flush:
```python
time.sleep(1.0)
while bus.recv(timeout=0) is not None:
    pass  # Flush startup frames
```

### test_hil_uds.py Hop 15 — Use uds_request() wrapper
Replaced raw `uds_send()` + `uds_recv()` with `uds_request()`, which includes a stale-frame flush before every send.

## 4. Rules

- Always start the listener/receiver BEFORE sending the request/stimulus
- Always flush stale frames from the bus before sending a new request
- Use wrapper functions (`uds_request()`) that enforce the flush, never raw send+recv
- For reset-and-verify workflows, open the capture channel in a background thread before the first reset
- Test each fix individually with a single-hop before running the full suite

## 5. Cross-Project Rule

Added `.claude/rules/protocol-testing.md` with the listener-first pattern, correct patterns, and anti-patterns to prevent recurrence across all projects.
