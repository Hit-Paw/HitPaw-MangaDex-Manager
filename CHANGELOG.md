# Changelog

All notable changes to HitPaw MangaDex Manager will be documented in this file.

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
