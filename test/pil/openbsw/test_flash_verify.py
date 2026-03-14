"""
PIL-FLASH: Standalone flash verification tests.

These tests only verify that STM32CubeProgrammer can connect, flash, and
verify the ELF without checking serial output. Use these to diagnose
flash-level issues before running the full boot tests.
"""

import os
import subprocess

import pytest

from conftest import BOARDS, STM32_PROGRAMMER


@pytest.fixture(scope="module", params=["g474re", "f413zh"])
def board_key(request):
    return request.param


class TestFlashVerify:
    """PIL-FLASH-001: STM32CubeProgrammer can connect and flash each board."""

    def test_programmer_exists(self):
        """STM32CubeProgrammer CLI is installed."""
        assert os.path.isfile(STM32_PROGRAMMER), (
            f"STM32_Programmer_CLI not found at: {STM32_PROGRAMMER}"
        )

    def test_elf_exists(self, board_key):
        """ELF binary exists for the target board."""
        elf = BOARDS[board_key]["elf"]
        assert os.path.isfile(elf), (
            f"ELF not found: {elf}\n"
            f"Run: cmake --build --preset "
            f"{'nucleo-g474re' if board_key == 'g474re' else 'nucleo-f413zh'}"
            f"-freertos-gcc"
        )

    def test_stlink_connects(self, board_key):
        """ST-LINK probe is connected and responds."""
        board = BOARDS[board_key]
        cmd = [
            STM32_PROGRAMMER,
            "-c", "port=SWD", f"sn={board['stlink_sn']}",
        ]
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=15)
        assert result.returncode == 0, (
            f"ST-LINK connection failed for {board['name']}:\n"
            f"stdout: {result.stdout}\nstderr: {result.stderr}"
        )

    def test_flash_and_verify(self, board_key):
        """Flash ELF and verify readback matches."""
        board = BOARDS[board_key]
        elf = board["elf"]
        if not os.path.isfile(elf):
            pytest.skip(f"ELF not built: {elf}")

        cmd = [
            STM32_PROGRAMMER,
            "-c", "port=SWD", f"sn={board['stlink_sn']}",
            "-d", elf,
            "-v",   # verify
            "-rst",  # reset after
        ]
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=60)

        # Check for successful verification
        assert result.returncode == 0, (
            f"Flash+verify failed for {board['name']}:\n"
            f"stdout: {result.stdout[-500:]}\nstderr: {result.stderr[-500:]}"
        )
        assert "verified" in result.stdout.lower() or "verification" in result.stdout.lower(), (
            f"Flash succeeded but verification string not found:\n"
            f"{result.stdout[-500:]}"
        )
