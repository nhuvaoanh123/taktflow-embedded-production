---
paths:
  - "firmware/src/**/*"
  - "scripts/**/*"
---
# Device Provisioning

Unique HW-derived identity per device. Per-device creds in encrypted storage, never shared. Support remote rotation.

Config separate from firmware (survives updates), schema with defaults, validate on load.

Decommission: secure wipe all creds/keys/data, triggerable local+remote.
