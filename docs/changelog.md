---
title: Changelog — HitPaw MangaDex Manager
description: All notable changes to HitPaw — v3.4.6 with HitPaw icon popups, Show All (N), and grid fixes.
outline: deep
---

# Changelog

All notable changes are documented here. Version is single-sourced in `CMakeLists.txt` `project(VERSION ...)` and tagged `v*`.

<div class="badge-row">
  <span class="badge badge--accent">Latest: v3.4.6 — Aug 27, 2026</span>
  <span class="badge">MIT</span>
  <span class="badge"><a href="https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases">Releases →</a></span>
</div>

> **TL;DR v3.4.6** — update popups now all use HitPaw icon (48×48), `Show All (N)` added alongside `Show more (60)`, grid fixed to 5 cols, status colors in filters, and `moc` build fix. See details below.

## [3.4.6] - 2026-08-27

### Fixed

- **Update popup icons** — `onUpdateCheckFinished()` now uses HitPaw icon `QPixmap(":/icons/icon_64.png")` 48×48 `setIconPixmap` (`main.cpp:3024`, `3035`, `3044`, `3105`, `3120`) for all manual `QMessageBox` popups instead of default blue `i` / yellow warning. Covers: `Update Check Failed` (network `reply->errorString()`, parse, tag, version), `Up to Date` (`You are up to date! vX (latest vY)`), fallback `Check for Updates`, and `Update Available` (`Download Latest Version` → [`releases/latest`](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest)).

## [3.4.5] - 2026-08-27

### Added

- **Manual update check popup** — `Check for updates` (`GhostButton` `main.cpp:1926`) now triggers `manualCheckForUpdates()` (`main.cpp:3067`) with `m_manualUpdateCheck` flag (`main.cpp:1231`). `onUpdateCheckFinished()` (`main.cpp:3010`) shows `QMessageBox`: if outdated → `Update Available! You are on vX, latest is vY. You are outdated.` + `Download Latest Version` (`QDesktopServices::openUrl(...)`); if up to date → `You are up to date! vX (latest vY)`; if failed → `Update Check Failed` warning. Silent `checkForUpdates()` (`QTimer::singleShot 3500`, `main.cpp:1089`) banner/logs remain.

## [3.4.4] - 2026-08-27

### Removed

- Grid density toggle: `m_densityBtn` removed, grid fixed to `GRID_COLS=5` (170×322 cards, cover 156×222), skeletons fixed to `W=170 H=322 CW=156 CH=222` (`main.cpp:3675`) and `i / GRID_COLS` (`main.cpp:3726`).

### Added

- **Load all** — `m_showAllBtn` (`GhostButton "Show All"` / `Hide`, tooltip *Load all remaining titles at once*) alongside `m_loadMoreBtn` in `paginationRow` (`main.cpp:2028`) — `Show All (N)` sets `m_paginationLimit = totalMatched` and `relayoutLibrary()` to load entire library at once; `Show more (N remaining)` keeps `PAGINATION_STEP=60` incremental.

## [3.4.3] - 2026-08-26

### Fixed

- `All status` dropdown now shows status colors via `statusColor(key)` + `QBrush`, current text colored via `updateFilterStatusColor()` stylesheet, matching `m_bulkStatusBox` fix in `3.4.1`.
- Filter bar text clipping fixed: `m_filterYear` / `m_filterTag` `setFixedHeight(30)` → `34` with `Pal::CARD` / `BORDER` styles.

## [3.4.2] - 2026-08-26

### Fixed

- Build failure: `Qt 6.11.1 moc` `No relevant classes found` due to raw string `R"([\\/:*?"<>|])"` containing `?"` — fixed via normal string `"[\\\\/:*?\"<>|]"` and `uuid` regex, restoring `main.moc` and `MangaDexExporter.exe`.

::: details Older releases — click to expand

## [3.4.1] - 2026-08-25

### Fixed

- Bulk move dropdown status colors — each item tinted via `statusColor(key)` + `QBrush`, current text colored via `updateBulkColor()`.

## [3.4.0] - 2026-08-25

### Added

- Toolbar grouping with `VLine` separators (`main.cpp:2065`), density toggle 5↔6, status-color filter chip (`Search: "x" · Status: Reading · Sort: Year`), skeleton loading with `QGraphicsOpacityEffect` pulse `900ms`.

## [3.3.4] - 2026-08-25

### Removed

- Export preview (first 10 table + estimated sizes) — per request to keep Export tab clean.

## [3.3.0] - 2026-08-25

### Added

- Bulk status editor (`Reading/Completed/...` via `POST /manga/{id}/status`, 350ms rate-limit), cover cache manager (`Clear Cover Cache` + `X files, Y MB`), stats dashboard (`Years: 2024:12 … Total N`), export preview.

## [3.2.x] - 2026-08-25

- **3.2.2** — Refresh requires loaded library (`Load your library first…`), diffs `added/removed/changed` with colored logs + popup if ≤12.
- **3.2.1** — `Redo` (mirrored Undo via `QTransform`) + `Refresh` (`icons8-refresh-48-text.png`) with `UNDO_MAX=50` stacks.
- **3.2.0** — Crazy long titles now wrap to as many lines as needed (`wordWrap:true` + card `MinimumHeight`), grid handles variable heights.

## [3.1.x] - 2026-08-25

- **3.1.9** — Long quoted titles fixed: `maxLines=3` via `QFontMetrics` + greedy `elideToLines()`, still fits `H=322`.
- **3.1.8** — Sorting (`Title A-Z/Z-A, Year Newest/Oldest, Status`), export validation (`QXmlStreamReader` + `user_total_manga` + `gzopen`), persistent download queue (`QSettings`), pagination (`PAGINATION_STEP=60`, `X / Y shown`).
- **3.1.7** — Silent update checker (3.5s after launch, `api.github.com/repos/.../releases/latest`, 8s timeout, `User-Agent: HitPaw-MangaDex-Manager`).
- **3.1.6** — Title truncation 4px overflow fixed via `QFontMetrics` + 2-line elide.
- **3.1.5** — Scroll glitching fixed (removed `QGraphicsOpacityEffect` offscreen buffer).
- **3.1.4** — Chapter order `insertWidget(count()-1)` → `addWidget()` fix.
- **3.1.3** — About dialog credits + Discord link, version string fix.
- **3.1.2** — Chapter sort via `QLocale::c()` (locale-independent).
- **3.1.1** — MAL XML field names (`manga_num_volumes`, `manga_num_chapters`, `<myinfo>`) fixed; AP gzipped same.

## [3.1.0] - 2026-08-24

### Added

- Initial public release — library grid, login via username/password or API client, bulk-select + export with undo, CSV/JSON/MAL/MB/AP exports, cover caching with retry/backoff, AMOLED UI, momentum scroll, Win/macOS/Linux.

:::

Full git log: [`CHANGELOG.md`](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/blob/main/CHANGELOG.md) at repo root.
