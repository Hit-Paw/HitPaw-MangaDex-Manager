---
title: Download — HitPaw MangaDex Manager
description: Download HitPaw MangaDex Manager for Windows, macOS, Linux. Portable, no install, SHA256 + SBOM on every release.
outline: deep
---

# Download

<div class="badge-row">
  <span class="badge badge--accent">Latest: v3.4.6</span>
  <span class="badge">Windows 10/11 x64</span>
  <span class="badge">macOS 13+ • Apple/Intel</span>
  <span class="badge">Linux x64 (Qt6)</span>
</div>

<div class="cta-banner">
  <div class="cta-title">Get HitPaw v3.4.6</div>
  <div class="cta-sub">Portable • ~80 MB • MIT • No ads</div>
  <a class="cta-btn" href="https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest" target="_blank" rel="noreferrer">Go to Latest Release →</a>
  <div style="margin-top:10px; font-size:0.84rem; opacity:.88"><a href="https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases" target="_blank" rel="noreferrer" style="color:white; text-decoration:underline; text-underline-offset:3px">All releases</a> • checksums + SBOM + provenance attached</div>
</div>

## Files

| Platform | File | Size* | Run |
|----------|------|-------|-----|
| **Windows** | `HitPawMangaDexManager-v3.4.6-windows-x64.zip` | ~78 MB | Extract → `MangaDexExporter.exe` |
| **macOS** | `HitPawMangaDexManager-v3.4.6-macOS.tar.gz` | ~65 MB | Extract → `MangaDexExporter.app` or binary |
| **Linux** | `HitPawMangaDexManager-v3.4.6-Linux.tar.gz` | ~62 MB | `tar xzf … && ./MangaDexExporter` |

\* Size varies slightly per release; all Qt DLLs bundled on Windows (no Qt install needed).

## System requirements

- **Windows**: 10 21H2+ or 11 • x64 • 200 MB free • no extra runtime
- **macOS**: 13+ (Ventura+) • Intel or Apple Silicon via Rosetta/bundled Qt • 200 MB
- **Linux**: Ubuntu 22.04+ / Fedora 39+ / Arch — `zlib1g` • 200 MB

Built with **Qt 6.8.3 + MinGW 13.1** (`CMakePresets.json` → `windows-release` / `macos-release` / `linux-release`). See [Building](/building) for exact toolchain.

## Verify your download

Every release publishes a `.sha256`, SBOM, and build provenance (see `release.yml` → *Generate SBOM*, *Attest build provenance*). The version inside the app must match `CMakeLists.txt` `project(VERSION 3.4.6)` and [Changelog](/changelog).

::: code-group

```powershell [Windows — PowerShell]
# In the folder you downloaded to
Get-FileHash .\HitPawMangaDexManager-v3.4.6-windows-x64.zip -Algorithm SHA256
# Compare to HitPawMangaDexManager-v3.4.6-windows-x64.zip.sha256 on the release page
```

```bash [macOS / Linux]
shasum -a 256 HitPawMangaDexManager-v3.4.6-macOS.tar.gz
# or
sha256sum HitPawMangaDexManager-v3.4.6-Linux.tar.gz
cat HitPawMangaDexManager-v3.4.6-Linux.tar.gz.sha256
```

:::

::: tip Mismatch?
Re-download — browsers sometimes truncate. If the hash still mismatches, open an [issue](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=bug_report.yml) and attach the filename + hash.
:::

## What’s inside the zip?

- `MangaDexExporter.exe` (or binary/app) — the app
- `*.dll` (Windows only) — Qt 6.8.3, platforms, imageformats, TLS — all bundled
- No installer, no registry, no admin needed. Delete the folder to uninstall. Covers cache lives at `AppDataLocation/covers` (see *Clear Cover Cache* in app).

## Need to build yourself?

See **[Building from Source](/building)** — one-liners for `qmake` and `cmake --preset` on all three OSes, plus `clang-format` / `scan-secrets` checks before push.

## Older versions

All tags at **[Releases](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases)**. Each tag’s notes are auto-generated from [Changelog](/changelog). We recommend Latest — it includes validated exports and the HitPaw update popup.
