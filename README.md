# HitPaw MangaDex Manager

A desktop app for browsing, filtering, and exporting your MangaDex library — built with Qt6/C++.

[![version](https://img.shields.io/badge/version-3.5.0-orange)](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases)
[![platform](https://img.shields.io/badge/platform-Windows%20%7C%20macOS%20%7C%20Linux-blue)](BUILDING.md)
[![license](https://img.shields.io/badge/license-MIT-green)](LICENSE)
[![CI](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/actions/workflows/ci.yml/badge.svg)](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/actions/workflows/ci.yml)
[![CodeQL](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/actions/workflows/codeql.yml/badge.svg)](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/actions/workflows/codeql.yml)
[![Discord](https://img.shields.io/badge/Discord-Join%20Server-5865F2)](https://discord.gg/z6yYYpcYYc)

> Not affiliated with or endorsed by MangaDex.

## Preview

![Library Grid — 3276 titles, 5 columns, Show All + Check for updates (v3.4.6)](screenshots/preview-1.png)
![Activity Log — Up to date v3.4.6, HitPaw icon](screenshots/preview-2.png)
![Export — Output Folder & Formats](screenshots/preview-3.png)
![Download — Paste URL & Chapters](screenshots/preview-4.png)

> Preview updated for **v3.4.6** — grid density toggle removed, `Show All (N)` added, manual update popup now uses HitPaw icon.

## Features

- Sign in with your MangaDex account (username/password or personal API client) — credentials stored locally via `QSettings`, never hardcoded
- Browse your full library as a cover grid (5 columns, 170×322 cards) with fast filtering/search by title, status, year, and tags
- Pagination that scales: `Show more (60)` for incremental loading + `Show All (N)` to load your entire library at once — `3276 shown` in one click
- Manual update check: `Check for updates` → popup with HitPaw icon showing `You are on vX / Latest vY / You are outdated` and `Download Latest Version` → `https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest` (silent auto-check still runs at launch)
- Bulk-select and export titles with undo/redo (`Ctrl+Z` / `Ctrl+Y`), and bulk status editor (`Reading/Completed/On Hold/Plan to Read/Dropped/Re-reading`)
- **Sync to MDList**: push your whole library (or just the selected titles) into a MangaDex custom list — reuses an existing list by name or creates one, skips duplicates, live progress, never touches reading statuses
- Export to CSV, JSON, MAL-compatible XML (MyAnimeList/AniList/MangaBaka/Kitsu/MangaFire), MangaBaka JSON, and Anime-Planet's gzipped XML — with validation and import guides
- Cover caching with disk-backed CDN retry/backoff + `Clear Cover Cache` and stats dashboard (`Years: 2026:143 … Total 3276`)
- Library sorting (`Title A-Z/Z-A, Year Newest/Oldest, Status`), year/tag filters, and status-color chips
- Dark AMOLED-style UI, smooth scrolling, and skeleton loading

## Download

Grab the latest build from the [Releases](../../releases) page:

| Platform | File | Notes |
|----------|------|-------|
| **Windows** | `HitPawMangaDexManager-v*-windows-x64.zip` | Extract and run `MangaDexExporter.exe` |
| **macOS** | `HitPawMangaDexManager-v*-macOS.tar.gz` | Extract and run the bundled app or executable |
| **Linux** | `HitPawMangaDexManager-v*-Linux.tar.gz` | Extract and run `./MangaDexExporter` |

No install required on any platform. You don't need Qt or anything else on your machine.

> **Build from source?** See [BUILDING.md](BUILDING.md) for exact Qt 6.8.3 + MinGW 13.1 steps, `CMakePresets.json`, and `clang-format`/`ctest` usage.

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

## Community

Join the Discord for support, suggestions, and updates: **https://discord.gg/z6yYYpcYYc**

## License

MIT — see [LICENSE](LICENSE).
