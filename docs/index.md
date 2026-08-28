---
layout: home

hero:
  name: HitPaw MangaDex Manager
  text: Browse, filter, and export your MangaDex library.
  tagline: Desktop app built with Qt6/C++ — 5-column AMOLED grid, Show All (N), offline exports. 100% local.
  image:
    src: /icon_256.png
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
      src: /icons/outline-library.svg
      alt: Library grid
      width: 24
      height: 24
    title: Library Grid
    details: 5 columns • 170×322 cards • filter by title/status/year/tags • Show All (N) + Show more (60). Instant.
  - icon:
      src: /icons/outline-sync.svg
      alt: Sync
      width: 24
      height: 24
    title: Stay Up to Date
    details: Manual + silent checks. HitPaw icon popups — “You are on vX / Latest vY” → one-click Download Latest.
  - icon:
      src: /icons/outline-export.svg
      alt: Export
      width: 24
      height: 24
    title: Export Anywhere
    details: CSV • JSON • MAL XML (MyAnimeList/AniList/MangaBaka/Kitsu/MangaFire) • AP .xml.gz • MangaBaka JSON. Validated.
  - icon:
      src: /icons/outline-lock.svg
      alt: Lock
      width: 24
      height: 24
    title: Secure & Local
    details: QSettings only, never hardcoded. OS keychain roadmap (QKeychain). No tracking, no ads.
  - icon:
      src: /icons/outline-palette.svg
      alt: Palette
      width: 24
      height: 24
    title: Dark AMOLED UI
    details: Pure #000, skeleton loading, status-color chips, cover caching with CDN retry/backoff. 60fps.
  - icon:
      src: /icons/outline-layers.svg
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

<div class="stats-bar">
  <div class="stat-card"><div class="stat-number stat-number--small">Your Library</div><div class="stat-label">All titles in one grid — N = your bookmarks</div></div>
  <div class="stat-card"><div class="stat-number">5+</div><div class="stat-label">Export formats • validated</div></div>
  <div class="stat-card"><div class="stat-number">3</div><div class="stat-label">Platforms • Qt 6.8.3</div></div>
  <div class="stat-card"><div class="stat-number">100%</div><div class="stat-label">Local & secure • offline</div></div>
</div>

## Preview

<div class="preview-grid">

  <div class="preview-card">
    <img src="/preview-1.png" alt="Library Grid — 3276 titles, 5 columns, filter by status/year/tags" width="1100" height="866" loading="lazy" decoding="async" />
    <div class="preview-caption"><span>Library Grid</span> <small>5 cols • filter • Show All (N)</small></div>
  </div>

  <div class="preview-card">
    <img src="/preview-2.png" alt="Activity Log & Update Check — HitPaw icon, You are up to date" width="800" height="520" loading="lazy" decoding="async" />
    <div class="preview-caption"><span>Activity Log</span> <small>HitPaw icon • update check</small></div>
  </div>

  <div class="preview-card">
    <img src="/preview-3.png" alt="Export — Output Folder & validated formats" width="800" height="520" loading="lazy" decoding="async" />
    <div class="preview-caption"><span>Export</span> <small>CSV / JSON / MAL / AP / MB</small></div>
  </div>

  <div class="preview-card">
    <img src="/preview-4.png" alt="Download — Paste URL & chapter queue" width="800" height="520" loading="lazy" decoding="async" />
    <div class="preview-caption"><span>Download</span> <small>Queue • retry/backoff</small></div>
  </div>

</div>

<p class="lightbox-hint"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="11" cy="11" r="7"/><path d="M11 8v6M8 11h6M21 21l-3.5-3.5"/></svg> Click any preview to enlarge — press Esc to close</p>

<div id="lightbox" class="lightbox" aria-hidden="true" role="dialog" aria-modal="true" aria-label="Image preview">
  <img alt="Preview enlarged" width="1200" height="800" decoding="async" />
  <div class="lightbox-caption"></div>
  <button class="lightbox-close" type="button" aria-label="Close preview"><svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M18 6L6 18"/><path d="M6 6l12 12"/></svg></button>
</div>

> **v3.4.6** — grid fixed to 5 cols, `Show All (N)` added, update popups now use HitPaw icon (48×48). See [Changelog](/changelog).

## Why HitPaw?

<div class="why-grid">
  <div class="why-card">
    <div class="icon-wrap"><img src="/icons/outline-zap.svg" alt="" width="22" height="22" aria-hidden="true" /></div>
    <div style="text-align:center"><strong style="color:#ff6a00">Instant Library</strong><br/>Load your entire filtered library at once — <code>Show All (N)</code> next to <code>Show more (60)</code>. No pagination limits, no waiting.</div>
  </div>
  <div class="why-card">
    <div class="icon-wrap"><img src="/icons/outline-puzzle.svg" alt="" width="22" height="22" aria-hidden="true" /></div>
    <div style="text-align:center"><strong style="color:#ff6a00">Smart Export</strong><br/>One click → CSV/JSON/MAL/AP/MangaBaka. Validated & import-ready for MyAnimeList, AniList, Kitsu and more.</div>
  </div>
  <div class="why-card">
    <div class="icon-wrap"><img src="/icons/outline-palette.svg" alt="" width="22" height="22" aria-hidden="true" /></div>
    <div style="text-align:center"><strong style="color:#ff6a00">AMOLED Craft</strong><br/>Pure black (#000) UI, 5-column grid, skeleton loading, smooth 60fps scroll — built for binge browsers.</div>
  </div>
</div>

## Quick Start

<div class="quick-grid">
  <div class="quick-card">
    <div class="icon-wrap"><img src="/icons/outline-download.svg" alt="" width="28" height="28" aria-hidden="true" /></div>
    <strong>1. Download</strong><br/><span style="opacity:.84">Releases → <code>HitPawMangaDexManager-v*-windows-x64.zip</code></span>
    <div style="margin-top:10px"><a href="https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest" target="_blank" rel="noreferrer">Latest release →</a></div>
  </div>
  <div class="quick-card">
    <div class="icon-wrap"><img src="/icons/outline-package.svg" alt="" width="28" height="28" aria-hidden="true" /></div>
    <strong>2. Extract & Run</strong><br/><span style="opacity:.84"><code>MangaDexExporter.exe</code> — no installer, DLLs bundled</span>
    <div style="margin-top:10px; font-size:0.84rem; opacity:.68">Windows 10/11 • 200 MB free • no Qt needed</div>
  </div>
  <div class="quick-card">
    <div class="icon-wrap"><img src="/icons/outline-key.svg" alt="" width="28" height="28" aria-hidden="true" /></div>
    <strong>3. Login</strong><br/><span style="opacity:.84">MangaDex + API client → <code>Load Library</code></span>
    <div style="margin-top:10px"><a href="/getting-started">Setup guide →</a></div>
  </div>
</div>

::: tip New here?
MangaDex issues personal API clients for free — **MangaDex → Settings → API Clients → Create**. You’ll paste the **Client ID + Secret** once. They stay on your machine via `QSettings` (see [Security](/security)).
:::

## HitPaw vs. manual

|  | **HitPaw** | Manual export | Browser bookmark |
|---|------------|---------------|----------------|
| Load 3k titles | One click `Show All (N)` | 50 at a time, paginate | — |
| Filter by year/tag/status | Instant client-side | Manual search | — |
| Export to MAL/AniList | Validated XML/JSON/.gz | Copy-paste each title | — |
| Update check | Silent + manual popup | Check GitHub yourself | — |
| Works offline exports | ✅ Cached covers, retries | ❌ | ❌ |
| Platform | Win / macOS / Linux portable | Browser only | — |

## What users get

- **Bulk-select + undo/redo** (`Ctrl+Z` / `Ctrl+Y`) with status chips — `Reading • Completed • On Hold • Plan to Read • Dropped • Re-reading`
- **Sort & filter** — Title A-Z/Z-A, Year Newest/Oldest, Status. Year/tag dropdowns that don’t clip.
- **Cover cache** — disk CDN, retry/backoff, `Clear Cover Cache`, stats dashboard (`Years: 2026:143 … Total 3276`)
- **No tracking** — MIT, no ads, no telemetry. Credentials never leave your machine except to MangaDex’s own API.

See [Getting Started](/getting-started) for full setup, [Building](/building) for source builds, and [Export Formats](/export) for import steps per site.

## Community

HitPaw is built in the open. Join the conversation, report a bug, or request a feature:

- 💬 **Discord**: [discord.gg/z6yYYpcYYc](https://discord.gg/z6yYYpcYYc) — support, suggestions, sneak peeks
- 🐛 **Bug report**: [Open an issue](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=bug_report.yml) — include `v3.4.6`, OS, steps
- 💡 **Feature request**: [Request a feature](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=feature_request.yml)
- ✏️ **Docs**: every page has **Edit this page on GitHub** — PRs welcome (see [Contributing](/contributing))

<div class="cta-banner">
  <div class="cta-title">Ready to manage your library?</div>
  <div class="cta-sub">Free, open-source, MIT — no ads, no tracking. Portable on Windows, macOS, Linux.</div>
  <a class="cta-btn" href="https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest" target="_blank" rel="noreferrer">Download HitPaw Manager →</a>
  <div style="margin-top:10px; font-size:0.82rem; opacity:.88">v3.4.6 • SHA256 + SBOM on each release • <a href="/download" style="color:white; text-decoration:underline; text-underline-offset:3px">All downloads</a> • <a href="/changelog" style="color:white; text-decoration:underline; text-underline-offset:3px">Changelog</a></div>
</div>
