# Building HitPaw MangaDex Manager

Exact toolchain is pinned in CI (`.github/workflows/ci.yml:36`, `release.yml:42`).

## Toolchain

| Platform | Qt | Compiler | Package manager |
|----------|-----|----------|----------------|
| Windows | 6.8.3 `win64_mingw` + `tools_mingw1310,qt.tools.win64_mingw1310` | MinGW 13.1.0 (ships with Qt) — **not** MinGW 15 | `jurplel/install-qt-action@v4` |
| Linux | 6.x | GCC 11+ / Clang | `qt6-base-dev qt6-tools-dev zlib1g-dev` |
| macOS | 6.x | Xcode Clang | `brew install qt@6` |

> Mismatch `MinGW 15 + Qt-MinGW 13` broke `build-check-windows` before `e97e092` — CI now installs matching `tools_mingw1310`.

## Quick start (Qt Creator)

1. Install Qt 6.8.3 via [Qt Online Installer](https://www.qt.io/download) — select `Qt 6.8.3` + `MinGW 13.1` + `Qt 5 Compatibility Module` if needed.
2. Open `MangaDexExporter.pro` (qmake) **or** `CMakeLists.txt` (CMake 3.20+) in Qt Creator → Configure with `Desktop Qt 6.8.3 MinGW 64-bit` → Build → Run.
3. Or open `CMakePresets.json` preset `windows-release` / `linux-release` / `macos-release`.

## Command line

### Windows (qmake, matches CI)

```bat
:: from repo root
if exist build_win rmdir /s /q build_win
mkdir build_win && cd build_win
qmake ..\MangaDexExporter.pro -spec win32-g++
mingw32-make -j4
:: exe at build_win\release\MangaDexExporter.exe
windeployqt release\MangaDexExporter.exe
```

### Windows (CMake + Ninja, preferred)

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
cmake --preset macos-release
cmake --build --preset macos
# or qmake
# qmake MangaDexExporter.pro && make -j$(sysctl -n hw.ncpu)
```

## Verify before push

```bash
git diff --check               # whitespace
grep -R -n -E "gho_[A-Za-z0-9_]{20,}|ghp_[A-Za-z0-9_]{20,}" --include="*.cpp" --include="*.h" --include="*.pro" .
# should print nothing — secrets are QSettings-only at runtime (see secure_store.h)
clang-format --dry-run --Werror main.cpp export.h domain.h pal.h secure_store.h
```

## Source layout

* `main.cpp` (4405 lines) — UI + API; being split into `src/api/`, `src/ui/`, `src/models/` (see `CMakeLists.txt` `project(version)` synced to `CHANGELOG.md` tag).
* `domain.h` / `export.h` / `pal.h` / `secure_store.h` — core headers.
* `icons.qrc` / `hitpaw.rc` — resources.
* `tests/` — Qt Test suite (`ctest`).

## Troubleshooting

* `qmake: command not found` — add Qt `6.8.3/mingw_64/bin` to PATH.
* `Project ERROR: Cannot run compiler 'g++'` — MinGW version mismatch; ensure Qt's MinGW 13.1 is first in PATH (`where g++` should point to Qt dir).
* `zlib` missing on Linux — `sudo apt install zlib1g-dev`.
