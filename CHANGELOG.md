# Changelog

All notable changes to HitPaw MangaDex Manager will be documented in this file.

## [3.5.0] - 2026-09-01

### Added
- **Sync to MDList**: Export tab card that pushes your bookmarks into a MangaDex custom list ("MDList") directly from the app. Enter a list name (default `My Favorites`) + visibility (Private/Public, used only when creating), then **Sync Entire Library** or **Sync Selected**. Existing list with the same name is reused (case-insensitive, found via paginated `GET /user/list`) — otherwise created via `POST /list` (`main.cpp:4152` `onMdlistSync`, `main.cpp:4224` `mdlistResolvePage`, `main.cpp:4259` `mdlistCreate`). Titles are added one at a time via `POST /manga/{id}/list/{listId}` (`main.cpp:4283` `mdlistAddNext`) — the list-update endpoint replaces the whole `manga[]` array and caps bodies at 8KB, so per-title adds are the only safe way to move a 3000+ title library. Duplicates (HTTP 400) count as *already in list*, live progress bar + `added / skipped / failed` counter, 350 ms pacing + 429 back-off (2 s, up to 3 retries), one automatic token refresh on mid-run 401, **Stop** button finishes the in-flight title and reports partial progress, done dialog offers `Open` → `https://mangadex.org/list/{id}`. Reading statuses are never touched; logout mid-run flags the loop to stop. UI card at `main.cpp:2244`, selection-aware buttons wired through `updateSelectionUi()`.

## [3.4.6] - 2026-08-27

### Fixed
- Update popup icons: `onUpdateCheckFinished()` manual `QMessageBox` popups now all use HitPaw icon `QPixmap(":/icons/icon_64.png")` 48×48 `setIconPixmap` (`main.cpp:3024`, `3035`, `3044`, `3105`, `3120`) instead of default blue `i` `QMessageBox::information` / yellow `QMessageBox::warning` — `Update Check Failed` (network `reply->errorString()`, parse, tag, version), `Up to Date` (`You are up to date! vX (latest vY)`), fallback `Check for Updates`, and `Update Available` (`Download Latest Version` → `https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest`) now consistently show HitPaw logo per user request.

## [3.4.5] - 2026-08-27

### Added
- Manual update check popup: `Check for updates` (`GhostButton` `main.cpp:1926` in cache row) now triggers `manualCheckForUpdates()` (`main.cpp:3067`) with `m_manualUpdateCheck` flag (`main.cpp:1231`). `onUpdateCheckFinished()` (`main.cpp:3010`) now shows `QMessageBox` popup: if outdated → `Update Available! You are on vX, latest is vY. You are outdated.` with `Download Latest Version` (`QDesktopServices::openUrl("https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest")`); if up to date → `You are up to date! vX (latest vY)`; if failed → `Update Check Failed` warning. Silent `checkForUpdates()` (`QTimer::singleShot 3500`, `main.cpp:1089`) banner/logs remain unchanged.

## [3.4.4] - 2026-08-27

### Removed
- Grid density toggle: `m_densityBtn` (`:/icons/nav/icons8-library-48-text.png`, `m_compactMode`/`m_gridCols` 5↔6) removed from toolbar (`main.cpp:1770`, `1800`), grid fixed to `GRID_COLS=5` (170×322 cards, cover 156×222), skeletons fixed to `W=170 H=322 CW=156 CH=222` (`main.cpp:3675`) and `i / GRID_COLS` (`main.cpp:3726`), per user request to remove grid icon.

### Added
- Load all option: `m_showAllBtn` (`GhostButton "Show All"` `Hide`, `ToolTip: Load all remaining titles at once`) alongside `m_loadMoreBtn` (`"Show more"`) in `paginationRow` `QHBoxLayout` (`main.cpp:2028`, `QHBoxLayout` 8px/12px centered) — `Show All (N)` sets `m_paginationLimit = totalMatched` (`cardMatchesFilter` count) and `relayoutLibrary()` to load entire library at once, while `Show more (N remaining)` keeps `PAGINATION_STEP=60` incremental. Both buttons share `relayoutLibrary()`/`appendCardsToGrid()`/`updateSelectionUi()` visibility (`main.cpp:3808`, `3849`, `4019`).

## [3.4.3] - 2026-08-26

### Fixed
- `All status` dropdown now shows status colors: `m_filterStatus` items tinted via `statusColor(key)` + `setItemData(..., QBrush)` and current text colored via `updateFilterStatusColor()` stylesheet (`QComboBox { color: ... }`), matching `m_bulkStatusBox` fix in `3.4.1`, dropdown `QAbstractItemView` background kept `Pal::CARD`.
- Filter bar text cut: `Year:` `m_filterYear` and `Tags:` `m_filterTag` increased `setFixedHeight(30)` -> `34` and added stylesheets (`QComboBox`/`QLineEdit` `background: Pal::CARD` `border: 1px solid Pal::BORDER` `border-radius: 6px` `padding: 4px 8px`) to prevent placeholder/text clipping, `Clear filters` button also `34`.

## [3.4.2] - 2026-08-26

### Fixed
- Build failure: `moc` reported `No relevant classes found` at `main.cpp:186`/`2643` due to `R"([\\/:*?"<>|])"` raw string containing `?"` confusing `Qt 6.11.1` `moc` parser (`test_illegal_raw` `0 B` vs `normal` `2853 B`). Fixed by using normal string `"[\\\\/:*?\"<>|]"` (`main.cpp:186`) and `uuid` regex `R"(([0-9a-f]{8}...))"` -> normal string (`main.cpp:2424`), restoring `20001 B` `main.moc` and successful `qmake`/`mingw32-make -j4` build (`MangaDexExporter.exe` `754688 B`).

## [3.4.1] - 2026-08-25

### Fixed
- Bulk move dropdown now shows status colors: each item tinted via `statusColor(key)` + `setItemData(..., QBrush)` and `m_bulkStatusBox` current text colored via `updateBulkColor()` stylesheet (`QComboBox { color: ... }`), matching `m_filterStatus` chip. Dropdown `QAbstractItemView` background/selection kept `Pal::CARD`/`ACCENT`.

## [3.4.0] - 2026-08-25

### Added
- Toolbar grouping + overflow: `Library` toolbar now grouped with `QFrame` `VLine` separators (`Pal::BORDER`, 1×24px) between filter/sort vs actions vs fetch (`main.cpp:2065`), `m_densityBtn` added after `m_sortBox`.
- Card density toggle: `m_densityBtn` (`GhostButton` 34×34, `:/icons/nav/icons8-library-48-text.png`, checkable) toggles `m_compactMode`/`m_gridCols` 5↔6, updates `m_grid->setColumnStretch` for 6 cols and calls `relayoutLibrary()` (`main.cpp:2035`). Window stays `setFixedWidth(1180)` — 5×170+4×12=898 vs 6×170+5×12=1080 both fit.
- Status color + filter chip: `m_filterChip`/`m_filterChipLabel` (`QWidget`/`QLabel` with `Pal::CARD`/`BORDER` pill, 12px radius) below toolbar shows `Search: "x" · Status: Reading · Sort: Year` (`updateFilterChip()` `main.cpp:2245`), `x` `GhostButton` 24×24 clears `m_filterEdit`/`m_filterStatus`/`m_sortBox`. `m_filterStatus` items tinted via `statusColor(key)` + `setItemData(..., QBrush)` (`main.cpp:2258`).
- Empty/loading skeletons: `m_skeletonCards` (`QList<QWidget*>`), `showSkeletons(10)`/`clearSkeletons()` (`main.cpp:3790`) create 10× `QFrame#skeleton` (170×322 or 112×208) with `QVBoxLayout` (cover 156×222 + 2× title 10px placeholders, `Pal::CARD`/`BORDER`), `QGraphicsOpacityEffect` 0.85→0.45 pulse `QPropertyAnimation` 900ms `InOutQuad` loop -1. Shown in `startLibraryFetch()` after `m_allIds`, cleared in `finishLibrary()`/`finishFetchError()`/`finishFetchStopped()` before real cards.

## [3.3.4] - 2026-08-25

### Removed
- Export preview (first 10 table + Refresh Preview + estimated sizes) — `m_exportPreview`/`m_previewInfo` (`main.cpp:1479`), `previewCard` UI (`main.cpp:2367`), `updateExportPreview()` (`main.cpp:3749`) and calls from `updateSelectionUi()`/`finishLibrary()` — per user request to keep Export tab clean. Export validation, bulk editor, cache manager, stats dashboard remain.

## [3.3.0] - 2026-08-25

### Added
- Bulk status editor: `Bulk move selected to:` combo (`Reading/Completed/On Hold/Plan to Read/Dropped/Re-reading`, `main.cpp:2075`) + `Apply` `GhostButton` (`m_bulkStatusBox`/`m_bulkApplyBtn`, `main.cpp:1445`). `onBulkStatusApply()` confirms `n` titles → `processBulkStatusBatch()` POSTs `POST /manga/{id}/status` `{"status":...}` via `apiPostJson()` (`main.cpp:2975`) with `Bearer` token, `350ms` rate-limit, updates `m_statusMap`/`m_entries` locally, re-sorts if `Status` sort active, `relayoutLibrary()` + `updateStats()` + `updateRefreshButton()`. `updateSelectionUi()` enables `Apply` only when `n>0 && !m_bulkRunning`.
- Cover cache manager: `m_cacheSizeLbl` + `Clear Cover Cache` `GhostButton` (`main.cpp:2090`) in Library tab. `updateCacheSizeLabel()` (`main.cpp:2242`) scans `AppDataLocation/covers` via `QDirIterator` recursive, shows `X files, Y MB`. `clearCoverCache()` (`main.cpp:2251`) removes files + empty dirs, logs `Cleared: N files`, `QMessageBox`.
- Statistics dashboard: `m_statsDashLbl` (`main.cpp:1455`) + `updateStatsDashboard()` (`main.cpp:3729`) shows `Years: 2024:12 2023:8 ...` top 5 years + `Total N` from `m_entries[].year`, sorted descending. `updateStats()` now calls dashboard; shown in `dashRow` below cache row.
- Export preview: `m_exportPreview` `QTableWidget` 3 cols (`Title/Status/Year`, `main.cpp:2360`) + `m_previewInfo` + `Refresh Preview` `GhostButton` in Export tab `previewCard`. `updateExportPreview()` (`main.cpp:3745`) shows first 10 of `selected ? selected : all`, fills rows, estimates `CSV ~150/title, JSON ~280, MAL ~420` → `Est. CSV X KB...` and auto-called from `updateSelectionUi()` and `finishLibrary()` (`+700ms`).

## [3.2.2] - 2026-08-25

### Fixed
- Refresh button now requires loaded library: `m_refreshBtn` disabled initially with tooltip `Load your library first...`, `updateRefreshButton()` (`main.cpp:2163`) enables only when `!m_accessToken.isEmpty() && !m_libraryOrder.isEmpty() && !m_fetching` — called after `finishLibrary`, `finishFetchError`, `setFetchingUi`, sign-out. `refreshLibrary()` (`main.cpp:3650`) now checks `m_libraryOrder.isEmpty()` → `QMessageBox::information` and saves `m_prevStatusMap`/`m_prevOrder` + `m_isRefresh=true` before `startLibraryFetch()`.

### Added
- Refresh detects recent activity: `finishLibrary()` (`main.cpp:3530`) diffs `currSet` vs `prevSet` — `added = curr-prev`, `removed = prev-curr`, `changed = status diff via m_prevStatusMap`. Logs colored `Refresh detected: X new bookmark(s), Y category changed` with per-title lines `+ title [status]`, `~ title: old -> new`, `- title (removed)` and `m_statsLbl` update; popup if ≤12 changes. Silent when no changes. `m_isRefresh`/`m_prev*` cleared after.

## [3.2.1] - 2026-08-25

### Added
- Toolbar buttons `Redo` and `Refresh` (`main.cpp:1463`, `2020`): `Redo` mirrors `Undo` icon via `QTransform` scale(-1,1) and `m_redoBtn` (`GhostButton` 34x34), backed by `m_selectionRedo` stack (`UNDO_MAX=50`). `pushUndoSnapshot()` now clears redo, `undoSelection()` pushes to redo and enables `m_redoBtn`, `redoSelection()` pops redo, pushes to undo, restores `m_selected` and calls `updateSelectionUi()`. `Refresh` uses `icons8-refresh-48-text.png` and `m_refreshBtn` → `refreshLibrary()` which checks `m_fetching`/`m_accessToken` then `startLibraryFetch()` with `Refreshing library...` log. Both respect pagination/sort state.

## [3.2.0] - 2026-08-25

### Fixed
- Crazy long titles now fit everything: removed `maxLines=3` cap and `elidedText` truncation. `MangaCard` (`main.cpp:1006`) now uses `setFixedWidth(W)` + `setMinimumHeight(H)` + `QSizePolicy::Fixed/Preferred` and `m_title` has no `setFixedHeight` — `wordWrap:true` + `sizeHint` lets the label wrap to as many lines as needed (tested with quoted 80+ char titles) and the card expands vertically (`cover 222 + title dynamic + status + margins`). No more `...` for any length; grid `QGridLayout` handles variable-height cards and `relayoutLibrary()`/`appendCardsToGrid()` pagination still shows `X / Y` correctly.

## [3.1.9] - 2026-08-25

### Fixed
- Library titles not fitting: quoted long titles like `"Munou wa Iranai" to Iwareta kara Zetsuen...`, `"Omae Gotoki ga Maou ni..."`, `"Osananajimi ga Hoshii" to Tsubuyaitara...` were elided to 2 lines and cut with `...` on the second line. `MangaCard` (`main.cpp:1032`) now uses `maxLines=3` for regular cards (2 for small) via `QFontMetrics(pixelSize)` + `lineSpacing*3+8` height (vs `*2+8`), and `elideToLines()` builds up to 3 lines greedily (word-boundary, `horizontalAdvance` ≤ `availW=158`) and only elides the final line — 3-line cards still fit in `H=322` (`cover 222 + title 48 + status 15 + margins 12`) so no layout break.

## [3.1.8] - 2026-08-25

### Added
- Library sorting: toolbar combo `Title A-Z / Z-A / Year Newest / Year Oldest / Status` (`main.cpp:2008`, `m_sortBox`). `applyLibrarySort()` / `sortLibrary()` sort `m_libraryOrder` and `m_libCards` via `QVersionNumber`-aware comparators (year parsed via `toInt`, status via `localeAwareCompare`), then `relayoutLibrary()` — no rebuild needed.
- Export validation: `validateExportFile()` (`main.cpp:3726`) checks every saved file — CSV header + line count, JSON array/object + `schema_version`, MAL XML well-formedness via `QXmlStreamReader` + `manga` count + `user_total_manga`, MangaBaka JSON `entries` size, AP `.gz` gzip magic + `gzopen` decompress + XML check. `onExport()` logs `Validated X - OK` in green or `Validation failed` in red and aborts with `QMessageBox::warning` on failure (`main.cpp:3791`).
- Persistent download queue: `saveDownloadState()` / `clearDownloadState()` / `loadDownloadState()` (`main.cpp:2775`) store `dl/mangaId`, `dl/queue`, `dl/current`, `dl/path` in `QSettings` (sync after each `++m_dlCurrent` and on `onDlStart`/`Stopped`/`Done`). `loadDownloadState()` at `+800ms` restores interrupted `m_dlQueue` notice in `m_dlStatusLbl` accent — resume without losing progress.
- Pagination for large libraries: `m_paginationLimit` / `PAGINATION_STEP=60` + `m_loadMoreBtn` (`main.cpp:1467`, `2145`). `relayoutLibrary()` / `appendCardsToGrid()` now show only `limit` matching cards, `m_countLbl` shows `X / Y shown`, button `Show more (N remaining)` increments `limit` — 3000-title libraries render 60 at a time instead of all widgets.

## [3.1.7] - 2026-08-25

### Added
- Silent update checker: 3.5s after launch the app queries `api.github.com/repos/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest` via a dedicated `QNetworkAccessManager` (8s timeout, `User-Agent: HitPaw-MangaDex-Manager`). Failures are silent — no crash, no popup, no log spam. If `tag_name` is newer than `applicationVersion()` (`QVersionNumber::compare`), it logs `Update available: vX -> vY - https://github.com/.../tag/vY` and updates the stats label to `Update available: vY - see Logs` in accent color.

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
