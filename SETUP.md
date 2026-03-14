# Setup Guide

Get a working development environment in 3 paths — pick the one that matches your goal:

| Path | What you need | What you can do |
|------|--------------|----------------|
| **A — SIL only (recommended start)** | Linux or WSL2, Docker | Build + test all 7 ECUs in simulation |
| **B — Host POSIX** | Linux, gcc, can-utils | Compile firmware to run natively on Linux |
| **C — Physical ECU** | Path B + STM32/TMS570 toolchain + hardware | Flash and debug real boards |

---

## Prerequisites (All Paths)

### Operating System

| OS | SIL | POSIX | Physical |
|----|-----|-------|---------|
| Ubuntu 22.04 / 24.04 | ✅ | ✅ | ✅ |
| Debian 12 | ✅ | ✅ | ✅ |
| WSL2 (Windows 11) | ✅ | ✅ | ⚠️ USB passthrough needed |
| macOS (ARM/Intel) | ⚠️ Docker only | ❌ | ❌ |
| Windows (native) | ❌ | ❌ | ❌ |

### Common Tools (All Paths)

```bash
# Ubuntu/Debian
sudo apt-get update && sudo apt-get install -y \
  git make python3 python3-pip python3-venv

# Verify
git --version        # >= 2.34
python3 --version    # >= 3.11
make --version       # >= 4.3
```

### Clone

```bash
git clone https://github.com/nhuvaoanh123/taktflow-embedded-production.git
cd taktflow-embedded-production
```

---

## Path A — SIL (Docker)

Everything runs inside containers. No compiler installation needed.

### 1. Install Docker

```bash
# Ubuntu
curl -fsSL https://get.docker.com | sudo bash
sudo usermod -aG docker $USER
newgrp docker

# Verify
docker --version          # >= 24.0
docker compose version    # >= 2.20
```

### 2. Load Virtual CAN Kernel Module

SIL uses a virtual CAN interface (`vcan0`). This requires the `vcan` kernel module.

```bash
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0

# Make persistent across reboots
echo 'vcan' | sudo tee /etc/modules-load.d/vcan.conf
```

**WSL2 note:** The default WSL2 kernel does not include `vcan`. You need a custom kernel build or use the provided `docker/Dockerfile.can-setup` approach (sets up vcan inside the container using `--privileged`).

### 3. Python Environment (SIL scripts)

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r test/sil/requirements.txt
```

### 4. Verify

```bash
docker compose -f docker/docker-compose.sil.yml build
docker compose -f docker/docker-compose.sil.yml up --abort-on-container-exit
# Expected: all ECU containers start, SIL harness runs, exits 0
```

---

## Path B — Host POSIX

Compiles and runs ECU firmware as a native Linux process. Faster iteration than Docker.

### 1. Install Compiler and CAN Tools

```bash
sudo apt-get install -y \
  gcc \
  gcc-arm-none-eabi \
  libsocketcan-dev \
  can-utils \
  libncurses-dev \
  lcov \
  cppcheck

# Verify
gcc --version              # >= 11.4
arm-none-eabi-gcc --version # >= 12.3 (for cross-compile checks)
cppcheck --version         # >= 2.10
```

### 2. Python Environment (test harnesses)

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r test/sil/requirements.txt
pip install -r tools/arxml/requirements.txt
```

### 3. Set Up vcan0

```bash
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```

### 4. Build and Run One ECU

```bash
# Build CVC ECU (Motor Controller) for POSIX
make -f firmware/platform/posix/Makefile.posix TARGET=cvc

# Run it (communicates on vcan0)
./build/posix/cvc/cvc_posix

# In another terminal, watch CAN traffic
candump vcan0
```

---

## Path C — Physical ECU

### STM32 Toolchain (BCM, FZC, GW, ICU, RZC)

```bash
# Install Arm GNU Toolchain
# Download from: https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
# Version: 12.3.Rel1 (aarch64-arm-none-eabi)
# Install to /opt/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi

export PATH=/opt/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi/bin:$PATH

# Install OpenOCD (for flashing + debugging)
sudo apt-get install -y openocd

# Install STM32CubeProgrammer (GUI alternative)
# Download from: https://www.st.com/en/development-tools/stm32cubeprog.html
```

See [docs/reference/lessons-learned/hardware/PROCESS-stm32-cubemx-bringup.md](docs/reference/lessons-learned/hardware/PROCESS-stm32-cubemx-bringup.md) for full bringup procedure.

### TMS570 Toolchain (Safety Controller — SC)

The Safety Controller ECU runs on TI TMS570LS0432 (ASIL-D target). This requires TI proprietary toolchain.

```
Required:
  Code Composer Studio (CCS) v20.4.1
  Download: https://www.ti.com/tool/CCSTUDIO

  TI ARM Clang Compiler v4.0.4.LTS
  Installed via CCS > Help > Install New Software

  HALCoGen v04.07.01 (BSW/MCAL code generator)
  Download: https://www.ti.com/tool/HALCOGEN
  Requires free TI account registration
```

See [docs/reference/sc-toolchain-setup.md](docs/reference/sc-toolchain-setup.md) for full setup procedure.

### USB-CAN Adapter (for CAN monitoring on real hardware)

See [docs/guides/usb-can-adapter-setup.md](docs/guides/usb-can-adapter-setup.md).

---

## Troubleshooting

| Error | Cause | Fix |
|-------|-------|-----|
| `RTNETLINK answers: File exists` | vcan0 already created | `sudo ip link del vcan0` then recreate |
| `permission denied /dev/vcan0` | Not in `dialout` group | `sudo usermod -aG dialout $USER`, re-login |
| `docker: Cannot connect to Docker daemon` | Docker not running | `sudo systemctl start docker` |
| `ModuleNotFoundError: cantools` | venv not activated | `source .venv/bin/activate` |
| `arm-none-eabi-gcc: command not found` | Toolchain not in PATH | Add to `~/.bashrc` and `source ~/.bashrc` |
| `vcan: Fatal: Module vcan not found` | Kernel module missing | On WSL2, build custom kernel with `CONFIG_CAN_VCAN=m` |
| `cppcheck: MISRA rules file not found` | MISRA config missing | See `tools/misra/README.md` |
