# Export Formats

HitPaw exports your MangaDex library to formats compatible with other trackers.

| Format | File | Compatible sites |
|--------|------|------------------|
| CSV | `mangadex_library.csv` | Excel, Google Sheets, any spreadsheet |
| JSON | `mangadex_library.json` | Raw backup, scripting (includes cover URLs) |
| MAL XML | `mangadex_library_MAL.xml` | MyAnimeList, AniList, MangaBaka, Kitsu, MangaFire |
| AP .gz | `mangadex_library_AP.xml.gz` | Anime-Planet (requires the gzipped file specifically) |
| MB JSON | `mangadex_library_MangaBaka.json` | MangaBaka's native import format (mangabaka.org/settings/import) |

## Import Instructions per Site

| Site | How to import |
|------|---------------|
| MyAnimeList | Profile → Import/Export → MyAnimeList Import → upload MAL XML |
| AniList | Settings → Import → upload MAL XML |
| MangaBaka | Settings → Import → MyAnimeList → upload MAL XML |
| Kitsu | Avatar → Settings → Import → upload MAL XML |
| MangaFire | Profile → Import/Export → upload MAL XML |
| Anime-Planet | Your manga list page → scroll to bottom → "Import it now" → upload AP .gz |

> MangaUpdates has no list import feature (export only). comix.to uses collections with no file-based import.

## Validation

The app validates exports and shows an import guide. The file headers are tested via `tests/test_export.cpp` (`ctest`) — `CSV header` must contain `title,status,year,tags` and `JSON` must be valid UTF-8.

Source: `export.h` (header-only, uses `QJsonDocument`, `QCsv` helpers).
