# SWC Onboarding Layer

**Taktflow private IP** — never delivered to customers. They only see the generated output.

A Python code-generator that takes 3 customer inputs and auto-generates all AUTOSAR BSW wiring artifacts needed to integrate a customer ECU into Taktflow's SIL (Software-in-the-Loop) Docker framework.

**Before**: Manual wiring of Com_Cfg, Rte_Cfg, main.c, Makefile, Docker takes weeks per ECU.
**After**: Customer drops 3 files, runs one command, `docker compose up` in one afternoon.

---

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Quick Start](#quick-start)
3. [What You Need from the Customer](#what-you-need-from-the-customer)
4. [Manifest YAML Reference](#manifest-yaml-reference)
5. [Generated Output](#generated-output)
6. [CLI Reference](#cli-reference)
7. [Verification](#verification)
8. [Working with the Generated Code](#working-with-the-generated-code)
9. [Complete Example](#complete-example)
10. [Troubleshooting](#troubleshooting)
11. [Architecture](#architecture)

---

## Prerequisites

### Python

Python 3.10+ required. Install dependencies:

```bash
cd tools/onboard
pip install -r requirements.txt
```

Dependencies:
- `cantools>=39.0` — DBC file parsing
- `jinja2>=3.1` — Template rendering
- `pyyaml>=6.0` — YAML manifest loading
- `jsonschema>=4.20` — Manifest validation

### Docker (optional, for `--verify` and running the ECU)

- Docker 24+ with Compose v2
- Linux host with `vcan` kernel module (or WSL2 with vcan support)

### Taktflow BSW (required at build time)

The generated Makefile expects the Taktflow shared BSW at `shared/bsw/` relative to the output directory. Either:
- Generate output inside the `taktflow-embedded/firmware/` tree, OR
- Symlink `shared/` into your output directory

---

## Quick Start

### 1. Prepare customer files

Create a working directory with the customer's deliverables:

```
customer_abs/
├── Swc_AbsControl.c     # Customer SWC source
├── Swc_AbsControl.h     # Customer SWC header
├── abs_can.dbc           # Customer DBC file
└── abs.yaml              # Manifest (you write this)
```

### 2. Write the manifest

The manifest maps the customer's SWC to the Taktflow BSW. You write this — it's our schema, not theirs.

```yaml
ecu:
  name: abs
  can_baudrate: 500000

dbc_file: abs_can.dbc

swc_files:
  sources: [Swc_AbsControl.c]
  headers: [Swc_AbsControl.h]

rte_signals:
  - id: 16
    name: ABS_WHEEL_SPEED_FL
    type: UINT16

tx_messages:
  - dbc_message: ABS_BrakeCmd
    cycle_ms: 10
    signals:
      - dbc_signal: BrakeForce_FL
        rte_signal: ABS_BRAKE_CMD_FL

rx_messages:
  - dbc_message: WheelSpeed_FL_Msg
    timeout_ms: 100
    signals:
      - dbc_signal: WheelSpeed_FL
        rte_signal: ABS_WHEEL_SPEED_FL

runnables:
  - func: Swc_AbsControl_MainFunction
    period_ms: 10
    priority: 7
    init_func: Swc_AbsControl_Init
```

### 3. Generate

```bash
# From the taktflow-embedded root:
python -m tools.onboard generate \
    --manifest customer_abs/abs.yaml \
    --output ./generated/abs/ \
    --force
```

### 4. Copy BSW and customer files

```bash
# Symlink shared BSW into generated output
ln -s ../../shared ./generated/abs/shared

# Copy customer SWC into the generated ECU directory
cp customer_abs/Swc_AbsControl.c ./generated/abs/abs/src/
cp customer_abs/Swc_AbsControl.h ./generated/abs/abs/src/
```

### 5. Build and run

```bash
cd ./generated/abs/
docker compose -f docker-compose.customer.yml up --build
```

---

## What You Need from the Customer

| Deliverable | Format | What it is |
|---|---|---|
| **SWC source** | `.c` / `.h` files | Their proprietary application code. We never modify it. |
| **DBC file** | `.dbc` | Standard CAN matrix. Every Tier-1 has one. Defines messages, signals, bit layouts. |
| **Technical spec** | Any | Enough info for you to write the manifest: which signals they read/write, timing, CAN messages. |

**You write the manifest** — it's our schema that maps their SWC into our framework. The customer never sees it.

---

## Manifest YAML Reference

### Top-level structure

```yaml
ecu:            # Required — ECU identity
dbc_file:       # Required — path to DBC file (relative to manifest)
swc_files:      # Required — customer source files
rte_signals:    # Required — RTE signal definitions (IDs 16+)
tx_messages:    # Required — CAN messages this ECU transmits
rx_messages:    # Required — CAN messages this ECU receives
runnables:      # Required — periodic functions to schedule
fault_scenarios: # Optional — fault injection scenarios
```

### `ecu` section

```yaml
ecu:
  name: abs              # Required. Lowercase, [a-z][a-z0-9_]{0,15}
  can_baudrate: 500000   # Optional. Default: 500000. Values: 125000, 250000, 500000, 1000000
```

The `name` drives all naming throughout the generated code:
- `name: abs` -> `ABS` (upper), `Abs` (pascal), `abs` (raw)
- Header: `Abs_Cfg.h`, main: `abs_main.c`, binary: `abs_posix`

### `dbc_file`

```yaml
dbc_file: customer_can/abs.dbc
```

Path to the DBC file, **relative to the manifest YAML file** (not the working directory).

### `swc_files` section

```yaml
swc_files:
  sources: [Swc_AbsControl.c, Swc_WheelSpeed.c]   # Required. At least 1.
  headers: [Swc_AbsControl.h, Swc_WheelSpeed.h]    # Optional. Default: []
```

These are the customer's source files. The generator references them in the Makefile. You must manually copy them into the generated `<ecu>/src/` directory.

### `rte_signals` section

```yaml
rte_signals:
  - id: 16                      # Required. Must be >= 16 (0-15 reserved for BSW)
    name: ABS_WHEEL_SPEED_FL    # Required. UPPER_SNAKE_CASE, pattern: ^[A-Z][A-Z0-9_]+$
    type: UINT16                # Required. One of: UINT8, UINT16, SINT16, UINT32, BOOL
    initial_value: 0            # Optional. Default: 0
```

**Signal ID rules:**
- IDs 0-15 are reserved for BSW well-known signals (torque, steering, speed, brake, etc.)
- Customer signals must start at ID 16
- IDs must be unique but don't need to be contiguous

**Signal types and their C mappings:**

| Manifest type | C type | Com type | Bit size |
|---|---|---|---|
| `UINT8` | `uint8` | `COM_UINT8` | 8 |
| `UINT16` | `uint16` | `COM_UINT16` | 16 |
| `SINT16` | `sint16` | `COM_SINT16` | 16 |
| `UINT32` | `uint32` | `COM_UINT32` | 32 |
| `BOOL` | `uint8` | `COM_UINT8` | 8 |

The generated signal macro name follows the pattern: `<ECU_UPPER>_SIG_<NAME>`.
Example: `id: 16, name: ABS_WHEEL_SPEED_FL` -> `#define ABS_SIG_ABS_WHEEL_SPEED_FL 16u`

### `tx_messages` section

```yaml
tx_messages:
  - dbc_message: ABS_BrakeCmd     # Required. Must match a message name in the DBC file.
    cycle_ms: 10                   # Required. TX cycle time in ms (1-10000).
    signals:                       # Required. At least 1 signal.
      - dbc_signal: BrakeForce_FL  # Required. Must match a signal in the DBC message.
        rte_signal: ABS_BRAKE_CMD_FL  # Required. Must match a name from rte_signals.
```

Each TX message becomes:
- A Com TX PDU with the CAN ID from the DBC
- Com signals mapped to the PDU, with bit position/size from the DBC
- A CanIf TX routing entry in `main.c`
- A `Com_SendSignal()` call in `TransmitSchedule`

### `rx_messages` section

```yaml
rx_messages:
  - dbc_message: WheelSpeed_FL_Msg  # Required. Must match a message name in the DBC file.
    timeout_ms: 100                  # Required. RX timeout in ms (10-60000).
    signals:                         # Required. At least 1 signal.
      - dbc_signal: WheelSpeed_FL    # Required. Must match a signal in the DBC message.
        rte_signal: ABS_WHEEL_SPEED_FL  # Required. Must match a name from rte_signals.
```

Each RX message becomes:
- A Com RX PDU with timeout monitoring
- Com signals mapped from the PDU
- A CanIf RX routing entry + PduR routing in `main.c`
- A `Com_ReceiveSignal()` call in `BridgeRxToRte`

### `runnables` section

```yaml
runnables:
  - func: Swc_AbsControl_MainFunction  # Required. C function name.
    period_ms: 10                        # Required. Execution period (1-10000 ms).
    priority: 7                          # Required. Higher = executes first (1-255).
    init_func: Swc_AbsControl_Init       # Optional. Called once at startup.
```

**Priority ordering in the generated runnable table:**

| Priority | Runnable | Purpose |
|---|---|---|
| 15 | `Can_MainFunction_Read` | CAN frame reception (always present) |
| 14 | `Com_MainFunction_Rx` | Com RX timeout monitoring (always present) |
| 13 | `Swc_<Ecu>Com_BridgeRxToRte` | Bridge CAN RX to RTE (always present) |
| 3-12 | *Customer runnables* | Your `runnables` entries go here |
| 2 | `Swc_<Ecu>Com_TransmitSchedule` | Bridge RTE to CAN TX (always present) |
| 1 | `Com_MainFunction_Tx` | Com TX scheduling (always present) |
| 0 | `Can_MainFunction_BusOff` | CAN bus-off recovery (always present) |

BSW runnables (priority 0, 1, 2, 13, 14, 15) are always generated. Customer runnables should use priorities 3-12.

### `fault_scenarios` section (optional)

```yaml
fault_scenarios:
  - name: wheel_speed_loss         # Required. snake_case, pattern: ^[a-z][a-z0-9_]+$
    type: can_timeout              # Required. One of: can_timeout, can_inject, container_stop
    target_message: WheelSpeed_FL_Msg  # Required. DBC message name to target.
```

**Scenario types:**

| Type | What it does |
|---|---|
| `can_timeout` | Simulates sender going silent — triggers Com RX timeout |
| `can_inject` | Sends a corrupted CAN frame (0xDEADBEEFDEADBEEF) |
| `container_stop` | Documents how to stop the ECU container |

Generated as a standalone Python script: `fault_inject/scenarios_<ecu>.py`

---

## Generated Output

Running the generator produces 11 files:

```
generated/<ecu>/
├── <ecu>/
│   ├── cfg/
│   │   ├── <Ecu>_Cfg.h                # Signal IDs, PDU IDs, SIG_COUNT
│   │   ├── Com_Cfg_<Ecu>.c            # Com shadow buffers, signal table, PDU tables
│   │   ├── Rte_Cfg_<Ecu>.c            # RTE signal init table, runnable scheduler
│   │   └── <ecu>_hw_posix.c           # POSIX hardware stubs (all return E_OK)
│   └── src/
│       ├── <ecu>_main.c               # BSW init + 10ms main loop
│       ├── Swc_<Ecu>Com.c             # Com<->RTE bridge
│       └── Swc_<Ecu>Com.h             # Bridge header
├── Makefile.customer                   # Standalone POSIX build
├── Dockerfile.customer                 # Multi-stage Docker build
├── docker-compose.customer.yml         # ECU + can-setup services
└── fault_inject/
    └── scenarios_<ecu>.py              # Fault injection script
```

### What each file does

#### `<Ecu>_Cfg.h` — Configuration header

Defines all signal and PDU IDs as `#define` macros. This is the single source of truth for IDs used across all other generated files and the customer's SWC.

```c
#define ABS_SIG_ABS_WHEEL_SPEED_FL    16u
#define ABS_SIG_ABS_WHEEL_SPEED_FR    17u
// ...
#define ABS_SIG_COUNT                  24u

#define ABS_COM_TX_ABS_BRAKECMD       0u  /* CAN 0x100 */
#define ABS_COM_RX_WHEELSPEED_FL_MSG  0u  /* CAN 0x200 */
```

#### `Com_Cfg_<Ecu>.c` — Com module configuration

Three-layer AUTOSAR Com configuration:
1. **Shadow buffers** — static RAM for each signal's last value
2. **Signal table** — maps signal ID to bit position, size, type, parent PDU, shadow buffer
3. **PDU tables** — TX PDUs with cycle time, RX PDUs with timeout

#### `Rte_Cfg_<Ecu>.c` — RTE configuration

Two tables:
1. **Signal init table** — BSW well-known signals (0-15, all zeroed) + customer signals (16+) with initial values
2. **Runnable table** — BSW runnables + customer runnables, sorted by priority (higher first)

#### `<ecu>_main.c` — Main entry point

BCM-pattern main function:
1. Register SIGINT/SIGTERM handlers for graceful Docker shutdown
2. BSW init in order: Can -> CanIf -> PduR -> Com -> Dem -> Rte
3. Generated Com bridge init
4. Customer SWC init functions
5. Start CAN controller
6. 10ms main loop: `Rte_MainFunction()` dispatches all runnables
7. Graceful shutdown on signal

Also includes static CanIf TX/RX routing tables and PduR routing — all derived from the manifest.

#### `Swc_<Ecu>Com.c/.h` — Com-RTE bridge

Two periodic functions generated from the manifest:
- **`BridgeRxToRte`** — For each RX signal: `Com_ReceiveSignal()` -> `Rte_Write()`
- **`TransmitSchedule`** — For each TX signal: `Rte_Read()` -> `Com_SendSignal()`

No E2E wrapping — E2E is contract-specific and added per engagement.

#### `<ecu>_hw_posix.c` — POSIX stubs

Hardware abstraction stubs for POSIX simulation. All functions return `E_OK`. Required by the BSW but irrelevant for SIL.

#### `Makefile.customer` — Build

Standalone Makefile. Compiles all BSW modules + generated code + customer SWC. Produces `build/<ecu>_posix`.

Key flags: `-Wall -Wextra -Werror -std=c99 -DPLATFORM_POSIX`

#### `Dockerfile.customer` — Docker image

Multi-stage build:
- **Build stage**: Ubuntu 22.04 + build-essential + can-utils, runs `make -f Makefile.customer`
- **Runtime stage**: Ubuntu 22.04 + can-utils only, copies binary, sets `ENTRYPOINT`

#### `docker-compose.customer.yml` — SIL environment

Two services:
- **can-setup**: Creates `vcan0` interface (runs once, exits)
- **<ecu>**: The ECU container, depends on can-setup, `network_mode: host`

If fault scenarios are defined, includes a commented-out fault-inject service.

#### `scenarios_<ecu>.py` — Fault injection

Standalone Python script with one function per fault scenario. Run with:

```bash
python scenarios_abs.py wheel_speed_loss
```

---

## CLI Reference

```bash
python -m tools.onboard generate [OPTIONS]
```

| Option | Required | Description |
|---|---|---|
| `--manifest PATH` | Yes | Path to manifest YAML file |
| `--output PATH` | Yes | Output directory for generated files |
| `--force` | No | Overwrite existing files (default: skip existing) |
| `--dry-run` | No | Print file list without writing (shows byte counts) |
| `--verify` | No | After generation: check files exist, Docker build + 5s boot test |

### Examples

```bash
# Basic generation
python -m tools.onboard generate --manifest customer/abs.yaml --output ./generated/abs/

# Overwrite previous generation
python -m tools.onboard generate --manifest customer/abs.yaml --output ./generated/abs/ --force

# Preview what would be generated
python -m tools.onboard generate --manifest customer/abs.yaml --output ./generated/abs/ --dry-run

# Generate + verify (requires Docker)
python -m tools.onboard generate --manifest customer/abs.yaml --output ./generated/abs/ --force --verify
```

---

## Verification

The `--verify` flag runs a two-step check:

### Step 1: File presence check

Verifies all required files exist in the output directory:
- `Makefile.customer`
- `Dockerfile.customer`
- `docker-compose.customer.yml`
- `<ecu>/src/<ecu>_main.c`
- `<ecu>/cfg/<Ecu>_Cfg.h`

### Step 2: Docker boot check (if Docker is available)

1. Builds the Docker image: `docker build -f Dockerfile.customer -t taktflow/<ecu>-verify .`
2. Runs the container with `--network none` for 5 seconds
3. Checks stderr for the BSW init message: `[<ECU>] BSW init complete`
4. Container will likely exit because there's no vcan0 — this is expected and not a failure

If Docker is not installed, the boot check is skipped gracefully.

---

## Working with the Generated Code

### Customer SWC requirements

The customer's SWC code must:
1. Include `"Rte.h"` for `Rte_Read()` / `Rte_Write()` calls
2. Include `"Std_Types.h"` for standard AUTOSAR types
3. Use signal IDs from `<Ecu>_Cfg.h` (or define fallback `#ifndef` guards)
4. Implement the functions declared in the manifest's `runnables` section
5. **Not** call `Com_SendSignal()` or `Com_ReceiveSignal()` directly — use `Rte_Read()`/`Rte_Write()` and let the generated bridge handle Com

### Signal flow

```
CAN bus
  -> Can_MainFunction_Read()          reads CAN frames
  -> CanIf RX routing                 routes to Com RX PDU
  -> PduR routing                     routes to Com
  -> Com_MainFunction_Rx()            unpacks into shadow buffers, monitors timeouts
  -> BridgeRxToRte()                  Com_ReceiveSignal() -> Rte_Write()
  -> Customer SWC                     Rte_Read() / Rte_Write()
  -> TransmitSchedule()              Rte_Read() -> Com_SendSignal()
  -> Com_MainFunction_Tx()           packs shadow buffers into PDU
  -> CanIf TX routing                routes to CAN
  -> CAN bus
```

### Adding E2E protection (post-generation)

E2E is not generated because it's safety-analysis-specific. To add E2E:
1. Add E2E configuration to `<Ecu>_Cfg.h` (Data IDs, profile)
2. Wrap `Com_ReceiveSignal()` calls in `BridgeRxToRte` with `E2E_P01Check()`
3. Wrap `Com_SendSignal()` calls in `TransmitSchedule` with `E2E_P01Protect()`
4. Add E2E state machine monitoring

### Integrating with existing Taktflow SIL

To run the customer ECU alongside existing Taktflow ECUs:

1. Add the ECU service to the main `docker/docker-compose.yml`
2. Remove the `can-setup` service from the customer compose (the main compose already handles it)
3. Ensure CAN IDs don't conflict with existing ECUs (check the DBC)

---

## Complete Example

The `test/` directory contains a complete working example:

### Files

| File | Purpose |
|---|---|
| `test/test_abs.yaml` | Manifest for a test ABS ECU |
| `test/test_abs.dbc` | DBC with 4 messages (2 TX, 2 RX) |
| `test/Swc_AbsControl.c` | Minimal SWC stub — reads wheel speed, writes brake command |
| `test/Swc_AbsControl.h` | SWC header |

### Running the example

```bash
# From taktflow-embedded root:

# 1. Generate
python -m tools.onboard generate \
    --manifest tools/onboard/test/test_abs.yaml \
    --output /tmp/test_onboard_abs \
    --force

# 2. Inspect the output
ls -la /tmp/test_onboard_abs/
ls -la /tmp/test_onboard_abs/abs/src/
ls -la /tmp/test_onboard_abs/abs/cfg/

# 3. Copy customer SWC
cp tools/onboard/test/Swc_AbsControl.c /tmp/test_onboard_abs/abs/src/
cp tools/onboard/test/Swc_AbsControl.h /tmp/test_onboard_abs/abs/src/

# 4. Symlink shared BSW
ln -s $(pwd)/shared /tmp/test_onboard_abs/shared

# 5. Build (requires gcc + linux headers for SocketCAN)
cd /tmp/test_onboard_abs
make -f Makefile.customer

# 6. Or use Docker
docker compose -f docker-compose.customer.yml up --build
```

### Example DBC

```dbc
VERSION ""
NS_ :
BS_:
BU_: ABS VCU

BO_ 256 ABS_BrakeCmd: 8 ABS
 SG_ BrakeForce_FL : 0|16@1+ (1,0) [0|65535] "" VCU
 SG_ BrakeForce_FR : 16|16@1+ (1,0) [0|65535] "" VCU

BO_ 257 ABS_Status: 8 ABS
 SG_ ABS_Active : 0|8@1+ (1,0) [0|1] "" VCU
 SG_ ABS_FaultCode : 8|8@1+ (1,0) [0|255] "" VCU

BO_ 512 WheelSpeed_FL_Msg: 8 VCU
 SG_ WheelSpeed_FL : 0|16@1+ (0.01,0) [0|655.35] "km/h" ABS
 SG_ WheelSpeed_FR : 16|16@1+ (0.01,0) [0|655.35] "km/h" ABS

BO_ 513 WheelSpeed_RL_Msg: 8 VCU
 SG_ WheelSpeed_RL : 0|16@1+ (0.01,0) [0|655.35] "km/h" ABS
 SG_ WheelSpeed_RR : 16|16@1+ (0.01,0) [0|655.35] "km/h" ABS
```

### Example manifest

See `test/test_abs.yaml` for the full manifest with 8 RTE signals, 2 TX messages, 2 RX messages, 1 runnable, and 1 fault scenario.

---

## Troubleshooting

### Manifest validation errors

**"'name' does not match pattern"**
ECU name must be lowercase, start with a letter: `^[a-z][a-z0-9_]{0,15}$`

**"Signal ID must be >= 16"**
IDs 0-15 are reserved for BSW well-known signals. Customer signals must start at 16.

**"rte_signal 'X' not found in rte_signals"**
Every `rte_signal` referenced in `tx_messages` or `rx_messages` must be defined in the `rte_signals` section.

**"DBC message 'X' not found"**
The `dbc_message` name must exactly match a message name in the DBC file (case-sensitive).

**"DBC signal 'X' not found in message 'Y'"**
The `dbc_signal` name must exactly match a signal name within the specified DBC message.

### Build errors

**"Cannot find Rte.h / Com.h / Std_Types.h"**
The shared BSW directory is not linked. Symlink or copy `shared/` into the output directory:
```bash
ln -s /path/to/taktflow-embedded/shared ./generated/abs/shared
```

**"undefined reference to Swc_AbsControl_MainFunction"**
Customer SWC source files are not in the `<ecu>/src/` directory. Copy them:
```bash
cp Swc_AbsControl.c ./generated/abs/abs/src/
```

**"redefinition of ABS_SIG_WHEEL_SPEED_FL"**
Customer SWC defines the signal ID with `#define` but the generated `<Ecu>_Cfg.h` also defines it. Use `#ifndef` guards in the customer SWC:
```c
#ifndef ABS_SIG_WHEEL_SPEED_FL
#define ABS_SIG_WHEEL_SPEED_FL 16u
#endif
```

### Docker errors

**"cannot create vcan0 — vcan module not loaded"**
On the Docker host, load the vcan kernel module:
```bash
sudo modprobe vcan
```

**Container exits immediately**
Expected when running with `--network none` (verify mode). With `network_mode: host` and vcan0 present, the container should stay running.

**"BSW init complete" not printed**
Check that all BSW modules link correctly. Run the build manually to see compiler errors:
```bash
make -f Makefile.customer 2>&1
```

### Common mistakes

| Mistake | Symptom | Fix |
|---|---|---|
| DBC signal name typo | Resolver error: "signal not found" | Match name exactly from DBC file |
| Forgot to copy customer .c/.h | Linker error: undefined reference | Copy files into `<ecu>/src/` |
| Signal ID conflict | Cfg header has duplicate `#define` | Ensure all IDs are unique (16+) |
| Wrong `dbc_file` path | "File not found" | Path is relative to manifest, not CWD |
| Missing `--force` on re-run | Files skipped (SKIP messages) | Add `--force` to overwrite |

---

## Architecture

### Pipeline

```
manifest.yaml + .dbc + .c/.h
    |
    v
manifest_loader.load()        # Validate YAML against JSON Schema
    |
    v
dbc_parser.parse_dbc()         # Parse CAN matrix with cantools
    |
    v
resolver.resolve()             # Cross-reference manifest <-> DBC -> EcuModel
    |
    v
generator/*.generate()         # Render all Jinja2 templates
    |
    v
cli._write_files()             # Write to output directory
    |
    v
[optional] verify.run()        # Docker build + 5s boot check
```

### Module overview

```
tools/onboard/
├── __init__.py              # Package marker
├── __main__.py              # Entry: python -m tools.onboard
├── cli.py                   # CLI orchestration
├── manifest_loader.py       # YAML load + jsonschema validation
├── dbc_parser.py            # cantools wrapper -> DbcDatabase
├── data_model.py            # EcuModel dataclass (generator IR)
├── resolver.py              # manifest + DBC -> EcuModel
├── verify.py                # File check + Docker boot test
├── requirements.txt         # Python dependencies
├── schema/
│   └── manifest.schema.json # JSON Schema for manifest validation
├── generator/
│   ├── __init__.py
│   ├── _render.py           # Shared Jinja2 rendering helper
│   ├── cfg_h.py             # <Ecu>_Cfg.h generator
│   ├── com_cfg.py           # Com_Cfg_<Ecu>.c generator
│   ├── rte_cfg.py           # Rte_Cfg_<Ecu>.c generator
│   ├── swc_com.py           # Swc_<Ecu>Com.c/.h generator
│   ├── main_c.py            # <ecu>_main.c generator
│   ├── hw_posix.py          # <ecu>_hw_posix.c generator
│   ├── makefile.py          # Makefile.customer generator
│   ├── dockerfile.py        # Dockerfile.customer generator
│   ├── compose.py           # docker-compose.customer.yml generator
│   └── fault_scenarios.py   # scenarios_<ecu>.py generator
├── templates/               # Jinja2 templates (one per generated file)
│   ├── cfg_h.j2
│   ├── com_cfg_c.j2
│   ├── rte_cfg_c.j2
│   ├── swc_com_c.j2
│   ├── swc_com_h.j2
│   ├── main_c.j2
│   ├── hw_posix_c.j2
│   ├── makefile.j2
│   ├── dockerfile.j2
│   ├── compose.j2
│   └── fault_scenarios.j2
└── test/                    # E2E test fixtures
    ├── test_abs.yaml        # Test manifest
    ├── test_abs.dbc         # Test DBC
    ├── Swc_AbsControl.c     # Test SWC source
    └── Swc_AbsControl.h     # Test SWC header
```

### Design decisions

1. **BCM pattern as main.c reference** — BCM is the simplest Taktflow ECU (no SPI/ADC/PWM/CanTp/Dcm/WdgM/E2E). Clean BSW core that customer ECUs plug into.

2. **No E2E in generated code** — E2E Data IDs and protection profiles are safety-analysis-specific. Generated code does plain `Com_ReceiveSignal`/`Com_SendSignal`. E2E is a per-contract add-on.

3. **Generators are pure functions** — Each generator returns `list[tuple[str, str]]` (relative path, content). No filesystem side effects. Unit-testable without disk.

4. **Standalone Makefile** — Doesn't modify the existing `Makefile.posix`. Can coexist without conflicts.

5. **cantools for DBC parsing** — Industry-standard library. Handles big-endian signals, multiplexed signals, extended CAN IDs, J1939.

6. **Com_MainFunction_Rx always included** — BCM reference omits it, but it's needed for RX timeout monitoring. Generator follows the CVC pattern here.

7. **Signal bit positions from DBC** — `cantools` provides absolute bit positions directly. No manual calculation needed.
