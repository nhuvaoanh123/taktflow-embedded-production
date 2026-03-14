"""
OpenBSW STM32 Platform Port — Processor-in-the-Loop (PIL) Test Configuration

Provides fixtures for flashing and communicating with NUCLEO boards
running the OpenBSW reference application.

Board layout:
  RZC  = NUCLEO-F413ZH (bxCAN,  96 MHz) — serial 0670FF383930434B43202436, COM15
  FZC  = NUCLEO-G474RE (FDCAN, 170 MHz) — serial 001A00363235510B37333439, COM3
"""

import os
import subprocess
import time

import pytest
import serial

# ── Tool paths ──────────────────────────────────────────────────────
STM32_PROGRAMMER = (
    r"C:\Program Files (x86)\STMicroelectronics\STM32Cube"
    r"\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe"
)

OPENBSW_ROOT = os.path.normpath(
    os.path.join(os.path.dirname(__file__), "..", "..", "..", "private",
                 "vendor", "openbsw-main")
)

# ── Board definitions ───────────────────────────────────────────────
BOARDS = {
    "g474re": {
        "name": "NUCLEO-G474RE (FZC)",
        "stlink_sn": "001A00363235510B37333439",
        "com_port": "COM3",
        "baud": 115200,
        "elf": os.path.join(
            OPENBSW_ROOT, "build", "nucleo-g474re-freertos-gcc",
            "executables", "referenceApp", "application",
            "RelWithDebInfo", "app.referenceApp.elf",
        ),
        "mcu_family": "STM32G4",
        "can_peripheral": "FDCAN1",
        "clock_mhz": 170,
    },
    "f413zh": {
        "name": "NUCLEO-F413ZH (RZC)",
        "stlink_sn": "0670FF383930434B43202436",
        "com_port": "COM15",
        "baud": 115200,
        "elf": os.path.join(
            OPENBSW_ROOT, "build", "nucleo-f413zh-freertos-gcc",
            "executables", "referenceApp", "application",
            "RelWithDebInfo", "app.referenceApp.elf",
        ),
        "mcu_family": "STM32F4",
        "can_peripheral": "bxCAN1",
        "clock_mhz": 96,
    },
}


# ── Helpers ─────────────────────────────────────────────────────────
def flash_board(board_key: str) -> subprocess.CompletedProcess:
    """Flash an ELF to the board via STM32CubeProgrammer CLI."""
    board = BOARDS[board_key]
    elf = board["elf"]
    assert os.path.isfile(elf), f"ELF not found: {elf}"

    cmd = [
        STM32_PROGRAMMER,
        "-c", f"port=SWD", f"sn={board['stlink_sn']}",
        "-d", elf,
        "-v",   # verify after programming
        "-rst",  # reset after flash
    ]
    result = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
    return result


def open_serial(board_key: str, timeout: float = 5.0) -> serial.Serial:
    """Open the VCP serial port for a board."""
    board = BOARDS[board_key]
    return serial.Serial(
        port=board["com_port"],
        baudrate=board["baud"],
        timeout=timeout,
    )


def capture_boot_output(board_key: str, duration: float = 5.0) -> str:
    """Flash board, reset, and capture serial output for `duration` seconds."""
    # Flash (includes reset)
    result = flash_board(board_key)
    assert result.returncode == 0, (
        f"Flash failed:\nstdout: {result.stdout}\nstderr: {result.stderr}"
    )

    # Capture serial output after reset
    ser = open_serial(board_key, timeout=0.5)
    output = ""
    deadline = time.monotonic() + duration
    try:
        while time.monotonic() < deadline:
            chunk = ser.read(256)
            if chunk:
                output += chunk.decode("ascii", errors="replace")
            else:
                time.sleep(0.05)
    finally:
        ser.close()

    return output


# ── Fixtures ────────────────────────────────────────────────────────
@pytest.fixture(scope="session")
def g474re_boot_log():
    """Flash G474RE and return boot serial output."""
    return capture_boot_output("g474re", duration=8.0)


@pytest.fixture(scope="session")
def f413zh_boot_log():
    """Flash F413ZH and return boot serial output."""
    return capture_boot_output("f413zh", duration=8.0)
