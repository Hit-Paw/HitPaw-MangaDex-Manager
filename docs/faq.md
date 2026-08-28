---
title: FAQ — HitPaw MangaDex Manager
description: Frequently asked questions — login, covers, export, performance, and troubleshooting for HitPaw.
outline: deep
---

# FAQ

<div class="badge-row">
  <span class="badge badge--accent">Common questions</span>
  <span class="badge">Updated for v3.4.6</span>
</div>

## Login & Library

<details>
<summary><strong>Why does “Load Library” stay on skeletons?</strong></summary>

MangaDex paginates (`limit=100`). HitPaw fetches `m_allIds` first, then titles in batches with retry/backoff. On 3k titles it takes 15–30s. Check **Activity Log** — `Fetching page 12/33…` means it’s working. If it stalls on `429 Ratelimit`, wait 60s — `Retry-After` is honored (`main.cpp` `apiGetJson`).
</details>

<details>
<summary><strong>Invalid Client ID / Secret?</strong></summary>

Regenerate at **MangaDex → Settings → API Clients**. The pair is **case-sensitive** — copy, don’t type. Ensure `Client Secret` wasn’t regenerated without updating HitPaw. `QSettings` keys: `api/client_id`, `api/client_secret`.
</details>

<details>
<summary><strong>Does “Remember credentials” store my password in plain text?</strong></summary>

Currently via `QSettings` (per-user, per-OS: Registry on Windows, plist on macOS, INI on Linux). It’s not encrypted on Windows until the planned `QKeychain` migration (see [Security](/security)). On shared machines, uncheck it and sign out.
</details>

## Covers & Grid

<details>
<summary><strong>Covers not loading / grey placeholders?</strong></summary>

Cover CDN (`uploads.mangadex.org`) can 429. HitPaw retries with exponential backoff and disk cache (`AppDataLocation/covers`). Use **`Clear Cover Cache`** then `Refresh library`. If still grey, check firewall / DNS — covers need `https://api.mangadex.org` + `uploads.mangadex.org`.
</details>

<details>
<summary><strong>Grid shows X / Y — why not all?</strong></summary>

Pagination: `Show more (60)` increments `m_paginationLimit` by 60, `Show All (N)` sets it to `totalMatched`. 3k widgets at once is heavy — 60 batches keep it 60fps. `m_countLbl` shows `3276 shown` when all loaded.
</details>

## Export

<details>
<summary><strong>Which file for MyAnimeList vs Anime-Planet?</strong></summary>

- **MAL XML** (`mangadex_library_MAL.xml`) → MyAnimeList, AniList, MangaBaka (MAL mode), Kitsu, MangaFire
- **MangaBaka JSON** (`mangadex_library_MangaBaka.json`) → `mangabaka.org/settings/import` native mode
- **AP .gz** (`mangadex_library_AP.xml.gz`) → Anime-Planet (requires gzip — plain XML will be rejected)

See [Export Formats](/export) for step-by-step.
</details>

<details>
<summary><strong>Export says “Validation failed”?</strong></summary>

HitPaw checks headers, XML well-formedness (`QXmlStreamReader`), and `user_total_manga`. It aborts save and logs red `Validation failed`. Re-try export with fewer titles or check disk space. If persistent, copy the log line + file header (first 3 lines) into a [bug report](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=bug_report.yml).
</details>

## Updates & Performance

<details>
<summary><strong>How does update check work?</strong></summary>

- **Manual**: `Check for updates` (`main.cpp:3067`) → popup with HitPaw icon: `You are on vX / Latest vY` or `Update Available` → `Download Latest Version` → [`releases/latest`](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest)
- **Silent**: 3.5s after launch queries `api.github.com/repos/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest` (8s timeout, `User-Agent: HitPaw-MangaDex-Manager`). Failures are silent; if newer `tag_name` than `applicationVersion()`, log + stats label `Update available: vY`.

</details>

<details>
<summary><strong>App feels slow with 3000 titles?</strong></summary>

Use filters first (status/year/tag/search) → `Show All (N)` on filtered subset is instant. Sorting is in-memory (`QVersionNumber`-aware) → `relayoutLibrary()` without re-fetch. Clear cover cache if disk is full (`AppDataLocation/covers` via `QDirIterator` shows `X files, Y MB`).
</details>

## Still stuck?

- **Discord**: [discord.gg/z6yYYpcYYc](https://discord.gg/z6yYYpcYYc) — fastest help
- **Bug report**: [template](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=bug_report.yml) — attach `v3.4.6`, OS, log snippet (from **Activity Log** tab)
- **Feature request**: [template](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=feature_request.yml)

::: tip Edit this page
Every docs page has **Edit this page on GitHub** — add your question + answer via PR (see [Contributing](/contributing)).
:::
