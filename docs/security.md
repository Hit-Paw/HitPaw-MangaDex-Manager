---
title: Security — HitPaw MangaDex Manager
description: How HitPaw stores credentials locally via QSettings, what is not in the repo, OS keychain roadmap, and how to report vulnerabilities.
outline: deep
---

# Security Policy

<div class="badge-row">
  <span class="badge badge--accent">No hardcoded secrets</span>
  <span class="badge">QSettings • local only</span>
  <span class="badge">scan-secrets CI</span>
  <span class="badge">MIT</span>
</div>

## Credentials — user-supplied at runtime only

HitPaw **ships with zero credentials**.

- All MangaDex authentication — **username/password, Client ID/Secret, access & refresh tokens** — is entered by you in the login UI (`MainWindow`).
- Stored locally via `QSettings` keys `api/client_id`, `api/client_secret`, `api/remember_creds` — Windows Registry / macOS plist / Linux INI, per-user, per-machine.
- Never committed to git, never sent anywhere except MangaDex’s own API (`https://api.mangadex.org`).

::: warning What is NOT in the repo
`build_win/` • `*.o` / `*.exe` / `*.dll` / `*.so` • any API keys, tokens, or passwords — all excluded by [`.gitignore`](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/blob/main/.gitignore). The [`scan-secrets`](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/blob/main/.github/workflows/ci.yml) job fails the build if `gho_` / `ghp_` or `client_secret` appears hardcoded.
:::

### Quick self-check

```bash
# Must print nothing — run before every push
grep -R -n -E "gho_[A-Za-z0-9_]{20,}|ghp_[A-Za-z0-9_]{20,}" --include="*.cpp" --include="*.h" --include="*.pro" .
clang-format --dry-run --Werror main.cpp export.h domain.h pal.h secure_store.h
```

Also covered by the local `pre-commit` hook (`.pre-commit-config.yaml`).

## Roadmap — OS keychain

`secure_store.h:4` currently uses `QSettings` (plaintext per-user on Windows). Planned migration keeps the same API (`clientId()` / `setClientId()` etc.) but swaps backend when available:

1. Add **`QKeychain` (`qtkeychain`)** optional dependency — `SecureStore` keeps API but uses `QKeychain::Job` → **Windows Credential Manager / macOS Keychain / libsecret (Linux)**.
2. Fallback to `QSettings` if keychain unavailable; `clearCredentials()` wipes both backends.
3. No caller changes — see `secure_store.h:16-67` wrapper. CI `scan-secrets` continues to enforce absence of `gho_/ghp_`.

Contributions welcome — please open an issue before large `secure_store.h` changes so API stays stable.

## Reporting a vulnerability

Do **not** file a public issue for sensitive reports.

- **Preferred**: GitHub → *Security* → *Report a vulnerability* (private advisory) on [Hit-Paw/HitPaw-MangaDex-Manager](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/security/advisories/new)
- **Alternative**: Discord DM to maintainer via [discord.gg/z6yYYpcYYc](https://discord.gg/z6yYYpcYYc) or email listed in the advisory
- Include: version (`v3.4.6`), OS, steps, impact, and whether credentials were exposed. We aim to acknowledge within 48h and fix within 14 days.

Non-sensitive bugs → [Bug report](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=bug_report.yml) is fine.

## Hardening tips for users

- Enable **Remember credentials** only on private machines — otherwise sign out before closing; `QSettings` is per-OS-user but not encrypted on Windows (until keychain lands).
- Use a **unique MangaDex API client** per device (MangaDex → Settings → API Clients) so you can revoke one without killing all.
- Keep HitPaw updated — `Check for updates` in-app plus the silent 3.5s startup check logs `Update available: vX → vY`.

See [Getting Started](/getting-started) for setup and [Contributing](/contributing) for the `scan-secrets` workflow.
