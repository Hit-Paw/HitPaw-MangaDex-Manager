@echo off
setlocal enabledelayedexpansion

echo =====================================================
echo  HitPaw MangaDex Exporter - Windows Build
echo =====================================================
echo.

:: ── Find Qt6 ──────────────────────────────────────────
set QT_ROOT=
set QT_BASE=

for %%R in (C:\Qt D:\Qt %USERPROFILE%\Qt) do (
    if exist "%%R" (
        for /d %%V in ("%%R\6.*") do (
            for /d %%C in ("%%V\mingw_64" "%%V\mingw64" "%%V\msvc2022_64" "%%V\msvc2019_64") do (
                if exist "%%C\bin\qmake.exe" (
                    if "!QT_ROOT!"=="" (
                        set QT_ROOT=%%C
                        set QT_BASE=%%R
                    )
                )
            )
        )
    )
)

if "!QT_ROOT!"=="" (
    echo [ERROR] Qt6 not found in C:\Qt, D:\Qt, or %USERPROFILE%\Qt
    echo Install Qt6 from https://www.qt.io/download-open-source
    pause & exit /b 1
)

echo [OK] Found Qt6 at: "!QT_ROOT!"
set PATH=!QT_ROOT!\bin;%PATH%

:: ── Find MinGW 1310 64-bit ONLY ───────────────────────
:: Qt 6.11 requires GCC 13. Never use 810 or older.
set MINGW_BIN=

for %%R in (C:\Qt D:\Qt %USERPROFILE%\Qt) do (
    if exist "%%R\Tools\mingw1310_64\bin\mingw32-make.exe" (
        if "!MINGW_BIN!"=="" set MINGW_BIN=%%R\Tools\mingw1310_64\bin
    )
)

if "!MINGW_BIN!"=="" (
    echo [ERROR] mingw1310_64 not found under Qt\Tools.
    echo Open Qt Maintenance Tool ^> Add/Remove Components ^> Tools ^> MinGW 13.1.0 64-bit
    pause & exit /b 1
)

set PATH=!MINGW_BIN!;!PATH!
echo [OK] Found MinGW at: "!MINGW_BIN!"
set MAKE_CMD=mingw32-make -j4
set SPEC=win32-g++

:build
echo.
echo Building...

if exist build_win rmdir /s /q build_win
mkdir build_win
cd build_win

qmake.exe ..\MangaDexExporter.pro -spec !SPEC!
if %errorlevel% neq 0 ( echo [ERROR] qmake failed & cd .. & pause & exit /b 1 )

!MAKE_CMD!
if %errorlevel% neq 0 ( echo [ERROR] Build failed & cd .. & pause & exit /b 1 )

cd ..

:: ── Find exe ──────────────────────────────────────────
set EXE=
for /r build_win %%F in (MangaDexExporter.exe) do set EXE=%%F

if "!EXE!"=="" (
    echo [WARN] .exe not found - check build_win\release\ manually
    pause & exit /b 1
)

echo.
echo [OK] Built: !EXE!
echo.
echo Deploying Qt DLLs...
windeployqt.exe "!EXE!"

echo.
echo =====================================================
echo  DONE. Run: !EXE!
echo =====================================================
pause
