---
layout: home

hero:
  name: HitPaw MangaDex Manager
  text: Browse, filter, and export your MangaDex library.
  tagline: Desktop app built with Qt6/C++ — 3276 titles at 170×322, dark AMOLED UI, offline exports.
  image:
    src: /icon_256.png
    alt: HitPaw Icon
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
      src: /icons/outline-library.svg
      alt: Library grid outline
      width: 24
      height: 24
    title: Library Grid
    details: 5 columns • fast filter by title/status/year/tags • 3276 shown in one click with Show All (N) + Show more (60)
  - icon:
      src: /icons/outline-sync.svg
      alt: Sync outline
      width: 24
      height: 24
    title: Stay Up to Date
    details: Check for updates popup with HitPaw icon — You are on vX / Latest vY / You are outdated → Download Latest
  - icon:
      src: /icons/outline-export.svg
      alt: Export outline
      width: 24
      height: 24
    title: Export Anywhere
    details: CSV, JSON, MAL XML (MyAnimeList/AniList/MangaBaka/Kitsu/MangaFire), MangaBaka JSON, Anime-Planet .xml.gz with validation
  - icon:
      src: /icons/outline-lock.svg
      alt: Lock outline
      width: 24
      height: 24
    title: Secure & Local
    details: Credentials via QSettings only (never hardcoded) — stored locally, OS keychain roadmap (QKeychain)
  - icon:
      src: /icons/outline-palette.svg
      alt: Palette outline
      width: 24
      height: 24
    title: Dark AMOLED UI
    details: Smooth scrolling, skeleton loading, status-color chips, cover caching with CDN retry/backoff
  - icon:
      src: /icons/outline-layers.svg
      alt: Layers outline
      width: 24
      height: 24
    title: Cross-Platform
    details: Windows 10/11 x64 (bundled DLLs) • macOS • Linux — Qt 6.8.3 + MinGW 13.1, CMakePresets + qmake
---

## Preview

<div style="display:grid;grid-template-columns:repeat(auto-fit,minmax(240px,1fr));gap:12px;margin:16px 0">
  <img src="/preview-1.png" alt="Library Grid 3276 titles" style="border-radius:8px;border:1px solid #222" />
  <img src="/preview-2.png" alt="Activity Log HitPaw icon" style="border-radius:8px;border:1px solid #222" />
  <img src="/preview-3.png" alt="Export Formats" style="border-radius:8px;border:1px solid #222" />
  <img src="/preview-4.png" alt="Download Chapters" style="border-radius:8px;border:1px solid #222" />
</div>

> Not affiliated with or endorsed by MangaDex. Preview updated for **v3.4.6**.

## Quick Start

1. Download from [Releases](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases) — `HitPawMangaDexManager-v*-windows-x64.zip` etc.
2. Extract and run `MangaDexExporter.exe` (Windows) or bundle (macOS/Linux)
3. Login with MangaDex username/password + personal API client (`Settings → API Clients`)

See [Getting Started](/getting-started) for full setup, [Building](/building) for source builds, and [Export Formats](/export) for site imports.

## Community

Join [Discord](https://discord.gg/z6yYYpcYYc) for support. Found a bug? Use [Bug report](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=bug_report.yml). Want a feature? [Feature request](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=feature_request.yml).
