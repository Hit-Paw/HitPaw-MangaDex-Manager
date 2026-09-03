---
title: Export Formats — HitPaw MangaDex Manager
description: Export your MangaDex library to CSV, JSON, MAL XML, MangaBaka JSON, Anime-Planet .gz — validated, import-ready. Also sync directly to MDList.
outline: deep
---

# Export Formats

One selection → **five validated files** + direct **MDList sync**. HitPaw checks headers, XML well-formedness, and gz magic before saving — if validation fails, you’ll know immediately.

<div class="badge-row">
  <span class="badge badge--accent">CSV</span>
  <span class="badge badge--accent">JSON</span>
  <span class="badge badge--accent">MAL XML</span>
  <span class="badge badge--accent">AP .gz</span>
  <span class="badge badge--accent">MangaBaka JSON</span>
  <span class="badge">MDList sync</span>
</div>

## At a glance

| Format | File | Compatible sites | Best for |
|--------|------|------------------|----------|
| **CSV** | `mangadex_library.csv` | Excel, Google Sheets, Numbers | Spreadsheet, stats |
| **JSON** | `mangadex_library.json` | Any editor — cover URLs, tags, status | Backup, scripts |
| **MAL XML** | `mangadex_library_MAL.xml` | **MyAnimeList • AniList • MangaBaka • Kitsu • MangaFire** | Universal import |
| **AP .gz** | `mangadex_library_AP.xml.gz` | **Anime-Planet** (must be `.gz`) | AP only |
| **MB JSON** | `mangadex_library_MangaBaka.json` | **MangaBaka** native import | MB native |
| **MDList** | _(no file)_ — pushes to MangaDex list | MangaDex custom lists | Stay on MangaDex |

> **MangaUpdates** has no list import (export only) — CSV/JSON are your backup there.

## Import — step by step

| Site | How to import | File to upload |
|------|---------------|----------------|
| **MyAnimeList** | Profile → Import/Export → MyAnimeList Import → upload | **MAL XML** |
| **AniList** | Settings → Import → upload | **MAL XML** |
| **MangaBaka** | Settings → Import → MyAnimeList → upload (or native MB JSON) | **MAL XML** or **MB JSON** |
| **Kitsu** | Avatar → Settings → Import → upload | **MAL XML** |
| **MangaFire** | Profile → Import/Export → upload | **MAL XML** |
| **Anime-Planet** | Manga list page → scroll to bottom → “Import it now” → upload | **AP .gz** only |

::: tip Anime-Planet requires the gzipped file
They reject plain XML. HitPaw's `*_AP.xml.gz` has gzip magic `1F 8B` + `gzopen` decompress check + MAL XML inside — exactly what AP expects.
:::

## Sync to MDList — no file needed <Badge type="tip" text="v3.5.0" />

Beyond file exports, the **Export tab → Sync to MDList** pushes your bookmarks **straight into a MangaDex custom list** — no intermediate file, no manual re-adding.

1. **Load Library** in the Library tab (or select individual titles).
2. In **Export → Sync to MDList**, enter a list name (default `My Favorites`) and pick visibility (**Private** / **Public**).
3. Click **Sync Entire Library** or **Sync Selected** and confirm.

<div class="why-grid">
  <div class="why-card">
    <strong>Reuse or create</strong><br/><span style="opacity:.78">Existing list with same name (case-insensitive) is <b>reused</b>; otherwise a new one is <b>created</b> with chosen visibility</span>
  </div>
  <div class="why-card">
    <strong>Gentle pacing</strong><br/><span style="opacity:.78"><code>POST /manga/{id}/list/{listId}</code> one-by-one, 350 ms spacing — safe for 3000+ titles</span>
  </div>
  <div class="why-card">
    <strong>Smart & safe</strong><br/><span style="opacity:.78">Already-in-list skipped • 429 backoff • session refresh once mid-run • <code>Stop</code> resumes later</span>
  </div>
</div>

How it works in detail:

- Titles already in the list are **skipped** (counted as *already in list*), rate limits **back off automatically**, and an expired session is **refreshed once** mid-run.
- **Stop** finishes the current title and reports progress — already-added titles stay, so just run sync again to resume.
- Your **reading statuses are never modified** — MDList sync only touches list membership, not `reading/completed` etc.

::: tip Done dialog
Includes an **Open** button that takes you straight to the list on `mangadex.org/list/{id}` — verify instantly.
:::

## File samples — copyable

::: code-group

```csv [CSV]
title,status,year,tags,coverUrl,mangaId
"One Piece",reading,1997,"adventure,shounen",https://...,8a3c...
"Solo Leveling",completed,2018,"action,adventure",https://...,d8...
```

```json [JSON]
{
  "schema_version": 1,
  "exported_at": "2026-08-27T18:00:00Z",
  "titles": [
    {
      "title": "One Piece",
      "status": "reading",
      "year": 1997,
      "tags": ["adventure","shounen"],
      "coverUrl": "https://...",
      "mangaId": "8a3c..."
    }
  ]
}
```

```xml [MAL XML]
<?xml version="1.0" encoding="UTF-8"?>
<myanimelist>
  <myinfo>
    <user_total_manga>3276</user_total_manga>
  </myinfo>
  <manga>
    <series_title><![CDATA[One Piece]]></series_title>
    <series_type>Manga</series_type>
    <my_status>Reading</my_status>
  </manga>
</myanimelist>
```

:::

## Validation — built-in

HitPaw runs `validateExportFile()` after every export — if it fails, the file isn’t left half-written:

- **CSV** → header contains `title,status,year,tags` + correct line count (+1 for header)
- **JSON** → valid JSON parse, `schema_version` present, UTF-8, titles array length matches selection
- **MAL XML** → well-formed via `QXmlStreamReader`, `<manga>` node count matches selection
- **AP .gz** → gzip magic `0x1F 0x8B` + `gzopen` decompress + XML well-formedness
- **MB JSON** → `entries` array size matches selection, UTF-8

CI also validates via `tests/test_export.cpp` (`ctest --output-on-failure`). See [Building](/building#verify-before-push).

## Which file for which site?

<div class="vs-table" role="table" aria-label="Export chooser">
  <div class="vs-row vs-header" role="row">
    <div role="columnheader">Goal</div>
    <div role="columnheader">Use</div>
    <div role="columnheader">Upload to</div>
    <div role="columnheader">Note</div>
  </div>
  <div class="vs-row" role="row">
    <div class="vs-feature" data-label="Goal">Spreadsheet / script</div>
    <div class="vs-hitpaw" data-label="Use"><code>CSV</code> / <code>JSON</code></div>
    <div class="vs-manual" data-label="Upload to">Excel / Sheets / code</div>
    <div class="vs-bookmark" data-label="Note">Fullest data</div>
  </div>
  <div class="vs-row" role="row">
    <div class="vs-feature" data-label="Goal">MAL / AniList / Kitsu / Fire</div>
    <div class="vs-hitpaw" data-label="Use"><code>MAL XML</code></div>
    <div class="vs-manual" data-label="Upload to">MAL import</div>
    <div class="vs-bookmark" data-label="Note">Most compatible</div>
  </div>
  <div class="vs-row" role="row">
    <div class="vs-feature" data-label="Goal">MangaBaka native</div>
    <div class="vs-hitpaw" data-label="Use"><code>MB JSON</code></div>
    <div class="vs-manual" data-label="Upload to">MB import</div>
    <div class="vs-bookmark" data-label="Note">Or MAL XML</div>
  </div>
  <div class="vs-row" role="row">
    <div class="vs-feature" data-label="Goal">Anime-Planet</div>
    <div class="vs-hitpaw" data-label="Use"><code>AP .gz</code></div>
    <div class="vs-manual" data-label="Upload to">AP list import</div>
    <div class="vs-bookmark" data-label="Note">Must be .gz</div>
  </div>
  <div class="vs-row" role="row">
    <div class="vs-feature" data-label="Goal">Stay on MangaDex</div>
    <div class="vs-hitpaw" data-label="Use"><code>MDList sync</code></div>
    <div class="vs-manual" data-label="Upload to">MangaDex list</div>
    <div class="vs-bookmark" data-label="Note">No file</div>
  </div>
</div>

::: info Need help?
If an import fails, re-export with fewer titles and check **Validation** above. Try the other compatible file (MAL XML works on 5 sites). Still stuck? Open a [bug report](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=bug_report.yml) with export log + target site.
:::
