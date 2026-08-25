#!/bin/bash
set -e

echo "====================================================="
echo " HitPaw MangaDex Manager - Linux Build"
echo "====================================================="

# ── Find Qt6 ──────────────────────────────────────────
QT_PATH=""

# Check standard locations
for DIR in /usr/lib/qt6 /usr/lib/x86_64-linux-gnu/qt6 ~/Qt/6.*; do
    if [ -z "$QT_PATH" ] && [ -d "$DIR" ]; then
        QT_PATH="$DIR"
    fi
done

if [ -z "$QT_PATH" ]; then
    echo "[ERROR] Qt6 not found."
    echo "Install with: sudo apt install qt6-base-dev zlib1g-dev"
    echo "          or: sudo dnf install qt6-qtbase-devel zlib-devel"
    exit 1
fi

echo "[OK] Found Qt6 at: $QT_PATH"

# ── Check dependencies ────────────────────────────────
echo ""
echo "Checking dependencies..."

MISSING=""
for CMD in qmake6 g++ pkg-config; do
    if ! command -v $CMD &> /dev/null; then
        MISSING="$MISSING $CMD"
    fi
done

if [ -n "$MISSING" ]; then
    echo "[ERROR] Missing:$MISSING"
    echo "Install with: sudo apt install qt6-base-dev zlib1g-dev g++ pkg-config"
    exit 1
fi

if ! pkg-config --exists zlib 2>/dev/null; then
    echo "[ERROR] zlib not found. Install with: sudo apt install zlib1g-dev"
    exit 1
fi

echo "[OK] All dependencies found"

# ── Build ──────────────────────────────────────────────
echo ""
echo "Building..."

if [ -d build_linux ]; then
    rm -rf build_linux
fi
mkdir build_linux
cd build_linux

qmake6 ../MangaDexExporter.pro
make -j$(nproc)

cd ..

# ── Find executable ───────────────────────────────────
EXE=$(find build_linux -name "MangaDexExporter" -type f | head -1)
if [ -z "$EXE" ]; then
    echo "[WARN] Executable not found - check build_linux/ manually"
    exit 1
fi

echo ""
echo "[OK] Built: $EXE"

# ── Deploy Qt libraries ──────────────────────────────
echo ""
echo "Bundling Qt libraries..."
mkdir -p build_linux/dist
cp "$EXE" build_linux/dist/
ldd "$EXE" | grep -oP '/[^\s]+' | while read LIB; do
    if echo "$LIB" | grep -q "libQt\|libicu"; then
        cp "$LIB" build_linux/dist/ 2>/dev/null || true
    fi
done

echo ""
echo "====================================================="
echo " DONE. Files in: build_linux/dist/"
echo "====================================================="
