# OpenBSW STM32 Platform Port — PIL Evidence Tests

Processor-in-the-Loop tests that flash the OpenBSW reference application
onto physical NUCLEO boards and capture evidence that the port works.

## Board Layout

| ECU | Board | MCU | CAN | ST-LINK Serial | COM |
|-----|-------|-----|-----|---------------|-----|
| RZC | NUCLEO-F413ZH | STM32F413ZH (96 MHz) | bxCAN1 | `0670FF383930434B43202436` | COM15 |
| FZC | NUCLEO-G474RE | STM32G474RE (170 MHz) | FDCAN1 | `001A00363235510B37333439` | COM3 |

## Prerequisites

```
pip install pytest pyserial
```

STM32CubeProgrammer v2.22+ installed at the default path.

## Running

```bash
# Full suite — flashes both boards, captures serial output
pytest test/pil/openbsw/ -v --tb=long

# Flash verification only (no serial capture)
pytest test/pil/openbsw/test_flash_verify.py -v

# Single board
pytest test/pil/openbsw/test_openbsw_g474re.py -v
pytest test/pil/openbsw/test_openbsw_f413zh.py -v
```

## Test Matrix

| ID | Test | What it proves |
|----|------|---------------|
| PIL-G4-001 | Flash + hello | SystemInit, clock (170 MHz PLL), UART (USART2 PA2, BRR=1476), linker script |
| PIL-G4-002 | Lifecycle boot | FreeRTOS scheduler, async framework, all runlevels |
| PIL-G4-003 | CAN system init | FDCAN1 register config, GPIO AF9, NVIC priorities |
| PIL-G4-004 | Clean UART | Baud rate correct, no clock drift |
| PIL-F4-001 | Flash + hello | SystemInit, clock (96 MHz PLL), UART (USART3 PD8, BRR=417), linker script |
| PIL-F4-002 | Lifecycle boot | FreeRTOS scheduler, async framework, all runlevels |
| PIL-F4-003 | CAN system init | bxCAN1 register config, GPIO AF9, NVIC priorities |
| PIL-F4-004 | Clean UART | Baud rate correct, no clock drift |
| PIL-FLASH  | Flash+verify | ST-LINK connection, ELF integrity, flash write+readback |

## Evidence Output

After a successful run, pytest produces a report showing all tests passed
with the captured serial output embedded in any failure messages.
Save the output as evidence:

```bash
pytest test/pil/openbsw/ -v --tb=long --junitxml=test/pil/openbsw/report.xml 2>&1 | tee test/pil/openbsw/evidence.log
```
