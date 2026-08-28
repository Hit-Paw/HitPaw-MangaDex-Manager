#pragma once
// SecureStore — centralized, credential-free storage for user-supplied secrets.
// All values come from QSettings (per-user, per-machine) and are never hardcoded.
// Future: swap QSettings backend for OS keychain (QKeychain / Credential Manager / libsecret)
// without changing call sites. For now this is a thin wrapper that makes the
// security boundary explicit and greppable.
//
// Usage: SecureStore store; store.clientId() / store.setClientId(id)
// Public repo CI scans for `gho_/ghp_` and `client_secret = "literal"` and fails
// if any hardcoded secret appears — this class ensures all secrets are
// QSettings-driven (see .github/workflows/release.yml scan-secrets).

#include <QSettings>
#include <QString>

class SecureStore {
public:
    explicit SecureStore(const QString &org = QStringLiteral("SentinelFlow"),
                         const QString &app = QStringLiteral("HitPawMangaDexExporter"))
        : m_settings(org, app) {}

    // QSettings accessor for non-secret prefs (export dir, pagination, etc.)
    QSettings &settings() { return m_settings; }
    const QSettings &settings() const { return m_settings; }

    // --- API credentials (user-supplied via Settings UI) ---
    QString clientId() const { return m_settings.value(kClientId, QString()).toString(); }
    QString clientSecret() const { return m_settings.value(kClientSecret, QString()).toString(); }
    bool rememberCreds() const { return m_settings.value(kRememberCreds, true).toBool(); }

    void setClientId(const QString &v) { m_settings.setValue(kClientId, v); }
    void setClientSecret(const QString &v) { m_settings.setValue(kClientSecret, v); }
    void setRememberCreds(bool on) { m_settings.setValue(kRememberCreds, on); }

    void clearCredentials() {
        m_settings.remove(kClientId);
        m_settings.remove(kClientSecret);
    }

    // --- Auth tokens (obtained at runtime, never shipped) ---
    QString accessToken() const { return m_settings.value(kAccessToken, QString()).toString(); }
    QString refreshToken() const { return m_settings.value(kRefreshToken, QString()).toString(); }
    QString username() const { return m_settings.value(kUsername, QString()).toString(); }

    void setAccessToken(const QString &v) { m_settings.setValue(kAccessToken, v); }
    void setRefreshToken(const QString &v) { m_settings.setValue(kRefreshToken, v); }
    void setUsername(const QString &v) { m_settings.setValue(kUsername, v); }

    void clearAuth() {
        m_settings.remove(kAccessToken);
        m_settings.remove(kRefreshToken);
    }

    void sync() { m_settings.sync(); }

private:
    // Keys are centralized here so a repo-wide grep for "client_secret" only
    // finds QSettings access via this wrapper, which the CI allowlists.
    static constexpr const char *kClientId      = "api/client_id";
    static constexpr const char *kClientSecret  = "api/client_secret";
    static constexpr const char *kRememberCreds = "api/remember_creds";
    static constexpr const char *kAccessToken   = "auth/access_token";
    static constexpr const char *kRefreshToken  = "auth/refresh_token";
    static constexpr const char *kUsername      = "auth/username";

    QSettings m_settings;
};
