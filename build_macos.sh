#!/bin/bash
set -e

echo "====================================================="
echo " HitPaw MangaDex Manager - macOS Build"
echo "====================================================="

# ── Find Qt6 ──────────────────────────────────────────
QT_PATH=""

# Check Homebrew Qt6
if [ -z "$QT_PATH" ] && [ -d "$(brew --prefix qt@6 2>/dev/null)/bin" ]; then
    QT_PATH="$(brew --prefix qt@6)"
fi

# Check standard locations
for DIR in /usr/local/opt/qt@6 /opt/homebrew/opt/qt@6 ~/Qt/6.*; do
    if [ -z "$QT_PATH" ] && [ -d "$DIR" ]; then
        QT_PATH="$DIR"
    fi
done

if [ -z "$QT_PATH" ]; then
    echo "[ERROR] Qt6 not found. Install with: brew install qt@6"
    exit 1
fi

echo "[OK] Found Qt6 at: $QT_PATH"
export PATH="$QT_PATH/bin:$PATH"

# ── Check for zlib ─────────────────────────────────────
if ! command -v pkg-config &> /dev/null || ! pkg-config --exists zlib 2>/dev/null; then
    echo "[WARN] zlib not found via pkg-config. Install with: brew install zlib"
    echo "       If already installed, you may need: export PKG_CONFIG_PATH=\"$(brew --prefix zlib)/lib/pkgconfig\""
fi

# ── Build ──────────────────────────────────────────────
echo ""
echo "Building..."

if [ -d build_macos ]; then
    rm -rf build_macos
fi
mkdir build_macos
cd build_macos

qmake ../MangaDexExporter.pro
make -j$(sysctl -n hw.ncpu)

cd ..

# ── Find app bundle ───────────────────────────────────
APP="build_macos/MangaDexExporter.app"
if [ ! -d "$APP" ]; then
    echo "[WARN] .app bundle not found - check build_macos/ manually"
    exit 1
fi

echo ""
echo "[OK] Built: $APP"
echo ""
echo "Deploying Qt frameworks..."
macdeployqt "$APP" -dmg

# ── Find DMG ──────────────────────────────────────────
DMG=$(find build_macos -name "*.dmg" | head -1)
if [ -n "$DMG" ]; then
    echo ""
    echo "====================================================="
    echo " DONE. DMG: $DMG"
    echo "====================================================="
else
    echo ""
    echo "====================================================="
    echo " DONE. App bundle: $APP"
    echo " (macdeployqt did not produce a DMG — distribute the .app)"
    echo "====================================================="
fi
