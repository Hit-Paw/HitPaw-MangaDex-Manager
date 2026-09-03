---
title: Getting Started
description: Download HitPaw, create your free MangaDex API client, load your library, and export. 100% local & secure — under 2 minutes.
outline: deep
---

# Getting Started

No installer, no Qt needed. From download to **"Library loaded" in under 2 minutes**.

<div class="badge-row">
  <a href="/download" class="badge badge--accent">Windows • macOS • Linux</a>
  <span class="badge">Portable • ~80 MB</span>
  <a href="/changelog" class="badge">v3.5.0 Latest</a>
  <span class="badge">Offline first</span>
</div>

## Download

Grab the matching archive from **[Latest Release](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest)**:

| Platform | File | Extract & Run | Size |
|----------|------|---------------|------|
| **Windows** | `HitPawMangaDexManager-v*-windows-x64.zip` | `MangaDexExporter.exe` — DLLs bundled | ~78 MB |
| **macOS** | `HitPawMangaDexManager-v*-macOS.tar.gz` | `MangaDexExporter.app` or binary | ~65 MB |
| **Linux** | `HitPawMangaDexManager-v*-Linux.tar.gz` | `tar xzf … && ./MangaDexExporter` | ~62 MB |

> **No install needed.** On Windows all Qt DLLs are inside the zip — just extract and run. On Linux ensure `zlib1g` is installed. See [Download](/download) for SHA256 + SBOM verification.

## Prerequisites

- A **MangaDex account** (free) — [mangadex.org](https://mangadex.org)
- A **personal API client** — free, one-click setup (next step). HitPaw never proxies your login.

## Setup — 4 steps <Badge type="tip" text="~2 min" />

### 1. Launch & sign in

Open HitPaw. Enter your **MangaDex username + password**. HitPaw talks directly to `api.mangadex.org` over HTTPS — no middle server.

### 2. Create API client (one-time, 30s)

MangaDex → **Settings → API Clients → Create** → copy **Client ID** and **Client Secret**. Keep the secret private — it's like a password for the API.

::: tip Copy, don't type
Client ID/Secret are case-sensitive. Use the copy button on MangaDex to avoid typos.
:::

### 3. Paste into HitPaw

Paste **Client ID + Client Secret** into the login screen. Check **Remember credentials** for auto-load on next launch (stored locally via `QSettings` — see [Security](/security)).

### 4. Load Library

Click **Load Library**. You'll see skeletons → covers → your grid. Use `Show All (N)` to load everything at once — smooth even at 3000+ titles.

<div class="why-grid">
  <div class="why-card" style="text-align:center">
    <div class="icon-wrap" style="margin-bottom:10px"><img src="/icons/outline-layers.svg" alt="" width="22" height="22" loading="lazy" /></div>
    <strong>Pagination</strong><br/><span style="opacity:.78"><code>Show more (60)</code> incremental<br/> <code>Show All (N)</code> loads entire filtered set</span>
  </div>
  <div class="why-card" style="text-align:center">
    <div class="icon-wrap" style="margin-bottom:10px"><img src="/icons/outline-library.svg" alt="" width="22" height="22" loading="lazy" /></div>
    <strong>Filtering</strong><br/><span style="opacity:.78">Search title, status, year, tags<br/> Status chips are color-coded</span>
  </div>
  <div class="why-card" style="text-align:center">
    <div class="icon-wrap" style="margin-bottom:10px"><img src="/icons/outline-sync.svg" alt="" width="22" height="22" loading="lazy" /></div>
    <strong>Status refresh</strong><br/><span style="opacity:.78"><code>Refresh library</code> re-fetches & diffs<br/> Shows + added, ~ changed, − removed</span>
  </div>
</div>

::: details Still on skeletons?
MangaDex paginates (`limit=100`). HitPaw fetches IDs then titles in batches with retry/backoff. For 3k titles expect 15–30 s. Check **Activity Log** — if it stalls on `429 Ratelimit`, wait 60 s and retry. Covers may 429 briefly — `Clear Cover Cache` + `Refresh library` fixes it.
:::

## Where credentials live

| What | Where stored | Sent where |
|------|--------------|------------|
| Username/password, Client ID/Secret, tokens | `QSettings` on your machine only (Registry / plist / INI, per-user) | Only to MangaDex `api.mangadex.org` over HTTPS |
| Covers | Disk cache + memory | `uploads.mangadex.org` CDN |

Never committed to git — `scan-secrets` CI fails the build if secrets appear. Roadmap: OS keychain via `QKeychain` (Windows Credential Manager / macOS Keychain / libsecret). See [Security](/security) for hardening tips.

## Features Overview

- **Library Grid**: 5 columns, filter + sort, `Show All (N)` — entire library in one click, virtualized smooth scroll
- **Sorting**: Title A-Z/Z-A, Year Newest/Oldest, Status — with year/tag filters and status-color chips
- **Bulk Actions**: Multi-select → export / status edit → undo/redo (`Ctrl+Z` / `Ctrl+Y`)
- **Update Check**: Manual `Check for updates` or silent at launch (3.5 s delay) → one-click download
- **Cover Caching**: Disk-backed with CDN retry/backoff + `Clear Cover Cache` + stats dashboard
- **Export**: One click → validated files (see [Export Formats](/export)) — never modifies reading statuses for MDList sync

::: tip First export?
Select 2–3 titles → **Export** → pick **CSV** (opens in Excel) or **MAL XML** (MyAnimeList/AniList). Validate before importing — HitPaw runs `validateExportFile()` automatically.
:::

## Troubleshooting

| Symptom | Fix |
|---------|-----|
| `Invalid Client ID / Secret` | Regenerate at **MangaDex → Settings → API Clients** — ensure you copied both, no extra spaces |
| Covers grey / placeholders | CDN 429 — wait 30 s, **Clear Cover Cache**, check firewall for `uploads.mangadex.org` |
| `Load Library` slow | Filter first, then `Show All (N)` on subset — sorting is in-memory, network is the bottleneck |
| Remember me on shared PC | Uncheck **Remember credentials** — see [Security](/security) |

## Next

- [Export Formats](/export) — which sites import which files + MDList sync
- [Download](/download) — system requirements, SHA256, SBOM, what's inside the zip
- [Building from Source](/building) — Qt 6.8.3 + MinGW 13.1 steps, CMake presets, clang-format
- [Security](/security) — credential storage & vulnerability reporting
- [FAQ](/faq) — login, covers, export, performance

