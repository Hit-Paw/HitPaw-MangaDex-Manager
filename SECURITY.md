# Security Policy

## Credentials

HitPaw MangaDex Manager **does not ship with any hardcoded credentials**.

- All MangaDex authentication (username/password, client ID/secret, access tokens) is **user-supplied at runtime** via the login UI (`MainWindow` in `main.cpp:998`).
- Values are stored locally via `QSettings` (`api/client_id`, `api/client_secret`, `api/remember_creds`) on the user machine only and never committed to git.
- The CI workflow (`.github/workflows/release.yml`, `ci.yml`) runs a `scan-secrets` job that fails the build if a pattern like `gho_`, `ghp_`, or hardcoded `client_secret` is found.

## What is NOT in the repo

- `build_win/`, `*.o`, `*.exe`, `*.dll` (excluded by `.gitignore`)
- Any API keys, tokens, or passwords

## Reporting a vulnerability

If you find a credential leak or security issue, please open a private security advisory on GitHub or contact the maintainer directly. Do not file a public issue for sensitive reports.
