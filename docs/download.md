---
title: Download
description: Download HitPaw MangaDex Manager for Windows, macOS, Linux. Portable, no install, SHA256 + SBOM on every release. Verify in 10s.
outline: deep
---

# Download

<div class="badge-row">
  <span class="badge badge--accent">v3.5.0 Latest</span>
  <span class="badge">Windows 10/11 x64</span>
  <span class="badge">macOS 13+ • Apple/Intel</span>
  <span class="badge">Linux x64</span>
</div>

<div class="cta-banner" id="download-cta">
  <div class="cta-title">Get HitPaw MangaDex Manager</div>
  <div class="cta-sub">Portable • ~80 MB • MIT • No ads • <span id="os-hint">Auto-detecting your OS…</span></div>
  <a class="cta-btn" href="https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases/latest" target="_blank" rel="noreferrer">Go to Latest Release →</a>
  <div style="margin-top:10px; font-size:0.84rem; opacity:.88"><a href="https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases" target="_blank" rel="noreferrer" style="color:var(--hp-orange-soft); text-decoration:underline; text-underline-offset:3px">All releases</a> • checksums + SBOM + provenance attached</div>
</div>

## Files

| Platform | File | Size | Run | Notes |
|----------|------|------|-----|-------|
| **Windows** | `HitPawMangaDexManager-v*-windows-x64.zip` | ~78 MB | Extract → `MangaDexExporter.exe` | DLLs bundled, no runtime |
| **macOS** | `HitPawMangaDexManager-v*-macOS.tar.gz` | ~65 MB | Extract → `MangaDexExporter.app` | Universal-ish via Qt 6.x |
| **Linux** | `HitPawMangaDexManager-v*-Linux.tar.gz` | ~62 MB | `tar xzf … && ./MangaDexExporter` | Needs `zlib1g` |

::: tip Tip: which file for me?
Windows → zip. macOS → macOS tar.gz. Linux → Linux tar.gz. All three are portable — no installer. Delete the folder to uninstall.
:::

## System requirements

<div class="why-grid">
  <div class="why-card">
    <div class="icon-wrap"><img src="/icons/outline-layers.svg" alt="" width="22" height="22" loading="lazy" /></div>
    <strong>Windows</strong>
    <p>10 21H2+ or 11 • x64 • 200 MB free • no extra runtime • SmartScreen “More info → Run anyway” first launch</p>
  </div>
  <div class="why-card">
    <div class="icon-wrap"><img src="/icons/outline-palette.svg" alt="" width="22" height="22" loading="lazy" /></div>
    <strong>macOS</strong>
    <p>13+ Ventura+ • Intel or Apple Silicon • 200 MB • <code>xattr -dr com.apple.quarantine</code> if gatekeeper blocks</p>
  </div>
  <div class="why-card">
    <div class="icon-wrap"><img src="/icons/outline-package.svg" alt="" width="22" height="22" loading="lazy" /></div>
    <strong>Linux</strong>
    <p>Ubuntu 22.04+ / Fedora 39+ / Arch • <code>zlib1g</code> + <code>libxcb</code> • 200 MB • Wayland & X11</p>
  </div>
</div>

Built with **Qt 6.8.3 + MinGW 13.1**. See [Building](/building) for exact pinned toolchain — CI matches `install-qt-action@v4`.

## Verify your download — 10 seconds

Every release publishes a `.sha256`, SBOM (CycloneDX), and build provenance (SLSA). Verify before you run:

::: code-group

```powershell [Windows]
Get-FileHash .\HitPawMangaDexManager-v*-windows-x64.zip -Algorithm SHA256
# Compare to the .sha256 file on the release page — must match exactly
type .\HitPawMangaDexManager-v*-windows-x64.zip.sha256
```

```bash [macOS]
shasum -a 256 HitPawMangaDexManager-v*-macOS.tar.gz
# or compare directly
shasum -a 256 -c HitPawMangaDexManager-v*-macOS.tar.gz.sha256
```

```bash [Linux]
sha256sum HitPawMangaDexManager-v*-Linux.tar.gz
sha256sum -c HitPawMangaDexManager-v*-Linux.tar.gz.sha256
```

:::

::: tip Mismatch?
Re-download — browsers sometimes truncate large zips. If it still fails, open an [issue](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=bug_report.yml) with OS, browser, and hash.
:::

## What's inside the archive?

- `MangaDexExporter` (or `.exe` / `.app`) — the app, no installer
- `*.dll` (Windows only) — all Qt 6.8.3 DLLs bundled, side-by-side, no system Qt needed
- No registry, no background service, no admin prompt. `QSettings` keeps your login locally only. Delete the extracted folder to uninstall.

## Update

HitPaw checks silently 3.5 s after launch via GitHub API. If newer version exists, Activity Log shows it — click **Download Latest Version** for one-click. Or **Check for updates** manually. Icon appears on every popup (v3.4.6+).

## Older versions

All tags at **[Releases](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/releases)**. We recommend **Latest** — stays in sync with MangaDex API changes. Check [Changelog](/changelog) for what's new in v3.5.0 (MDList sync).

<script setup>
import { onMounted } from 'vue'
onMounted(() => {
  try {
    const ua = navigator.userAgent || ''
    const platform = navigator.platform || ''
    const el = document.getElementById('os-hint')
    if (!el) return
    let msg = ''
    let file = ''
    if (/Windows/.test(ua)) { msg = 'Detected Windows — get the .zip'; file = 'windows-x64.zip' }
    else if (/Mac/.test(platform) || /Mac/.test(ua)) { msg = 'Detected macOS — get the macOS tar.gz'; file = 'macOS.tar.gz' }
    else if (/Linux/.test(ua)) { msg = 'Detected Linux — get the Linux tar.gz'; file = 'Linux.tar.gz' }
    else if (/Android/.test(ua)) { msg = 'On mobile — download on desktop for best experience' }
    if (msg) el.textContent = msg
    // Highlight matching row
    if (file) {
      document.querySelectorAll('table tbody tr').forEach(tr => {
        if (tr.textContent.includes(file)) {
          tr.style.background = 'rgba(255,106,0,0.08)'
          tr.style.outline = '1px solid rgba(255,106,0,0.18)'
          tr.style.borderRadius = '8px'
        }
      })
    }
  } catch {}
})
</script>
