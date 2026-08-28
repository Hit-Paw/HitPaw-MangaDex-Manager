# Getting Started

## Download

Grab the latest build from [Releases](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases):

| Platform | File | Notes |
|----------|------|-------|
| **Windows** | `HitPawMangaDexManager-v*-windows-x64.zip` | Extract and run `MangaDexExporter.exe` |
| **macOS** | `HitPawMangaDexManager-v*-macOS.tar.gz` | Extract and run the bundled app |
| **Linux** | `HitPawMangaDexManager-v*-Linux.tar.gz` | Extract and run `./MangaDexExporter` |

> No install required. All Qt DLLs are bundled on Windows.

## Setup

1. Launch the app and sign in with your MangaDex **username/password**.
2. Get a personal API client: **MangaDex → Settings → API Clients** → Create → copy **Client ID** and **Client Secret**.
3. Paste them into the app's login screen. Check **Remember credentials** if desired.
4. Click **Load Library**.

Credentials are stored locally via `QSettings` (`api/client_id`, `api/client_secret`, `api/remember_creds`) on your machine only — never sent except to MangaDex's API. See [Security](/security) for the OS keychain roadmap.

## Features Overview

* **Library Grid**: 5 columns, 170×322 cards, filter by title/status/year/tags, pagination `Show more (60)` + `Show All (N)` (e.g., `3276 shown`).
* **Sorting**: Title A-Z/Z-A, Year Newest/Oldest, Status. Year/tag filters + status-color chips.
* **Bulk Actions**: Select, export, undo/redo (`Ctrl+Z` / `Ctrl+Y`), status editor (`Reading`/`Completed`/`On Hold`/`Plan to Read`/`Dropped`/`Re-reading`).
* **Update Check**: `Check for updates` → HitPaw icon popup (`You are on vX / Latest vY`) → `Download Latest Version` → `https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest`.
* **Cover Caching**: Disk-backed CDN retry/backoff, `Clear Cover Cache`, stats dashboard.

## Next

* [Building from Source](/building) — Qt 6.8.3 + MinGW 13.1, CMakePresets
* [Export Formats](/export) — which sites import which files
