---
title: Building from Source — HitPaw MangaDex Manager
description: Build HitPaw with pinned Qt 6.8.3 + MinGW 13.1 on Windows, Qt6 on Linux/macOS. CMake presets or qmake.
outline: deep
---

# Building from Source

The exact toolchain is pinned in CI — follow it and your build will match.

<div class="badge-row">
  <span class="badge badge--accent">Qt 6.8.3</span>
  <span class="badge">MinGW 13.1</span>
  <span class="badge">CMake 3.20+ / qmake</span>
</div>

## Toolchain

| Platform | Qt | Compiler | Install |
|----------|-----|----------|---------|
| **Windows** | 6.8.3 `win64_mingw` | MinGW 13.1 (ships with Qt) | Qt Online Installer |
| **Linux** | 6.x | GCC 11+ / Clang | `qt6-base-dev qt6-tools-dev zlib1g-dev` |
| **macOS** | 6.x | Xcode Clang | `brew install qt@6` |

> MinGW 15 + Qt-MinGW 13 mismatch breaks builds. Use the matching MinGW that ships with Qt.

## Quick start (Qt Creator)

1. Install Qt 6.8.3 via [Qt Online Installer](https://www.qt.io/download) — select `MinGW 13.1`
2. Open `MangaDexExporter.pro` or `CMakeLists.txt` → Build → Run

## Command line

### Windows — CMake + Ninja (preferred)

```bat
cmake --preset windows-release
cmake --build --preset windows
```

### Windows — qmake

```bat
if exist build_win rmdir /s /q build_win
mkdir build_win && cd build_win
qmake ..\MangaDexExporter.pro -spec win32-g++
mingw32-make -j4
```

### Linux

```bash
sudo apt install qt6-base-dev qt6-tools-dev zlib1g-dev build-essential ninja-build
cmake --preset linux-release && cmake --build --preset linux
```

### macOS

```bash
brew install qt@6 zlib ninja
cmake --preset macos-release && cmake --build --preset macos
```

## Verify before push

```bash
# 1. No secrets (must print nothing)
grep -R -n -E "gho_[A-Za-z0-9_]{20,}|ghp_[A-Za-z0-9_]{20,}" --include="*.cpp" --include="*.h" .

# 2. Whitespace
git diff --check

# 3. Format
clang-format --dry-run --Werror main.cpp export.h domain.h pal.h secure_store.h

# 4. Tests
cmake -S tests -B build_tests && ctest --test-dir build_tests --output-on-failure
```

::: tip Pre-commit hook
`pip install pre-commit && pre-commit install` — mirrors CI checks locally.
:::

## Source layout

| Path | Notes |
|------|-------|
| `main.cpp` | UI + API (splitting into `src/api`, `src/ui`, `src/models`) |
| `domain.h` / `export.h` / `pal.h` / `secure_store.h` | Core headers |
| `CMakeLists.txt` | `project(VERSION 3.5.0)` — must match changelog |
| `CMakePresets.json` | `windows-release` / `linux-release` / `macos-release` |
| `tests/` | Qt Test suite (`ctest`) |
| `docs/` | VitePress site — `npm run docs:dev` |

## Troubleshooting

- `qmake: command not found` → add Qt's `bin` to PATH
- `Cannot run compiler 'g++'` → MinGW version mismatch — check `g++ --version`
- `zlib not found` → `sudo apt install zlib1g-dev`
- VitePress blank → `npm ci && npm run docs:dev`

Next: [Export Formats](/export) or [Getting Started](/getting-started).
