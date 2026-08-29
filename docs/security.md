---
title: Security — HitPaw MangaDex Manager
description: How HitPaw stores credentials locally via QSettings, OS keychain roadmap, and how to report vulnerabilities.
outline: deep
---

# Security Policy

<div class="badge-row">
  <span class="badge badge--accent">No hardcoded secrets</span>
  <span class="badge">QSettings • local only</span>
  <span class="badge">scan-secrets CI</span>
</div>

## Credentials — user-supplied only

HitPaw **ships with zero credentials**.

- All authentication (username/password, Client ID/Secret, tokens) is entered by you in the login UI
- Stored locally via `QSettings` — Windows Registry / macOS plist / Linux INI, per-user, per-machine
- Never committed to git, never sent anywhere except MangaDex's API (`api.mangadex.org`)

::: warning What is NOT in the repo
Any API keys, tokens, or passwords — all excluded by `.gitignore`. The `scan-secrets` job fails the build if secrets appear hardcoded.
:::

### Quick self-check

```bash
# Must print nothing — run before every push
grep -R -n -E "gho_[A-Za-z0-9_]{20,}|ghp_[A-Za-z0-9_]{20,}" --include="*.cpp" --include="*.h" .
```

Also covered by the local `pre-commit` hook.

## Roadmap — OS keychain

Planned migration from `QSettings` to `QKeychain`:

1. **QKeychain** (`qtkeychain`) — uses Windows Credential Manager / macOS Keychain / libsecret
2. Fallback to `QSettings` if keychain unavailable
3. No caller changes — same API (`clientId()` / `setClientId()`)

Contributions welcome — please open an issue before large `secure_store.h` changes.

## Reporting a vulnerability

Do **not** file a public issue for sensitive reports.

- **Preferred**: GitHub → Security → [Report a vulnerability](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/security/advisories/new)
- **Alternative**: Discord DM via [discord.gg/z6yYYpcYYc](https://discord.gg/z6yYYpcYYc)

Include: version, OS, steps, impact. We aim to acknowledge within 48h and fix within 14 days.

Non-sensitive bugs → [Bug report](https://github.com/Hit-Paw/HitPaw-MangaDex-Manager/issues/new?template=bug_report.yml).

## Hardening tips

- Enable **Remember credentials** only on private machines
- Use a **unique API client** per device (MangaDex → Settings → API Clients)
- Keep HitPaw updated — silent check runs at launch

See [Getting Started](/getting-started) for setup and [Contributing](/contributing) for the `scan-secrets` workflow.
