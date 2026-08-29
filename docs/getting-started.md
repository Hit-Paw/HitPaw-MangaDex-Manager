---
title: Getting Started — HitPaw MangaDex Manager
description: Download HitPaw, create your free MangaDex API client, load your library, and export. 100% local & secure.
outline: deep
---

# Getting Started

No installer, no Qt needed. From download to "Library loaded" in under 2 minutes.

<div class="badge-row">
  <span class="badge badge--accent">Windows • macOS • Linux</span>
  <span class="badge">Portable • ~80 MB</span>
  <span class="badge">v3.4.6</span>
</div>

## Download

Grab the matching archive from **[Latest Release](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest)**:

| Platform | File | Extract & Run |
|----------|------|---------------|
| **Windows** | `HitPawMangaDexManager-v*-windows-x64.zip` | `MangaDexExporter.exe` — DLLs bundled |
| **macOS** | `HitPawMangaDexManager-v*-macOS.tar.gz` | `MangaDexExporter.app` or binary |
| **Linux** | `HitPawMangaDexManager-v*-Linux.tar.gz` | `./MangaDexExporter` |

> No install needed. On Windows all Qt DLLs are inside the zip — just extract and run.

## Prerequisites

- A **MangaDex account** (free)
- A **personal API client** — free, one-click setup (next step)

## Setup — 4 steps

### 1. Launch & sign in

Open HitPaw. Enter your **MangaDex username + password**.

### 2. Create API client (one-time)

MangaDex → **Settings → API Clients → Create** → copy **Client ID** and **Client Secret**.

### 3. Paste into HitPaw

Paste **Client ID + Client Secret** into the login screen. Check **Remember credentials** for auto-load on next launch.

### 4. Load Library

Click **Load Library**. You'll see skeletons → covers → your grid. Use `Show All (N)` to load everything at once.

<div class="why-grid">
  <div class="why-card" style="text-align:center">
    <strong>Pagination</strong><br/><span style="opacity:.78"><code>Show more (60)</code> for incremental loading<br/> <code>Show All (N)</code> loads your entire library</span>
  </div>
  <div class="why-card" style="text-align:center">
    <strong>Filtering</strong><br/><span style="opacity:.78">Search by title, status, year, or tags<br/> Status chips are color-coded</span>
  </div>
  <div class="why-card" style="text-align:center">
    <strong>Status refresh</strong><br/><span style="opacity:.78"><code>Refresh library</code> re-fetches & diffs<br/> Shows + added, ~ changed, − removed</span>
  </div>
</div>

## Where credentials live

| What | Where | Sent where |
|------|-------|------------|
| Username/password, Client ID/Secret, tokens | `QSettings` on your machine only | Only to MangaDex's API (`api.mangadex.org`) |

Never committed to git — `scan-secrets` CI fails the build if secrets appear. See [Security](/security) for the OS keychain roadmap.

## Features Overview

- **Library Grid**: 5 columns, filter + sort, `Show All (N)` — entire library in one click
- **Sorting**: Title, Year, Status — with year/tag filters and status-color chips
- **Bulk Actions**: Multi-select, export, undo/redo (`Ctrl+Z` / `Ctrl+Y`), status editor
- **Update Check**: Manual or silent auto-check at launch — one-click download when new version drops
- **Cover Caching**: Disk-backed with CDN retry/backoff, `Clear Cover Cache` option
- **Export**: One click → validated files (see [Export Formats](/export))

::: tip First export?
Select a few titles → **Export** → pick **CSV** (opens in Excel) or **MAL XML** (MyAnimeList/AniList).
:::

## Next

- [Export Formats](/export) — which sites import which files
- [Download](/download) — system requirements, checksums
- [Building from Source](/building) — Qt 6.8.3 + MinGW 13.1 steps
- [Security](/security) — credential storage & vulnerability reporting
