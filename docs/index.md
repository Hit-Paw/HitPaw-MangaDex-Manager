---
layout: home

hero:
  name: HitPaw MangaDex Manager
  text: Browse, filter, and export your MangaDex library.
  tagline: Desktop app built with Qt6/C++ — 5-column AMOLED grid, Show All (N), offline exports. 100% local.
  image:
    src: https://raw.githubusercontent.com/Hit-Paw/HitPaw-MangaDex-Manager/main/icon_256.png
    alt: HitPaw MangaDex Manager — AMOLED library grid
  actions:
    - theme: brand
      text: Download Latest
      link: https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest
    - theme: alt
      text: Getting Started
      link: /getting-started
    - theme: alt
      text: View on GitHub
      link: https://github.com/Hit-Paw/HitPaw-MangaDex-Manager
features:
  - icon:
      src: https://raw.githubusercontent.com/Hit-Paw/HitPaw-MangaDex-Manager/main/icons/outline-library.svg
      alt: Library grid
      width: 24
      height: 24
    title: Library Grid
    details: 5 columns • 170×322 cards • filter by title/status/year/tags • Show All (N) + Show more (60). Instant.
  - icon:
      src: https://raw.githubusercontent.com/Hit-Paw/HitPaw-MangaDex-Manager/main/icons/outline-sync.svg
      alt: Sync
      width: 24
      height: 24
    title: Stay Up to Date
    details: Manual + silent checks. HitPaw icon popups — “You are on vX / Latest vY” → one-click Download Latest.
  - icon:
      src: https://raw.githubusercontent.com/Hit-Paw/HitPaw-MangaDex-Manager/main/icons/outline-export.svg
      alt: Export
      width: 24
      height: 24
    title: Export Anywhere
    details: CSV • JSON • MAL XML (MyAnimeList/AniList/MangaBaka/Kitsu/MangaFire) • AP .xml.gz • MangaBaka JSON. Validated.
  - icon:
      src: https://raw.githubusercontent.com/Hit-Paw/HitPaw-MangaDex-Manager/main/icons/outline-lock.svg
      alt: Lock
      width: 24
      height: 24
    title: Secure & Local
    details: QSettings only, never hardcoded. OS keychain roadmap (QKeychain). No tracking, no ads.
  - icon:
      src: https://raw.githubusercontent.com/Hit-Paw/HitPaw-MangaDex-Manager/main/icons/outline-palette.svg
      alt: Palette
      width: 24
      height: 24
    title: Dark AMOLED UI
    details: Pure #000, skeleton loading, status-color chips, cover caching with CDN retry/backoff. 60fps.
  - icon:
      src: https://raw.githubusercontent.com/Hit-Paw/HitPaw-MangaDex-Manager/main/icons/outline-layers.svg
      alt: Layers
      width: 24
      height: 24
    title: Cross-Platform
    details: Windows 10/11 x64 (bundled DLLs) • macOS • Linux — Qt 6.8.3 + MinGW 13.1, CMake & qmake.
---

<div class="badge-row">
  <span class="badge badge--accent">v3.4.6 • Latest</span>
  <span class="badge">Windows • macOS • Linux</span>
  <span class="badge">MIT • Open Source</span>
  <span class="badge">No install • Portable</span>
  <span class="badge badge--muted">Not affiliated with MangaDex</span>
</div>

## Preview

<div class="preview-grid">

  <div class="preview-card">
    <img src="https://raw.githubusercontent.com/Hit-Paw/HitPaw-MangaDex-Manager/main/screenshots/preview-1.png" alt="Library Grid — 3276 titles" loading="lazy" decoding="async" />
    <div class="preview-caption"><span>Library Grid</span> <small>5 cols • filter • Show All (N)</small></div>
  </div>

  <div class="preview-card">
    <img src="https://raw.githubusercontent.com/Hit-Paw/HitPaw-MangaDex-Manager/main/screenshots/preview-2.png" alt="Activity Log & Update Check" loading="lazy" decoding="async" />
    <div class="preview-caption"><span>Activity Log</span> <small>HitPaw icon • update check</small></div>
  </div>

  <div class="preview-card">
    <img src="https://raw.githubusercontent.com/Hit-Paw/HitPaw-MangaDex-Manager/main/screenshots/preview-3.png" alt="Export — Output Folder & validated formats" loading="lazy" decoding="async" />
    <div class="preview-caption"><span>Export</span> <small>CSV / JSON / MAL / AP / MB</small></div>
  </div>

  <div class="preview-card">
    <img src="https://raw.githubusercontent.com/Hit-Paw/HitPaw-MangaDex-Manager/main/screenshots/preview-4.png" alt="Download — Paste URL & chapter queue" loading="lazy" decoding="async" />
    <div class="preview-caption"><span>Download</span> <small>Queue • retry/backoff</small></div>
  </div>

</div>

> **v3.4.6** — grid fixed to 5 cols, `Show All (N)` added, update popups now use HitPaw icon (48×48). See [CHANGELOG.md](/CHANGELOG.md).

## Quick Start

- Download the latest release → https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases
- Extract and run (no installer)
- Sign in & create a personal MangaDex API client (MangaDex → Settings → API Clients)

## Community

- Discord: https://discord.gg/z6yYYpcYYc
- Issues: https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues