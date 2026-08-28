---
title: Building from Source — HitPaw MangaDex Manager
description: Build HitPaw with pinned Qt 6.8.3 + MinGW 13.1 on Windows, Qt6 on Linux/macOS. CMake presets or qmake, plus verify steps.
outline: deep
---

# Building HitPaw MangaDex Manager

The exact toolchain is pinned in CI ([`ci.yml:36`](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/blob/main/.github/workflows/ci.yml#L36), [`release.yml:42`](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/blob/main/.github/workflows/release.yml#L42)) — follow it and your local build will match CI.

<div class="badge-row">
  <span class="badge badge--accent">Qt 6.8.3</span>
  <span class="badge">MinGW 13.1.0</span>
  <span class="badge">CMake 3.20+ / qmake</span>
  <span class="badge">Ninja</span>
</div>

## Toolchain

| Platform | Qt | Compiler | Install via |
|----------|-----|----------|----------------|
| **Windows** | 6.8.3 `win64_mingw` + `tools_mingw1310` | MinGW 13.1.0 (ships with Qt) — **not** MinGW 15 | `jurplel/install-qt-action@v4` (CI) or Qt Online Installer |
| **Linux** | 6.x | GCC 11+ / Clang | `qt6-base-dev qt6-tools-dev zlib1g-dev` |
| **macOS** | 6.x | Xcode Clang | `brew install qt@6` |

> ⚠️ Mismatch `MinGW 15 + Qt-MinGW 13` broke `build-check-windows` before `e97e092` — CI now installs matching `tools_mingw1310`. If you see `Cannot run compiler 'g++'`, your PATH points to the wrong MinGW (see Troubleshooting).

## Quick start (Qt Creator)

1. Install Qt 6.8.3 via [Qt Online Installer](https://www.qt.io/download) — select `Qt 6.8.3` + `MinGW 13.1` + `Qt 5 Compatibility Module` if offered.
2. Open `MangaDexExporter.pro` (qmake) **or** `CMakeLists.txt` (CMake 3.20+) in Qt Creator → Configure with `Desktop Qt 6.8.3 MinGW 64-bit` → Build → Run.
3. Or open `CMakePresets.json` preset `windows-release` / `linux-release` / `macos-release`.

## Command line

### Windows — qmake (matches CI)

```bat
:: from repo root
if exist build_win rmdir /s /q build_win
mkdir build_win && cd build_win
qmake ..\MangaDexExporter.pro -spec win32-g++
mingw32-make -j4
:: exe at build_win\release\MangaDexExporter.exe
windeployqt release\MangaDexExporter.exe
```

### Windows — CMake + Ninja (preferred)

```bat
cmake --preset windows-release
cmake --build --preset windows
:: exe at build\MangaDexExporter.exe
```

### Linux

```bash
sudo apt update && sudo apt install -y qt6-base-dev qt6-tools-dev zlib1g-dev build-essential pkg-config ninja-build
cmake --preset linux-release
cmake --build --preset linux
# or qmake
# qmake6 MangaDexExporter.pro && make -j$(nproc)
```

### macOS

```bash
brew install qt@6 zlib ninja
echo "$(brew --prefix qt@6)/bin" >> ~/.zshrc
# restart shell or: export PATH="$(brew --prefix qt@6)/bin:$PATH"
cmake --preset macos-release
cmake --build --preset macos
# or qmake
# qmake MangaDexExporter.pro && make -j$(sysctl -n hw.ncpu)
```

## Verify before push

Run what CI runs — `scan-secrets` + format + tests:

```bash
# 1. No secrets in repo (must print nothing)
grep -R -n -E "gho_[A-Za-z0-9_]{20,}|ghp_[A-Za-z0-9_]{20,}" --include="*.cpp" --include="*.h" --include="*.pro" .

# 2. Whitespace
git diff --check

# 3. Format (uses .clang-format)
clang-format --dry-run --Werror main.cpp export.h domain.h pal.h secure_store.h

# 4. Shell
shellcheck build_linux.sh build_macos.sh

# 5. Tests (Qt Test — see tests/test_export.cpp)
cmake -S tests -B build_tests && ctest --test-dir build_tests --output-on-failure
```

::: tip Pre-commit hook
Install `pip install pre-commit && pre-commit install` — it mirrors CI (`scan-secrets`, `clang-format`, `whitespace`, `shellcheck`) via `.pre-commit-config.yaml`. Commit will be blocked locally if you slip.
:::

## Source layout

| Path | Notes |
|------|-------|
| `main.cpp` (4405 lines) | UI + API — being split into `src/api/`, `src/ui/`, `src/models/` |
| `domain.h` / `export.h` / `pal.h` / `secure_store.h` | Core headers — `export.h` is header-only |
| `icons.qrc` / `hitpaw.rc` / `hitpaw.ico` | Resources + Windows icon |
| `CMakeLists.txt` `project(VERSION 3.4.6)` | Must match `CHANGELOG.md` tag & `docs/index.md` badge |
| `CMakePresets.json` | `windows-release` / `linux-release` / `macos-release` |
| `tests/` | Qt Test suite (`ctest`) — `test_export.cpp` checks CSV/JSON/MAL/AP |
| `docs/` | VitePress site — `npm run docs:dev` |

## Troubleshooting

- `qmake: command not found` → add `Qt/6.8.3/mingw_64/bin` to PATH (`where qmake` / `which qmake` should point inside Qt, not system).
- `Project ERROR: Cannot run compiler 'g++'` → MinGW version mismatch; `where g++` / `g++ --version` must show Qt’s MinGW 13.1 first in PATH. Remove stray MinGW 15 or reorder PATH.
- `zlib not found` on Linux → `sudo apt install zlib1g-dev`.
- `moc: No relevant classes found` → you used raw string `R"([\\/:*?"<>|])"` with `?"` confusing Qt 6.11 `moc` (fixed in `3.4.2` → normal string). Rebuild clean (`rm -rf build`).
- VitePress `docs:dev` blank → `npm ci && npm run docs:dev` — not `npm install`.

Next: [Export Formats](/export) for file specs, or [Getting Started](/getting-started) to run the app.
