---
title: Changelog
description: Version history and release notes for HitPaw MangaDex Manager — v3.7.1 text-safe button sizing, v3.7.0 reading progress + context menu + prefetch, and earlier.
outline: deep
---

# Changelog

<div class="badge-row">
  <span class="badge badge--accent">Current: v3.7.1</span>
  <span class="badge">MIT • Open Source</span>
  <a href="https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases" target="_blank" rel="noreferrer" class="badge">All Releases →</a>
</div>

For commit-level details see **[CHANGELOG.md on GitHub](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/blob/main/CHANGELOG.md)**.

## v3.7.1 — September 10, 2026 <Badge type="tip" text="Latest" />

- **Text-safe button sizing**: hard-coded-width buttons ("Clear", "All"/"None", "Copy", and more) no longer clip their right edge when the platform font renders wider than designed — most visible on Linux with the DejaVu Sans fallback and at larger scale factors
- **Dynamic labels stay fitted**: "Export Selected (N)", "Sync Entire Library (N)" and "Sync Selected (N)" re-fit their width as the count changes, so 5-digit libraries no longer clip the sync buttons
- `GhostButton`/`AccentButton` now resolve the UI font eagerly, never report a size hint smaller than label + padding, and route every hard-coded width through `fitWidth()` — designed width when it fits, grows only as far as the text demands

## v3.7.0 — September 6, 2026

- **Reading progress tracking**: per-manga chapters read / total persisted locally via `QSettings`, 3px gradient bar + `Ch. X / Y · N%` label on cards, auto-flips Plan to Read → Reading → Completed
- **Right-click context menu**: Open on MangaDex, Copy URL, Reading progress (+1 / +5 / set / reset), Set status via API, Select/Deselect — double-click now opens on MangaDex
- **Cover prefetch**: next 20 covers queued in background after each 30-card chunk — scrolling shows covers instantly

## v3.5.0 — September 1, 2026

- **Sync to MDList**: push your whole library (or selected) into a MangaDex custom list — reuses existing by name or creates Private/Public, skips duplicates, 350 ms pacing, live progress + Stop, never touches reading statuses
- **Open** button on done dialog goes straight to `mangadex.org/list/{id}`

## v3.4.6 — August 27, 2026

- HitPaw icon on all update popups (manual + silent auto-check)
- `Show All (N)` loads entire filtered library at once — smooth at any library size
- Grid fixed to **5 columns** (density toggle removed for density + performance)

## v3.4.3 — August 26, 2026

- Status dropdowns now show color chips (reading/completed/on hold etc.)
- Filter bar text clipping fixed on narrow windows

## v3.4.0 — August 25, 2026

- **Bulk status editor** (Reading / Completed / On Hold / Plan to Read / Dropped / Re-reading)
- **Undo/redo** (`Ctrl+Z` / `Ctrl+Y`) for grid edits
- **Export validation** (`validateExportFile`) for CSV, JSON, MAL XML, AP .gz, MangaBaka JSON + `ctest` suite
- Cover cache stats + `Clear Cover Cache`

---

See **[All Releases](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases)** for download links, SHA256, SBOM, and build provenance. New to HitPaw? Start at [Getting Started](/getting-started).
