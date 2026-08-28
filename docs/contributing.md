---
title: Contributing — HitPaw MangaDex Manager
description: How to contribute to HitPaw — dev setup, style, tests, and PR process.
outline: deep
---

# Contributing

Thanks for helping improve HitPaw MangaDex Manager — whether it’s a bug report, a feature idea, or a code PR.

<div class="badge-row">
  <span class="badge badge--accent">MIT</span>
  <span class="badge">Qt 6.8.3 • MinGW 13.1</span>
  <span class="badge">clang-format • ctest</span>
</div>

## Ways to contribute

- **Bug reports** → [Bug report template](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=bug_report.yml) — include version (`v3.4.6`), OS, and steps to reproduce.
- **Feature requests** → [Feature request template](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=feature_request.yml) — describe the problem + proposed solution.
- **Code** → Fork, branch from `main`, make your change, open a PR (see below).
- **Docs** → Every page has *Edit this page on GitHub* — small fixes don’t need an issue.

## Development — 3-minute setup

See **[Building](/building)** for the full pinned toolchain. Short version:

::: code-group

```bash [Linux]
sudo apt install qt6-base-dev qt6-tools-dev zlib1g-dev build-essential pkg-config ninja-build
cmake --preset linux-release && cmake --build --preset linux
# or: qmake6 MangaDexExporter.pro && make -j$(nproc)
```

```bat [Windows — Qt 6.8.3 + MinGW 13.1]
# Open MangaDexExporter.pro in Qt Creator → Build → Run
# or:
build_windows.bat
# or:
cmake --preset windows-release && cmake --build --preset windows
```

```bash [macOS]
brew install qt@6 zlib ninja
cmake --preset macos-release && cmake --build --preset macos
# or: qmake MangaDexExporter.pro && make -j$(sysctl -n hw.ncpu)
```

:::

- Source is **C++/Qt6** (`main.cpp` 4405 lines, splitting into `src/api`, `src/models`, `src/ui` — see `src/README.md`). `CMakeLists.txt` `project(VERSION 3.4.6)` must match `CHANGELOG.md` tag.
- Formatting: `.clang-format` — run before push (see verify below).
- Tests: Qt Test (`tests/test_export.cpp`) via `ctest`.

## Verify before push — what CI runs

```bash
# secrets — must print nothing
grep -R -n -E "gho_[A-Za-z0-9_]{20,}|ghp_[A-Za-z0-9_]{20,}" --include="*.cpp" --include="*.h" .

# whitespace
git diff --check

# format
clang-format --dry-run --Werror main.cpp export.h domain.h pal.h secure_store.h

# shell
shellcheck build_linux.sh build_macos.sh

# tests
cmake -S tests -B build_tests && ctest --test-dir build_tests --output-on-failure

# docs (optional)
npm ci && npm run docs:build
```

::: tip Pre-commit
`pip install pre-commit && pre-commit install` — it runs the same checks locally via `.pre-commit-config.yaml` (scan-secrets, clang-format, whitespace, shellcheck). Commit is blocked if anything fails.
:::

## Pull requests

- Keep PRs **focused** (one feature/fix). Include a clear description + **screenshots for UI changes**.
- Update `CHANGELOG.md` under `## [Unreleased]`.
- Ensure CI passes (`scan-secrets` + `build-check-windows` must be green).
- Maintainer will **squash-merge** and tag a release (`v*` → auto-creates GitHub Release with notes from `CHANGELOG.md`).

### Commit style

- Use present tense: “Add Show All button”, “Fix filter chip color”
- Reference issues: `Fixes #42`
- No secrets, no `build/` artifacts — they’re `.gitignore`’d

## Community

- Discord: **[discord.gg/z6yYYpcYYc](https://discord.gg/z6yYYpcYYc)** — ask questions, share ideas
- Be respectful — see [Code of Conduct](/code-of-conduct)
- Security issues → **private advisory**, not public issue — see [Security](/security)

## License

By contributing you agree your changes are **MIT** licensed (see [`LICENSE`](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/blob/main/LICENSE)). Thanks for building HitPaw with us.
