---
title: Building from Source
description: Build HitPaw with pinned Qt 6.8.3 + MinGW 13.1 on Windows, Qt6 on Linux/macOS. CMake presets or qmake — verify before push.
outline: deep
---

# Building from Source

The exact toolchain is pinned in CI (`.github/workflows/ci.yml:36`, `release.yml:42`) — follow it and your build will match release binaries bit-for-bit.

<div class="badge-row">
  <span class="badge badge--accent">Qt 6.8.3</span>
  <span class="badge">MinGW 13.1</span>
  <span class="badge">CMake 3.20+ / qmake</span>
  <span class="badge">Ninja</span>
</div>

## Toolchain — must match

| Platform | Qt | Compiler | Install command / notes |
|----------|-----|----------|-------------------------|
| **Windows** | 6.8.3 `win64_mingw` | MinGW 13.1.0 (ships with Qt — **not** MinGW 15) | Qt Online Installer → `Qt 6.8.3` + `MinGW 13.1` + `Qt 5 Compatibility Module` |
| **Linux** | 6.x | GCC 11+ / Clang 14+ | `qt6-base-dev qt6-tools-dev zlib1g-dev` |
| **macOS** | 6.x | Xcode Clang 15+ | `brew install qt@6` |

> **MinGW 15 + Qt-MinGW 13 mismatch broke `build-check-windows` before `e97e092`.** Always use the MinGW that ships inside Qt’s `Tools/mingw1310_64` and put it first in PATH (`where g++` should point to Qt dir).

## Quick start (Qt Creator — 2 min)

1. Install Qt 6.8.3 via [Qt Online Installer](https://www.qt.io/download) — select `Qt 6.8.3` + `MinGW 13.1` + `Qt 5 Compatibility Module` if needed.
2. Open `MangaDexExporter.pro` (qmake) **or** `CMakeLists.txt` (CMake 3.20+) in Qt Creator → Configure with `Desktop Qt 6.8.3 MinGW 64-bit` → Build → Run. Or open preset `windows-release` / `linux-release` / `macos-release` from `CMakePresets.json`.

## Command line

::: code-group

```bat [Windows — CMake + Ninja ★ preferred]
cmake --preset windows-release
cmake --build --preset windows
:: exe at build\MangaDexExporter.exe (or build\release\)
:: Deploy Qt DLLs if needed:
:: windeployqt build\MangaDexExporter.exe
```

```bat [Windows — qmake]
if exist build_win rmdir /s /q build_win
mkdir build_win && cd build_win
qmake ..\MangaDexExporter.pro -spec win32-g++
mingw32-make -j4
:: exe at build_win\release\MangaDexExporter.exe
windeployqt release\MangaDexExporter.exe
```

```bash [Linux]
sudo apt update && sudo apt install -y qt6-base-dev qt6-tools-dev zlib1g-dev build-essential pkg-config ninja-build
cmake --preset linux-release
cmake --build --preset linux
# or qmake
# qmake6 MangaDexExporter.pro && make -j$(nproc)
```

```bash [macOS]
brew install qt@6 zlib ninja
echo "$(brew --prefix qt@6)/bin" >> ~/.zshrc && source ~/.zshrc
cmake --preset macos-release
cmake --build --preset macos
# or qmake
# qmake MangaDexExporter.pro && make -j$(sysctl -n hw.ncpu)
```

:::

::: tip Docs site locally
`npm ci && npm run docs:dev` → http://localhost:5173/HitPaw-MangaDex-Manager/ — hot reload. Build with `npm run docs:build` then `npm run docs:preview`.
:::

## Verify before push — must all pass

Run these exactly as CI does — `pre-commit` mirrors them locally:

::: code-group

```bash [Secrets — must print nothing]
grep -R -n -E "gho_[A-Za-z0-9_]{20,}|ghp_[A-Za-z0-9_]{20,}" --include="*.cpp" --include="*.h" --include="*.pro" .
# should print nothing — secrets are QSettings-only at runtime (see secure_store.h)
```

```bash [Whitespace]
git diff --check
```

```bash [Format]
clang-format --dry-run --Werror main.cpp export.h domain.h pal.h secure_store.h
# or auto-fix: clang-format -i main.cpp export.h domain.h pal.h secure_store.h
```

```bash [Tests]
cmake -S tests -B build_tests
cmake --build build_tests
ctest --test-dir build_tests --output-on-failure
```

```bash [Docs build]
npm ci
npm run docs:build
# Check no dead links — VitePress will warn
```

:::

::: tip Pre-commit hook — run once
`pip install pre-commit && pre-commit install` — runs `git diff --check`, `clang-format`, secret scan, and `ctest` on every commit.
:::

## Source layout

| Path | Notes |
|------|-------|
| `main.cpp` (~4400 lines) | UI + API — being split into `src/api`, `src/ui`, `src/models` |
| `domain.h` / `export.h` / `pal.h` / `secure_store.h` | Core headers — `secure_store.h` handles credential storage |
| `icons.qrc` / `hitpaw.rc` / `hitpaw.ico` | Resources & Windows icon |
| `CMakeLists.txt` | `project(VERSION 3.5.0)` — must match `CHANGELOG.md` tag & `docs/.vitepress/config.mts` `softwareVersion` |
| `CMakePresets.json` | `windows-release` / `linux-release` / `macos-release` — single source of truth for flags |
| `tests/` | Qt Test suite (`test_export.cpp` validates export formats) |
| `docs/` | VitePress site — `npm run docs:dev` (see above) |
| `.github/workflows/` | `ci.yml` (clang-format + ctest + scan-secrets) / `docs.yml` / `release.yml` |

## Troubleshooting

<details>
<summary><strong><code>qmake: command not found</code></strong></summary>

Add Qt `6.8.3/mingw_64/bin` to PATH. On Windows with Qt default path:
`C:\Qt\6.8.3\mingw_64\bin` and `C:\Qt\Tools\mingw1310_64\bin`. Verify with `where qmake` / `qmake --version`.
</details>

<details>
<summary><strong><code>Project ERROR: Cannot run compiler 'g++'</code></strong></summary>

MinGW version mismatch. `g++ --version` must show 13.1.0, not 15.x. Put Qt's `Tools\mingw1310_64\bin` **before** any other MinGW in PATH. Restart Qt Creator / terminal after change.
</details>

<details>
<summary><strong><code>zlib not found</code> on Linux</strong></summary>

`sudo apt install zlib1g-dev` (Debian/Ubuntu) or `sudo dnf install zlib-devel` (Fedora) or `sudo pacman -S zlib` (Arch). Then re-run cmake preset.
</details>

<details>
<summary><strong>VitePress blank / styles missing</strong></summary>

`rm -rf node_modules && npm ci && npm run docs:dev`. Check Node >=20 (`node -v`). Ensure `base` in `config.mts` is `/HitPaw-MangaDex-Manager/` for project pages, `/` for custom domain.
</details>

<details>
<summary><strong>Export validation fails in tests</strong></summary>

Run `ctest --test-dir build_tests --output-on-failure -V` for verbose. CSV header must be `title,status,year,tags` + correct count; JSON needs `schema_version`; MAL XML must be well-formed via `QXmlStreamReader`.
</details>

Next: [Export Formats](/export) for validation details or [Getting Started](/getting-started) to run your build.

