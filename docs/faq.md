---
title: FAQ
description: Frequently asked questions — login, covers, export, performance, and troubleshooting for HitPaw.
outline: deep
---

# FAQ

<div class="badge-row">
  <span class="badge badge--accent">Latest</span>
  <a href="https://discord.gg/z6yYYpcYYc" class="badge badge--brand">Discord</a>
</div>

<!-- Live filter UI mounts here from the theme (theme/index.ts) -->
<div id="faq-filter-root"></div>

## Login & Library

<details>
<summary><strong>Why does "Load Library" stay on skeletons?</strong></summary>

MangaDex paginates (`limit=100`). HitPaw fetches all IDs first, then titles in batches with retry/backoff. On 3k titles it takes 15–30s. Check **Activity Log** — if it stalls on `429 Ratelimit`, wait 60s.
</details>

<details>
<summary><strong>Invalid Client ID / Secret?</strong></summary>

Regenerate at **MangaDex → Settings → API Clients**. The pair is case-sensitive — copy, don't type. Ensure the secret wasn't regenerated without updating HitPaw.
</details>

<details>
<summary><strong>Does "Remember credentials" store my password in plain text?</strong></summary>

Currently via `QSettings` (Registry on Windows, plist on macOS, INI on Linux). Not encrypted on Windows until the planned `QKeychain` migration. On shared machines, uncheck it.
</details>

## Covers & Grid

<details>
<summary><strong>Covers not loading / grey placeholders?</strong></summary>

Cover CDN can 429. HitPaw retries with exponential backoff and disk cache. Use **Clear Cover Cache** then **Refresh library**. If still grey, check firewall — covers need `uploads.mangadex.org`.
</details>

<details>
<summary><strong>Grid shows X / Y — why not all?</strong></summary>

`Show more (60)` loads 60 at a time. `Show All (N)` loads your entire filtered library. 60 batches keep it smooth at 3k titles.
</details>

## Export

<details>
<summary><strong>Which file for which site?</strong></summary>

- **MAL XML** → MyAnimeList, AniList, MangaBaka, Kitsu, MangaFire
- **MB JSON** → MangaBaka native import
- **AP .gz** → Anime-Planet (must be gzipped)

See [Export Formats](/export) for step-by-step instructions.
</details>

<details>
<summary><strong>Export says "Validation failed"?</strong></summary>

HitPaw checks headers, XML well-formedness, and node counts. Re-try with fewer titles or check disk space. If persistent, open a [bug report](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=bug_report.yml).
</details>

## Updates & Performance

<details>
<summary><strong>How does update check work?</strong></summary>

Manual: `Check for updates` → popup with version info → one-click download. Silent: 3.5s after launch, queries GitHub API. If newer version exists, logs it in the Activity Log.
</details>

<details>
<summary><strong>App feels slow with 3000 titles?</strong></summary>

Filter first (status/year/tag/search) → `Show All (N)` on filtered subset is instant. Sorting is in-memory. Clear cover cache if disk is full.
</details>

## Still stuck?

[![Discord](https://img.shields.io/badge/Discord-Join%20Server-5865F2)](https://discord.gg/z6yYYpcYYc)
[![Bug Report](https://img.shields.io/badge/GitHub-Issues-181717)](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=bug_report.yml)

::: tip Edit this page
Every docs page has **Edit this page on GitHub** — add your question via PR (see [Contributing](/contributing)).
:::
