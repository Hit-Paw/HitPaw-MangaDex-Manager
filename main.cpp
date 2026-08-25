/*
 * HitPaw — MangaDex Manager
 * C++ / Qt6 — refreshed orange "HitPaw" theme (matches app icon),
 *             gradient accents, pill tabs, layered surfaces, cover grid, persistent login
 */

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QProgressBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QScrollArea>
#include <QWheelEvent>
#include <QFrame>
#include <QTabWidget>
#include <QStackedWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDate>
#include <QMap>
#include <QHash>
#include <QSet>
#include <QList>
#include <QStringList>
#include <QPointer>
#include <QRegularExpression>
#include <QClipboard>
#include <QGuiApplication>
#include <QUrlQuery>
#include <QScrollBar>
#include <QIcon>
#include <QPixmap>
#include <QSettings>
#include <QCryptographicHash>
#include <QDesktopServices>
#include <QSharedMemory>
#include <QShortcut>
#include <QKeySequence>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPaintEvent>
#include <QEnterEvent>
#include <QTabBar>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QStyleOptionButton>

#include <zlib.h>
#include <algorithm>
#include <functional>

// ── Constants ─────────────────────────────────────────────────────────────────

static const char UA[] = "HitPawMangaDexExporter/3.0";

static const char API_BASE[]   = "https://api.mangadex.org";
static const char TOKEN_URL[]  = "https://auth.mangadex.org/realms/mangadex/protocol/openid-connect/token";
static const char COVER_BASE[] = "https://uploads.mangadex.org/covers";

// No app-wide default client ID/secret — every user provides their own via
// Settings (see m_clientId/m_clientSecret, stored per-user in QSettings).
// A public GitHub repo can't ship a personal MangaDex API credential in
// source — anyone who cloned it would be using it as your app identity.

// ── Palette — matched to the HitPaw icon (orange / white / near-black) ────────

namespace Pal {
    // PDark-derived palette — deep charcoal-orange-purple, matches the executor theme
    constexpr auto BG       = "#0a0a0c";   // outer shell — near-black
    constexpr auto SURFACE  = "#0e0e10";   // nav rail / inputs
    constexpr auto CARD     = "#141416";   // card body
    constexpr auto ELEV     = "#1b1b1e";   // elevated card
    constexpr auto BORDER   = "#242429";   // default border
    constexpr auto BORDER_H = "#3c3c43";   // hover border
    constexpr auto ACCENT   = "#ff6a00";   // primary orange  (executor keyword #ff6a00)
    constexpr auto ACCENT_H = "#ff8c3a";   // hover orange  (executor string #ff8c3a)
    constexpr auto ACCENT_D = "#cc5500";   // pressed
    constexpr auto ACCENT2  = "#BB9AF7";   // purple secondary  (executor keyword purple)
    constexpr auto ON_ACCENT= "#1a0800";   // dark text on orange
    constexpr auto TEXT     = "#D3DBE8";   // body text — brightened slightly for contrast
    constexpr auto MUTED    = "#83838c";   // muted text
    constexpr auto GREEN    = "#9ECE6A";   // success  (executor string green)
    constexpr auto RED      = "#F7768E";   // error  (executor self/error red)
    constexpr auto ENTRY    = "#0a0a0c";   // input field bg
    constexpr auto CODE     = "#111113";   // code block bg
}

// ── Domain types ──────────────────────────────────────────────────────────────

struct MangaEntry {
    QString id;
    QString title;
    QString status;
    QString statusLabel;
    QString year;
    QString contentRating;
    QString languages;
    bool    hasEnglish = false;
    QString authors;
    QString artists;       // distinct from authors
    QString genres;        // comma-joined tag names (genre + theme groups)
    QString demographic;   // shounen / shoujo / seinen / josei
    QString url;
    QString coverUrl;
};

static const QMap<QString,QString> STATUS_LABELS = {
    {"reading",      "Reading"},
    {"completed",    "Completed"},
    {"on_hold",      "On Hold"},
    {"dropped",      "Dropped"},
    {"plan_to_read", "Plan to Read"},
    {"re_reading",   "Re-reading"},
};

static const QMap<QString,QString> MAL_STATUS = {
    {"reading",      "Reading"},
    {"completed",    "Completed"},
    {"on_hold",      "On-Hold"},
    {"dropped",      "Dropped"},
    {"plan_to_read", "Plan to Read"},
    {"re_reading",   "Reading"},
};

static QString statusColor(const QString& status) {
    if (status == "reading")      return Pal::ACCENT;
    if (status == "completed")    return Pal::GREEN;
    if (status == "on_hold")      return "#d29922";
    if (status == "dropped")      return Pal::RED;
    if (status == "re_reading")   return Pal::ACCENT_H;
    return Pal::MUTED;
}


// MangaBaka uses its own status vocabulary
static const QMap<QString,QString> MB_STATUS = {
    {"reading",      "reading"},
    {"completed",    "completed"},
    {"on_hold",      "paused"},
    {"dropped",      "dropped"},
    {"plan_to_read", "plan_to_read"},
    {"re_reading",   "rereading"},
};

// Parse one /manga item (with includes[] author + cover_art) into a MangaEntry.
static MangaEntry parseManga(const QJsonObject& obj, const QString& status) {
    const auto attrs = obj["attributes"].toObject();

    const auto titleMap = attrs["title"].toObject();
    QString title;
    if (titleMap.contains("en"))       title = titleMap["en"].toString();
    else if (!titleMap.isEmpty())      title = titleMap.begin()->toString();
    else                               title = "Unknown";

    QStringList authors;
    QStringList artists;
    QString coverFile;
    for (const auto& rel : obj["relationships"].toArray()) {
        const auto r = rel.toObject();
        const auto t = r["type"].toString();
        if (t == "author" && r.contains("attributes"))
            authors << r["attributes"].toObject()["name"].toString();
        else if (t == "artist" && r.contains("attributes"))
            artists << r["attributes"].toObject()["name"].toString();
        else if (t == "cover_art" && r.contains("attributes"))
            coverFile = r["attributes"].toObject()["fileName"].toString();
    }

    // Deduplicate artists that are also listed as authors
    QStringList artistsOnly;
    for (const auto& a : artists)
        if (!authors.contains(a)) artistsOnly << a;

    QStringList langs;
    for (const auto& l : attrs["availableTranslatedLanguages"].toArray())
        langs << l.toString();

    // Tags → genres (group "genre" and "theme"), demographic from publicationDemographic
    QStringList genreList;
    for (const auto& tagVal : attrs["tags"].toArray()) {
        const auto tag   = tagVal.toObject();
        const auto tAttrs = tag["attributes"].toObject();
        const auto group = tAttrs["group"].toString();
        if (group == "genre" || group == "theme") {
            const auto nameMap = tAttrs["name"].toObject();
            const QString name = nameMap.contains("en") ? nameMap["en"].toString()
                                                        : nameMap.begin()->toString();
            if (!name.isEmpty()) genreList << name;
        }
    }
    const QString demographic = attrs["publicationDemographic"].toString();

    MangaEntry e;
    e.id            = obj["id"].toString();
    e.title         = title;
    e.status        = status;
    e.statusLabel   = STATUS_LABELS.value(status, "Not in Library");
    e.year          = attrs["year"].isNull() ? "" : QString::number(attrs["year"].toInt());
    e.contentRating = attrs["contentRating"].toString();
    e.languages     = langs.join(", ");
    e.hasEnglish    = langs.contains("en");
    e.authors       = authors.join(", ");
    e.artists       = artistsOnly.join(", ");
    e.genres        = genreList.join(", ");
    e.demographic   = demographic;
    e.url           = "https://mangadex.org/title/" + e.id;
    if (!coverFile.isEmpty())
        e.coverUrl = QString("%1/%2/%3.256.jpg").arg(COVER_BASE, e.id, coverFile);
    return e;
}

// ── Download domain ───────────────────────────────────────────────────────────

struct ChapterInfo {
    QString id;
    QString volume;
    QString chapter;
    QString title;
    QString lang;
    QString group;
    int     pages = 0;
};

static QString sanitizeName(const QString& name) {
    // Strip characters illegal in Windows/macOS/Linux filenames
    QString s = name;
    static const QRegularExpression illegal(R"([\\/:*?"<>|])");
    s.replace(illegal, "_");
    s = s.trimmed();
    if (s.isEmpty()) s = "Unknown";
    if (s.length() > 200) s = s.left(200);
    return s;
}

static QString chapterFolderName(const ChapterInfo& c) {
    QString label = "Chapter ";
    if (!c.volume.isEmpty())  label = "Vol." + c.volume + " Ch." + (c.chapter.isEmpty() ? "0" : c.chapter);
    else if (!c.chapter.isEmpty()) label += c.chapter;
    else label += "Oneshot";
    if (!c.title.isEmpty()) label += " - " + c.title;
    return sanitizeName(label);
}

// ── Exporters ─────────────────────────────────────────────────────────────────

namespace Export {

static const QStringList CSV_FIELDS = {
    "title","status_label","year","content_rating",
    "languages","has_english","authors","url","id"
};

void toCSV(const QList<MangaEntry>& lib, const QString& path) {
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream s(&f);
    s.setEncoding(QStringConverter::Utf8);
    s << CSV_FIELDS.join(",") << "\n";
    for (const auto& e : lib) {
        auto esc = [](const QString& v) -> QString {
            if (v.contains(',') || v.contains('"') || v.contains('\n'))
                return '"' + QString(v).replace("\"","\"\"") + '"';
            return v;
        };
        s << esc(e.title)          << ","
          << esc(e.statusLabel)    << ","
          << esc(e.year)           << ","
          << esc(e.contentRating)  << ","
          << esc(e.languages)      << ","
          << (e.hasEnglish ? "true" : "false") << ","
          << esc(e.authors)        << ","
          << esc(e.url)            << ","
          << esc(e.id)             << "\n";
    }
}

void toJSON(const QList<MangaEntry>& lib, const QString& path) {
    QJsonArray arr;
    for (const auto& e : lib) {
        QJsonObject o;
        o["id"]             = e.id;
        o["title"]          = e.title;
        o["status"]         = e.status;
        o["status_label"]   = e.statusLabel;
        o["year"]           = e.year;
        o["content_rating"] = e.contentRating;
        o["languages"]      = e.languages;
        o["has_english"]    = e.hasEnglish;
        o["authors"]        = e.authors;
        o["url"]            = e.url;
        o["cover_url"]      = e.coverUrl;
        arr.append(o);
    }
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    f.write(QJsonDocument(arr).toJson(QJsonDocument::Indented));
}

void toMALXML(const QList<MangaEntry>& lib, const QString& path) {
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream s(&f);
    s.setEncoding(QStringConverter::Utf8);

    s << R"(<?xml version="1.0" encoding="UTF-8"?>)" "\n"
      << "<myanimelist>\n"
      << "  <myinfo>\n"
      << "    <user_export_type>2</user_export_type>\n"
      << "    <user_total_manga>" << lib.size() << "</user_total_manga>\n"
      << "  </myinfo>\n";

    const QString today = QDate::currentDate().toString("yyyy-MM-dd");
    for (const auto& e : lib) {
        const QString malSt     = MAL_STATUS.value(e.status, "Plan to Read");
        const bool    isReading = (e.status == "reading" || e.status == "re_reading");
        const bool    isDone    = (e.status == "completed");
        const QString startDate = isReading || isDone ? today : "0000-00-00";
        const QString finDate   = isDone ? today : "0000-00-00";
        const QString rereading = (e.status == "re_reading") ? "YES" : "NO";
        s << "<manga>"
          << "<manga_mangadb_id>0</manga_mangadb_id>"
          << "<manga_title><![CDATA[" << e.title << "]]></manga_title>"
          << "<manga_volumes>0</manga_volumes>"
          << "<manga_chapters>0</manga_chapters>"
          << "<my_id>0</my_id>"
          << "<my_read_volumes>0</my_read_volumes>"
          << "<my_read_chapters>0</my_read_chapters>"
          << "<my_start_date>" << startDate << "</my_start_date>"
          << "<my_finish_date>" << finDate << "</my_finish_date>"
          << "<my_scanalation_group><![CDATA[]]></my_scanalation_group>"
          << "<my_score>0</my_score>"
          << "<my_storage></my_storage>"
          << "<my_retail_volumes>0</my_retail_volumes>"
          << "<my_status>" << malSt << "</my_status>"
          << "<my_comments><![CDATA[]]></my_comments>"
          << "<my_times_read>0</my_times_read>"
          << "<my_tags><![CDATA[]]></my_tags>"
          << "<my_priority>Medium</my_priority>"
          << "<my_reread_value></my_reread_value>"
          << "<my_rereading>" << rereading << "</my_rereading>"
          << "<my_discuss>NO</my_discuss>"
          << "<my_sns>default</my_sns>"
          << "<update_on_import>1</update_on_import>"
          << "</manga>\n";
    }
    s << "</myanimelist>\n";
}


// Write raw bytes to a gzip file using zlib
static bool writeGzip(const QByteArray& data, const QString& path) {
    QByteArray pathBytes = path.toLocal8Bit();
    gzFile gz = gzopen(pathBytes.constData(), "wb9");
    if (!gz) return false;
    gzwrite(gz, data.constData(), static_cast<unsigned>(data.size()));
    gzclose(gz);
    return true;
}

// Anime-Planet requires the MAL XML wrapped in a gzip file (.xml.gz)

// MangaBaka native JSON format — schema v2 expected by mangabaka.org/settings/import
// Top-level: { schema_version: 2, exported_at: ISO8601, lists: [], entries: [...] }
// Each entry: { entry: { full fields }, source: { all tracker IDs }, titles: { primary, romanized, native }, lists: [] }
void toMangaBakaJSON(const QList<MangaEntry>& lib, const QString& path) {
    const QString now = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs);

    QJsonArray entries;
    for (const auto& e : lib) {
        const QString mbStatus = MB_STATUS.value(e.status, "plan_to_read");

        QJsonObject entry;
        entry["note"]              = QJsonValue::Null;
        entry["read_link"]         = QJsonValue::Null;
        entry["rating"]            = QJsonValue::Null;  // 0-100, unknown when importing from MangaDex
        entry["state"]             = mbStatus;
        entry["priority"]          = 20;                // medium priority (matches MangaBaka default)
        entry["is_private"]        = false;
        entry["number_of_rereads"] = 0;
        entry["progress_chapter"]  = QJsonValue::Null;
        entry["progress_volume"]   = QJsonValue::Null;
        entry["start_date"]        = QJsonValue::Null;
        entry["finish_date"]       = QJsonValue::Null;
        entry["imported_at"]       = now;
        entry["created_at"]        = now;
        entry["updated_at"]        = now;

        // NOTE: "mangabaka" is deliberately omitted rather than set to null.
        // MangaBaka's importer treats presence of this key as "resolve by
        // mangabaka series id" and coerces a JSON null to 0, which it then
        // looks up as a real series (id 0) and fails with unknown_series.
        // Omitting the key entirely makes the importer fall through to the
        // mangadex id below, which is the source we actually have.
        // Only include keys we have data for.
        // MangaBaka iterates source fields in insertion order and resolves
        // on the first non-null value — null fields serialize as NaN and
        // cause parse_error before the importer reaches the mangadex key.
        QJsonObject source;
        source["mangadex"] = e.id;

        QJsonObject titles;
        titles["primary"]   = e.title;
        titles["native"]    = QJsonValue::Null;
        titles["romanized"] = QJsonValue::Null;

        QJsonObject obj;
        obj["entry"]  = entry;
        obj["source"] = source;
        obj["titles"] = titles;
        obj["lists"]  = QJsonArray();

        entries.append(obj);
    }

    QJsonObject root;
    root["schema_version"] = 2;
    root["exported_at"]    = now;
    root["lists"]          = QJsonArray();
    root["entries"]        = entries;

    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    f.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
}

void toAnimeplanetGZ(const QList<MangaEntry>& lib, const QString& path) {
    // Build the XML in memory first
    QByteArray buf;
    QTextStream s(&buf);
    s.setEncoding(QStringConverter::Utf8);

    s << R"(<?xml version="1.0" encoding="UTF-8"?>)" "\n"
      << "<myanimelist>\n"
      << "  <myinfo>\n"
      << "    <user_export_type>2</user_export_type>\n"
      << "    <user_total_manga>" << lib.size() << "</user_total_manga>\n"
      << "  </myinfo>\n";

    const QString today = QDate::currentDate().toString("yyyy-MM-dd");
    for (const auto& e : lib) {
        const QString malSt     = MAL_STATUS.value(e.status, "Plan to Read");
        const bool    isReading = (e.status == "reading" || e.status == "re_reading");
        const bool    isDone    = (e.status == "completed");
        const QString startDate = isReading || isDone ? today : "0000-00-00";
        const QString finDate   = isDone ? today : "0000-00-00";
        const QString rereading = (e.status == "re_reading") ? "YES" : "NO";
        s << "<manga>"
          << "<manga_mangadb_id>0</manga_mangadb_id>"
          << "<manga_title><![CDATA[" << e.title << "]]></manga_title>"
          << "<manga_volumes>0</manga_volumes>"
          << "<manga_chapters>0</manga_chapters>"
          << "<my_id>0</my_id>"
          << "<my_read_volumes>0</my_read_volumes>"
          << "<my_read_chapters>0</my_read_chapters>"
          << "<my_start_date>" << startDate << "</my_start_date>"
          << "<my_finish_date>" << finDate << "</my_finish_date>"
          << "<my_scanalation_group><![CDATA[]]></my_scanalation_group>"
          << "<my_score>0</my_score>"
          << "<my_storage></my_storage>"
          << "<my_retail_volumes>0</my_retail_volumes>"
          << "<my_status>" << malSt << "</my_status>"
          << "<my_comments><![CDATA[]]></my_comments>"
          << "<my_times_read>0</my_times_read>"
          << "<my_tags><![CDATA[]]></my_tags>"
          << "<my_priority>Medium</my_priority>"
          << "<my_reread_value></my_reread_value>"
          << "<my_rereading>" << rereading << "</my_rereading>"
          << "<my_discuss>NO</my_discuss>"
          << "<my_sns>default</my_sns>"
          << "<update_on_import>1</update_on_import>"
          << "</manga>\n";
    }
    s << "</myanimelist>\n";
    s.flush();
    writeGzip(buf, path);
}

} // namespace Export

// ── Cover loader (shared cache, async, rate-limited drip queue) ───────────────
//
// Design: one QTimer ticks every SLOT_MS and releases one pending download
// per tick, up to MAX_CONCURRENT in-flight at once.  This is a true drip
// queue — it doesn't matter how many load() calls arrive simultaneously; they
// all go into m_queue and the timer drains them one at a time.  The old
// m_dispatched stagger was broken: it gave the first slot a 0ms delay
// (singleShot(0*120, …)), which is functionally synchronous, and the per-wave
// accumulated counter didn't survive inter-wave gaps correctly, letting bursts
// through whenever m_dispatched was reset after a drain.

class CoverLoader : public QObject {
    Q_OBJECT
public:
    static CoverLoader& inst() { static CoverLoader c; return c; }

    using LogFn = std::function<void(const QString&)>;
    void setLogger(LogFn fn) { m_log = std::move(fn); }

    void load(const QString& url, QLabel* label, QSize size) {
        if (url.isEmpty() || !label) return;
        if (m_cache.contains(url)) { apply(label, m_cache.value(url), size); return; }

        // Disk cache — covers persist between runs so relaunches are instant.
        const QString diskPath = cachePath(url);
        if (QFile::exists(diskPath)) {
            QPixmap px(diskPath);
            if (!px.isNull()) {
                m_cache.insert(url, px);
                apply(label, px, size);
                ++m_cacheHits;
                return;
            }
        }

        m_waiting[url].append({QPointer<QLabel>(label), size});
        if (m_waiting[url].size() > 1) return;   // already queued/in-flight

        m_queue.append(url);
        startDrip();   // ensure the drip timer is running
    }

private:
    // One request leaves the queue every SLOT_MS; at most MAX_CONCURRENT
    // are in-flight at once.  SLOT_MS = 750ms means ≤1.3 req/s to the CDN.
    // With lazy loading only visible cards ever queue covers, so the queue
    // stays small and the lower rate is imperceptible in practice.
    static constexpr int MAX_CONCURRENT = 2;
    static constexpr int SLOT_MS        = 750;   // inter-request gap

    CoverLoader() {
        m_cacheDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/covers";
        QDir().mkpath(m_cacheDir);
        // Same reasoning as MainWindow's manager — a hung cover download used
        // to just sit in m_active forever with nothing to time it out.
        m_nam.setTransferTimeout(15000);

        // Drip timer — single-shot so it goes idle when the queue empties.
        m_dripTimer = new QTimer(this);
        m_dripTimer->setSingleShot(false);
        m_dripTimer->setInterval(SLOT_MS);
        connect(m_dripTimer, &QTimer::timeout, this, &CoverLoader::drip);
    }

    QString cachePath(const QString& url) const {
        const auto h = QCryptographicHash::hash(url.toUtf8(), QCryptographicHash::Sha1).toHex();
        return m_cacheDir + "/" + h + ".jpg";
    }

    void startDrip() {
        if (!m_loggedCacheDir && m_log) {
            m_log(QString("Cover cache dir: %1").arg(m_cacheDir));
            m_loggedCacheDir = true;
        }
        if (!m_dripTimer->isActive())
            m_dripTimer->start();
    }

    // Called every SLOT_MS — dispatch one pending URL if under the concurrency cap.
    void drip() {
        if (m_queue.isEmpty()) {
            m_dripTimer->stop();
            if (m_active == 0 && m_log && (m_cacheHits + m_cacheWrites) > 0) {
                m_log(QString("Covers done — %1 from disk cache, %2 freshly downloaded.")
                      .arg(m_cacheHits).arg(m_cacheWrites));
                m_cacheHits = 0; m_cacheWrites = 0;
            }
            return;
        }
        if (m_active >= MAX_CONCURRENT) return;   // wait for a slot to free up

        const QString url = m_queue.takeFirst();
        ++m_active;
        start(url);
    }

    void start(const QString& url) {
        QNetworkRequest req{QUrl(url)};
        req.setRawHeader("User-Agent", UA);
        auto* reply = m_nam.get(req);
        connect(reply, &QNetworkReply::finished, this, [this, reply, url] {
            QPixmap px;
            QByteArray raw;
            if (reply->error() == QNetworkReply::NoError) {
                raw = reply->readAll();
                px.loadFromData(raw);
            } else if (m_log) {
                m_log(QString("Cover download failed: %1").arg(reply->errorString()));
            }
            reply->deleteLater();

            if (!px.isNull()) {
                m_cache.insert(url, px);
                QFile f(cachePath(url));
                if (f.open(QIODevice::WriteOnly)) {
                    f.write(raw);
                    ++m_cacheWrites;
                } else if (m_log) {
                    m_log(QString("Could not write cover cache: %1").arg(f.errorString()));
                }
            }
            const auto waiters = m_waiting.take(url);
            for (const auto& w : waiters)
                if (w.first) apply(w.first, px, w.second);
            --m_active;
            // drip() handles scheduling the next slot via the timer.
            // No manual pump() call needed — the timer is already running.
        });
    }

    void apply(QLabel* l, const QPixmap& px, QSize s) {
        if (px.isNull()) return;
        l->setText("");
        l->setPixmap(px.scaled(s, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Fade-in: start at 0 opacity, animate to 1 over 300ms with OutCubic easing
        auto* eff = new QGraphicsOpacityEffect(l);
        eff->setOpacity(0.0);
        l->setGraphicsEffect(eff);

        auto* anim = new QPropertyAnimation(eff, "opacity", l);
        anim->setDuration(300);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->setEasingCurve(QEasingCurve::OutCubic);
        // Clean up the effect after animation so it doesn't interfere with repaints
        QObject::connect(anim, &QPropertyAnimation::finished, l, [l]{
            l->setGraphicsEffect(nullptr);
        });
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }

    QNetworkAccessManager m_nam;
    QTimer*               m_dripTimer   = nullptr;
    QHash<QString, QPixmap> m_cache;
    QHash<QString, QList<QPair<QPointer<QLabel>, QSize>>> m_waiting;
    QStringList m_queue;
    int  m_active      = 0;
    int  m_cacheHits   = 0;
    int  m_cacheWrites = 0;
    bool m_loggedCacheDir = false;
    QString m_cacheDir;
    LogFn   m_log;
};

// ── Styled widgets ────────────────────────────────────────────────────────────

class Card : public QFrame {
public:
    explicit Card(QWidget* parent = nullptr) : QFrame(parent) {
        setObjectName("card");
        setStyleSheet(
            "QFrame#card {"
            "  background: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 " + QString(Pal::ELEV) + ",stop:1 " + QString(Pal::CARD) + ");"
            "  border: 1px solid " + Pal::BORDER + ";"
            "  border-radius: 18px;"
            "}"
        );
    }
};

// ── Animated AccentButton ─────────────────────────────────────────────────────
// Hover: orange outer glow fades in (painted in paintEvent, 180ms ease-out)
// Press: quick scale-down pop (0.93×) that springs back (120ms + 100ms)
class AccentButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(qreal glowOpacity READ glowOpacity WRITE setGlowOpacity)
    Q_PROPERTY(qreal scaleF       READ scaleF       WRITE setScaleF)
public:
    explicit AccentButton(const QString& text, QWidget* parent = nullptr)
        : QPushButton(text, parent)
    {
        setCursor(Qt::PointingHandCursor);
        setFixedHeight(36);
        setAttribute(Qt::WA_Hover);
        // Base style — no :hover/:pressed rules; we paint the glow manually
        setStyleSheet(
            "QPushButton {"
            "  background: qlineargradient(x1:0,y1:0,x2:1,y2:1,stop:0 " + QString(Pal::ACCENT_H) + ",stop:1 " + QString(Pal::ACCENT) + ");"
            "  color: #1a1206; border: none; border-radius: 11px;"
            "  padding: 0 18px; font-size: 13px; font-weight: 700;"
            "}"
            "QPushButton:disabled { background: transparent; color: #8a6a4d;"
            " border: 1px solid #8a6a4d; }"
        );
        // This button paints its own background with QPainter instead of a flat
        // QSS background-color, so Qt can't calibrate ClearType's subpixel color
        // correction against what's actually behind the glyphs — the result is
        // a red/blue fringe around the text (worst on hover/disabled states).
        // Forcing plain grayscale antialiasing removes the fringe entirely.
        {
            QFont f = font();
            f.setStyleStrategy(QFont::StyleStrategy(QFont::PreferAntialias | QFont::NoSubpixelAntialias));
            setFont(f);
        }

        // Glow animator
        m_glowAnim = new QPropertyAnimation(this, "glowOpacity", this);
        m_glowAnim->setDuration(180);
        m_glowAnim->setEasingCurve(QEasingCurve::OutCubic);

        // Scale pop animator (press down then spring back)
        m_scaleAnim = new QPropertyAnimation(this, "scaleF", this);
    }

    qreal glowOpacity() const { return m_glowOpacity; }
    void  setGlowOpacity(qreal v) { m_glowOpacity = v; update(); }

    qreal scaleF() const { return m_scaleF; }
    void  setScaleF(qreal v) { m_scaleF = v; update(); }

protected:
    void enterEvent(QEnterEvent* e) override {
        QPushButton::enterEvent(e);
        animateGlow(1.0);
    }
    void leaveEvent(QEvent* e) override {
        QPushButton::leaveEvent(e);
        animateGlow(0.0);
    }
    void mousePressEvent(QMouseEvent* e) override {
        QPushButton::mousePressEvent(e);
        if (e->button() == Qt::LeftButton) animateScale(0.93, 120);
    }
    void mouseReleaseEvent(QMouseEvent* e) override {
        QPushButton::mouseReleaseEvent(e);
        if (e->button() == Qt::LeftButton) animateScale(1.0, 100);
    }

    void paintEvent(QPaintEvent* e) override {
        // Scale transform from centre — draw bg + text directly via the style
        // under a transformed painter. (Previously this called
        // QPushButton::render() into an offscreen QPixmap, but that's a
        // re-entrant render of the widget from inside its own paintEvent —
        // Qt can grab a stale/incomplete backing store from that, which is
        // why the label text would vanish on press while still showing the
        // background fill.)
        if (!qFuzzyCompare(m_scaleF, 1.0)) {
            QPainter p(this);
            p.setRenderHint(QPainter::Antialiasing);
            p.setRenderHint(QPainter::TextAntialiasing);
            p.translate(width() / 2.0, height() / 2.0);
            p.scale(m_scaleF, m_scaleF);
            p.translate(-width() / 2.0, -height() / 2.0);
            QStyleOptionButton opt;
            initStyleOption(&opt);
            style()->drawControl(QStyle::CE_PushButton, &opt, &p, this);
        } else {
            QPushButton::paintEvent(e);
        }
        // Glow ring removed per design change — hover now shows no outline.
    }

private:
    void animateGlow(qreal to) {
        m_glowAnim->stop();
        m_glowAnim->setStartValue(m_glowOpacity);
        m_glowAnim->setEndValue(to);
        m_glowAnim->start();
    }
    void animateScale(qreal to, int ms) {
        m_scaleAnim->stop();
        m_scaleAnim->setDuration(ms);
        m_scaleAnim->setEasingCurve(to < 1.0 ? QEasingCurve::OutCubic : QEasingCurve::OutBack);
        m_scaleAnim->setStartValue(m_scaleF);
        m_scaleAnim->setEndValue(to);
        m_scaleAnim->start();
    }

    qreal               m_glowOpacity = 0.0;
    qreal               m_scaleF      = 1.0;
    QPropertyAnimation* m_glowAnim    = nullptr;
    QPropertyAnimation* m_scaleAnim   = nullptr;
};

// ── Animated GhostButton ──────────────────────────────────────────────────────
// Hover: border fades from grey → orange, text shifts to ACCENT_H (180ms)
// Press: scale-down pop same as AccentButton
class GhostButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(qreal hoverT READ hoverT WRITE setHoverT)
    Q_PROPERTY(qreal scaleF  READ scaleF  WRITE setScaleF)
public:
    explicit GhostButton(const QString& text, QWidget* parent = nullptr)
        : QPushButton(text, parent)
    {
        setCursor(Qt::PointingHandCursor);
        setFixedHeight(36);
        setAttribute(Qt::WA_Hover);
        // Static base — no hover rules, we paint the border manually
        setStyleSheet(
            "QPushButton {"
            "  background: transparent; color: " + QString(Pal::TEXT) + ";"
            "  border: none; border-radius: 11px;"
            "  padding: 0 14px; font-size: 13px;"
            "}"
        );
        // Same ClearType-fringe fix as AccentButton — this button's fills and
        // border are hand-painted (see paintEvent below), so Qt has no flat
        // background color to calibrate subpixel text AA against. Plain
        // grayscale AA sidesteps the color fringe on the label entirely.
        {
            QFont f = font();
            f.setStyleStrategy(QFont::StyleStrategy(QFont::PreferAntialias | QFont::NoSubpixelAntialias));
            setFont(f);
        }

        m_hoverAnim = new QPropertyAnimation(this, "hoverT", this);
        m_hoverAnim->setDuration(180);
        m_hoverAnim->setEasingCurve(QEasingCurve::OutCubic);

        m_scaleAnim = new QPropertyAnimation(this, "scaleF", this);
    }

    qreal hoverT() const { return m_hoverT; }
    void  setHoverT(qreal v) { m_hoverT = v; update(); }

    qreal scaleF() const { return m_scaleF; }
    void  setScaleF(qreal v) { m_scaleF = v; update(); }

protected:
    void enterEvent(QEnterEvent* e) override { QPushButton::enterEvent(e); animateHover(1.0); }
    void leaveEvent(QEvent* e)      override { QPushButton::leaveEvent(e);  animateHover(0.0); }
    void mousePressEvent(QMouseEvent* e) override {
        QPushButton::mousePressEvent(e);
        if (e->button() == Qt::LeftButton) animateScale(0.93, 120);
    }
    void mouseReleaseEvent(QMouseEvent* e) override {
        QPushButton::mouseReleaseEvent(e);
        if (e->button() == Qt::LeftButton) animateScale(1.0, 100);
    }

    void paintEvent(QPaintEvent* e) override {
        // All background fills happen BEFORE the icon/text render, so nothing
        // ever paints a translucent rect on top of the icon (that was the
        // bug: the old hover tint was drawn after QPushButton::paintEvent
        // and washed the icon out on hover).

        // Subtle idle fill so the pill reads as a distinct control at rest.
        {
            QPainter p(this);
            p.setRenderHint(QPainter::Antialiasing);
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(Pal::ELEV));
            p.drawRoundedRect(rect(), 11, 11);
        }
        // Tint bg on hover — drawn under the icon/text, not over it.
        if (m_hoverT > 0.01) {
            QPainter p(this);
            p.setRenderHint(QPainter::Antialiasing);
            QColor bg(Pal::SURFACE);
            bg.setAlphaF(m_hoverT * 0.8);
            p.setPen(Qt::NoPen);
            p.setBrush(bg);
            p.drawRoundedRect(rect(), 11, 11);
        }

        // Icon + text on top of the fills.
        // Drawn directly via the style under the transformed painter — not
        // via QPushButton::render(), which re-enters the widget's own
        // paintEvent and can grab a stale/incomplete backing store, dropping
        // the label text while the fills above still show.
        if (!qFuzzyCompare(m_scaleF, 1.0)) {
            QPainter p(this);
            p.setRenderHint(QPainter::Antialiasing);
            p.setRenderHint(QPainter::TextAntialiasing);
            p.translate(width() / 2.0, height() / 2.0);
            p.scale(m_scaleF, m_scaleF);
            p.translate(-width() / 2.0, -height() / 2.0);
            QStyleOptionButton opt;
            initStyleOption(&opt);
            opt.palette.setBrush(QPalette::Button, Qt::NoBrush);
            style()->drawControl(QStyle::CE_PushButton, &opt, &p, this);
        } else {
            QPushButton::paintEvent(e);
        }

        // No border on hover — removed per design change. Only the fill tint
        // (painted above, before the text) signals hover state now.
    }

private:
    static QColor lerpColor(const QColor& a, const QColor& b, qreal t) {
        return QColor::fromRgbF(
            a.redF()   + (b.redF()   - a.redF())   * t,
            a.greenF() + (b.greenF() - a.greenF()) * t,
            a.blueF()  + (b.blueF()  - a.blueF())  * t);
    }
    void animateHover(qreal to) {
        m_hoverAnim->stop();
        m_hoverAnim->setStartValue(m_hoverT);
        m_hoverAnim->setEndValue(to);
        m_hoverAnim->start();
    }
    void animateScale(qreal to, int ms) {
        m_scaleAnim->stop();
        m_scaleAnim->setDuration(ms);
        m_scaleAnim->setEasingCurve(to < 1.0 ? QEasingCurve::OutCubic : QEasingCurve::OutBack);
        m_scaleAnim->setStartValue(m_scaleF);
        m_scaleAnim->setEndValue(to);
        m_scaleAnim->start();
    }

    qreal               m_hoverT   = 0.0;
    qreal               m_scaleF   = 1.0;
    QPropertyAnimation* m_hoverAnim = nullptr;
    QPropertyAnimation* m_scaleAnim = nullptr;
};

class StatusBadge : public QLabel {
public:
    explicit StatusBadge(QWidget* parent = nullptr) : QLabel(parent) {
        setFixedHeight(24);
        setAlignment(Qt::AlignCenter);
        setNeutral("Not connected");
    }
    void setOk(const QString& text)   { paint(text, "#1d2f1d", Pal::GREEN); }
    void setFail(const QString& text) { paint(text, "#361b18", Pal::RED); }
    void setNeutral(const QString& text) { paint(text, Pal::BORDER, Pal::MUTED); }
private:
    void paint(const QString& text, const QString& bg, const QString& fg) {
        setText(text);
        setStyleSheet(QString("QLabel { background: %1; color: %2;"
                              " padding: 0 10px; border-radius: 12px;"
                              " font-size: 11px; font-weight: 600; }").arg(bg, fg));
    }
};

// ── Manga cover card ──────────────────────────────────────────────────────────

class MangaCard : public QFrame {
    Q_OBJECT
    Q_PROPERTY(qreal hoverT READ hoverT WRITE setHoverT)
public:
    qreal hoverT() const { return m_hoverT; }
    void  setHoverT(qreal v) { m_hoverT = v; update(); }

    MangaCard(const MangaEntry& e, bool small, QWidget* parent = nullptr)
        : QFrame(parent), m_entry(e), m_small(small),
          m_coverW(small ? 100 : 156), m_coverH(small ? 142 : 222)
    {
        setCursor(Qt::PointingHandCursor);
        const int W = small ? 112 : 170;
        const int H = small ? 208 : 322;
        setFixedSize(W, H);
        setObjectName("mcard");
        updateStyle();

        auto* lay = new QVBoxLayout(this);
        lay->setContentsMargins(6, 6, 6, 6);
        lay->setSpacing(5);

        m_cover = new QLabel(this);
        m_cover->setFixedSize(m_coverW, m_coverH);
        m_cover->setAlignment(Qt::AlignCenter);
        m_cover->setStyleSheet(
            "QLabel { background: #161618; border: none; border-radius: 6px;"
            " color: #3a3a44; font-size: 24px; }");
        m_cover->setText("🐾");
        lay->addWidget(m_cover, 0, Qt::AlignHCenter);

        m_title = new QLabel(e.title, this);
        m_title->setWordWrap(true);
        m_title->setFixedHeight(small ? 30 : 34);
        m_title->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        m_title->setStyleSheet(QString(
            "QLabel { background: transparent; border: none; color: %1;"
            " font-size: %2px; font-weight: 600; }")
            .arg(Pal::TEXT).arg(small ? 10 : 11));
        lay->addWidget(m_title);

        if (!small) {
            QString sub = e.statusLabel;
            if (!e.year.isEmpty()) sub += "  ·  " + e.year;
            auto* st = new QLabel(sub, this);
            st->setStyleSheet(QString(
                "QLabel { background: transparent; border: none; color: %1;"
                " font-size: 10px; font-weight: 600; }").arg(statusColor(e.status)));
            lay->addWidget(st);
        }
        lay->addStretch();

        setToolTip(e.title + "\n" + e.statusLabel
                   + (e.authors.isEmpty() ? "" : "\nBy " + e.authors)
                   + "\n\nClick: select / deselect for export"
                   + "\nRight-click: open on MangaDex");

        // Selection check badge (top-left overlay)
        m_check = new QLabel("✓", this);
        m_check->setFixedSize(22, 22);
        m_check->move(10, 10);
        m_check->setAlignment(Qt::AlignCenter);
        m_check->setStyleSheet(QString(
            "QLabel { background: %1; color: #1a1206; border-radius: 11px;"
            " font-size: 13px; font-weight: 800; }").arg(Pal::ACCENT));
        m_check->hide();

        // Hover animator — purple glow ring fades in on hover (executor card vibe)
        m_hoverAnim = new QPropertyAnimation(this, "hoverT", this);
        m_hoverAnim->setDuration(180);
        m_hoverAnim->setEasingCurve(QEasingCurve::OutCubic);

        // Cover load is deferred to showEvent — no download happens until
        // the card is actually made visible, so hidden cards never hit the network.
    }

    const MangaEntry& entry() const { return m_entry; }
    QString id() const { return m_entry.id; }
    bool isSelected() const { return m_selected; }

    void setSelected(bool sel) {
        if (m_selected == sel) return;
        m_selected = sel;
        m_check->setVisible(sel);
        updateStyle();
    }

signals:
    void toggled(const QString& id, bool selected);

protected:
    // Fires when the card transitions from hidden → visible.
    // Only the first call does anything — m_coverLoaded is a one-shot guard.
    // This means covers only download for cards the user can actually see,
    // instead of every card in the library regardless of scroll position.
    void enterEvent(QEnterEvent* e) override { QFrame::enterEvent(e); animateHover(1.0); }
    void leaveEvent(QEvent* e)      override { QFrame::leaveEvent(e);  animateHover(0.0); }

    void paintEvent(QPaintEvent* e) override {
        QFrame::paintEvent(e);
        if (m_hoverT > 0.01 && !m_selected) {
            QPainter p(this);
            p.setRenderHint(QPainter::Antialiasing);
            QColor glow(Pal::ACCENT2);           // purple glow — executor keyword color
            glow.setAlphaF(m_hoverT * 0.6);
            p.setPen(QPen(glow, 2));
            p.setBrush(Qt::NoBrush);
            p.drawRoundedRect(rect().adjusted(1,1,-1,-1), 8, 8);
        }
    }

    void showEvent(QShowEvent* ev) override {
        QFrame::showEvent(ev);
        if (!m_coverLoaded) {
            m_coverLoaded = true;
            if (!m_entry.coverUrl.isEmpty())
                CoverLoader::inst().load(m_entry.coverUrl,
                                         m_cover,
                                         QSize(m_coverW, m_coverH));

            // Card entrance: quick opacity + scale pop from 0.92
            auto* eff = new QGraphicsOpacityEffect(this);
            eff->setOpacity(0.0);
            setGraphicsEffect(eff);

            auto* fadeIn = new QPropertyAnimation(eff, "opacity", this);
            fadeIn->setDuration(200);
            fadeIn->setStartValue(0.0);
            fadeIn->setEndValue(1.0);
            fadeIn->setEasingCurve(QEasingCurve::OutCubic);
            QObject::connect(fadeIn, &QPropertyAnimation::finished, this, [this]{
                setGraphicsEffect(nullptr);
            });
            fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }

    void mouseReleaseEvent(QMouseEvent* ev) override {
        if (ev->button() == Qt::RightButton) {
            QDesktopServices::openUrl(QUrl(m_entry.url));
            return;
        }
        if (ev->button() == Qt::LeftButton)
            emit toggled(m_entry.id, !m_selected);
    }

private:
    void updateStyle() {
        // No CSS hover rule — hover glow is painted manually in paintEvent
        setStyleSheet(QString(
            "QFrame#mcard {"
            "  background: %1;"
            "  border: %2;"
            "  border-radius: 8px;"
            "}")
            .arg(Pal::CARD,
                 m_selected ? "2px solid " + QString(Pal::ACCENT)
                            : "1px solid " + QString(Pal::BORDER)));
    }

    void animateHover(qreal to) {
        m_hoverAnim->stop();
        m_hoverAnim->setStartValue(m_hoverT);
        m_hoverAnim->setEndValue(to);
        m_hoverAnim->start();
    }

    MangaEntry m_entry;
    bool       m_small       = false;
    bool       m_selected    = false;
    bool       m_coverLoaded = false;
    int        m_coverW      = 0;
    int        m_coverH      = 0;
    qreal      m_hoverT      = 0.0;
    QLabel*    m_cover       = nullptr;
    QLabel*    m_title       = nullptr;
    QLabel*    m_check       = nullptr;
    QPropertyAnimation* m_hoverAnim = nullptr;
};

// ── OutlineBoxIcon — line-art package glyph for empty states ───────────────────
// Single-stroke hexagon + seam lines (no fill), matching the muted icon language
// used everywhere else in the app instead of a color emoji glyph.

class OutlineBoxIcon : public QWidget {
public:
    explicit OutlineBoxIcon(QWidget* parent = nullptr) : QWidget(parent) {
        setFixedSize(44, 44);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        QPen pen(QColor(Pal::MUTED));
        pen.setWidthF(1.6);
        pen.setJoinStyle(Qt::RoundJoin);
        pen.setCapStyle(Qt::RoundCap);
        p.setPen(pen);
        p.setBrush(Qt::NoBrush);

        const qreal w = width(), h = height();
        const qreal cx = w / 2.0, top = h * 0.14, mid = h * 0.42, bot = h * 0.88;
        const qreal lx = w * 0.14, rx = w * 0.86;

        // Hexagon outline — open box silhouette
        QPolygonF hex;
        hex << QPointF(cx, top) << QPointF(rx, mid * 0.72) << QPointF(rx, bot - (bot - mid) * 0.3)
            << QPointF(cx, bot) << QPointF(lx, bot - (bot - mid) * 0.3) << QPointF(lx, mid * 0.72);
        p.drawPolygon(hex);

        // Seam lines meeting at the box's center — the classic "package" Y
        p.drawLine(QPointF(cx, top), QPointF(cx, mid));
        p.drawLine(QPointF(lx, mid * 0.72), QPointF(cx, mid));
        p.drawLine(QPointF(rx, mid * 0.72), QPointF(cx, mid));
        p.drawLine(QPointF(cx, mid), QPointF(cx, bot));
    }
};

// ── SmoothScrollArea — QScrollArea with real momentum-based wheel scroll ───────
// A fixed-duration animation per wheel notch (the old approach) always takes
// the same time no matter how hard you scroll, which reads as mechanical.
// This instead runs a proper physics model: each notch adds velocity, a
// 60fps timer integrates position from that velocity every frame, and
// friction bleeds the velocity off — so a light nudge stops quickly and a
// hard flick glides and decelerates, the way trackpad/phone scrolling does.
// Rapid notches keep adding to the same in-flight velocity instead of
// restarting anything, so fast continuous scrolling stays fluid.

class SmoothScrollArea : public QScrollArea {
    Q_OBJECT
public:
    explicit SmoothScrollArea(QWidget* parent = nullptr) : QScrollArea(parent) {
        m_timer.setInterval(8);   // ~120hz integration — finer steps than the
                                   // display refresh so motion reads as continuous
                                   // rather than tickable, even on 60Hz screens.
        connect(&m_timer, &QTimer::timeout, this, &SmoothScrollArea::tick);
    }

protected:
    void wheelEvent(QWheelEvent* e) override {
        QScrollBar* bar = verticalScrollBar();
        const int delta = e->angleDelta().y();
        if (!bar || delta == 0 || bar->maximum() == bar->minimum()) {
            QScrollArea::wheelEvent(e);
            return;
        }

        // Re-sync our tracked float position to the scrollbar if nothing was
        // in motion (covers the user dragging the scrollbar by hand between
        // wheel gestures — otherwise we'd snap back to a stale position).
        if (!m_timer.isActive())
            m_pos = bar->value();

        // Total distance this notch should ultimately travel — same feel as
        // before (3 lines/notch, scaled for high-res wheels/trackpads).
        const qreal distance = bar->singleStep() * 3.0 * (delta / 120.0);
        // Geometric decay means an impulse of distance*(1-friction) sums to
        // `distance` total travel once it fully decelerates — so multiple
        // quick notches add up to their combined natural travel distance.
        m_velocity -= distance * (1.0 - kFriction);

        if (!m_timer.isActive()) m_timer.start();
        e->accept();
    }

private slots:
    void tick() {
        QScrollBar* bar = verticalScrollBar();
        if (!bar) { m_timer.stop(); m_velocity = 0; return; }

        m_pos += m_velocity;
        m_velocity *= kFriction;

        if (m_pos <= bar->minimum()) { m_pos = bar->minimum(); m_velocity = 0; }
        if (m_pos >= bar->maximum()) { m_pos = bar->maximum(); m_velocity = 0; }

        bar->setValue(qRound(m_pos));

        if (qAbs(m_velocity) < 0.05) {
            m_velocity = 0;
            m_timer.stop();
        }
    }

private:
    static constexpr qreal kFriction = 0.90;   // lower = stops sooner, higher = glides longer
    QTimer m_timer;
    qreal  m_velocity = 0.0;
    qreal  m_pos       = 0.0;
};

class ShrinkableStack : public QStackedWidget {
public:
    explicit ShrinkableStack(QWidget* parent = nullptr) : QStackedWidget(parent) {
        connect(this, &QStackedWidget::currentChanged, this, [this]{ updateGeometry(); });
    }
    QSize sizeHint()    const override { return currentWidget() ? currentWidget()->sizeHint()    : QStackedWidget::sizeHint(); }
    QSize minimumSizeHint() const override { return currentWidget() ? currentWidget()->minimumSizeHint() : QStackedWidget::minimumSizeHint(); }
};

// ── SmoothProgressBar — QProgressBar that eases toward its target value ────────
// setValue() no longer jumps the chunk; it animates from the current displayed
// value to the target over ~260ms with an OutCubic ease, retargeting cleanly if
// a new value lands mid-animation. Rounded pill track/chunk come from the QSS.

class SmoothProgressBar : public QProgressBar {
    Q_OBJECT
public:
    explicit SmoothProgressBar(QWidget* parent = nullptr) : QProgressBar(parent) {
        m_anim = new QPropertyAnimation(this, "value", this);
        m_anim->setDuration(260);
        m_anim->setEasingCurve(QEasingCurve::OutCubic);
    }

    // Jump instantly, no animation (e.g. resetting to 0 on a new run).
    void setValueInstant(int v) {
        m_anim->stop();
        QProgressBar::setValue(v);
    }

public slots:
    void setValue(int v) {
        if (v == QProgressBar::value()) return;
        m_anim->stop();
        m_anim->setStartValue(QProgressBar::value());
        m_anim->setEndValue(v);
        m_anim->start();
    }

private:
    QPropertyAnimation* m_anim = nullptr;
};

// ── Main window ───────────────────────────────────────────────────────────────

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr)
        : QMainWindow(parent),
          m_settings("SentinelFlow", "HitPawMangaDexExporter")
    {
        setWindowTitle("HitPaw — MangaDex Manager");

        QIcon appIcon;
        for (int s : {16,32,48,64,128,256})
            appIcon.addPixmap(QPixmap(QString(":/icons/icon_%1.png").arg(s)));
        setWindowIcon(appIcon);
        qApp->setWindowIcon(appIcon);

        // Fixed width — only height is resizable
        setFixedWidth(1180);
        setMinimumHeight(640);

        m_nam = new QNetworkAccessManager(this);
        // Without this, a stalled connection (dead wifi, MangaDex hiccup) hangs
        // the request forever with no error ever reaching the UI/log — every
        // request through m_nam now fails cleanly after 15s instead.
        m_nam->setTransferTimeout(15000);
        m_refreshTimer = new QTimer(this);
        m_refreshTimer->setSingleShot(true);
        connect(m_refreshTimer, &QTimer::timeout, this, &MainWindow::doRefresh);

        // API credentials — user-editable, default to the built-in personal client
        m_rememberCreds = m_settings.value("api/remember_creds", true).toBool();
        m_clientId     = m_settings.value("api/client_id",     "").toString();
        m_clientSecret = m_settings.value("api/client_secret", "").toString();

        applyGlobalStyle();
        buildUI();
        CoverLoader::inst().setLogger([this](const QString& msg) { appendLog(msg); });
        resize(1180, 900);

        // Keyboard shortcuts — window-wide (WidgetWithChildrenShortcut would
        // limit these to whichever widget has focus, which isn't what you
        // want for global actions like "focus the search box").
        auto* focusFilterShortcut = new QShortcut(QKeySequence::Find, this);   // Ctrl+F
        connect(focusFilterShortcut, &QShortcut::activated, this, [this] {
            if (m_filterEdit) { m_filterEdit->setFocus(); m_filterEdit->selectAll(); }
        });
        auto* selectAllShortcut = new QShortcut(QKeySequence::SelectAll, this); // Ctrl+A
        connect(selectAllShortcut, &QShortcut::activated, this, [this] { selectVisible(true); });
        auto* clearSelShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);
        connect(clearSelShortcut, &QShortcut::activated, this, [this] { clearSelection(); });

        // Auto-reconnect from saved session
        QTimer::singleShot(150, this, &MainWindow::tryAutoConnect);
    }

private:
    static constexpr int GRID_COLS = 5;

    // ── State ─────────────────────────────────────────────────────────────────
    QSettings                 m_settings;
    QNetworkAccessManager*    m_nam = nullptr;
    QTimer*                   m_refreshTimer = nullptr;

    QString                   m_accessToken;
    QString                   m_refreshToken;
    QString                   m_username;
    QString                   m_clientId;        // user-supplied API client id
    QString                   m_clientSecret;    // user-supplied API secret

    QMap<QString, QString>    m_statusMap;       // manga id → status
    QMap<QString, MangaEntry> m_entries;         // every entry we know about
    QStringList               m_libraryOrder;    // library ids, sorted by title
    QSet<QString>             m_selected;        // ids selected for export
    QStringList               m_allIds;          // pending batch fetch
    bool                      m_fetching = false;
    bool                      m_stopRequested = false;
    QPointer<QNetworkReply>   m_curReply;        // in-flight library request

    QList<MangaCard*>         m_libCards;
    int                       m_gridPlaced = 0;   // count of cards currently laid out in the grid (see appendCardsToGrid)


    // ── Widgets ───────────────────────────────────────────────────────────────
    StatusBadge*    m_authBadge      = nullptr;
    Card*           m_authCard       = nullptr;
    ShrinkableStack* m_authStack      = nullptr;
    QLineEdit*      m_userEdit       = nullptr;
    QLineEdit*      m_passEdit       = nullptr;
    QCheckBox*      m_stayChk        = nullptr;
    AccentButton*   m_loginBtn       = nullptr;
    QLineEdit*      m_tokenEdit      = nullptr;
    AccentButton*   m_tokenBtn       = nullptr;
    QLineEdit*      m_clientIdEdit   = nullptr;
    QLineEdit*      m_clientSecEdit  = nullptr;
    QCheckBox*      m_rememberCredsChk = nullptr;
    bool            m_rememberCreds  = true;

    QWidget*        m_connectedBar   = nullptr;
    QLabel*         m_connectedLbl   = nullptr;

    QTabWidget*     m_tabs           = nullptr;
    QButtonGroup*   m_navGroup       = nullptr;

    // Library tab
    QComboBox*      m_filterStatus   = nullptr;
    QLineEdit*      m_filterEdit     = nullptr;
    QLabel*         m_countLbl       = nullptr;
    QLabel*         m_statsLbl       = nullptr;
    QLabel*         m_noResultsLbl   = nullptr;
    SmoothProgressBar* m_libProgress = nullptr;
    QGridLayout*    m_grid           = nullptr;
    AccentButton*   m_loadBtn        = nullptr;
    GhostButton*    m_stopBtn        = nullptr;
    GhostButton*    m_undoBtn        = nullptr;
    GhostButton*    m_selAllBtn      = nullptr;
    GhostButton*    m_clearSelBtn    = nullptr;
    QStackedWidget* m_libraryStack   = nullptr;   // empty-state placeholder ⇄ cover grid

    // Undo history for manual export-selection changes.
    QList<QSet<QString>> m_selectionUndo;



    // Export tab
    QLineEdit*      m_outEdit        = nullptr;
    QCheckBox*      m_chkCSV         = nullptr;
    QCheckBox*      m_chkJSON        = nullptr;
    QCheckBox*      m_chkMAL         = nullptr;
    QCheckBox*      m_chkAP          = nullptr;
    QCheckBox*      m_chkMB          = nullptr;
    AccentButton*   m_exportAllBtn   = nullptr;
    GhostButton*    m_exportSelBtn   = nullptr;
    QLabel*         m_selInfo        = nullptr;
    QTextEdit*      m_log            = nullptr;
    QPropertyAnimation* m_logScrollAnim = nullptr;

    // Download tab
    QLineEdit*      m_dlUrlEdit      = nullptr;
    AccentButton*   m_dlLookupBtn    = nullptr;
    QLabel*         m_dlMangaTitle   = nullptr;
    QLabel*         m_dlMangaCover   = nullptr;
    QLabel*         m_dlMangaAuthor  = nullptr;
    QLabel*         m_dlMangaArtist  = nullptr;
    QLabel*         m_dlMangaGenres  = nullptr;
    QLabel*         m_dlMangaDemog   = nullptr;
    QList<QLabel*>  m_dlInfoLabels;   // headings + values for Author/Artist/Genres/Demographic — hidden while signed out
    QComboBox*      m_dlLangFilter   = nullptr;
    QWidget*        m_dlChapterList  = nullptr;
    QVBoxLayout*    m_dlChapterLay   = nullptr;
    SmoothScrollArea* m_dlChapterScroll= nullptr;
    QLineEdit*      m_dlPathEdit     = nullptr;
    AccentButton*   m_dlStartBtn     = nullptr;
    GhostButton*    m_dlStopBtn      = nullptr;
    SmoothProgressBar* m_dlProgress  = nullptr;
    QLabel*         m_dlStatusLbl    = nullptr;

    // Download state
    QString                 m_dlMangaId;
    QString                 m_dlMangaTitleStr;
    QVector<ChapterInfo>    m_dlChapters;
    QList<QCheckBox*>       m_dlChkBoxes;
    bool                    m_dlRunning   = false;
    bool                    m_dlStop      = false;
    int                     m_dlCurrent   = 0;
    QStringList             m_dlQueue;      // chapter ids to download
    QMap<QString,int>       m_dlTotalPages; // chapter id → page count

    // ── Style ─────────────────────────────────────────────────────────────────
    void applyGlobalStyle() {
        setStyleSheet(QString(
            "QMainWindow, QWidget { background: %1; color: %2; font-family: 'Segoe UI Variable','Segoe UI',Arial,sans-serif; font-size: 13px; }"
            "QLineEdit {"
            "  background: %3; color: %2;"
            "  border: 1px solid %4; border-radius: 11px;"
            "  padding: 9px 13px; font-size: 13px;"
            "  selection-background-color: %5; selection-color: #1a1206;"
            "}"
            "QLineEdit:hover { border-color: %8; }"
            "QLineEdit:focus { border-color: %5; background: %9; }"
            "QCheckBox { color: %2; font-size: 13px; spacing: 8px; background: transparent; }"
            "QCheckBox::indicator {"
            "  width: 18px; height: 18px; border-radius: 6px;"
            "  border: 1px solid %4; background: %3;"
            "}"
            "QCheckBox::indicator:hover { border-color: %5; }"
            "QCheckBox::indicator:checked {"
            "  background: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 %11,stop:1 %5);"
            "  border-color: %5;"
            "}"
            "QScrollBar:vertical { background: transparent; width: 12px; margin: 3px; border-radius: 6px; }"
            "QScrollBar::handle:vertical { background: %4; border-radius: 6px; min-height: 30px; }"
            "QScrollBar::handle:vertical:hover { background: %5; }"
            "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"
            "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: transparent; }"
            "QScrollBar:horizontal { background: transparent; height: 12px; margin: 3px; border-radius: 6px; }"
            "QScrollBar::handle:horizontal { background: %4; border-radius: 6px; min-width: 30px; }"
            "QScrollBar::handle:horizontal:hover { background: %5; }"
            "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0; }"
            "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: transparent; }"
            "QTextEdit {"
            "  background: %3; color: %2;"
            "  border: 1px solid %4; border-radius: 11px;"
            "  font-family: 'Cascadia Code','Consolas','Fira Code',monospace; font-size: 12px;"
            "  padding: 9px; selection-background-color: %5; selection-color: #1a1206;"
            "}"
            "QProgressBar { background: %6; border: none; border-radius: 3px; min-height: 6px; max-height: 6px; }"
            "QProgressBar::chunk { background: %5; border-radius: 3px; }"
            "QTabWidget::pane { border: none; top: 4px; }"
            "QTabBar { background: transparent; qproperty-drawBase: 0; }"
            "QTabBar::tab {"
            "  background: transparent; color: %7;"
            "  padding: 10px 20px; margin-right: 6px; font-size: 13px; font-weight: 700;"
            "  border: none; border-radius: 10px;"
            "}"
            "QTabBar::tab:selected {"
            "  color: #1a1206;"
            "  background: qlineargradient(x1:0,y1:0,x2:1,y2:1,stop:0 %5,stop:1 %10);"
            "}"
            "QTabBar::tab:hover:!selected { color: %2; background: %9; }"
            "QComboBox {"
            "  background: %3; color: %2; border: 1px solid %4;"
            "  border-radius: 11px; padding: 8px 12px; font-size: 12px;"
            "}"
            "QComboBox:hover { border-color: %8; }"
            "QComboBox:focus { border-color: %5; }"
            "QComboBox::drop-down { border: none; width: 24px; }"
            "QComboBox QAbstractItemView {"
            "  background: %6; color: %2; border: 1px solid %4; border-radius: 10px; padding: 5px;"
            "  selection-background-color: %5; selection-color: #1a1206;"
            "  outline: none;"
            "}"
            "QToolTip { background: %6; color: %2; border: 1px solid %4; border-radius: 8px; padding: 6px 9px; font-size: 12px; }"
        ).arg(Pal::BG, Pal::TEXT, Pal::ENTRY, Pal::BORDER,
              Pal::ACCENT, Pal::SURFACE, Pal::MUTED,
              Pal::BORDER_H, Pal::ELEV)
         .arg(Pal::ACCENT2, Pal::ACCENT_H));
    }

    // ── Sidebar navigation rail ──────────────────────────────────────────
    static QString navBtnQss() {
        return QString(
            "QPushButton{ border:none; border-radius:10px; background:transparent; }"
            "QPushButton:hover{ background:%1; }"
            "QPushButton:checked{ background:%2; }"
        ).arg(Pal::SURFACE, Pal::ACCENT);
    }

    QWidget* buildNavRail() {
        auto* rail = new Card;
        rail->setFixedWidth(64);
        auto* v = new QVBoxLayout(rail);
        v->setContentsMargins(10, 16, 10, 16);
        v->setSpacing(6);

        m_navGroup = new QButtonGroup(this);
        m_navGroup->setExclusive(true);
        const QStringList items    = { "Library", "Export", "Download", "Logs" };
        const QStringList iconBase = { "books",   "export", "download", "log"  };
        for (int i = 0; i < items.size(); ++i) {
            auto* b = new QPushButton(rail);
            b->setCheckable(true);
            b->setCursor(Qt::PointingHandCursor);
            b->setFixedSize(40, 40);
            b->setToolTip(items[i]);
            b->setStyleSheet(navBtnQss());

            // Icon swaps color automatically with the checked (On/Off) state —
            // muted grey when unselected, dark-on-orange when the pill is active.
            QIcon icon;
            icon.addPixmap(QPixmap(QString(":/icons/nav/icons8-%1-48-muted.png").arg(iconBase[i])),
                            QIcon::Normal, QIcon::Off);
            icon.addPixmap(QPixmap(QString(":/icons/nav/icons8-%1-48-onaccent.png").arg(iconBase[i])),
                            QIcon::Normal, QIcon::On);
            b->setIcon(icon);
            b->setIconSize(QSize(19, 19));

            m_navGroup->addButton(b, i);
            v->addWidget(b, 0, Qt::AlignHCenter);
            connect(b, &QPushButton::clicked, this, [this, i]{ m_tabs->setCurrentIndex(i); });
        }
        v->addStretch(1);
        if (auto* first = m_navGroup->button(0)) first->setChecked(true);

        // Keep the sidebar highlight synced with programmatic tab changes
        connect(m_tabs, &QTabWidget::currentChanged, this, [this](int idx){
            if (auto* b = m_navGroup->button(idx)) b->setChecked(true);
        });
        return rail;
    }

    // ── UI construction ───────────────────────────────────────────────────────
    void buildUI() {
        auto* root = new QWidget(this);
        setCentralWidget(root);

        auto* lay = new QVBoxLayout(root);
        lay->setContentsMargins(22, 18, 22, 16);
        lay->setSpacing(12);

        lay->addWidget(buildTopBar());
        m_authCard = buildAuthCard();
        lay->addWidget(m_authCard);

        m_tabs = new QTabWidget(root);
        m_tabs->addTab(buildLibraryTab(),  "Library");
        m_tabs->addTab(buildExportTab(),   "Export");
        m_tabs->addTab(buildDownloadTab(), "Download");
        m_tabs->addTab(buildLogsTab(),     "Logs");
        m_tabs->tabBar()->hide();              // native tab bar hidden — sidebar drives it
        m_tabs->setEnabled(false);

        // New layout: left sidebar navigation rail + stacked content on the right
        auto* content = new QHBoxLayout;
        content->setSpacing(16);
        content->addWidget(buildNavRail());
        content->addWidget(m_tabs, 1);
        lay->addLayout(content, 1);

        // ── Tab-switch animation: fade + 18px upward slide ────────────────────
        connect(m_tabs, &QTabWidget::currentChanged, this, [this](int idx) {
            QWidget* page = m_tabs->widget(idx);
            if (!page) return;

            // Opacity fade-in
            auto* eff = new QGraphicsOpacityEffect(page);
            eff->setOpacity(0.0);
            page->setGraphicsEffect(eff);

            auto* fadeAnim = new QPropertyAnimation(eff, "opacity", page);
            fadeAnim->setDuration(220);
            fadeAnim->setStartValue(0.0);
            fadeAnim->setEndValue(1.0);
            fadeAnim->setEasingCurve(QEasingCurve::OutCubic);
            QObject::connect(fadeAnim, &QPropertyAnimation::finished, page, [page]{
                page->setGraphicsEffect(nullptr);
            });

            // Slide-up: animate pos from +18px below to natural position
            const QPoint natural = page->pos();
            const QPoint start   = natural + QPoint(0, 18);
            page->move(start);

            auto* slideAnim = new QPropertyAnimation(page, "pos", page);
            slideAnim->setDuration(220);
            slideAnim->setStartValue(start);
            slideAnim->setEndValue(natural);
            slideAnim->setEasingCurve(QEasingCurve::OutCubic);

            fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);
            slideAnim->start(QAbstractAnimation::DeleteWhenStopped);
        });
    }

    QWidget* buildTopBar() {
        auto* w   = new QWidget;
        auto* lay = new QHBoxLayout(w);
        lay->setContentsMargins(0, 0, 0, 2);
        lay->setSpacing(10);

        // HitPaw mascot mark — smaller now that the header is a slim single row,
        // not a standalone block competing with the auth card below it.
        auto* icon = new QLabel(w);
        QPixmap px(":/icons/icon_64.png");
        if (!px.isNull())
            icon->setPixmap(px.scaled(34, 34, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        else
            icon->setText("🐾");
        icon->setStyleSheet("QLabel { background: transparent; }");
        icon->setFixedSize(34, 34);

        auto* col = new QVBoxLayout;
        // "Hit" white / "Paw" orange — brand wordmark
        auto* h1 = new QLabel(QString(
            "<span style='color:%1'>Hit</span><span style='color:%2'>Paw</span>")
            .arg(Pal::TEXT, Pal::ACCENT), w);
        h1->setStyleSheet("QLabel { font-size: 16px; font-weight: 800; background: transparent; }");

        auto* h2 = new QLabel("MANGADEX MANAGER", w);
        h2->setStyleSheet(QString(
            "QLabel { font-size: 9px; font-weight: 700; letter-spacing: 1px;"
            " color: %1; background: transparent; }").arg(Pal::ACCENT));

        col->setSpacing(0);
        col->addWidget(h1);
        col->addWidget(h2);

        m_authBadge = new StatusBadge(w);

        // Signed-in account chip — lives inline in the top bar instead of a
        // separate full-width row. Only m_connectedBar's visibility toggles
        // (see onLoginOk / onLogout); the chip itself never leaves the layout.
        m_connectedBar = new QWidget(w);
        auto* cbLay = new QHBoxLayout(m_connectedBar);
        cbLay->setContentsMargins(0, 0, 0, 0);
        cbLay->setSpacing(10);
        m_connectedLbl = new QLabel("Signed in", m_connectedBar);
        m_connectedLbl->setStyleSheet(QString(
            "QLabel { background: transparent; color: %1; font-size: 12px; font-weight: 600; }").arg(Pal::TEXT));
        auto* signOutBtn = new GhostButton("Sign Out", m_connectedBar);
        signOutBtn->setFixedHeight(26);
        connect(signOutBtn, &QPushButton::clicked, this, &MainWindow::onLogout);
        cbLay->addWidget(m_connectedLbl);
        cbLay->addWidget(signOutBtn);
        m_connectedBar->hide();

        // About button — always visible regardless of sign-in state, unlike
        // the sign-out chip. Small ghost pill so it doesn't compete visually
        // with the brand mark or the auth chip.
        auto* aboutBtn = new GhostButton("About", w);
        aboutBtn->setFixedHeight(26);
        connect(aboutBtn, &QPushButton::clicked, this, &MainWindow::showAboutDialog);

        lay->addWidget(icon);
        lay->addLayout(col);
        lay->addStretch();
        lay->addWidget(m_connectedBar);
        lay->addWidget(m_authBadge);
        lay->addWidget(aboutBtn);

        return w;
    }

    Card* buildAuthCard() {
        auto* card = new Card;
        auto* lay  = new QVBoxLayout(card);
        lay->setContentsMargins(16, 14, 16, 14);
        lay->setSpacing(8);

        m_authStack = new ShrinkableStack(card);
        m_authStack->setStyleSheet("QStackedWidget { background: transparent; }");
        // Size the stack to the *current* page, not the tallest page.
        // Without this the login page inherits the token page's tutorial height,
        // leaving a blank void above the form fields and making it look like the
        // wrong page is showing.
        m_authStack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

        // ── Page 0: username / password login ──
        auto* loginPage = new QWidget;
        loginPage->setStyleSheet("background: transparent;");
        {
            auto* v = new QVBoxLayout(loginPage);
            v->setContentsMargins(0, 0, 0, 0);
            v->setSpacing(8);

            auto* t = new QLabel("Sign in to MangaDex", loginPage);
            t->setStyleSheet(QString("QLabel { background: transparent; color: %1; font-size: 13px; font-weight: 700; }").arg(Pal::TEXT));
            v->addWidget(t);

            // ── API credentials (personal API client) ──
            auto* credRow = new QHBoxLayout;
            m_clientIdEdit = new QLineEdit(loginPage);
            m_clientIdEdit->setPlaceholderText("API Client ID (personal-client-…)");
            m_clientIdEdit->setText(m_clientId);
            m_clientIdEdit->addAction(QIcon(":/icons/nav/icons8-key-48-text.png"), QLineEdit::LeadingPosition);
            m_clientSecEdit = new QLineEdit(loginPage);
            m_clientSecEdit->setPlaceholderText("API Secret Key");
            m_clientSecEdit->setEchoMode(QLineEdit::Password);
            m_clientSecEdit->setText(m_clientSecret);
            m_clientSecEdit->addAction(QIcon(":/icons/nav/icons8-password-48-text.png"), QLineEdit::LeadingPosition);
            auto* clearCredsBtn = new GhostButton("Clear", loginPage);
            clearCredsBtn->setFixedWidth(64);
            clearCredsBtn->setToolTip("Wipe the saved API client id & secret from this device");
            connect(clearCredsBtn, &QPushButton::clicked, this, &MainWindow::onClearCredentials);
            credRow->addWidget(m_clientIdEdit, 1);
            credRow->addWidget(m_clientSecEdit, 1);
            credRow->addWidget(clearCredsBtn);
            v->addLayout(credRow);

            auto* credHintRow = new QHBoxLayout;
            auto* credHint = new QLabel(
                "From MangaDex → Settings → API Clients.", loginPage);
            credHint->setStyleSheet(QString("QLabel { background: transparent; color: %1; font-size: 10px; }").arg(Pal::MUTED));
            m_rememberCredsChk = new QCheckBox("Remember on this device", loginPage);
            m_rememberCredsChk->setChecked(m_rememberCreds);
            m_rememberCredsChk->setStyleSheet("QCheckBox { font-size: 10px; }");
            connect(m_rememberCredsChk, &QCheckBox::toggled, this, &MainWindow::onRememberCredsToggled);
            credHintRow->addWidget(credHint, 1);
            credHintRow->addWidget(m_rememberCredsChk);
            v->addLayout(credHintRow);

            auto* row = new QHBoxLayout;
            m_userEdit = new QLineEdit(loginPage);
            m_userEdit->setPlaceholderText("Username");
            m_userEdit->addAction(QIcon(":/icons/nav/icons8-profile-48-text.png"), QLineEdit::LeadingPosition);
            m_passEdit = new QLineEdit(loginPage);
            m_passEdit->setPlaceholderText("Password");
            m_passEdit->setEchoMode(QLineEdit::Password);
            m_loginBtn = new AccentButton("Sign In", loginPage);
            m_loginBtn->setFixedWidth(110);
            row->addWidget(m_userEdit, 1);
            row->addWidget(m_passEdit, 1);
            row->addWidget(m_loginBtn);
            v->addLayout(row);

            auto* row2 = new QHBoxLayout;
            m_stayChk = new QCheckBox("Stay signed in on this device", loginPage);
            m_stayChk->setChecked(true);

            auto* alt = new QPushButton("Use an access token instead", loginPage);
            alt->setCursor(Qt::PointingHandCursor);
            alt->setFlat(true);
            alt->setStyleSheet(QString(
                "QPushButton { background: transparent; border: none; color: %1;"
                " font-size: 11px; text-decoration: underline; }"
                "QPushButton:hover { color: %2; }").arg(Pal::MUTED, Pal::ACCENT_H));
            connect(alt, &QPushButton::clicked, [this]{ m_authStack->setCurrentIndex(1); });

            row2->addWidget(m_stayChk);
            row2->addStretch();
            row2->addWidget(alt);
            v->addLayout(row2);

            connect(m_loginBtn, &QPushButton::clicked, this, &MainWindow::onLogin);
            connect(m_passEdit, &QLineEdit::returnPressed, this, &MainWindow::onLogin);
        }

        // ── Page 1: raw access token ──
        auto* tokenPage = new QWidget;
        tokenPage->setStyleSheet("background: transparent;");
        {
            auto* v = new QVBoxLayout(tokenPage);
            v->setContentsMargins(0, 0, 0, 0);
            v->setSpacing(8);

            auto* t = new QLabel("Paste an access token (F12 console on mangadex.org)", tokenPage);
            t->setStyleSheet(QString("QLabel { background: transparent; color: %1; font-size: 13px; font-weight: 700; }").arg(Pal::TEXT));
            v->addWidget(t);

            auto* row = new QHBoxLayout;
            m_tokenEdit = new QLineEdit(tokenPage);
            m_tokenEdit->setEchoMode(QLineEdit::Password);
            m_tokenEdit->setPlaceholderText("Paste your access token here…");
            m_tokenBtn = new AccentButton("Connect", tokenPage);
            m_tokenBtn->setFixedWidth(110);
            row->addWidget(m_tokenEdit, 1);
            row->addWidget(m_tokenBtn);
            v->addLayout(row);

            auto* back = new QPushButton("Use username && password instead", tokenPage);
            back->setCursor(Qt::PointingHandCursor);
            back->setFlat(true);
            back->setStyleSheet(QString(
                "QPushButton { background: transparent; border: none; color: %1;"
                " font-size: 11px; text-decoration: underline; text-align: left; }"
                "QPushButton:hover { color: %2; }").arg(Pal::MUTED, Pal::ACCENT_H));
            connect(back, &QPushButton::clicked, [this]{ m_authStack->setCurrentIndex(0); });
            v->addWidget(back, 0, Qt::AlignLeft);

            // ── Token tutorial ──
            auto* tutFrame = new QFrame(tokenPage);
            tutFrame->setFrameShape(QFrame::StyledPanel);
            tutFrame->setStyleSheet(QString(
                "QFrame { background: %1; border: 1px solid %2; border-radius: 8px; }"
            ).arg(Pal::CODE, Pal::BORDER));
            auto* tutLay = new QVBoxLayout(tutFrame);
            tutLay->setContentsMargins(12, 10, 12, 10);
            tutLay->setSpacing(4);

            auto* tutTitle = new QLabel("How to get your Access Token", tutFrame);
            tutTitle->setStyleSheet(QString("QLabel { background: transparent; color: %1; font-size: 11px; font-weight: 700; }").arg(Pal::ACCENT));
            tutLay->addWidget(tutTitle);

            // Steps 1-3 first, then the command row, then steps 4-5.
            // Built in order — no insertWidget/removeWidget/reparenting tricks
            // that corrupt the layout and blow out the card height.
            auto addStep = [&](const QString& html) {
                auto* lbl = new QLabel(html, tutFrame);
                lbl->setWordWrap(true);
                lbl->setTextFormat(Qt::RichText);
                lbl->setStyleSheet(QString("QLabel { background: transparent; color: %1; font-size: 11px; }").arg(Pal::TEXT));
                tutLay->addWidget(lbl);
            };

            addStep("1.  Open <b>mangadex.org</b> in your browser and sign in.");
            addStep("2.  Press <b>F12</b> to open DevTools, then click the <b>Console</b> tab.");
            addStep("3.  Paste this command and press <b>Enter</b>:");

            // Command row — built directly in order, no layout surgery
            auto* cmdBox = new QLineEdit(tutFrame);
            cmdBox->setReadOnly(true);
            cmdBox->setText("Object.values(localStorage).map(v=>{try{return JSON.parse(v).access_token}catch{}}).find(t=>t)");
            cmdBox->setStyleSheet(QString(
                "QLineEdit { background: %1; color: %2; border: 1px solid %3;"
                " border-radius: 4px; padding: 4px 8px; font-family: monospace; font-size: 10px; }"
            ).arg(Pal::ENTRY, Pal::TEXT, Pal::BORDER));
            cmdBox->setCursorPosition(0);

            auto* copyBtn = new GhostButton("Copy", tutFrame);
            copyBtn->setFixedWidth(52);
            copyBtn->setFixedHeight(24);
            connect(copyBtn, &QPushButton::clicked, [cmdBox]{
                QGuiApplication::clipboard()->setText(cmdBox->text());
            });

            auto* cmdRow = new QHBoxLayout;
            cmdRow->setContentsMargins(0, 0, 0, 0);
            cmdRow->setSpacing(6);
            copyBtn->setFixedSize(58, 28);   // explicit fixed size — no size policy fight
            copyBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            cmdBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            cmdRow->addWidget(cmdBox, 1);
            cmdRow->addWidget(copyBtn, 0, Qt::AlignVCenter);
            tutLay->addLayout(cmdRow);

            addStep("4.  Copy the long string that appears — that is your access token.");
            addStep("5.  Paste it into the field above and click <b>Connect</b>.");

            auto* apiNote = new QLabel(
                "<b>Also need:</b> API Client ID &amp; Secret — get them at "
                "<a href='https://mangadex.org/settings' style='color:%1;'>mangadex.org/settings</a> → API Clients → Create. "
                "Fill those in the fields on the Sign-in tab first."
            );
            apiNote->setOpenExternalLinks(true);
            apiNote->setWordWrap(true);
            apiNote->setTextFormat(Qt::RichText);
            apiNote->setText(apiNote->text().replace("%1", Pal::ACCENT));
            apiNote->setStyleSheet(QString("QLabel { background: transparent; color: %1; font-size: 10px; }").arg(Pal::MUTED));
            tutLay->addWidget(apiNote);

            v->addWidget(tutFrame);

            connect(m_tokenBtn, &QPushButton::clicked, this, &MainWindow::onTokenConnect);
            connect(m_tokenEdit, &QLineEdit::returnPressed, this, &MainWindow::onTokenConnect);
        }

        m_authStack->addWidget(loginPage);
        m_authStack->addWidget(tokenPage);
        m_authStack->setCurrentIndex(0);   // explicit default: always open on login page, never the token page
        lay->addWidget(m_authStack);
        return card;
    }

    QWidget* buildLibraryTab() {
        auto* page = new QWidget;
        auto* v    = new QVBoxLayout(page);
        v->setContentsMargins(0, 10, 0, 0);
        v->setSpacing(10);

        // ── Toolbar — search, filter, and actions in one slim row. Page identity
        //    comes from the active rail icon now, so no repeated "Library" title. ──
        auto* toolbar = new QHBoxLayout;
        toolbar->setSpacing(8);

        m_filterEdit = new QLineEdit(page);
        m_filterEdit->setPlaceholderText("Search titles");
        m_filterEdit->setClearButtonEnabled(true);
        m_filterEdit->setFixedHeight(34);
        connect(m_filterEdit, &QLineEdit::textChanged, [this]{ relayoutLibrary(); });

        m_filterStatus = new QComboBox(page);
        m_filterStatus->addItem("All status", "");
        for (auto it = STATUS_LABELS.begin(); it != STATUS_LABELS.end(); ++it)
            m_filterStatus->addItem(it.value(), it.key());
        m_filterStatus->setFixedWidth(140);
        m_filterStatus->setFixedHeight(34);
        connect(m_filterStatus, &QComboBox::currentIndexChanged, [this]{ relayoutLibrary(); });

        // Compact icon-only utility actions — tooltip carries the label instead
        // of a permanent text column, so the toolbar doesn't fight the search box.
        auto makeIconBtn = [&](const QString& iconFile, const QString& tip) {
            auto* b = new GhostButton(QString(), page);
            b->setFixedSize(34, 34);
            b->setIcon(QIcon(iconFile));
            b->setIconSize(QSize(15, 15));
            b->setToolTip(tip);
            return b;
        };
        m_selAllBtn   = makeIconBtn(":/icons/nav/icons8-select-all-48-text.png", "Select All");
        m_clearSelBtn = makeIconBtn(":/icons/nav/icons8-clear-48-text.png",      "Clear selection");
        m_undoBtn     = makeIconBtn(":/icons/nav/icons8-undo-48-text.png",       "Undo the last manual selection change");
        m_undoBtn->setEnabled(false);
        connect(m_selAllBtn,   &QPushButton::clicked, [this]{ selectVisible(true); });
        connect(m_clearSelBtn, &QPushButton::clicked, [this]{ clearSelection(); });
        connect(m_undoBtn,     &QPushButton::clicked, this, &MainWindow::undoSelection);

        m_loadBtn = new AccentButton("Load Library", page);
        m_loadBtn->setFixedHeight(34);
        connect(m_loadBtn, &QPushButton::clicked, this, &MainWindow::startLibraryFetch);

        m_stopBtn = new GhostButton("Stop", page);
        m_stopBtn->setFixedHeight(34);
        m_stopBtn->setEnabled(false);
        connect(m_stopBtn, &QPushButton::clicked, this, &MainWindow::stopLibraryFetch);

        toolbar->addWidget(m_filterEdit, 1);
        toolbar->addWidget(m_filterStatus);
        toolbar->addWidget(m_selAllBtn);
        toolbar->addWidget(m_clearSelBtn);
        toolbar->addWidget(m_undoBtn);
        toolbar->addWidget(m_loadBtn);
        toolbar->addWidget(m_stopBtn);
        v->addLayout(toolbar);

        // ── Stats strip — one thin line: live status text, selection count,
        //    and the fetch progress bar, only ever a few px tall. ──
        auto* statsRow = new QHBoxLayout;
        statsRow->setSpacing(10);

        m_statsLbl = new QLabel("Sign in to load your library.", page);
        m_statsLbl->setStyleSheet(QString("QLabel { color: %1; font-size: 11px; background: transparent; }").arg(Pal::MUTED));

        m_libProgress = new SmoothProgressBar(page);
        m_libProgress->setRange(0, 100);
        m_libProgress->setValueInstant(0);
        m_libProgress->setFixedHeight(6);
        m_libProgress->setTextVisible(false);
        m_libProgress->hide();

        m_countLbl = new QLabel("", page);
        m_countLbl->setStyleSheet(QString("QLabel { color: %1; font-size: 11px; font-weight: 700; background: transparent; }").arg(Pal::ACCENT));

        statsRow->addWidget(m_statsLbl);
        statsRow->addWidget(m_libProgress, 1);
        statsRow->addWidget(m_countLbl);
        v->addLayout(statsRow);

        // ── Body stack: dashed empty-state placeholder  ⇄  cover grid ──
        m_libraryStack = new ShrinkableStack(page);

        auto* emptyState = new QFrame(page);
        emptyState->setObjectName("emptyState");
        emptyState->setStyleSheet(QString(
            "QFrame#emptyState {"
            "  background: transparent;"
            "  border: 2px dashed %1;"
            "  border-radius: 14px;"
            "}").arg(Pal::BORDER));
        auto* esLay = new QVBoxLayout(emptyState);
        esLay->setAlignment(Qt::AlignCenter);
        esLay->setSpacing(8);

        auto* esIcon = new OutlineBoxIcon(emptyState);

        auto* esTitle = new QLabel("Your library will appear here", emptyState);
        esTitle->setAlignment(Qt::AlignCenter);
        esTitle->setStyleSheet(QString(
            "QLabel { background: transparent; color: %1; font-size: 14px; font-weight: 700; }").arg(Pal::TEXT));

        auto* esSub = new QLabel("Sign in and load your MangaDex library to get started.", emptyState);
        esSub->setAlignment(Qt::AlignCenter);
        esSub->setStyleSheet(QString(
            "QLabel { background: transparent; color: %1; font-size: 12px; }").arg(Pal::MUTED));

        esLay->addWidget(esIcon, 0, Qt::AlignHCenter);
        esLay->addWidget(esTitle, 0, Qt::AlignHCenter);
        esLay->addWidget(esSub, 0, Qt::AlignHCenter);
        emptyState->setMinimumHeight(260);

        // Cover grid
        auto* scroll = new SmoothScrollArea(page);
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);
        scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        auto* gridHost = new QWidget;
        auto* hostLay  = new QVBoxLayout(gridHost);
        hostLay->setContentsMargins(0, 0, 6, 0);

        // Shown instead of a blank grid when the library has titles but the
        // current filter/search matches none of them — previously the grid
        // just went empty with zero explanation of why.
        m_noResultsLbl = new QLabel("No titles match your filter.", gridHost);
        m_noResultsLbl->setAlignment(Qt::AlignCenter);
        m_noResultsLbl->setStyleSheet(QString(
            "QLabel { background: transparent; color: %1; font-size: 13px; padding: 40px 0; }").arg(Pal::MUTED));
        m_noResultsLbl->hide();
        hostLay->addWidget(m_noResultsLbl);

        auto* gridW = new QWidget(gridHost);
        m_grid = new QGridLayout(gridW);
        m_grid->setContentsMargins(0, 0, 0, 0);
        m_grid->setSpacing(12);
        for (int c = 0; c < GRID_COLS; ++c)
            m_grid->setColumnStretch(c, 1);
        hostLay->addWidget(gridW);
        hostLay->addStretch();
        scroll->setWidget(gridHost);

        m_libraryStack->addWidget(emptyState);  // index 0 — shown when the library is empty/not loaded
        m_libraryStack->addWidget(scroll);      // index 1 — shown once titles are loaded
        m_libraryStack->setCurrentIndex(0);
        v->addWidget(m_libraryStack, 1);

        return page;
    }

    // Swap between the dashed empty-state placeholder and the cover grid.
    void updateLibraryStackVisibility() {
        if (!m_libraryStack) return;
        m_libraryStack->setCurrentIndex(m_libraryOrder.isEmpty() ? 0 : 1);
    }

    QWidget* buildExportTab() {
        auto* page = new QWidget;
        auto* v    = new QVBoxLayout(page);
        v->setContentsMargins(0, 10, 0, 0);
        v->setSpacing(10);

        // Output folder + formats
        auto* card = new Card(page);
        auto* lay  = new QVBoxLayout(card);
        lay->setContentsMargins(16, 14, 16, 14);
        lay->setSpacing(10);

        auto sectionLabel = [&](const QString& t) {
            auto* l = new QLabel(t, card);
            l->setStyleSheet(QString("QLabel { background: transparent; color: %1; font-size: 13px; font-weight: 700; }").arg(Pal::TEXT));
            lay->addWidget(l);
        };

        sectionLabel("Output Folder");
        auto* outRow = new QHBoxLayout;
        m_outEdit = new QLineEdit(card);
        m_outEdit->setText(m_settings.value("export/dir",
            QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)).toString());
        auto* browseBtn = new GhostButton("Browse", card);
        browseBtn->setFixedWidth(80);
        connect(browseBtn, &QPushButton::clicked, this, &MainWindow::onBrowse);
        auto* openBtn = new GhostButton("Open Folder", card);
        openBtn->setFixedWidth(110);
        connect(openBtn, &QPushButton::clicked, [this]{
            QDesktopServices::openUrl(QUrl::fromLocalFile(m_outEdit->text()));
        });
        outRow->addWidget(m_outEdit, 1);
        outRow->addWidget(browseBtn);
        outRow->addWidget(openBtn);
        lay->addLayout(outRow);

        auto* sep = new QFrame(card);
        sep->setFrameShape(QFrame::HLine);
        sep->setStyleSheet(QString("QFrame { color: %1; }").arg(Pal::BORDER));
        lay->addWidget(sep);

        sectionLabel("Export Formats");
        auto* fmtRow = new QHBoxLayout;
        fmtRow->setSpacing(24);
        auto mkCheck = [&](const QString& label, const QString& desc, const QString& key) -> QCheckBox* {
            auto* col = new QVBoxLayout;
            auto* cb  = new QCheckBox(label, card);
            cb->setChecked(m_settings.value("export/" + key, true).toBool());
            auto* d = new QLabel(desc, card);
            d->setStyleSheet(QString("QLabel { background: transparent; color: %1; font-size: 11px; }").arg(Pal::MUTED));
            col->setSpacing(2);
            col->addWidget(cb);
            col->addWidget(d);
            fmtRow->addLayout(col);
            return cb;
        };
        m_chkCSV  = mkCheck("CSV",     "General spreadsheet",           "csv");
        m_chkJSON = mkCheck("JSON",    "Raw data backup (with covers)", "json");
        m_chkMAL  = mkCheck("MAL XML", "MAL · AniList · MangaBaka · Kitsu · MangaFire", "mal");
        m_chkAP   = mkCheck("AP .gz",  "Anime-Planet (gzipped MAL XML)",              "ap");
        m_chkMB   = mkCheck("MB JSON", "MangaBaka native format (mangabaka.org)",      "mb");
        fmtRow->addStretch();
        lay->addLayout(fmtRow);
        v->addWidget(card);

        // Action row — two explicit, independent export actions
        auto* aRow = new QHBoxLayout;
        m_exportAllBtn = new AccentButton("Export Entire Library", page);
        m_exportAllBtn->setFixedWidth(200);
        connect(m_exportAllBtn, &QPushButton::clicked, this, [this]{ onExport(/*selectedOnly*/false); });

        m_exportSelBtn = new GhostButton("Export Selected", page);
        m_exportSelBtn->setFixedWidth(200);
        m_exportSelBtn->setEnabled(false);
        connect(m_exportSelBtn, &QPushButton::clicked, this, [this]{ onExport(/*selectedOnly*/true); });

        aRow->addWidget(m_exportAllBtn);
        aRow->addWidget(m_exportSelBtn);
        aRow->addSpacing(4);
        v->addLayout(aRow);

        m_selInfo = new QLabel("Nothing selected — pick titles in Library to enable Export Selected.", page);
        m_selInfo->setStyleSheet(QString("QLabel { color: %1; font-size: 12px; background: transparent; }").arg(Pal::MUTED));
        v->addWidget(m_selInfo);

        auto* sep2 = new QFrame(page);
        sep2->setFrameShape(QFrame::HLine);
        sep2->setStyleSheet(QString("QFrame { color: %1; }").arg(Pal::BORDER));
        v->addWidget(sep2);

        // Import guide
        auto* guide = new QLabel(
            "MAL XML → MyAnimeList · AniList (Settings→Import) · MangaBaka (Settings→Import→MAL) · Kitsu (Settings→Import) · MangaFire (Profile→Import/Export)\nAP .gz → Anime-Planet (your list → Import it now)    |    MangaUpdates & comix.to have no list import.", page);
        guide->setStyleSheet(QString("QLabel { color: %1; font-size: 11px; background: transparent; }").arg(Pal::MUTED));
        v->addWidget(guide);
        v->addStretch();

        return page;
    }

    QWidget* buildDownloadTab() {
        auto* page = new QWidget;
        auto* v    = new QVBoxLayout(page);
        v->setContentsMargins(0, 10, 0, 0);
        v->setSpacing(10);

        // ── URL input card ────────────────────────────────────────────────────
        auto* urlCard = new Card(page);
        auto* ulLay   = new QVBoxLayout(urlCard);
        ulLay->setContentsMargins(16, 14, 16, 14);
        ulLay->setSpacing(8);

        auto* urlHdr = new QLabel("Paste a MangaDex manga URL or UUID", urlCard);
        urlHdr->setStyleSheet(QString("QLabel{background:transparent;color:%1;font-size:13px;font-weight:700;}").arg(Pal::TEXT));
        ulLay->addWidget(urlHdr);

        auto* urlRow = new QHBoxLayout;
        m_dlUrlEdit = new QLineEdit(urlCard);
        m_dlUrlEdit->setPlaceholderText("https://mangadex.org/title/xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx");
        m_dlLookupBtn = new AccentButton("Look Up", urlCard);
        m_dlLookupBtn->setFixedWidth(110);
        connect(m_dlLookupBtn, &QPushButton::clicked, this, &MainWindow::onDlLookup);
        connect(m_dlUrlEdit, &QLineEdit::returnPressed, this, &MainWindow::onDlLookup);
        urlRow->addWidget(m_dlUrlEdit, 1);
        urlRow->addWidget(m_dlLookupBtn);
        ulLay->addLayout(urlRow);

        // hint
        auto* hint = new QLabel("You can also right-click any manga card in Library → Open on MangaDex, then copy the URL here.", urlCard);
        hint->setStyleSheet(QString("QLabel{background:transparent;color:%1;font-size:10px;}").arg(Pal::MUTED));
        hint->setWordWrap(true);
        ulLay->addWidget(hint);

        v->addWidget(urlCard);

        // ── Manga info + chapter list ─────────────────────────────────────────
        auto* midRow = new QHBoxLayout;
        midRow->setSpacing(10);

        // Cover + info panel (left)
        auto* infoCard = new Card(page);
        infoCard->setFixedWidth(200);
        auto* infoLay = new QVBoxLayout(infoCard);
        infoLay->setContentsMargins(12, 12, 12, 12);
        infoLay->setSpacing(4);

        m_dlMangaCover = new QLabel(infoCard);
        m_dlMangaCover->setFixedSize(154, 220);
        m_dlMangaCover->setAlignment(Qt::AlignCenter);
        m_dlMangaCover->setStyleSheet(
            "QLabel{background:#161618;border-radius:6px;color:#3a3a44;"
            "font-size:24px;overflow:hidden;}");
        m_dlMangaCover->setScaledContents(false);
        m_dlMangaCover->setText("🐾");
        infoLay->addWidget(m_dlMangaCover, 0, Qt::AlignHCenter);

        // m_dlMangaTitle kept as a hidden no-op so onDlLookup setText calls don't crash
        m_dlMangaTitle = new QLabel(page);
        m_dlMangaTitle->hide();

        // ── Author / Artist / Genres / Demographic ────────────────────────────
        // Both the heading and the "—" placeholder are collected so the whole
        // section can be hidden while signed out (see setDownloadInfoVisible).
        auto makeInfoSection = [&](const QString& heading) -> QLabel* {
            infoLay->addSpacing(6);
            auto* hdr = new QLabel(heading, infoCard);
            hdr->setStyleSheet(QString(
                "QLabel{background:transparent;color:%1;font-size:9px;font-weight:700;}").arg(Pal::ACCENT));
            infoLay->addWidget(hdr);
            auto* val = new QLabel("—", infoCard);
            val->setWordWrap(true);
            val->setAlignment(Qt::AlignTop | Qt::AlignLeft);
            val->setStyleSheet(QString(
                "QLabel{background:transparent;color:%1;font-size:10px;}").arg(Pal::TEXT));
            infoLay->addWidget(val);
            m_dlInfoLabels << hdr << val;
            return val;
        };

        m_dlMangaAuthor = makeInfoSection("AUTHOR");
        m_dlMangaArtist = makeInfoSection("ARTIST");
        m_dlMangaGenres = makeInfoSection("GENRES");
        m_dlMangaDemog  = makeInfoSection("DEMOGRAPHIC");
        setDownloadInfoVisible(false);   // signed out on launch — no placeholders shown

        infoLay->addStretch();
        midRow->addWidget(infoCard, 0);

        // Chapter list (right)
        auto* chCard = new Card(page);
        auto* chLay  = new QVBoxLayout(chCard);
        chLay->setContentsMargins(12, 12, 12, 12);
        chLay->setSpacing(8);

        // Chapter controls row
        auto* chCtrl = new QHBoxLayout;
        auto* chHdr  = new QLabel("Chapters", chCard);
        chHdr->setStyleSheet(QString("QLabel{background:transparent;color:%1;font-size:13px;font-weight:700;}").arg(Pal::TEXT));
        chCtrl->addWidget(chHdr);
        chCtrl->addStretch();

        m_dlLangFilter = new QComboBox(chCard);
        m_dlLangFilter->setFixedWidth(80);
        m_dlLangFilter->addItem("All", "");
        m_dlLangFilter->setToolTip("Filter chapters by language");
        connect(m_dlLangFilter, &QComboBox::currentIndexChanged, this, &MainWindow::onDlLangFilter);
        chCtrl->addWidget(m_dlLangFilter);

        auto* selAllCh  = new GhostButton("All",  chCard);
        selAllCh->setFixedSize(52, 28);
        connect(selAllCh, &QPushButton::clicked, [this]{ for(auto*c:m_dlChkBoxes) c->setChecked(true); });
        auto* selNoneCh = new GhostButton("None", chCard);
        selNoneCh->setFixedSize(58, 28);
        connect(selNoneCh, &QPushButton::clicked, [this]{ for(auto*c:m_dlChkBoxes) c->setChecked(false); });
        chCtrl->addWidget(selAllCh);
        chCtrl->addWidget(selNoneCh);
        chLay->addLayout(chCtrl);

        m_dlChapterScroll = new SmoothScrollArea(chCard);
        m_dlChapterScroll->setWidgetResizable(true);
        m_dlChapterScroll->setFrameShape(QFrame::NoFrame);
        m_dlChapterScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_dlChapterList = new QWidget;
        m_dlChapterLay  = new QVBoxLayout(m_dlChapterList);
        m_dlChapterLay->setContentsMargins(0, 0, 0, 0);
        m_dlChapterLay->setSpacing(3);
        m_dlChapterLay->addStretch();
        m_dlChapterScroll->setWidget(m_dlChapterList);
        chLay->addWidget(m_dlChapterScroll, 1);

        midRow->addWidget(chCard, 1);
        v->addLayout(midRow, 1);

        // ── Download controls card ─────────────────────────────────────────────
        auto* dlCard = new Card(page);
        auto* dlLay  = new QVBoxLayout(dlCard);
        dlLay->setContentsMargins(16, 12, 16, 12);
        dlLay->setSpacing(8);

        // Output path row
        auto* pathRow = new QHBoxLayout;
        auto* pathLbl = new QLabel("Download to:", dlCard);
        pathLbl->setStyleSheet(QString("QLabel{background:transparent;color:%1;font-size:12px;}").arg(Pal::MUTED));
        pathLbl->setFixedWidth(90);
        m_dlPathEdit = new QLineEdit(dlCard);
        m_dlPathEdit->setPlaceholderText("Same as Export folder…");
        m_dlPathEdit->setText(m_settings.value("download/dir", "").toString());
        auto* dlBrowse = new GhostButton("Browse", dlCard);
        dlBrowse->setFixedWidth(80);
        connect(dlBrowse, &QPushButton::clicked, this, [this]{
            QString d = QFileDialog::getExistingDirectory(this, "Download Folder",
                m_dlPathEdit->text().isEmpty() ? m_outEdit->text() : m_dlPathEdit->text());
            if (!d.isEmpty()) { m_dlPathEdit->setText(d); m_settings.setValue("download/dir", d); }
        });
        pathRow->addWidget(pathLbl);
        pathRow->addWidget(m_dlPathEdit, 1);
        pathRow->addWidget(dlBrowse);
        dlLay->addLayout(pathRow);

        auto* pathNote = new QLabel(
            "Each manga is saved to:  <Download folder> / <Manga Title> / <Chapter folder> / 001.jpg …", dlCard);
        pathNote->setStyleSheet(QString("QLabel{background:transparent;color:%1;font-size:10px;}").arg(Pal::MUTED));
        dlLay->addWidget(pathNote);

        // Progress + buttons
        m_dlProgress = new SmoothProgressBar(dlCard);
        m_dlProgress->setRange(0, 100);
        m_dlProgress->setValueInstant(0);
        m_dlProgress->setTextVisible(false);
        m_dlProgress->setFixedHeight(6);
        dlLay->addWidget(m_dlProgress);

        m_dlStatusLbl = new QLabel("Select chapters and press Download.", dlCard);
        m_dlStatusLbl->setStyleSheet(QString("QLabel{background:transparent;color:%1;font-size:12px;}").arg(Pal::MUTED));
        dlLay->addWidget(m_dlStatusLbl);

        auto* btnRow = new QHBoxLayout;
        m_dlStartBtn = new AccentButton("Download Selected Chapters", dlCard);
        m_dlStartBtn->setEnabled(false);
        connect(m_dlStartBtn, &QPushButton::clicked, this, &MainWindow::onDlStart);
        m_dlStopBtn  = new GhostButton("Stop", dlCard);
        m_dlStopBtn->setEnabled(false);
        connect(m_dlStopBtn, &QPushButton::clicked, this, [this]{
            m_dlStop = true;
            m_dlStopBtn->setEnabled(false);
            m_dlStatusLbl->setText("Stopping after current page…");
        });
        btnRow->addWidget(m_dlStartBtn);
        btnRow->addWidget(m_dlStopBtn);
        btnRow->addStretch();
        dlLay->addLayout(btnRow);

        v->addWidget(dlCard);
        return page;
    }

    // ── Download logic ────────────────────────────────────────────────────────

    // Extract UUID from URL like https://mangadex.org/title/<uuid>[/...]
    static QString extractMangaId(const QString& input) {
        QString s = input.trimmed();
        // Bare UUID
        static const QRegularExpression uuid(
            R"(([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}))",
            QRegularExpression::CaseInsensitiveOption);
        auto m = uuid.match(s);
        return m.hasMatch() ? m.captured(1).toLower() : QString();
    }

    void onDlLookup() {
        const QString id = extractMangaId(m_dlUrlEdit->text());
        if (id.isEmpty()) {
            m_dlStatusLbl->setText("Couldn't find a MangaDex UUID in that URL. Paste the full manga page URL.");
            return;
        }
        m_dlMangaId = id;
        m_dlLookupBtn->setEnabled(false);
        m_dlStatusLbl->setText("Looking up manga…");
        m_dlMangaTitle->setText("Loading…");
        m_dlMangaCover->setText("🐾");
        m_dlMangaAuthor->setText("—");
        m_dlMangaArtist->setText("—");
        m_dlMangaGenres->setText("—");
        m_dlMangaDemog->setText("—");

        // Clear old chapter list
        m_dlChapters.clear();
        m_dlChkBoxes.clear();
        QLayoutItem* item;
        while ((item = m_dlChapterLay->takeAt(0))) { delete item->widget(); delete item; }
        m_dlChapterLay->addStretch();

        // Fetch manga metadata (for title + cover)
        QUrl url(QString(API_BASE) + "/manga/" + id);
        QUrlQuery q;
        q.addQueryItem("includes[]", "cover_art");
        q.addQueryItem("includes[]", "author");
        q.addQueryItem("includes[]", "artist");
        url.setQuery(q);
        auto* reply = apiGet(url);
        connect(reply, &QNetworkReply::finished, this, [this, reply] {
            reply->deleteLater();
            m_dlLookupBtn->setEnabled(true);
            if (reply->error() != QNetworkReply::NoError) {
                m_dlStatusLbl->setText("Manga lookup failed: " + reply->errorString());
                return;
            }
            const auto doc  = QJsonDocument::fromJson(reply->readAll());
            const auto data = doc["data"].toObject();
            const auto e    = parseManga(data, "");
            m_dlMangaTitleStr = e.title;
            m_dlMangaTitle->setText(e.title);
            // Populate info panel
            m_dlMangaAuthor->setText(e.authors.isEmpty()   ? "—" : e.authors);
            m_dlMangaArtist->setText(e.artists.isEmpty()   ? "—" : e.artists);
            m_dlMangaGenres->setText(e.genres.isEmpty()    ? "—" : e.genres);
            m_dlMangaDemog->setText( e.demographic.isEmpty()? "—" : e.demographic[0].toUpper()
                                                                     + e.demographic.mid(1));
            // Load cover
            if (!e.coverUrl.isEmpty())
                CoverLoader::inst().load(e.coverUrl, m_dlMangaCover, QSize(154, 220));
            m_dlStatusLbl->setText(QString("Found: %1. Fetching chapter list…").arg(e.title));
            fetchChapters(0);
        });
    }

    void fetchChapters(int offset) {
        QUrl url(QString(API_BASE) + "/manga/" + m_dlMangaId + "/feed");
        QUrlQuery q;
        q.addQueryItem("limit",           "500");
        q.addQueryItem("offset",          QString::number(offset));
        q.addQueryItem("order[volume]",   "asc");
        q.addQueryItem("order[chapter]",  "asc");
        q.addQueryItem("includes[]",      "scanlation_group");
        for (const char* cr : {"safe","suggestive","erotica","pornographic"})
            q.addQueryItem("contentRating[]", cr);
        url.setQuery(q);

        auto* reply = apiGet(url);
        connect(reply, &QNetworkReply::finished, this, [this, reply, offset] {
            reply->deleteLater();
            if (reply->error() != QNetworkReply::NoError) {
                m_dlStatusLbl->setText("Chapter fetch failed: " + reply->errorString());
                return;
            }
            const auto doc   = QJsonDocument::fromJson(reply->readAll());
            const auto arr   = doc["data"].toArray();
            const int  total = doc["total"].toInt();

            for (const auto& item : arr) {
                const auto obj   = item.toObject();
                const auto attrs = obj["attributes"].toObject();
                if (attrs["externalUrl"].toString().isEmpty() == false) continue; // external link only
                if (attrs["pages"].toInt() == 0) continue; // no pages

                ChapterInfo ci;
                ci.id      = obj["id"].toString();
                ci.volume  = attrs["volume"].toString();
                ci.chapter = attrs["chapter"].toString();
                ci.title   = attrs["title"].toString();
                ci.lang    = attrs["translatedLanguage"].toString();
                ci.pages   = attrs["pages"].toInt();

                for (const auto& rel : obj["relationships"].toArray()) {
                    auto ro = rel.toObject();
                    if (ro["type"].toString() == "scanlation_group")
                        ci.group = ro["attributes"].toObject()["name"].toString();
                }
                m_dlChapters.append(ci);
            }

            if (offset + arr.size() < total && !arr.isEmpty()) {
                // More pages
                QTimer::singleShot(300, this, [this, offset, arr]{
                    fetchChapters(offset + arr.size());
                });
            } else {
                populateChapterList();
            }
        });
    }

    void populateChapterList(const QString& langFilter = "") {
        // Sort numerically every time we (re-)populate — handles decimal chapters like 7.5, 10.1
        std::sort(m_dlChapters.begin(), m_dlChapters.end(),
            [](const ChapterInfo& a, const ChapterInfo& b) {
                // Sort purely by chapter number — volume is unreliable on MangaDex.
                // Empty chapter string (rare API quirk) treated as 0 so it sorts first.
                bool okA, okB;
                double na = a.chapter.isEmpty() ? 0.0 : a.chapter.toDouble(&okA);
                double nb = b.chapter.isEmpty() ? 0.0 : b.chapter.toDouble(&okB);
                if (a.chapter.isEmpty()) okA = true;
                if (b.chapter.isEmpty()) okB = true;
                if (okA && okB) return na < nb;
                if (okA) return true;    // numbered before oneshots
                if (okB) return false;
                return a.chapter < b.chapter;
            });
        // Rebuild language combo
        if (langFilter.isEmpty()) {
            QSet<QString> langs;
            for (const auto& c : m_dlChapters) langs.insert(c.lang);
            m_dlLangFilter->blockSignals(true);
            m_dlLangFilter->clear();
            m_dlLangFilter->addItem("All", "");
            QStringList sorted = langs.values();
            std::sort(sorted.begin(), sorted.end());
            for (const auto& l : sorted) m_dlLangFilter->addItem(l.toUpper(), l);
            // Default to English if available
            int enIdx = m_dlLangFilter->findData("en");
            if (enIdx >= 0) m_dlLangFilter->setCurrentIndex(enIdx);
            m_dlLangFilter->blockSignals(false);
        }

        const QString activeLang = m_dlLangFilter->currentData().toString();

        // Clear old widgets
        m_dlChkBoxes.clear();
        QLayoutItem* item;
        while ((item = m_dlChapterLay->takeAt(0))) { delete item->widget(); delete item; }

        int shown = 0;
        for (const auto& ci : m_dlChapters) {
            if (!activeLang.isEmpty() && ci.lang != activeLang) continue;

            auto* row = new QWidget(m_dlChapterList);
            auto* rl  = new QHBoxLayout(row);
            rl->setContentsMargins(6, 3, 8, 3);
            rl->setSpacing(10);

            auto* chk = new QCheckBox(row);
            chk->setChecked(true);
            chk->setProperty("chapterId", ci.id);
            rl->addWidget(chk);
            m_dlChkBoxes.append(chk);

            QString label = "Ch." + (ci.chapter.isEmpty() ? "Oneshot" : ci.chapter);
            if (!ci.title.isEmpty()) label += "  " + ci.title;

            auto* lbl = new QLabel(label, row);
            lbl->setStyleSheet(QString("QLabel{background:transparent;color:%1;font-size:11px;}").arg(Pal::TEXT));
            lbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            lbl->setMinimumWidth(0);
            rl->addWidget(lbl, 1);

            auto* langLbl = new QLabel(ci.lang.toUpper(), row);
            langLbl->setFixedSize(28, 18);
            langLbl->setAlignment(Qt::AlignCenter);
            langLbl->setStyleSheet(QString(
                "QLabel{background:%1;color:%2;border-radius:3px;font-size:9px;font-weight:700;padding:1px 3px;}")
                .arg(Pal::SURFACE, Pal::ACCENT));
            rl->addWidget(langLbl);

            auto* pgLbl = new QLabel(QString("%1p").arg(ci.pages), row);
            pgLbl->setFixedWidth(36);
            pgLbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            pgLbl->setStyleSheet(QString("QLabel{background:transparent;color:%1;font-size:10px;}").arg(Pal::MUTED));
            rl->addWidget(pgLbl);

            // Always add group column — empty string keeps layout aligned across all rows
            {
                auto* grp = new QLabel(row);
                grp->setFixedWidth(150);
                grp->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                grp->setStyleSheet(QString("QLabel{background:transparent;color:%1;font-size:10px;}").arg(Pal::MUTED));
                if (!ci.group.isEmpty()) {
                    // Elide long group names instead of overflowing or wrapping
                    QFontMetrics fm(grp->font());
                    grp->setText(fm.elidedText(ci.group, Qt::ElideRight, 146));
                    grp->setToolTip(ci.group);
                }
                rl->addWidget(grp);
            }

            m_dlChapterLay->insertWidget(m_dlChapterLay->count() - 1, row);

            // Staggered slide-in: each row fades + slides left from +24px
            // Delay capped at 400ms total so large chapter counts don't drag
            const int delay = qMin(shown * 18, 400);
            row->setVisible(false);
            QTimer::singleShot(delay, row, [row] {
                if (!row) return;
                row->setVisible(true);

                auto* eff = new QGraphicsOpacityEffect(row);
                eff->setOpacity(0.0);
                row->setGraphicsEffect(eff);

                auto* fadeAnim = new QPropertyAnimation(eff, "opacity", row);
                fadeAnim->setDuration(160);
                fadeAnim->setStartValue(0.0);
                fadeAnim->setEndValue(1.0);
                fadeAnim->setEasingCurve(QEasingCurve::OutCubic);
                QObject::connect(fadeAnim, &QPropertyAnimation::finished, row, [row]{
                    if (row) row->setGraphicsEffect(nullptr);
                });
                fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);
            });

            ++shown;
        }

        m_dlStartBtn->setEnabled(shown > 0);
        m_dlStatusLbl->setText(
            m_dlChapters.isEmpty()
            ? "No downloadable chapters found."
            : QString("%1 chapters loaded (%2 shown). Select and press Download.").arg(m_dlChapters.size()).arg(shown));
        appendLog(QString("Chapter list loaded: %1 total, %2 shown.").arg(m_dlChapters.size()).arg(shown));
    }

    void onDlLangFilter() {
        populateChapterList("__filter__");  // non-empty sentinel → skip combo rebuild
    }

    void onDlStart() {
        if (m_dlRunning || m_dlMangaId.isEmpty()) return;

        // Collect selected chapter ids in display order
        m_dlQueue.clear();
        QMap<QString, ChapterInfo> ciById;
        for (const auto& ci : m_dlChapters) ciById[ci.id] = ci;

        for (auto* chk : m_dlChkBoxes) {
            if (chk->isChecked()) {
                const QString id = chk->property("chapterId").toString();
                if (!id.isEmpty()) m_dlQueue.append(id);
            }
        }

        if (m_dlQueue.isEmpty()) {
            m_dlStatusLbl->setText("No chapters selected.");
            return;
        }

        // Resolve download path
        QString basePath = m_dlPathEdit->text().trimmed();
        if (basePath.isEmpty()) basePath = m_outEdit->text().trimmed();
        if (basePath.isEmpty())
            basePath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        m_settings.setValue("download/dir", basePath);

        m_dlRunning = true;
        m_dlStop    = false;
        m_dlCurrent = 0;
        m_dlStartBtn->setEnabled(false);
        m_dlStopBtn->setEnabled(true);
        m_dlProgress->setValueInstant(0);
        appendLog(QString("Starting download: %1 chapters for \"%2\"").arg(m_dlQueue.size()).arg(m_dlMangaTitleStr));
        downloadNextChapter(basePath, ciById);
    }

    void downloadNextChapter(const QString& basePath, const QMap<QString,ChapterInfo>& ciById) {
        if (m_dlStop || m_dlCurrent >= m_dlQueue.size()) {
            m_dlRunning = false;
            m_dlStartBtn->setEnabled(!m_dlChapters.isEmpty());
            m_dlStopBtn->setEnabled(false);
            m_dlProgress->setValue(100);
            if (m_dlStop)
                m_dlStatusLbl->setText("Stopped.");
            else {
                m_dlStatusLbl->setText(QString("Done. %1 chapters downloaded.").arg(m_dlCurrent));
                appendLog(QString("Download complete: %1 chapters.").arg(m_dlCurrent));
                QDesktopServices::openUrl(QUrl::fromLocalFile(
                    basePath + "/" + sanitizeName(m_dlMangaTitleStr)));
            }
            return;
        }

        const QString chId = m_dlQueue.at(m_dlCurrent);
        const ChapterInfo ci = ciById.value(chId);

        const int pct = m_dlCurrent * 100 / m_dlQueue.size();
        m_dlProgress->setValue(pct);
        m_dlStatusLbl->setText(QString("[%1/%2] Fetching page list for Ch.%3…")
            .arg(m_dlCurrent + 1).arg(m_dlQueue.size())
            .arg(ci.chapter.isEmpty() ? "Oneshot" : ci.chapter));

        // Fetch at-home server URL for the chapter
        QUrl url(QString("https://api.mangadex.org/at-home/server/") + chId);
        auto* reply = apiGet(url);
        connect(reply, &QNetworkReply::finished, this,
                [this, reply, basePath, ci, ciById] {
            reply->deleteLater();
            if (reply->error() != QNetworkReply::NoError) {
                appendLog(QString("Ch.%1: server lookup failed — %2").arg(ci.chapter, reply->errorString()));
                ++m_dlCurrent;
                QTimer::singleShot(500, this, [this, basePath, ciById]{ downloadNextChapter(basePath, ciById); });
                return;
            }
            const auto doc    = QJsonDocument::fromJson(reply->readAll());
            const QString host = doc["baseUrl"].toString();
            const auto chapter = doc["chapter"].toObject();
            const QString hash = chapter["hash"].toString();
            const auto pagesArr = chapter["data"].toArray();

            QStringList pages;
            for (const auto& p : pagesArr) pages << p.toString();

            if (pages.isEmpty()) {
                appendLog(QString("Ch.%1: no pages returned.").arg(ci.chapter));
                ++m_dlCurrent;
                QTimer::singleShot(500, this, [this, basePath, ciById]{ downloadNextChapter(basePath, ciById); });
                return;
            }

            // Build chapter folder: <basePath>/<Manga Title>/<Ch folder>/
            const QString mangaDir   = basePath + "/" + sanitizeName(m_dlMangaTitleStr);
            const QString chapterDir = mangaDir  + "/" + chapterFolderName(ci);
            QDir().mkpath(chapterDir);

            appendLog(QString("Ch.%1: downloading %2 pages → %3")
                .arg(ci.chapter.isEmpty() ? "Oneshot" : ci.chapter)
                .arg(pages.size())
                .arg(chapterDir));

            downloadPages(host, hash, pages, chapterDir, 0, basePath, ci, ciById);
        });
    }

    void downloadPages(const QString& host, const QString& hash,
                       const QStringList& pages, const QString& dir,
                       int pageIdx, const QString& basePath,
                       const ChapterInfo& ci,
                       const QMap<QString,ChapterInfo>& ciById)
    {
        if (m_dlStop || pageIdx >= pages.size()) {
            ++m_dlCurrent;
            const int pct = m_dlCurrent * 100 / m_dlQueue.size();
            m_dlProgress->setValue(pct);
            // 1.5s cooldown between chapters — MangaDex rate-limit courtesy
            QTimer::singleShot(1500, this, [this, basePath, ciById]{
                downloadNextChapter(basePath, ciById);
            });
            return;
        }

        const QString pageFile = pages.at(pageIdx);
        const QString pageUrl  = host + "/data/" + hash + "/" + pageFile;
        const QString ext      = pageFile.contains('.') ? pageFile.mid(pageFile.lastIndexOf('.')) : ".jpg";
        const QString savePath = dir + "/" + QString("%1").arg(pageIdx + 1, 3, 10, QChar('0')) + ext;

        // Skip if already downloaded
        if (QFile::exists(savePath)) {
            m_dlStatusLbl->setText(QString("[%1/%2] Ch.%3 — page %4/%5 (cached)")
                .arg(m_dlCurrent + 1).arg(m_dlQueue.size())
                .arg(ci.chapter.isEmpty() ? "Oneshot" : ci.chapter)
                .arg(pageIdx + 1).arg(pages.size()));
            downloadPages(host, hash, pages, dir, pageIdx + 1, basePath, ci, ciById);
            return;
        }

        m_dlStatusLbl->setText(QString("[%1/%2] Ch.%3 — page %4/%5")
            .arg(m_dlCurrent + 1).arg(m_dlQueue.size())
            .arg(ci.chapter.isEmpty() ? "Oneshot" : ci.chapter)
            .arg(pageIdx + 1).arg(pages.size()));

        QUrl pageQUrl(pageUrl);
        QNetworkRequest req{pageQUrl};
        req.setRawHeader("User-Agent", UA);
        req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
        auto* reply = m_nam->get(req);
        connect(reply, &QNetworkReply::finished, this,
                [this, reply, savePath, host, hash, pages, dir, pageIdx, basePath, ci, ciById] {
            reply->deleteLater();
            if (reply->error() == QNetworkReply::NoError) {
                QFile f(savePath);
                if (f.open(QIODevice::WriteOnly)) f.write(reply->readAll());
            } else {
                appendLog(QString("Page %1 failed: %2").arg(pageIdx + 1).arg(reply->errorString()));
            }
            // 250ms between pages — MangaDex requests at least 40ms; 250 is safe
            QTimer::singleShot(250, this, [this, host, hash, pages, dir, pageIdx, basePath, ci, ciById]{
                downloadPages(host, hash, pages, dir, pageIdx + 1, basePath, ci, ciById);
            });
        });
    }

    QWidget* buildLogsTab() {
        auto* page = new QWidget;
        auto* v    = new QVBoxLayout(page);
        v->setContentsMargins(0, 10, 0, 0);
        v->setSpacing(10);

        auto* logCard = new Card(page);
        auto* ll = new QVBoxLayout(logCard);
        ll->setContentsMargins(12, 10, 12, 12);
        ll->setSpacing(6);

        auto* hdr = new QHBoxLayout;
        auto* logLabel = new QLabel("Activity Log", logCard);
        logLabel->setStyleSheet(QString("QLabel { background: transparent; color: %1; font-size: 13px; font-weight: 700; }").arg(Pal::TEXT));
        auto* clearBtn = new GhostButton("Clear", logCard);
        clearBtn->setFixedSize(58, 26);
        connect(clearBtn, &QPushButton::clicked, [this]{ m_log->clear(); });
        hdr->addWidget(logLabel);
        hdr->addStretch();
        hdr->addWidget(clearBtn);
        ll->addLayout(hdr);

        m_log = new QTextEdit(logCard);
        m_log->setReadOnly(true);
        m_log->setMinimumHeight(120);
        ll->addWidget(m_log, 1);
        v->addWidget(logCard, 1);

        return page;
    }

    // ── Networking helpers ────────────────────────────────────────────────────

    QNetworkReply* apiGet(const QUrl& url) {
        QNetworkRequest req(url);
        req.setRawHeader("User-Agent", UA);
        if (!m_accessToken.isEmpty())
            req.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
        req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
        return m_nam->get(req);
    }

    void tokenGrant(const QUrlQuery& form, bool silent) {
        QNetworkRequest req{QUrl(TOKEN_URL)};
        req.setRawHeader("User-Agent", UA);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        auto* reply = m_nam->post(req, form.toString(QUrl::FullyEncoded).toUtf8());
        connect(reply, &QNetworkReply::finished, this, [this, reply, silent] {
            const auto doc = QJsonDocument::fromJson(reply->readAll());
            reply->deleteLater();
            m_loginBtn->setEnabled(true);

            const QString access  = doc["access_token"].toString();
            const QString refresh = doc["refresh_token"].toString();
            const int     expires = doc["expires_in"].toInt(900);

            if (access.isEmpty()) {
                const QString why = doc["error_description"].toString(doc["error"].toString());
                m_authBadge->setFail("Sign-in failed");
                appendLog("Sign-in failed: " + (why.isEmpty() ? "no response from auth server" : why));
                if (!silent)
                    QMessageBox::warning(this, "Sign-in failed",
                        why.isEmpty() ? "Could not sign in. Check username/password." : why);
                else
                    showAuthUI();   // silent auto-reconnect failed → show login form
                return;
            }
            onAuthSuccess(access, refresh, expires);
        });
    }

    // ── Auth flow ─────────────────────────────────────────────────────────────

    void tryAutoConnect() {
        const QString savedRefresh = m_settings.value("auth/refresh_token").toString();
        const QString savedAccess  = m_settings.value("auth/access_token").toString();
        m_userEdit->setText(m_settings.value("auth/username").toString());

        // Load persisted API credentials before using them in any token grant.
        // m_clientId/m_clientSecret are initialised to the hardcoded constants at
        // construction; syncCredentials() hasn't run yet at this point, so without
        // this block the refresh-token request would use stale/wrong credentials,
        // get rejected by the auth server, and fall back to showing the login form
        // on every cold start even when a valid saved session exists.
        // Reload credentials (empty string if never saved)
        m_clientId     = m_settings.value("api/client_id",     "").toString();
        m_clientSecret = m_settings.value("api/client_secret", "").toString();
        // Mirror into the UI fields so the user sees what's actually being used.
        if (m_clientIdEdit)  m_clientIdEdit->setText(m_clientId);
        if (m_clientSecEdit) m_clientSecEdit->setText(m_clientSecret);

        if (!savedRefresh.isEmpty()) {
            m_authBadge->setNeutral("Reconnecting…");
            appendLog("Found saved session — reconnecting…");
            m_refreshToken = savedRefresh;
            QUrlQuery q;
            q.addQueryItem("grant_type",    "refresh_token");
            q.addQueryItem("refresh_token", m_refreshToken);
            q.addQueryItem("client_id",     m_clientId);
            q.addQueryItem("client_secret", m_clientSecret);
            tokenGrant(q, /*silent*/true);
        } else if (!savedAccess.isEmpty()) {
            m_authBadge->setNeutral("Reconnecting…");
            m_accessToken = savedAccess;
            validateToken(/*silent*/true);
        }
    }

    void syncCredentials() {
        if (m_clientIdEdit)  m_clientId     = m_clientIdEdit->text().trimmed();
        if (m_clientSecEdit) m_clientSecret = m_clientSecEdit->text().trimmed();
        if (m_rememberCreds) {
            m_settings.setValue("api/client_id",     m_clientId);
            m_settings.setValue("api/client_secret", m_clientSecret);
        } else {
            m_settings.remove("api/client_id");
            m_settings.remove("api/client_secret");
        }
    }

    void onRememberCredsToggled(bool on) {
        m_rememberCreds = on;
        m_settings.setValue("api/remember_creds", on);
        if (on) {
            // Persist whatever is currently in the fields right away.
            syncCredentials();
            appendLog("API credentials will be remembered on this device.");
        } else {
            // Forget immediately — don't wait for the next login attempt.
            m_settings.remove("api/client_id");
            m_settings.remove("api/client_secret");
            appendLog("API credentials will not be saved on this device.");
        }
    }

    void onClearCredentials() {
        m_settings.remove("api/client_id");
        m_settings.remove("api/client_secret");
        m_clientId.clear();
        m_clientSecret.clear();
        if (m_clientIdEdit)  { m_clientIdEdit->clear();  m_clientIdEdit->setPlaceholderText("API Client ID (personal-client-…)"); }
        if (m_clientSecEdit) { m_clientSecEdit->clear(); m_clientSecEdit->setPlaceholderText("API Secret Key"); }
        appendLog("API credentials cleared. Enter your own client id & secret.");
    }

    void onLogin() {
        const QString user = m_userEdit->text().trimmed();
        const QString pass = m_passEdit->text();
        if (user.isEmpty() || pass.isEmpty()) {
            m_authBadge->setFail("Enter username & password");
            return;
        }
        syncCredentials();
        if (m_clientId.isEmpty() || m_clientSecret.isEmpty()) {
            m_authBadge->setFail("Enter API client id & secret");
            return;
        }
        m_loginBtn->setEnabled(false);
        m_authBadge->setNeutral("Signing in…");

        QUrlQuery q;
        q.addQueryItem("grant_type",    "password");
        q.addQueryItem("username",      user);
        q.addQueryItem("password",      pass);
        q.addQueryItem("client_id",     m_clientId);
        q.addQueryItem("client_secret", m_clientSecret);
        tokenGrant(q, /*silent*/false);
    }

    void onTokenConnect() {
        const QString token = m_tokenEdit->text().trimmed();
        if (token.isEmpty()) { m_authBadge->setFail("Paste a token first"); return; }
        m_accessToken  = token;
        m_refreshToken.clear();
        m_authBadge->setNeutral("Validating…");
        validateToken(/*silent*/false);
    }

    void validateToken(bool silent) {
        auto* reply = apiGet(QUrl(QString(API_BASE) + "/auth/check"));
        connect(reply, &QNetworkReply::finished, this, [this, reply, silent] {
            const auto doc = QJsonDocument::fromJson(reply->readAll());
            const bool ok  = reply->error() == QNetworkReply::NoError
                             && doc["isAuthenticated"].toBool();
            reply->deleteLater();
            if (!ok) {
                m_accessToken.clear();
                m_authBadge->setFail(silent ? "Session expired" : "Token rejected");
                appendLog(silent ? "Saved session expired — please sign in again."
                                 : "Token not authenticated — grab a fresh one.");
                showAuthUI();
                return;
            }
            onAuthSuccess(m_accessToken, m_refreshToken, 890);
        });
    }

    void onAuthSuccess(const QString& access, const QString& refresh, int expiresSec) {
        m_accessToken  = access;
        m_refreshToken = refresh;

        // Persist session
        const bool stay = !m_stayChk || m_stayChk->isChecked();
        if (stay) {
            m_settings.setValue("auth/access_token",  m_accessToken);
            m_settings.setValue("auth/refresh_token", m_refreshToken);
            m_settings.setValue("auth/username",      m_userEdit->text().trimmed());
        }

        // Auto-refresh a minute before expiry so we stay connected
        if (!m_refreshToken.isEmpty())
            m_refreshTimer->start(qMax(60, expiresSec - 60) * 1000);

        m_passEdit->clear();
        m_authBadge->setOk("✓ Connected");
        m_authCard->hide();
        m_connectedBar->show();
        m_connectedLbl->setText("Signed in — session will refresh automatically");
        m_tabs->setEnabled(true);
        setDownloadInfoVisible(true);
        appendLog("Connected to MangaDex. Press \"Load Library\" to fetch your titles.");

        fetchUserName();
        if (m_libraryOrder.isEmpty())
            m_statsLbl->setText("Connected. Press \"Load Library\" to fetch your titles.");
    }

    void doRefresh() {
        if (m_refreshToken.isEmpty()) return;
        QUrlQuery q;
        q.addQueryItem("grant_type",    "refresh_token");
        q.addQueryItem("refresh_token", m_refreshToken);
        q.addQueryItem("client_id",     m_clientId);
        q.addQueryItem("client_secret", m_clientSecret);

        QNetworkRequest req{QUrl(TOKEN_URL)};
        req.setRawHeader("User-Agent", UA);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        auto* reply = m_nam->post(req, q.toString(QUrl::FullyEncoded).toUtf8());
        connect(reply, &QNetworkReply::finished, this, [this, reply] {
            const auto doc = QJsonDocument::fromJson(reply->readAll());
            reply->deleteLater();
            const QString access = doc["access_token"].toString();
            if (access.isEmpty()) {
                m_authBadge->setFail("Session expired");
                appendLog("Token refresh failed — sign in again.");
                showAuthUI();
                return;
            }
            m_accessToken = access;
            const QString newRefresh = doc["refresh_token"].toString();
            if (!newRefresh.isEmpty()) m_refreshToken = newRefresh;
            if (!m_stayChk || m_stayChk->isChecked()) {
                m_settings.setValue("auth/access_token",  m_accessToken);
                m_settings.setValue("auth/refresh_token", m_refreshToken);
            }
            m_refreshTimer->start(qMax(60, doc["expires_in"].toInt(900) - 60) * 1000);
            appendLog("Session refreshed.");
        });
    }

    void fetchUserName() {
        auto* reply = apiGet(QUrl(QString(API_BASE) + "/user/me"));
        connect(reply, &QNetworkReply::finished, this, [this, reply] {
            const auto doc = QJsonDocument::fromJson(reply->readAll());
            reply->deleteLater();
            const QString name = doc["data"].toObject()["attributes"].toObject()["username"].toString();
            if (!name.isEmpty()) {
                m_username = name;
                m_connectedLbl->setText("Signed in as  " + name + "  —  session refreshes automatically");
                m_authBadge->setOk("✓ " + name);
            }
        });
    }

    void onLogout() {
        m_refreshTimer->stop();
        m_accessToken.clear();
        m_refreshToken.clear();
        m_username.clear();
        m_settings.remove("auth/access_token");
        m_settings.remove("auth/refresh_token");

        clearCards(m_libCards,    m_grid);
        m_entries.clear();
        m_libraryOrder.clear();
        m_selected.clear();
        m_statusMap.clear();
        m_selectionUndo.clear();
        if (m_undoBtn) m_undoBtn->setEnabled(false);
        updateSelectionUi();
        m_statsLbl->setText("Sign in to load your library.");
        m_countLbl->clear();
        updateLibraryStackVisibility();

        showAuthUI();
        appendLog("Signed out.");
    }

    void showAboutDialog() {
        QMessageBox box(this);
        box.setWindowTitle("About HitPaw MangaDex Manager");
        box.setIconPixmap(QPixmap(":/icons/icon_64.png").scaled(
            56, 56, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        box.setTextFormat(Qt::RichText);
        box.setText(QString(
            "<b>HitPaw MangaDex Manager</b><br>"
            "Version %1<br><br>"
            "Library size: %2 title(s) loaded<br>"
            "Cover cache: %3<br><br>"
            "<span style='color:%4'>Not affiliated with MangaDex.</span>")
            .arg(QApplication::applicationVersion())
            .arg(m_entries.size())
            .arg(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/covers")
            .arg(Pal::MUTED));
        box.setStandardButtons(QMessageBox::Ok);
        box.exec();
    }

    void showAuthUI() {
        m_authBadge->setNeutral("Not connected");
        m_connectedBar->hide();
        m_authCard->show();
        m_authStack->setCurrentIndex(0);   // always land on the username/password page, not the token page
        m_tabs->setEnabled(false);
        setDownloadInfoVisible(false);
    }

    // Signed-out state hides the Author/Artist/Genres/Demographic block on the
    // Download tab entirely (heading + "—" placeholder), rather than showing
    // empty-looking labels for a tab the user can't even interact with yet.
    void setDownloadInfoVisible(bool visible) {
        for (auto* l : m_dlInfoLabels)
            if (l) l->setVisible(visible);
    }

    // ── Library fetch ─────────────────────────────────────────────────────────

    void setFetchingUi(bool on) {
        m_fetching = on;
        if (m_loadBtn) m_loadBtn->setEnabled(!on);
        if (m_stopBtn) m_stopBtn->setEnabled(on);
    }

    void stopLibraryFetch() {
        if (!m_fetching) return;
        m_stopRequested = true;
        if (m_curReply) m_curReply->abort();
        appendLog("Stopping — finishing with titles loaded so far…");
    }

    void startLibraryFetch() {
        if (m_fetching || m_accessToken.isEmpty()) return;
        m_stopRequested = false;
        setFetchingUi(true);
        m_libProgress->setValueInstant(0);
        m_libProgress->show();
        m_statsLbl->setText("Fetching library statuses…");
        appendLog("Fetching library…");

        auto* reply = apiGet(QUrl(QString(API_BASE) + "/manga/status"));
        m_curReply = reply;
        connect(reply, &QNetworkReply::finished, this, [this, reply] {
            m_curReply = nullptr;
            if (m_stopRequested) { reply->deleteLater(); finishFetchStopped(); return; }
            if (reply->error() != QNetworkReply::NoError) {
                finishFetchError("Status fetch failed: " + reply->errorString());
                reply->deleteLater();
                return;
            }
            const auto doc = QJsonDocument::fromJson(reply->readAll());
            reply->deleteLater();

            m_statusMap.clear();
            const auto obj = doc["statuses"].toObject();
            for (auto it = obj.begin(); it != obj.end(); ++it)
                m_statusMap[it.key()] = it.value().toString();

            if (m_statusMap.isEmpty()) {
                m_statsLbl->setText("Your library is empty.");
                appendLog("Library is empty.");
                setFetchingUi(false);
                m_libProgress->hide();
                return;
            }

            // Reset library state, keep selection
            clearCards(m_libCards, m_grid);
            m_libraryOrder.clear();
            m_allIds = m_statusMap.keys();
            m_statsLbl->setText(QString("Found %1 titles. Loading details…").arg(m_allIds.size()));
            appendLog(QString("Found %1 titles. Resolving details & covers…").arg(m_allIds.size()));
            processBatch(0);
        });
    }

    void processBatch(int b) {
        if (m_stopRequested) { finishFetchStopped(); return; }
        const int BATCH = 100;
        const int totalBatches = (m_allIds.size() + BATCH - 1) / BATCH;
        if (b >= totalBatches) { finishLibrary(); return; }

        const QStringList batch = m_allIds.mid(b * BATCH, BATCH);
        QUrl url(QString(API_BASE) + "/manga");
        QUrlQuery q;
        for (const auto& id : batch) q.addQueryItem("ids[]", id);
        q.addQueryItem("limit", QString::number(batch.size()));
        q.addQueryItem("includes[]", "author");
        q.addQueryItem("includes[]", "cover_art");
        for (const char* cr : {"safe","suggestive","erotica","pornographic"})
            q.addQueryItem("contentRating[]", cr);
        url.setQuery(q);

        auto* reply = apiGet(url);
        m_curReply = reply;
        connect(reply, &QNetworkReply::finished, this,
                [this, reply, b, batch, totalBatches] {
            m_curReply = nullptr;
            if (m_stopRequested) { reply->deleteLater(); finishFetchStopped(); return; }
            if (reply->error() != QNetworkReply::NoError) {
                finishFetchError("Batch fetch failed: " + reply->errorString());
                reply->deleteLater();
                return;
            }
            const auto doc  = QJsonDocument::fromJson(reply->readAll());
            const auto data = doc["data"].toArray();
            reply->deleteLater();

            QSet<QString> returned;
            for (const auto& item : data) {
                const auto obj = item.toObject();
                const QString id = obj["id"].toString();
                const auto e = parseManga(obj, m_statusMap.value(id, "unknown"));
                m_entries[id] = e;
                m_libraryOrder << id;
                returned.insert(id);
            }
            // Entries MangaDex no longer serves
            for (const auto& id : batch) {
                if (!returned.contains(id)) {
                    MangaEntry e;
                    e.id          = id;
                    e.title       = "[Unavailable]";
                    e.status      = m_statusMap.value(id, "unknown");
                    e.statusLabel = STATUS_LABELS.value(e.status, "Unknown");
                    e.url         = "https://mangadex.org/title/" + id;
                    m_entries[id] = e;
                    m_libraryOrder << id;
                }
            }

            m_libProgress->setValue((b + 1) * 100 / totalBatches);
            m_statsLbl->setText(QString("Loading details…  batch %1 / %2").arg(b + 1).arg(totalBatches));

            // 1 second between batches — keeps the API load low for large libraries
            QTimer::singleShot(1000, this, [this, b] { processBatch(b + 1); });
        });
    }

    void finishFetchError(const QString& msg) {
        setFetchingUi(false);
        m_libProgress->hide();
        m_statsLbl->setText(msg);
        appendLog("Error: " + msg);
    }

    void finishFetchStopped() {
        m_stopRequested = false;
        m_libProgress->hide();
        appendLog(QString("Stopped. %1 titles loaded.").arg(m_libraryOrder.size()));
        // Render whatever we managed to fetch
        finishLibrary();
    }

    void finishLibrary() {
        setFetchingUi(false);
        m_libProgress->hide();

        // Sort alphabetically for the grid
        std::sort(m_libraryOrder.begin(), m_libraryOrder.end(),
                  [this](const QString& a, const QString& b) {
                      return m_entries[a].title.localeAwareCompare(m_entries[b].title) < 0;
                  });

        updateStats();
        updateLibraryStackVisibility();
        appendLog(QString("Library loaded: %1 titles.").arg(m_libraryOrder.size()));

        // Build cards in small chunks — creating hundreds of widgets in one
        // pass froze the UI and caused flicker right after fetching finished.
        buildCardsChunk(0);
    }

    void buildCardsChunk(int from) {
        if (m_fetching) return;   // a new fetch started — abandon this stale chain
        constexpr int CHUNK = 30;
        const int to = qMin(from + CHUNK, static_cast<int>(m_libraryOrder.size()));
        for (int i = from; i < to; ++i) {
            const QString& id = m_libraryOrder.at(i);
            // Parent to the grid host immediately so the card can never
            // become a floating top-level window.
            auto* card = new MangaCard(m_entries[id], /*small*/false, m_grid->parentWidget());
            card->hide();                       // shown by appendCardsToGrid()
            card->setSelected(m_selected.contains(id));
            connect(card, &MangaCard::toggled, this, &MainWindow::onCardToggled);
            m_libCards << card;
        }
        // Only lay out the cards this chunk just created — relayoutLibrary()
        // used to run here and re-add every card built so far on every single
        // chunk, which is O(n^2) over the whole load (a 3000+ title library
        // meant tens of thousands of redundant addWidget calls and got visibly
        // slower as it progressed). Appending just the new range keeps each
        // chunk's cost proportional to CHUNK, not to how much has loaded.
        appendCardsToGrid(from, to);
        if (to < m_libraryOrder.size())
            QTimer::singleShot(0, this, [this, to] { buildCardsChunk(to); });
        else
            updateSelectionUi();
    }

    void updateStats() {
        QMap<QString,int> counts;
        for (const auto& id : m_libraryOrder) counts[m_entries[id].statusLabel]++;
        QStringList parts;
        parts << QString("Total %1").arg(m_libraryOrder.size());
        QList<QPair<QString,int>> sorted;
        for (auto it = counts.begin(); it != counts.end(); ++it)
            sorted.append({it.key(), it.value()});
        std::sort(sorted.begin(), sorted.end(),
                  [](const auto& a, const auto& b){ return a.second > b.second; });
        for (const auto& [k, v] : sorted) parts << QString("%1 %2").arg(k).arg(v);
        m_statsLbl->setText(parts.join("   ·   "));
    }

    // ── Grid / selection plumbing ─────────────────────────────────────────────

    void clearCards(QList<MangaCard*>& cards, QGridLayout* grid) {
        while (grid->count() > 0) {
            auto* item = grid->takeAt(0);
            delete item;
        }
        for (auto* c : cards) c->deleteLater();
        cards.clear();
        if (grid == m_grid) m_gridPlaced = 0;   // grid cells are empty again — resume placement at 0
    }

    bool cardMatchesFilter(const MangaCard* c) const {
        const QString st = m_filterStatus->currentData().toString();
        if (!st.isEmpty() && c->entry().status != st) return false;
        const QString needle = m_filterEdit->text().trimmed();
        if (!needle.isEmpty()
            && !c->entry().title.contains(needle, Qt::CaseInsensitive)
            && !c->entry().authors.contains(needle, Qt::CaseInsensitive))
            return false;
        return true;
    }

    void relayoutLibrary() {
        auto* host = m_grid->parentWidget();
        if (host) host->setUpdatesEnabled(false);   // no repaints mid-shuffle
        while (m_grid->count() > 0) {
            auto* item = m_grid->takeAt(0);
            delete item;
        }
        int i = 0, shown = 0;
        for (auto* c : m_libCards) {
            if (cardMatchesFilter(c)) {
                // Add to the grid BEFORE showing — showing a parentless widget
                // turns it into its own top-level window (blank popup bug).
                m_grid->addWidget(c, i / GRID_COLS, i % GRID_COLS);
                c->setVisible(true);
                ++i; ++shown;
            } else {
                c->setVisible(false);
            }
        }
        if (host) host->setUpdatesEnabled(true);
        m_gridPlaced = shown;   // keep in sync so buildCardsChunk's appends resume from the right cell
        m_countLbl->setText(QString("%1 shown  ·  %2 selected").arg(shown).arg(m_selected.size()));
        // Zero matches, but the library itself has titles (as opposed to
        // nothing loaded yet, which is m_libraryStack's own empty state).
        if (m_noResultsLbl) m_noResultsLbl->setVisible(shown == 0 && !m_libCards.isEmpty());
    }

    // Lays out only the freshly-created cards in m_libCards[from, to) at the
    // next free grid cells, instead of re-adding every card built so far.
    // Used while a library is still streaming in during buildCardsChunk();
    // a filter/search change still goes through the full relayoutLibrary().
    void appendCardsToGrid(int from, int to) {
        auto* host = m_grid->parentWidget();
        if (host) host->setUpdatesEnabled(false);
        for (int idx = from; idx < to; ++idx) {
            auto* c = m_libCards.at(idx);
            if (cardMatchesFilter(c)) {
                m_grid->addWidget(c, m_gridPlaced / GRID_COLS, m_gridPlaced % GRID_COLS);
                c->setVisible(true);
                ++m_gridPlaced;
            } else {
                c->setVisible(false);
            }
        }
        if (host) host->setUpdatesEnabled(true);
        m_countLbl->setText(QString("%1 shown  ·  %2 selected").arg(m_gridPlaced).arg(m_selected.size()));
        if (m_noResultsLbl) m_noResultsLbl->setVisible(m_gridPlaced == 0 && !m_libCards.isEmpty());
    }

    static constexpr int UNDO_MAX = 50;

    void pushUndoSnapshot() {
        m_selectionUndo.append(m_selected);
        if (m_selectionUndo.size() > UNDO_MAX) m_selectionUndo.removeFirst();
        if (m_undoBtn) m_undoBtn->setEnabled(true);
    }

    void undoSelection() {
        if (m_selectionUndo.isEmpty()) return;
        const QSet<QString> prev = m_selectionUndo.takeLast();
        m_selected = prev;
        for (auto* c : m_libCards) c->setSelected(m_selected.contains(c->id()));
        updateSelectionUi();
        if (m_undoBtn) m_undoBtn->setEnabled(!m_selectionUndo.isEmpty());
        appendLog("Undid last selection change.");
    }

    void onCardToggled(const QString& id, bool sel) {
        pushUndoSnapshot();
        if (sel) m_selected.insert(id); else m_selected.remove(id);
        // The toggling card is the signal sender — no need to scan every
        // card in the library to find it by id.
        if (auto* c = qobject_cast<MangaCard*>(sender())) c->setSelected(sel);
        updateSelectionUi();
    }

    void selectVisible(bool sel) {
        pushUndoSnapshot();
        for (auto* c : m_libCards) {
            if (!cardMatchesFilter(c)) continue;
            if (sel) m_selected.insert(c->id()); else m_selected.remove(c->id());
            c->setSelected(sel);
        }
        updateSelectionUi();
    }

    void clearSelection() {
        pushUndoSnapshot();
        m_selected.clear();
        for (auto* c : m_libCards) c->setSelected(false);
        updateSelectionUi();
    }

    void updateSelectionUi() {
        const int n = m_selected.size();
        if (m_exportSelBtn) {
            m_exportSelBtn->setEnabled(n > 0);
            m_exportSelBtn->setText(n > 0 ? QString("Export Selected (%1)").arg(n)
                                           : "Export Selected");
        }
        if (m_selInfo) {
            m_selInfo->setText(n == 0
                ? "Nothing selected — pick titles in Library to enable Export Selected."
                : QString("%1 title(s) selected. Use Library → Clear (or Undo) to change the selection.").arg(n));
        }
        if (m_countLbl) {
            int shown = 0;
            for (auto* c : m_libCards) if (!c->isHidden()) ++shown;
            m_countLbl->setText(QString("%1 shown  ·  %2 selected").arg(shown).arg(n));
        }
    }

    // ── Export ────────────────────────────────────────────────────────────────

    void onBrowse() {
        const QString dir = QFileDialog::getExistingDirectory(
            this, "Select Output Folder", m_outEdit->text());
        if (!dir.isEmpty()) m_outEdit->setText(dir);
    }

    void onExport(bool selectedOnly) {
        if (!m_chkCSV->isChecked() && !m_chkJSON->isChecked() && !m_chkMAL->isChecked() && !m_chkAP->isChecked() && !m_chkMB->isChecked()) {
            QMessageBox::warning(this, "Error", "Select at least one export format.");
            return;
        }

        if (selectedOnly && m_selected.isEmpty()) {
            QMessageBox::warning(this, "Nothing selected",
                "No titles are selected. Pick some in Library, or use Export Entire Library instead.");
            return;
        }

        QList<MangaEntry> list;
        if (!selectedOnly) {
            for (const auto& id : m_libraryOrder) list << m_entries[id];
        } else {
            for (const auto& id : m_libraryOrder)
                if (m_selected.contains(id)) list << m_entries[id];
        }

        if (list.isEmpty()) {
            QMessageBox::warning(this, "Nothing to export",
                "The library hasn't loaded yet and nothing is selected.");
            return;
        }

        const QString outDir = m_outEdit->text().trimmed();
        QDir().mkpath(outDir);
        m_settings.setValue("export/dir",  outDir);
        m_settings.setValue("export/csv",  m_chkCSV->isChecked());
        m_settings.setValue("export/json", m_chkJSON->isChecked());
        m_settings.setValue("export/mal",  m_chkMAL->isChecked());
        m_settings.setValue("export/ap",   m_chkAP->isChecked());
        m_settings.setValue("export/mb",   m_chkMB->isChecked());

        QStringList saved;
        if (m_chkCSV->isChecked()) {
            const QString p = outDir + "/mangadex_library.csv";
            Export::toCSV(list, p); saved << p;
        }
        if (m_chkJSON->isChecked()) {
            const QString p = outDir + "/mangadex_library.json";
            Export::toJSON(list, p); saved << p;
        }
        if (m_chkMAL->isChecked()) {
            const QString p = outDir + "/mangadex_library_MAL.xml";
            Export::toMALXML(list, p); saved << p;
        }
        if (m_chkAP->isChecked()) {
            const QString p = outDir + "/mangadex_library_AP.xml.gz";
            Export::toAnimeplanetGZ(list, p); saved << p;
        }
        if (m_chkMB->isChecked()) {
            const QString p = outDir + "/mangadex_library_MangaBaka.json";
            Export::toMangaBakaJSON(list, p); saved << p;
        }

        QMap<QString,int> counts;
        for (const auto& e : list) counts[e.statusLabel]++;
        QString summary = QString("Exported %1 title(s):").arg(list.size());
        for (auto it = counts.begin(); it != counts.end(); ++it)
            summary += QString("  %1 %2 ·").arg(it.value()).arg(it.key());
        if (summary.endsWith(" ·")) summary.chop(2);
        appendLog(summary);
        for (const auto& p : saved) appendLog("Saved: " + p);

        QMessageBox::information(this, "Export complete",
            QString("Exported %1 title(s).\n\nFiles saved to:\n%2").arg(list.size()).arg(outDir));
    }

    // ── Logging ───────────────────────────────────────────────────────────────

    // Picks a color for a log line based on its content, so errors, warnings,
    // and successes are scannable at a glance instead of one flat text color.
    static QString logColorFor(const QString& msg) {
        const QString m = msg.toLower();
        if (m.contains("failed") || m.contains("error") || m.contains("expired")
            || m.contains("could not") || m.contains("no pages returned"))
            return Pal::RED;
        if (m.contains("signed out") || m.contains("stopping") || m.contains("stopped")
            || m.contains("cleared") || m.contains("not be saved"))
            return Pal::ACCENT_H;
        if (m.contains("loaded") || m.contains("connected") || m.contains("complete")
            || m.contains("done") || m.contains("refreshed") || m.contains("found")
            || m.contains("remembered") || m.contains("saved:"))
            return Pal::GREEN;
        return Pal::TEXT;
    }

    void appendLog(const QString& msg) {
        if (!m_log) return;
        const QString ts = QDateTime::currentDateTime().toString("HH:mm:ss");
        m_log->append(QString("<span style='color:%1'>[%2]</span> "
                               "<span style='color:%3'>%4</span>")
                      .arg(Pal::MUTED, ts, logColorFor(msg), msg.toHtmlEscaped()));

        // Smooth auto-scroll to bottom instead of an instant snap — animates
        // the scrollbar value over 220ms so a burst of log lines glides
        // instead of jittering the view on every append.
        auto* bar = m_log->verticalScrollBar();
        if (!m_logScrollAnim) {
            m_logScrollAnim = new QPropertyAnimation(bar, "value", this);
            m_logScrollAnim->setEasingCurve(QEasingCurve::OutCubic);
        }
        m_logScrollAnim->stop();
        m_logScrollAnim->setDuration(220);
        m_logScrollAnim->setStartValue(bar->value());
        m_logScrollAnim->setEndValue(bar->maximum());
        m_logScrollAnim->start();
    }
};

// ── Crash/warning log — writes qWarning/qCritical/qFatal to a file in AppData ──
// so a bug report from someone else's machine isn't a total black box. Debug
// messages are skipped to keep the file from ballooning; qDebug still goes to
// the console as normal during development.
static void fileMessageHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg) {
    if (type == QtDebugMsg) return;
    static QFile logFile;
    if (!logFile.isOpen()) {
        const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(dir);
        logFile.setFileName(dir + "/crash.log");
        if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
            fprintf(stderr, "%s\n", qPrintable(msg));   // can't log to file — at least don't lose the message
            return;
        }
    }
    const char* level = type == QtWarningMsg ? "WARN" : type == QtCriticalMsg ? "CRIT"
                       : type == QtFatalMsg  ? "FATAL" : "INFO";
    QTextStream ts(&logFile);
    ts << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << " [" << level << "] "
       << msg << " (" << ctx.file << ":" << ctx.line << ")\n";
    ts.flush();
    fprintf(stderr, "%s\n", qPrintable(msg));   // still show it during development
}

// ── Entry point ───────────────────────────────────────────────────────────────

int main(int argc, char* argv[]) {
    qInstallMessageHandler(fileMessageHandler);

    QApplication app(argc, argv);
    app.setApplicationName("HitPaw MangaDex Manager");
    app.setApplicationVersion("3.1.0");
    app.setOrganizationName("SentinelFlow");

    // Single-instance guard — a second launch would point at the same
    // AppData settings/cover-cache files as the first and race on writes.
    // QSharedMemory's mere existence across processes (not its contents) is
    // what we use as the lock; whichever instance created it first wins.
    QSharedMemory singleInstanceLock("SentinelFlow.HitPaw.MangaDexManager.instance-lock");
    if (!singleInstanceLock.create(1)) {
        // create() fails if a segment with this key already exists, i.e.
        // another instance is running (or crashed without releasing it on
        // some platforms — attach()+detach() below recovers from that case).
        if (singleInstanceLock.attach()) singleInstanceLock.detach();
        if (!singleInstanceLock.create(1)) {
            QMessageBox::information(nullptr, "HitPaw MangaDex Manager",
                "HitPaw is already running. Check your taskbar.");
            return 0;
        }
    }

    QPalette dark;
    dark.setColor(QPalette::Window,          QColor(Pal::BG));
    dark.setColor(QPalette::WindowText,      QColor(Pal::TEXT));
    dark.setColor(QPalette::Base,            QColor(Pal::ENTRY));
    dark.setColor(QPalette::AlternateBase,   QColor(Pal::SURFACE));
    dark.setColor(QPalette::ToolTipBase,     QColor(Pal::SURFACE));
    dark.setColor(QPalette::ToolTipText,     QColor(Pal::TEXT));
    dark.setColor(QPalette::Text,            QColor(Pal::TEXT));
    dark.setColor(QPalette::Button,          QColor(Pal::CARD));
    dark.setColor(QPalette::ButtonText,      QColor(Pal::TEXT));
    dark.setColor(QPalette::BrightText,      QColor(Pal::ACCENT_H));
    dark.setColor(QPalette::Highlight,       QColor(Pal::ACCENT));
    dark.setColor(QPalette::HighlightedText, QColor("#1a1206"));
    dark.setColor(QPalette::Link,            QColor(Pal::ACCENT));
    app.setPalette(dark);

    MainWindow w;
    w.show();
    return app.exec();
}

#include "main.moc"