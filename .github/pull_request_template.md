# Pull Request

## Description

<!-- What does this PR do? Link related issue if any: Fixes #<number> -->

## Screenshots (for UI changes)

<!-- Before / After -->

## Checklist

- [ ] Tested on Windows (Qt 6.8.3 + MinGW 13.1, see `BUILDING.md`)
- [ ] Updated `CHANGELOG.md` under `## [Unreleased]`
- [ ] No hardcoded secrets (`QSettings` only, `scan-secrets` passes, `git diff --check` clean)
- [ ] Ran `clang-format --dry-run --Werror` (if touching `*.cpp`/`*.h`)
- [ ] `tests/` pass (`ctest` / `qmake && make check`)
- [ ] Single focused change, ready for squash-merge

## Notes

Maintainer will tag a release (`v*`) after merge — release notes are pulled from `CHANGELOG.md`.
