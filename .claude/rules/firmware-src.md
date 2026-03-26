---
paths:
  - "firmware/src/**/*.c"
  - "firmware/src/**/*.cpp"
  - "firmware/src/**/*.h"
---
# Error Handling

**Fail-closed**: every error → safe, defined state. Never silently continue.
**Returns**: every fallible function returns `error_t` enum. Check EVERY return value. Propagate up, log at detection point. Never swallow errors or leave empty handlers.
**Safe states**: motor→STOP, network→disconnect+backoff, sensor→last-good+stale, OTA→abort+keep current, auth→deny.
**Watchdog**: feed ONLY from main loop. N consecutive resets → safe mode.
**Assert**: debug-only (`assert`). Runtime checks for real conditions — never stripped from production.

# Input Validation

Validate ALL data at system boundaries BEFORE processing. Never trust external input.
**Boundaries**: UART, MQTT, BLE, HTTP, GPIO/ADC, Flash/EEPROM, OTA — all require length/range/format/schema validation.
**Order**: size check → encoding → parse structure → schema → field values → THEN process.
**Strings**: check length before copy, null-terminate, reject control chars. **Numbers**: range check, overflow check, `strtol` not `atoi`. **Structured**: schema-first, max nesting depth, max message size. **Commands**: whitelist, validate args, rate-limit.
**Never**: parse then validate, log raw input, use input in format strings, assume encoding.

# State Machines

Use for any process with 3+ states (lifecycle, protocols, sensors, user flows).
**Rules**: all states/events as enums. Explicit transition table (preferred over nested switch). Log every transition. Reject invalid transitions. Timeout every waiting state. Every error state has a recovery path.
One machine per concern. Keep state data separate from logic. `STATE_COUNT`/`EVENT_COUNT` as last enum value.
