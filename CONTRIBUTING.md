# Contributing

Thanks for helping improve HitPaw MangaDex Manager!

## Ways to contribute

- **Bug reports**: Use the `Bug report` template (`.github/ISSUE_TEMPLATE/bug_report.yml`) — include version (`v3.4.6`), OS, and steps to reproduce.
- **Feature requests**: Use `Feature request` template — describe the problem and proposed solution.
- **Code**: Fork, branch from `main`, make your change, and open a PR.

## Development

```bash
# Linux
sudo apt install qt6-base-dev zlib1g-dev
qmake6 MangaDexExporter.pro && make -j$(nproc)

# Windows (Qt 6.11 + MinGW 13.1)
# Open MangaDexExporter.pro in Qt Creator → Build → Run
# or
build_windows.bat

# macOS
brew install qt@6
qmake MangaDexExporter.pro && make
```

- Source is C++/Qt6, kept local for private builds — commits to this repo are typically `CHANGELOG.md` + version bumps. If you add `MangaDexExporter.pro`/`main.cpp`, the CI `build-check` will run `qmake + mingw32-make` and must pass.
- Run `scan-secrets` locally before pushing: `grep -R "gho_\|ghp_" --include="*.cpp" --include="*.h"` should be empty — all auth is `QSettings`-driven at runtime.

## Pull requests

- Keep PRs focused and include a clear description + screenshots for UI changes.
- Ensure `CHANGELOG.md` is updated under `## [Unreleased]`.
- CI must pass (`scan-secrets` + `build-check-windows`).
- Maintainer will squash-merge and tag a release (`v*` → auto-creates GitHub Release with notes from `CHANGELOG.md`).

## Community

- Discord: https://discord.gg/z6yYYpcYYc
- Be respectful — see [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md).

## License

By contributing you agree your changes are MIT licensed.
