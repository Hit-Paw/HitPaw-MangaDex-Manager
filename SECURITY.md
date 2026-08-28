# Security Policy

## Credentials

HitPaw MangaDex Manager **does not ship with any hardcoded credentials**.

- All MangaDex authentication (username/password, client ID/secret, access tokens) is **user-supplied at runtime** via the login UI (`MainWindow`).
- Values are stored locally via `QSettings` (`api/client_id`, `api/client_secret`, `api/remember_creds`) on the user machine only and never committed to git.
- The CI workflow (`.github/workflows/release.yml`, `ci.yml`) runs a `scan-secrets` job that fails the build if a pattern like `gho_`, `ghp_`, or hardcoded `client_secret` is found.

## What is NOT in the repo

- `build_win/`, `*.o`, `*.exe`, `*.dll` (excluded by `.gitignore`)
- Any API keys, tokens, or passwords

## Roadmap: OS keychain

`secure_store.h:4` currently uses `QSettings` (registry/plist/ini — per-user, per-machine, plaintext on Windows). Planned migration:

1. Add `QKeychain` (`qtkeychain`) as optional dependency — `SecureStore` keeps same API (`clientId()`/`setClientId()` etc.) but swaps backend to `QKeychain::Job` on Windows Credential Manager / macOS Keychain / libsecret on Linux.
2. Fallback to `QSettings` if keychain unavailable; `clearCredentials()` wipes both backends.
3. No API change for callers — see `secure_store.h:16-67` wrapper; CI `scan-secrets` continues to enforce `gho_/ghp_` absence.

Contributions welcome — open an issue before large `secure_store.h` changes.

## Reporting a vulnerability

If you find a credential leak or security issue, please open a private security advisory on GitHub or contact the maintainer directly. Do not file a public issue for sensitive reports.
