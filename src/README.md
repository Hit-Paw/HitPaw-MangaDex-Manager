# Source layout (in progress)

`main.cpp` (4405 lines at `84bc441`) is being split. New code should go into:

* `src/api/` — MangaDex HTTP, auth (`client_id`/`client_secret` via `secure_store.h`), rate-limit/backoff
* `src/models/` — `domain.h` types (Manga, Chapter, Tag), sorting/filtering
* `src/ui/` — `pal.h` palette, MainWindow, dialogs
* `src/export/` — `export.h` CSV/JSON/MAL/AP/MangaBaka exporters (covered by `tests/test_export.cpp`)

Existing headers (`domain.h`, `export.h`, `pal.h`, `secure_store.h`) are the seed for those modules. `main.cpp` remains the build entry until the split lands; `CMakeLists.txt` + `MangaDexExporter.pro` still compile `main.cpp` only.

## How to split next

1. Move one class from `main.cpp` to `src/ui/MainWindow.{h,cpp}` + update `CMakeLists.txt` `qt_add_executable` + `MangaDexExporter.pro` `SOURCES/HEADERS`
2. Run `clang-format -i` (config at `.clang-format`) and `ctest` (`tests/`)
3. Ensure `ci.yml` matrix (`build-check-windows/linux/macos`) stays green

Do not bundle `build_win/`, `*.zip`, or `hitpaw.png` (1.4 MB) changes in the same PR as the split — keep PRs focused per `CONTRIBUTING.md:9`.
