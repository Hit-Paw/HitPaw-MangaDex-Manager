---
title: Contributing — HitPaw MangaDex Manager
description: How to contribute to HitPaw — dev setup, style, tests, and PR process.
outline: deep
---

# Contributing

Thanks for helping improve HitPaw — whether it's a bug report, feature idea, or code PR.

<div class="badge-row">
  <span class="badge badge--accent">MIT</span>
  <span class="badge">Qt 6.8.3 • MinGW 13.1</span>
  <a href="https://discord.gg/z6yYYpcYYc" class="badge badge--brand">Discord</a>
</div>

## Ways to contribute

- **Bug reports** → [Bug report template](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=bug_report.yml) — include version, OS, steps
- **Feature requests** → [Feature request template](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=feature_request.yml)
- **Code** → Fork, branch from `main`, open a PR
- **Docs** → Every page has *Edit this page on GitHub*

## Development — 3-minute setup

See **[Building](/building)** for full toolchain. Short version:

::: code-group

```bash [Linux]
sudo apt install qt6-base-dev qt6-tools-dev zlib1g-dev build-essential ninja-build
cmake --preset linux-release && cmake --build --preset linux
```

```bat [Windows]
cmake --preset windows-release && cmake --build --preset windows
```

```bash [macOS]
brew install qt@6 zlib ninja
cmake --preset macos-release && cmake --build --preset macos
```

:::

- **C++/Qt6** codebase — `CMakeLists.txt` `project(VERSION 3.4.6)` must match changelog
- Formatting: `.clang-format` — run before push
- Tests: Qt Test (`tests/test_export.cpp`) via `ctest`

## Verify before push

```bash
# secrets (must print nothing)
grep -R -n -E "gho_[A-Za-z0-9_]{20,}|ghp_[A-Za-z0-9_]{20,}" --include="*.cpp" --include="*.h" .

# format
clang-format --dry-run --Werror main.cpp export.h domain.h pal.h secure_store.h

# tests
cmake -S tests -B build_tests && ctest --test-dir build_tests --output-on-failure
```

::: tip Pre-commit
`pip install pre-commit && pre-commit install` — runs CI checks locally.
:::

## Pull requests

- Keep PRs **focused** (one feature/fix). Include screenshots for UI changes.
- Update `CHANGELOG.md` under `## [Unreleased]`
- Ensure CI passes — maintainer will squash-merge

### Commit style

- Present tense: "Add Show All button", "Fix filter chip color"
- Reference issues: `Fixes #42`

## Community

- [![Discord](https://img.shields.io/badge/Discord-Join%20Server-5865F2)](https://discord.gg/z6yYYpcYYc)
- Be respectful — see [Code of Conduct](/code-of-conduct)
- Security issues → private advisory — see [Security](/security)

## License

By contributing you agree your changes are **MIT** licensed. Thanks for building HitPaw with us.
