---
title: Changelog
description: Version history and release notes for HitPaw MangaDex Manager — v3.7.0 reading progress + context menu + prefetch, v3.5.0 MDList sync, and earlier.
outline: deep
---

# Changelog

<div class="badge-row">
  <span class="badge badge--accent">Current: v3.7.0</span>
  <span class="badge">MIT • Open Source</span>
  <a href="https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases" target="_blank" rel="noreferrer" class="badge">All Releases →</a>
</div>

For commit-level details see **[CHANGELOG.md on GitHub](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/blob/main/CHANGELOG.md)**.

## v3.7.0 — September 6, 2026 <Badge type="tip" text="Latest" />

- **Reading progress tracking**: per-manga chapters read / total persisted locally via `QSettings`, 3px gradient bar + `Ch. X / Y · N%` label on cards, auto-flips Plan to Read → Reading → Completed
- **Right-click context menu**: Open on MangaDex, Copy URL, Reading progress (+1 / +5 / set / reset), Set status via API, Select/Deselect — double-click now opens on MangaDex
- **Cover prefetch**: next 20 covers queued in background after each 30-card chunk — scrolling shows covers instantly

## v3.5.0 — September 1, 2026

- **Sync to MDList**: push your whole library (or selected) into a MangaDex custom list — reuses existing by name or creates Private/Public, skips duplicates, 350 ms pacing, live progress + Stop, never touches reading statuses
- **Open** button on done dialog goes straight to `mangadex.org/list/{id}`

## v3.4.6 — August 27, 2026

- HitPaw icon on all update popups (manual + silent auto-check)
- `Show All (N)` loads entire filtered library at once — smooth at 3000+ titles
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
