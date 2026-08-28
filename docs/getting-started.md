---
title: Getting Started — HitPaw MangaDex Manager
description: Download HitPaw, create your free MangaDex API client, load your library, and export. 100% local & secure.
outline: deep
---

# Getting Started

HitPaw runs portably — no installer, no Qt needed. Get from download to “Library loaded” in under 2 minutes.

<div class="badge-row">
  <span class="badge badge--accent">Windows • macOS • Linux</span>
  <span class="badge">Portable • 200 MB</span>
  <span class="badge">v3.4.6</span>
</div>

## Download

Grab the matching archive from **[Latest Release](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest)** (or [All Releases](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases)):

| Platform | File | Extract & Run |
|----------|------|---------------|
| **Windows** | `HitPawMangaDexManager-v*-windows-x64.zip` | `MangaDexExporter.exe` — DLLs bundled |
| **macOS** | `HitPawMangaDexManager-v*-macOS.tar.gz` | `MangaDexExporter.app` or binary |
| **Linux** | `HitPawMangaDexManager-v*-Linux.tar.gz` | `./MangaDexExporter` |

> No install. On Windows all Qt 6.8.3 DLLs are inside the zip — just extract and double-click.

::: details Verify checksum (optional but recommended)
Each release attaches SHA256 + SBOM. Quick check:

```bash
# Windows PowerShell
Get-FileHash HitPawMangaDexManager-v3.4.6-windows-x64.zip -Algorithm SHA256

# macOS / Linux
shasum -a 256 HitPawMangaDexManager-v3.4.6-macOS.tar.gz
```

Compare to the `.sha256` file on the release page. See [Download](/download) for full hashes list.
:::

## Prerequisites

- A **MangaDex account** (free) — you’ll sign in inside HitPaw.
- A **personal API client** — MangaDex issues these for free in one click (next step). No coding needed.

## Setup — 4 steps

### 1. Launch & sign in

Open HitPaw. Enter your **MangaDex username + password**.

### 2. Create API client (one-time)

MangaDex → **Settings → API Clients → Create** → copy **Client ID** and **Client Secret**.

> Screenshot path: MangaDex top-right avatar → Settings → scroll to *API Clients* → *Create new client* → name it “HitPaw” → copy both fields.

### 3. Paste into HitPaw

Paste **Client ID + Client Secret** into the login screen. Check **Remember credentials** if you want auto-load next launch.

### 4. Load Library

Click **Load Library**. You’ll see skeletons → covers → your grid. Use pagination or `Show All (N)` to see everything.

<div class="why-grid">
  <div class="why-card" style="text-align:center">
    <strong>Pagination</strong><br/><span style="opacity:.78"><code>Show more (60)</code> loads 60 at a time<br/> <code>Show All (N)</code> loads your entire filtered library at once</span>
  </div>
  <div class="why-card" style="text-align:center">
    <strong>Filtering</strong><br/><span style="opacity:.78">Search title • Status • Year • Tags<br/> Status chips are color-coded</span>
  </div>
  <div class="why-card" style="text-align:center">
    <strong>Status refresh</strong><br/><span style="opacity:.78"><code>Refresh library</code> re-fetches & diffs<br/> Shows <em>+ added • ~ changed • − removed</em></span>
  </div>
</div>

## Where credentials live

| What | Where | Sent where |
|------|-------|------------|
| Username/password, Client ID/Secret, tokens | `QSettings` on your machine only (`api/client_id`, `api/client_secret`, `api/remember_creds`) | Only to MangaDex’s own API (`api.mangadex.org`) |
| Nothing else | — | — |

Never committed to git — `scan-secrets` CI fails the build if `gho_`/`ghp_` appears. See [Security](/security) for OS keychain roadmap (`QKeychain` → Windows Credential Manager / macOS Keychain / libsecret).

## Features Overview

- **Library Grid**: 5 columns • 170×322 cards (cover 156×222) • filter + sort • `Show All (N)` — 3276 titles in one click if that’s your library.
- **Sorting**: Title A-Z/Z-A • Year Newest/Oldest • Status. Year/tag filters + status-color chips + clear pill.
- **Bulk Actions**: Multi-select • export • undo/redo (`Ctrl+Z` / `Ctrl+Y`) • status editor → `Reading/Completed/On Hold/Plan to Read/Dropped/Re-reading` (batches with 350ms rate-limit).
- **Update Check**: `Check for updates` → HitPaw icon popup (`You are on vX / Latest vY / You are outdated → Download Latest Version` → `releases/latest`). Silent auto-check also runs at launch (3500ms).
- **Cover Caching**: Disk-backed `AppDataLocation/covers`, CDN retry/backoff, `Clear Cover Cache`, stats dashboard (`Years: … Total N`).
- **Export**: One click → validated files (see [Export Formats](/export)).

::: tip First export?
Select a few titles → **Export** → pick **CSV** (opens in Excel) or **MAL XML** (MyAnimeList/AniList). The app validates files and shows an import guide. Try `tests/test_export.cpp` via `ctest` to see validation in CI.
:::

## Next

- [Building from Source](/building) — exact Qt 6.8.3 + MinGW 13.1, `CMakePresets.json`, `clang-format`
- [Export Formats](/export) — which sites import which files + import steps
- [Download](/download) — system requirements, checksums, SBOM
- [Security](/security) — credential storage & reporting vulnerabilities
