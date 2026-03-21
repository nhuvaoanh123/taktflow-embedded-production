# HMI Fault Notification Specification

**Scope:** How CAN communication faults are presented to the operator via ICU dashboard.

## Data Sources

1. **Vehicle_State (0x100):** `FaultMask` byte — bit field of active faults
2. **DTC_Broadcast (0x500):** Individual DTC codes with severity
3. **Signal Quality API:** `Com_GetRxPduQuality(PduId)` — per-PDU freshness

## Fault Mask Display

| Bit | Fault | Icon | Color | Action |
|-----|-------|------|-------|--------|
| 0 | E-Stop Active | STOP | Red | Immediate stop, contact service |
| 1 | SC Relay Kill | POWER | Red | Power cut, restart required |
| 2 | Motor Cutoff | MOTOR | Amber | Motor disabled, check FZC DTCs |
| 3 | Brake Fault | BRAKE | Red | Braking compromised, stop safely |
| 4 | Steering Fault | STEER | Red | Steering compromised, stop safely |
| 5 | Pedal Fault | PEDAL | Amber | Throttle limited, limp mode |
| 6 | FZC CAN Timeout | COMM | Amber | Front zone offline |
| 7 | RZC CAN Timeout | COMM | Amber | Rear zone offline |

## Vehicle State Display

| State | Display | Color | Indicator |
|-------|---------|-------|-----------|
| INIT (0) | "STARTING" | Blue | Pulsing |
| RUN (1) | "READY" | Green | Steady |
| DEGRADED (2) | "DEGRADED" | Amber | Steady + fault icon |
| LIMP (3) | "LIMP MODE" | Amber | Flashing |
| SAFE_STOP (4) | "STOPPING" | Red | Flashing |
| SHUTDOWN (5) | "SHUTDOWN" | Red | Steady |

## DTC Display

- DTCs received on 0x500 displayed in scrollable list
- Format: `ECU-XXXX` (e.g., `FZC-E201 Brake PWM Mismatch`)
- Severity color: Red (safety), Amber (warning), White (info)
- Occurrence count shown

## Signal Staleness Indicator

ICU SWC queries `Com_GetRxPduQuality()` for each received PDU:
- **FRESH:** Normal display, white/green values
- **E2E_FAIL:** Value shown dimmed with "?" suffix (e.g., "42 RPM?")
- **TIMED_OUT:** Value replaced with "---" and grayed out

## Update Rates

- Vehicle state icon: 100ms refresh (matches 0x100 cycle)
- Fault mask: immediate on reception
- DTC list: 1s refresh
- Signal quality indicators: 200ms refresh
