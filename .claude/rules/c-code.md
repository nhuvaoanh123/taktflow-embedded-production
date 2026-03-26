---
paths:
  - "firmware/**/*.c"
  - "firmware/**/*.h"
  - "firmware/**/*.cpp"
  - "firmware/**/*.hpp"
---
# Code Style (C/C++)

**Naming**: `snake_case` (funcs/vars), `g_` prefix (globals), `UPPER_SNAKE` (constants/macros/enums), `_t` suffix (types), `module_` prefix.
**File order**: header → includes (system→RTOS→project→local) → defines → types → static protos → globals → static vars → public → static funcs.
**Format**: 4 spaces, 100 chars, Allman braces (functions), K&R (control flow). No magic numbers. Designated struct initializers.

# Firmware Safety

**Banned**: `gets`, `strcpy`, `sprintf`, `strcat`, `atoi`, `system()` → use bounded alternatives (`fgets`, `strncpy`, `snprintf`, `strncat`, `strtol`).
**Memory**: static/stack alloc preferred. No `malloc` in ISR. Check bounds before write. Init all vars. NULL after free.
**Types**: fixed-width (`uint8_t` etc). Check overflow. Explicit signed/unsigned casts. NULL-check pointers.
**Concurrency**: mutex/critical sections for shared data. `volatile` for HW/ISR vars. ISRs: set flags only, no blocking.
**Watchdog**: feed ONLY from main loop. Safe-state on trigger. Log reset reason.
**Build**: `-Wall -Wextra -Werror`. Header guards. Prefer `const`/`enum` over `#define`.

# MISRA C

Mandatory rules: ZERO deviations. Required: formal deviation with rationale+risk+independent review. Advisory: tracked, compliance expected at ASIL D.
**Critical rules**: type safety (10.x, 11.x), control flow (14.3, 15.1, 16.4, 17.2), memory (17.7, 18.1, 21.3), side effects (2.2, 13.x), preprocessor (20.x).
**Enforcement**: cppcheck with MISRA addon. Compliance report per build. Zero mandatory violations in release.
