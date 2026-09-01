---
title: Download — HitPaw MangaDex Manager
description: Download HitPaw MangaDex Manager for Windows, macOS, Linux. Portable, no install, SHA256 + SBOM on every release.
outline: deep
---

# Download

<div class="badge-row">
  <span class="badge badge--accent">Latest</span>
  <span class="badge">Windows 10/11 x64</span>
  <span class="badge">macOS 13+ • Apple/Intel</span>
  <span class="badge">Linux x64</span>
</div>

<div class="cta-banner">
  <div class="cta-title">Get HitPaw MangaDex Manager</div>
  <div class="cta-sub">Portable • ~80 MB • MIT • No ads</div>
  <a class="cta-btn" href="https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest" target="_blank" rel="noreferrer">Go to Latest Release →</a>
  <div style="margin-top:10px; font-size:0.84rem; opacity:.88"><a href="https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases" target="_blank" rel="noreferrer" style="color:white; text-decoration:underline; text-underline-offset:3px">All releases</a> • checksums + SBOM attached</div>
</div>

## Files

| Platform | File | Size | Run |
|----------|------|------|-----|
| **Windows** | `HitPawMangaDexManager-v*-windows-x64.zip` | ~78 MB | Extract → `MangaDexExporter.exe` |
| **macOS** | `HitPawMangaDexManager-v*-macOS.tar.gz` | ~65 MB | Extract → `MangaDexExporter.app` |
| **Linux** | `HitPawMangaDexManager-v*-Linux.tar.gz` | ~62 MB | `tar xzf … && ./MangaDexExporter` |

## System requirements

- **Windows**: 10 21H2+ or 11 • x64 • 200 MB free • no extra runtime
- **macOS**: 13+ (Ventura+) • Intel or Apple Silicon • 200 MB
- **Linux**: Ubuntu 22.04+ / Fedora 39+ / Arch • `zlib1g` • 200 MB

Built with **Qt 6.8.3 + MinGW 13.1**. See [Building](/building) for exact toolchain.

## Verify your download

Every release publishes a `.sha256`, SBOM, and build provenance.

::: code-group

```powershell [Windows]
Get-FileHash .\HitPawMangaDexManager-v*-windows-x64.zip -Algorithm SHA256
# Compare to the .sha256 file on the release page
```

```bash [macOS / Linux]
shasum -a 256 HitPawMangaDexManager-v*-macOS.tar.gz
# or
sha256sum HitPawMangaDexManager-v*-Linux.tar.gz
```

:::

::: tip Mismatch?
Re-download — browsers sometimes truncate. If it still fails, open an [issue](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=bug_report.yml).
:::

## What's inside the zip?

- `MangaDexExporter.exe` (or binary/app) — the app
- `*.dll` (Windows only) — all Qt DLLs bundled
- No installer, no registry, no admin needed. Delete the folder to uninstall.

## Older versions

All tags at **[Releases](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases)**. We recommend Latest.
