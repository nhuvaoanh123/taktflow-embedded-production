# Build Guide

Three build targets: SIL (Docker), POSIX (host), Physical ECU. Follow [SETUP.md](SETUP.md) first.

---

## ECU Reference

| ECU | ID | Role | ASIL | Platform |
|-----|----|------|------|----------|
| BCM | `bcm` | Body Control — pedal, mode switch | B | STM32G4 / POSIX |
| CVC | `cvc` | Motor Controller — speed, torque | D | STM32G4 / POSIX |
| FZC | `fzc` | Fault Zone Controller | C | STM32G4 / POSIX |
| GW  | `gw`  | CAN Gateway | B | STM32G4 / POSIX |
| ICU | `icu` | Instrument Cluster | A | STM32G4 / POSIX |
| RZC | `rzc` | Redundancy Zone Controller | D | STM32G4 / POSIX |
| SC  | `sc`  | Safety Controller | D | TMS570LS0432 |

---

## SIL — All ECUs in Docker

```bash
# Build all images
docker compose -f docker/docker-compose.sil.yml build

# Run full SIL system (all 7 ECUs + vCAN network)
docker compose -f docker/docker-compose.sil.yml up

# Run in background
docker compose -f docker/docker-compose.sil.yml up -d

# Tail logs for one ECU
docker compose -f docker/docker-compose.sil.yml logs -f cvc

# Stop
docker compose -f docker/docker-compose.sil.yml down
```

**Success indicator:** All containers reach `RUNNING` state. `candump vcan0` shows heartbeat frames from all 7 ECUs within 5 seconds of startup.

**SIL development compose** (mounts source, enables faster iteration):
```bash
docker compose -f docker/docker-compose.dev.yml up
```

---

## POSIX — Single ECU on Host

```bash
# Build one ECU
make -f firmware/platform/posix/Makefile.posix TARGET=<ecu_id>

# Examples
make -f firmware/platform/posix/Makefile.posix TARGET=cvc
make -f firmware/platform/posix/Makefile.posix TARGET=bcm

# Build all ECUs
for ecu in bcm cvc fzc gw icu rzc sc; do
  make -f firmware/platform/posix/Makefile.posix TARGET=$ecu
done

# Output binary location
./build/posix/<ecu_id>/<ecu_id>_posix
```

**Clean build:**
```bash
make -f firmware/platform/posix/Makefile.posix TARGET=cvc clean
```

**Debug build** (no optimisation, debug symbols):
```bash
make -f firmware/platform/posix/Makefile.posix TARGET=cvc DEBUG=1
```

---

## BSW — AUTOSAR Basic Software Stack

```bash
# Build BSW modules only (no ECU application)
cd firmware/bsw
make all

# Output: static libraries in firmware/bsw/build/
```

---

## Physical ECU — STM32 (BCM, CVC, FZC, GW, ICU, RZC)

```bash
# Requires: arm-none-eabi-gcc in PATH (see SETUP.md Path C)

# Build
make -f firmware/platform/stm32/Makefile.stm32 TARGET=cvc build

# Output
./build/stm32/cvc/cvc.elf    # ELF with debug symbols
./build/stm32/cvc/cvc.bin    # Raw binary for flashing
./build/stm32/cvc/cvc.hex    # Intel HEX format

# Flash via OpenOCD (ST-Link connected)
make -f firmware/platform/stm32/Makefile.stm32 TARGET=cvc flash

# Flash via STM32CubeProgrammer (GUI)
STM32_Programmer_CLI -c port=SWD -d build/stm32/cvc/cvc.hex --verify
```

---

## Physical ECU — TMS570 Safety Controller (SC)

The SC ECU must be built using TI Code Composer Studio (CCS). There is no command-line build path for TMS570 — CCS project import is required.

```
1. Open CCS
2. File > Import CCS Projects > Browse to firmware/ecu/sc/
3. Select the sc_tms570 project
4. Build > Build Project (Ctrl+B)
5. Output: Debug/sc_tms570.out
6. Flash: Run > Load > Load Program > Browse to sc_tms570.out
```

See [docs/reference/sc-toolchain-setup.md](docs/reference/sc-toolchain-setup.md) for CCS project configuration.

---

## Code Generation (ARXML → C)

The ARXML-driven code generator produces RTE stubs and COM configuration. Run this when `arxml/TaktflowSystem.arxml` or `gateway/taktflow.dbc` changes.

```bash
# Activate Python venv
source .venv/bin/activate

# DBC → ARXML (if DBC changed)
python tools/arxml/dbc2arxml.py gateway/taktflow.dbc arxml/

# ARXML → C (RTE stubs, COM config, E2E config)
python tools/arxml/codegen.py \
  --arxml arxml/TaktflowSystem.arxml \
  --config project.yaml \
  --output generated/

# Verify generated output matches committed output (drift check)
git diff --exit-code generated/
```

**CI enforces:** Generated files must match committed output. If `git diff generated/` shows changes, the CI build fails. Commit the regenerated files before pushing.

---

## Static Analysis (MISRA-C)

```bash
# Run Cppcheck with MISRA on one ECU
cppcheck \
  --addon=misra \
  --suppress=misra-c2012-1.1 \
  --error-exitcode=1 \
  firmware/ecu/cvc/src/

# Run on all firmware
make -f firmware/platform/posix/Makefile.posix misra
```

Zero MISRA violations are required to pass CI. Approved deviations are listed in [docs/safety/analysis/misra-deviation-register.md](docs/safety/analysis/misra-deviation-register.md).

---

## Build Output Summary

| Build | Output Location | Success Indicator |
|-------|----------------|-------------------|
| SIL Docker | Container logs | All ECUs `RUNNING`, CAN heartbeats visible |
| POSIX | `build/posix/<ecu>/` | Binary exists, exits 0 on `--selftest` |
| STM32 | `build/stm32/<ecu>/` | `.elf` + `.bin` present, size < flash limit |
| TMS570 | `Debug/sc_tms570.out` | CCS reports 0 errors, 0 warnings |
| BSW | `firmware/bsw/build/` | All `.a` archives present |
| Codegen | `generated/` | `git diff generated/` shows no changes |
