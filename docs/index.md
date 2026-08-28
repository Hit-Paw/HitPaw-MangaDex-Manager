---
layout: home

hero:
  name: HitPaw MangaDex Manager
  text: Browse, filter, and export your MangaDex library.
  tagline: Desktop app built with Qt6/C++ — your library at 170×322, dark AMOLED UI, offline exports.
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
    details: 5 columns • fast filter by title/status/year/tags • your titles shown in one click with Show All (N) + Show more (60)
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

<div class="stats-bar">
  <div class="stat-card"><div class="stat-number" style="font-size:1.35em">Your Library</div><div class="stat-label">All titles in one grid — N = your bookmarks</div></div>
  <div class="stat-card"><div class="stat-number">5+</div><div class="stat-label">Export formats</div></div>
  <div class="stat-card"><div class="stat-number">3</div><div class="stat-label">Platforms</div></div>
  <div class="stat-card"><div class="stat-number">100%</div><div class="stat-label">Local & secure</div></div>
</div>

## Preview

<div class="preview-grid" style="display:grid;grid-template-columns:repeat(auto-fit,minmax(240px,1fr));gap:12px;margin:16px 0">
  <img src="/preview-1.png" alt="Library Grid — your titles" style="border-radius:12px;border:1px solid rgba(255,106,0,0.18);box-shadow:0 8px 24px rgba(0,0,0,0.18);cursor:zoom-in" data-full="/preview-1.png" />
  <img src="/preview-2.png" alt="Activity Log HitPaw icon" style="border-radius:12px;border:1px solid rgba(255,106,0,0.18);box-shadow:0 8px 24px rgba(0,0,0,0.18);cursor:zoom-in" data-full="/preview-2.png" />
  <img src="/preview-3.png" alt="Export Formats" style="border-radius:12px;border:1px solid rgba(255,106,0,0.18);box-shadow:0 8px 24px rgba(0,0,0,0.18);cursor:zoom-in" data-full="/preview-3.png" />
  <img src="/preview-4.png" alt="Download Chapters" style="border-radius:12px;border:1px solid rgba(255,106,0,0.18);box-shadow:0 8px 24px rgba(0,0,0,0.18);cursor:zoom-in" data-full="/preview-4.png" />
</div>

<div id="lightbox" class="lightbox" aria-hidden="true"><img alt="Preview enlarged" /><div class="lightbox-caption"></div></div>

> Not affiliated with or endorsed by MangaDex. Preview updated for **v3.4.6**.

## Why HitPaw?

<div class="why-grid">
  <div class="why-card" style="background:linear-gradient(135deg,rgba(255,106,0,0.06),rgba(0,0,0,0));border:1px solid rgba(255,106,0,0.12);border-radius:12px;padding:18px">
    <div class="icon-wrap"><img src="/icons/outline-zap.svg" alt="" width="22" height="22" /></div><div style="text-align:center"><strong style="color:#ff6a00">Instant Library</strong><br/>Load your entire filtered library at once — <code>Show All (N)</code> next to <code>Show more (60)</code>. No pagination limits.</div>
  </div>
  <div class="why-card" style="background:linear-gradient(135deg,rgba(255,106,0,0.06),rgba(0,0,0,0));border:1px solid rgba(255,106,0,0.12);border-radius:12px;padding:18px">
    <div class="icon-wrap"><img src="/icons/outline-puzzle.svg" alt="" width="22" height="22" /></div><div style="text-align:center"><strong style="color:#ff6a00">Smart Export</strong><br/>One click → CSV/JSON/MAL/AP/MangaBaka. Validated and ready for MyAnimeList, AniList, Kitsu and more.</div>
  </div>
  <div class="why-card" style="background:linear-gradient(135deg,rgba(255,106,0,0.06),rgba(0,0,0,0));border:1px solid rgba(255,106,0,0.12);border-radius:12px;padding:18px">
    <div class="icon-wrap"><img src="/icons/outline-palette.svg" alt="" width="22" height="22" /></div><div style="text-align:center"><strong style="color:#ff6a00">AMOLED Craft</strong><br/>Pure black (#000) UI, 5-column grid, skeleton loading, smooth 60fps scroll — built for binge browsers.</div>
  </div>
</div>

## Quick Start

<div class="quick-grid">
  <div class="quick-card" style="border:1px solid rgba(255,106,0,0.18);border-radius:12px;padding:18px;text-align:center">
    <div class="icon-wrap"><img src="/icons/outline-download.svg" alt="" width="28" height="28" /></div><strong>1. Download</strong><br/><span style="opacity:.85">Releases → <code>*-windows-x64.zip</code></span>
  </div>
  <div class="quick-card" style="border:1px solid rgba(255,106,0,0.18);border-radius:12px;padding:18px;text-align:center">
    <div class="icon-wrap"><img src="/icons/outline-package.svg" alt="" width="28" height="28" /></div><strong>2. Extract & Run</strong><br/><span style="opacity:.85"><code>MangaDexExporter.exe</code> — no installer</span>
  </div>
  <div class="quick-card" style="border:1px solid rgba(255,106,0,0.18);border-radius:12px;padding:18px;text-align:center">
    <div class="icon-wrap"><img src="/icons/outline-key.svg" alt="" width="28" height="28" /></div><strong>3. Login</strong><br/><span style="opacity:.85">MangaDex + API client → <code>Load Library</code></span>
  </div>
</div>

See [Getting Started](/getting-started) for full setup, [Building](/building) for source builds, and [Export Formats](/export) for site imports.

## Community

Join [Discord](https://discord.gg/z6yYYpcYYc) for support. Found a bug? Use [Bug report](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=bug_report.yml). Want a feature? [Feature request](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=feature_request.yml).

<div style="margin:24px 0;padding:20px;border-radius:12px;background:linear-gradient(135deg,#ff6a00 0%,#ff8c1a 100%);color:white;text-align:center">
  <div style="font-size:1.3em;font-weight:800;margin-bottom:6px">Ready to manage your library?</div>
  <div style="opacity:.9;margin-bottom:14px">Free, open-source, MIT — no ads, no tracking.</div>
  <a href="https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest" style="display:inline-block;background:white;color:#ff6a00;font-weight:700;padding:10px 18px;border-radius:8px;text-decoration:none;transition:transform .2s" onmouseover="this.style.transform='scale(1.04)'" onmouseout="this.style.transform='scale(1)'">Download HitPaw Manager →</a>
</div>
