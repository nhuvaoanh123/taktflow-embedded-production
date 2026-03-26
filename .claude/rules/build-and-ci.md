---
paths:
  - "scripts/**/*"
  - "firmware/**/*"
  - ".github/**/*"
---
# Build & CI

**Commands**: `make build` / `build-debug` / `build-release` / `test` / `flash` / `clean`. Output in `build/`.

**Flags**: all: `-Wall -Wextra -Wshadow -Wdouble-promotion -Wformat=2 -Wundef`. Release: add `-Werror -Os -DNDEBUG -fstack-protector`. Debug: `-Og -g3 -DDEBUG -fsanitize=address,undefined`.

**CI commit**: compile+test+static analysis+format+secret scan. **PR**: +integration+fuzz+coverage+size comparison. **Release**: +HIL+sign binary+claim audit.

**Size**: track binary size, budget per partition, block merge if exceeded. Pin all toolchain versions.
