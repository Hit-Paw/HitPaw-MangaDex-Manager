# Contributing

Thanks for helping improve HitPaw MangaDex Manager!

## Ways to contribute

- **Bug reports**: Use the `Bug report` template (`.github/ISSUE_TEMPLATE/bug_report.yml`) — include version (`v3.4.6`), OS, and steps to reproduce.
- **Feature requests**: Use `Feature request` template — describe the problem and proposed solution.
- **Code**: Fork, branch from `main`, make your change, and open a PR.

## Development

See [Building](/building) for full toolchain (Qt 6.8.3 + MinGW 13.1 via `jurplel/install-qt-action@v4`).

```bash
# Linux
sudo apt install qt6-base-dev qt6-tools-dev zlib1g-dev
cmake --preset linux-release && cmake --build --preset linux
# or: qmake6 MangaDexExporter.pro && make -j$(nproc)

# Windows (Qt 6.8.3 + MinGW 13.1)
# Open MangaDexExporter.pro in Qt Creator → Build → Run
# or: build_windows.bat
# or: cmake --preset windows-release && cmake --build --preset windows

# macOS
brew install qt@6
cmake --preset macos-release && cmake --build --preset macos
# or: qmake MangaDexExporter.pro && make
```

- Source is C++/Qt6 (`main.cpp` 4405 lines, splitting into `src/api`, `src/models`, `src/ui` — see `src/README.md`). `CMakeLists.txt` `project(VERSION 3.4.6)` must match `CHANGELOG.md` tag.
- Formatting: `clang-format --dry-run --Werror` (config `.clang-format`), whitespace `git diff --check`, `shellcheck` for `build_*.sh`.
- Tests: `cmake -S tests -B build_tests && ctest --test-dir build_tests` (Qt Test, see `tests/test_export.cpp`).
- Secrets: `grep -R -n -E "gho_[A-Za-z0-9_]{20,}|ghp_[A-Za-z0-9_]{20,}" --include="*.cpp" --include="*.h"` must be empty — all auth is `QSettings`-driven at runtime (`secure_store.h`). Also `pre-commit` hook (`.pre-commit-config.yaml`) mirrors CI `scan-secrets`.

## Pull requests

- Keep PRs focused and include a clear description + screenshots for UI changes.
- Ensure `CHANGELOG.md` is updated under `## [Unreleased]`.
- CI must pass (`scan-secrets` + `build-check-windows`).
- Maintainer will squash-merge and tag a release (`v*` → auto-creates GitHub Release with notes from `CHANGELOG.md`).

## Community

- Discord: https://discord.gg/z6yYYpcYYc
- Be respectful — see [Code of Conduct](/code-of-conduct).

## License

By contributing you agree your changes are MIT licensed.
