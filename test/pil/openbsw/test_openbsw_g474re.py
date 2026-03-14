"""
PIL-G4: OpenBSW Reference Application on NUCLEO-G474RE (FZC)

Evidence tests:
  1. Flash succeeds and board produces UART output
  2. Lifecycle boots through all runlevels (proves FreeRTOS + async framework)
  3. FDCAN1 initializes and transmits CAN frames (id=0x558)
  4. No HardFault (proves memory layout, stack, linker script correct)
  5. UART output is clean at 115200 baud (proves 170 MHz clock config)
"""

import re


class TestG474ReFlash:
    """PIL-G4-001: Verify ELF flashes and board boots."""

    def test_boot_produces_output(self, g474re_boot_log):
        """Board produces UART output after flash+reset."""
        assert len(g474re_boot_log) > 0, (
            "No serial output received — board may not have booted. "
            "Check: COM3 connected? USART2 PA2 TX routed to ST-LINK VCP?"
        )

    def test_lifecycle_log_present(self, g474re_boot_log):
        """First output is lifecycle initialization (proves UART + main + lifecycle)."""
        assert "LIFECYCLE" in g474re_boot_log or "Initialize" in g474re_boot_log, (
            f"No lifecycle output. Got:\n{g474re_boot_log[:500]}"
        )


class TestG474ReLifecycle:
    """PIL-G4-002: Verify lifecycle manager boots all runlevels."""

    def test_lifecycle_init_level_1(self, g474re_boot_log):
        """Level 1 (RuntimeSystem + Safety) initializes."""
        assert re.search(r"Initialize level 1", g474re_boot_log), (
            f"Level 1 init not found. Output:\n{g474re_boot_log[:500]}"
        )

    def test_lifecycle_run_level_1(self, g474re_boot_log):
        """Level 1 reaches run phase."""
        assert re.search(r"Run level 1", g474re_boot_log), (
            f"Level 1 run not found. Output:\n{g474re_boot_log[:500]}"
        )

    def test_lifecycle_init_level_2(self, g474re_boot_log):
        """Level 2 (CanSystem) initializes."""
        assert re.search(r"Initialize level 2", g474re_boot_log), (
            f"Level 2 init not found. Output:\n{g474re_boot_log[:500]}"
        )

    def test_lifecycle_init_can(self, g474re_boot_log):
        """CanSystem component specifically initializes."""
        assert re.search(r"Initialize can", g474re_boot_log, re.IGNORECASE), (
            f"CAN init not found. Output:\n{g474re_boot_log[:500]}"
        )

    def test_lifecycle_run_can(self, g474re_boot_log):
        """CanSystem component reaches run phase."""
        assert re.search(r"Run can", g474re_boot_log, re.IGNORECASE), (
            f"CAN run not found. Output:\n{g474re_boot_log[:500]}"
        )

    def test_lifecycle_init_level_9(self, g474re_boot_log):
        """Level 9 (DemoSystem) initializes — proves full lifecycle progression."""
        assert re.search(r"Initialize level 9", g474re_boot_log), (
            f"Level 9 init not found. Output:\n{g474re_boot_log[:500]}"
        )

    def test_lifecycle_run_level_9(self, g474re_boot_log):
        """Level 9 reaches run phase."""
        assert re.search(r"Run level 9", g474re_boot_log), (
            f"Level 9 run not found. Output:\n{g474re_boot_log[:500]}"
        )

    def test_no_hardfault(self, g474re_boot_log):
        """No HardFault handler triggered (would print CFSR/PC/LR)."""
        assert "hardfault" not in g474re_boot_log.lower(), (
            f"HardFault detected:\n{g474re_boot_log[:500]}"
        )
        assert "CFSR" not in g474re_boot_log, (
            f"HardFault CFSR dump detected:\n{g474re_boot_log[:500]}"
        )


class TestG474ReCan:
    """PIL-G4-003: Verify FDCAN1 transmits CAN frames."""

    def test_can_frame_sent(self, g474re_boot_log):
        """DemoSystem sends CAN frames on FDCAN1 (id=0x558)."""
        assert "CAN frame sent" in g474re_boot_log, (
            f"No CAN frame sent log. Output:\n{g474re_boot_log[:500]}"
        )

    def test_can_frame_id(self, g474re_boot_log):
        """CAN frames use expected demo ID 0x558."""
        assert "id=0x558" in g474re_boot_log, (
            f"Expected id=0x558 in CAN frame log. Output:\n{g474re_boot_log[:500]}"
        )

    def test_multiple_frames_sent(self, g474re_boot_log):
        """Multiple CAN frames sent (proves periodic scheduling works)."""
        frame_count = g474re_boot_log.count("Sending frame")
        assert frame_count >= 3, (
            f"Only {frame_count} frames sent in capture window (expected >=3). "
            f"DemoSystem may not be scheduling correctly."
        )

    def test_no_bus_error(self, g474re_boot_log):
        """No CAN bus-off or error reported at boot."""
        log_lower = g474re_boot_log.lower().replace("-", ".").replace("_", ".")
        assert "bus.off" not in log_lower, (
            f"Bus-off detected:\n{g474re_boot_log[:500]}"
        )


class TestG474ReHardwareConfig:
    """PIL-G4-004: Verify hardware configuration (clock, UART)."""

    def test_clock_170mhz_via_uart(self, g474re_boot_log):
        """Clock is 170 MHz — proven by clean UART at 115200 baud (BRR=1476).
        If clock were wrong, BRR would produce garbled output."""
        assert "LIFECYCLE" in g474re_boot_log, (
            "UART output doesn't contain readable lifecycle text — "
            "clock config may be wrong (BRR=1476 requires APB1=170 MHz)"
        )

    def test_uart_clean_output(self, g474re_boot_log):
        """UART output is clean ASCII (no garbled bytes from wrong baud rate).
        ANSI escape codes are expected for colored log output."""
        # Strip ANSI escape sequences before checking
        clean = re.sub(r"\x1b\[[0-9;]*m", "", g474re_boot_log)
        garbage = sum(
            1 for c in clean
            if not c.isprintable() and c not in "\r\n\t"
        )
        total = len(clean)
        if total > 0:
            garbage_pct = garbage / total * 100
            assert garbage_pct < 1.0, (
                f"UART output has {garbage_pct:.1f}% non-printable chars — "
                f"baud rate mismatch? First 200 bytes: {repr(g474re_boot_log[:200])}"
            )

    def test_freertos_tick_advancing(self, g474re_boot_log):
        """FreeRTOS tick counter advances (timestamp increases between frames).
        Format: 'NNNNN: RefApp: ...'"""
        timestamps = re.findall(r"^(\d+):", g474re_boot_log, re.MULTILINE)
        if len(timestamps) >= 2:
            first = int(timestamps[0])
            last = int(timestamps[-1])
            assert last > first, (
                f"Tick counter not advancing: first={first}, last={last}"
            )
