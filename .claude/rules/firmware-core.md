---
paths:
  - "firmware/src/**/*"
  - "firmware/include/**/*"
---
# Logging

FATAL/ERROR/WARN always. DEBUG/TRACE stripped from release. `[LEVEL][timestamp][module] msg`. Never log secrets/PII. Rate-limit.
Must log: boot, auth, connections, OTA, input rejections, watchdog, tamper. Crash data → reserved memory → report next connect.

# Networking

**TLS mandatory** on all comms. TLS 1.2+. Verify certs. Pin keys for known servers.
**MQTT**: port 8883, per-device auth, QoS 1/2 critical, LWT, validate payloads, backoff reconnect.
**BLE**: secure connections+bonding, validate writes, MTU negotiate, rate-limit, RPA.
**HTTP**: HTTPS only, auth all endpoints, rate-limit. **Wi-Fi**: WPA2+, encrypted cred storage.
**Resilience**: heartbeat, exponential backoff+jitter, queue during disconnect, circuit breaker.

# Power Management

States: ACTIVE→IDLE→LIGHT_SLEEP→DEEP_SLEEP→HIBERNATE. Document current per state in `docs/power-budget.md`.
Orderly transition: save→close connections→flush→disable peripherals→sleep. Reverse on wake. Disable unused peripherals. Event-driven wake over polling.
