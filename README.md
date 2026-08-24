# HitPaw MangaDex Manager

A Windows desktop app for browsing, filtering, and exporting your MangaDex library — built with Qt6/C++.

![version](https://img.shields.io/badge/version-3.1.0-orange)
![platform](https://img.shields.io/badge/platform-Windows-blue)
![license](https://img.shields.io/badge/license-MIT-green)

> Not affiliated with or endorsed by MangaDex.

## Features

- Sign in with your MangaDex account (username/password or a personal API client)
- Browse your full library as a cover grid, with fast filtering/search
- Bulk-select and export titles (with undo)
- Export to CSV, JSON, MAL-compatible XML (works for MyAnimeList/AniList/MangaBaka/Kitsu/MangaFire), and Anime-Planet's gzipped XML
- Cover caching with disk-backed CDN retry/backoff, so re-opening the app is fast
- Dark AMOLED-style UI throughout

## Download

Grab the latest build from the [Releases](../../releases) page — download the zip, extract it anywhere, and run `MangaDexExporter.exe`. No install required, and you don't need Qt or anything else on your machine.

## Setup

1. Launch the app and sign in with your MangaDex username/password.
2. You'll also need a personal API client ID & secret — MangaDex issues these for free. Get one at **MangaDex → Settings → API Clients**, then paste it into the app's login screen.
3. Click **Load Library** once signed in.

Your credentials are stored locally on your machine (via Qt's `QSettings`) and are never sent anywhere except MangaDex's own API.

## Export formats & site compatibility

| Format          | File                              | Compatible sites                                                          |
|-----------------|------------------------------------|-----------------------------------------------------------------------------|
| CSV             | `mangadex_library.csv`            | Excel, Google Sheets, any spreadsheet                                     |
| JSON            | `mangadex_library.json`           | Raw backup, scripting (includes cover URLs)                               |
| MAL XML         | `mangadex_library_MAL.xml`        | MyAnimeList, AniList, MangaBaka, Kitsu, MangaFire                         |
| AP .gz          | `mangadex_library_AP.xml.gz`      | Anime-Planet (requires the gzipped file specifically)                     |
| MB JSON         | `mangadex_library_MangaBaka.json` | MangaBaka's native import format (mangabaka.org/settings/import)          |

### Import instructions per site

| Site            | How to import                                                              |
|-----------------|------------------------------------------------------------------------------|
| MyAnimeList     | Profile → Import/Export → MyAnimeList Import → upload MAL XML             |
| AniList         | Settings → Import → upload MAL XML                                        |
| MangaBaka       | Settings → Import → MyAnimeList → upload MAL XML                          |
| Kitsu           | Avatar → Settings → Import → upload MAL XML                               |
| MangaFire       | Profile → Import/Export → upload MAL XML                                  |
| Anime-Planet    | Your manga list page → scroll to bottom → "Import it now" → upload AP .gz |

> MangaUpdates has no list import feature (export only). comix.to uses collections with no file-based import.

## Building from source

Requires Qt 6.x and a matching MinGW toolchain (built and tested against Qt 6.11.1 / MinGW 13.1.0, 64-bit).

```
qmake MangaDexExporter.pro -spec win32-g++
mingw32-make
windeployqt release\MangaDexExporter.exe
```

Or just run `build_windows.bat`, which locates your Qt/MinGW install automatically and does the above for you.

## Project structure

- `main.cpp` — the entire application (UI, networking, caching — single-file by design)
- `MangaDexExporter.pro` — qmake project file
- `icons.qrc` — bundled icon resources
- `build_windows.bat` — one-command local build
- `.github/workflows/build-windows.yml` — CI that builds a release automatically on tag push
- `CHANGELOG.md` — version history

## Contributing

Issues and PRs welcome. This started as a personal tool, so expect some rough edges outside the core library/export flow — flag anything that breaks.

## License

MIT — see [LICENSE](LICENSE).
