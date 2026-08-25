# Changelog

All notable changes to HitPaw MangaDex Manager will be documented in this file.

## [3.1.6] - 2026-08-25

### Fixed
- Library title truncation: long titles like `Biryuu Enshoutan ~Jikokouteikan ga Gekitei na` were cut off mid-letter (4px overflow on second line + 2px vertical descender clip) because `MangaCard` used a fixed 34px height with no eliding. During `SmoothScrollArea` momentum scrolling this clipped edge tore and looked "cut up". Cards now compute `availW = W-12`, use `QFontMetrics(pixelSize)` to size to `lineSpacing*2+8`, and elide to 2 lines (`line1 + "\n" + elided line2`) so overflow shows clean `…` instead of a half-letter.

## [3.1.5] - 2026-08-25

### Fixed
- Library text glitching during scrolling: removed `QGraphicsOpacityEffect` from card entrance animation, which forced Qt to render each widget into an offscreen pixmap buffer. Cards now render directly to screen.

### Removed
- Card entrance fade-in animation (200ms opacity pop) — source of the offscreen buffer causing scroll glitches

## [3.1.4] - 2026-08-25

### Fixed
- Chapter display order: root cause was `insertWidget(count()-1)` inserting into a layout whose trailing stretch spacer had been cleared, causing Ch.1 to always appear at the bottom. Fixed by using `addWidget()` to correctly append chapters in sorted order.

## [3.1.3] - 2026-08-25

### Added
- About dialog now shows credits ("Created by HitPaw") and a clickable Discord community link

### Fixed
- Version string in About dialog now matches the actual release version (was stuck on 3.1.0)

## [3.1.2] - 2026-08-25

### Fixed
- Chapter sort order: chapters now sort correctly numerically (Ch.1 before Ch.2) on all system locales, using locale-independent parsing via QLocale::c()

## [3.1.1] - 2026-08-25

### Fixed
- MAL XML export: corrected field names (`manga_num_volumes`, `manga_num_chapters`) that caused import failures on MyAnimeList, AniList, Kitsu, and MangaFire
- MAL XML `<myinfo>` section: added missing required fields (`user_id`, `user_name`, per-status counts) for proper import validation
- Anime-Planet gzipped export: same MAL XML corrections applied

## [3.1.0] - 2026-08-24

### Added
- Initial public release
- MangaDex library browser with cover grid view
- Login via username/password or personal API client
- Bulk-select and export with undo
- Export to CSV, JSON, MAL XML, MangaBaka JSON, Anime-Planet `.xml.gz`
- Cover caching with disk-backed CDN retry/backoff
- Dark AMOLED-style UI with animated widgets
- Smooth momentum-based scrolling
- Cross-platform support (Windows, macOS, Linux)
