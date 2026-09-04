#pragma once

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDate>
#include <QDateTime>
#include <QStringList>
#include <zlib.h>
#include "domain.h"

namespace Export {

inline const QStringList CSV_FIELDS = {
    "title","status_label","year","content_rating",
    "languages","has_english","authors","url","id"
};

inline void toCSV(const QList<MangaEntry>& lib, const QString& path) {
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

inline void toJSON(const QList<MangaEntry>& lib, const QString& path) {
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

inline void toMALXML(const QList<MangaEntry>& lib, const QString& path) {
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream s(&f);
    s.setEncoding(QStringConverter::Utf8);
    int totalReading = 0, totalCompleted = 0, totalOnHold = 0;
    int totalDropped = 0, totalPlanToRead = 0;
    for (const auto& e : lib) {
        if (e.status == "reading" || e.status == "re_reading")  ++totalReading;
        else if (e.status == "completed")    ++totalCompleted;
        else if (e.status == "on_hold")      ++totalOnHold;
        else if (e.status == "dropped")      ++totalDropped;
        else if (e.status == "plan_to_read") ++totalPlanToRead;
    }
    s << R"(<?xml version="1.0" encoding="UTF-8"?>)" "\n"
      << "<myanimelist>\n"
      << "  <myinfo>\n"
      << "    <user_id>0</user_id>\n"
      << "    <user_name>mangadex_export</user_name>\n"
      << "    <user_export_type>2</user_export_type>\n"
      << "    <user_total_manga>" << lib.size() << "</user_total_manga>\n"
      << "    <user_total_reading>" << totalReading << "</user_total_reading>\n"
      << "    <user_total_completed>" << totalCompleted << "</user_total_completed>\n"
      << "    <user_total_onhold>" << totalOnHold << "</user_total_onhold>\n"
      << "    <user_total_dropped>" << totalDropped << "</user_total_dropped>\n"
      << "    <user_total_plantoread>" << totalPlanToRead << "</user_total_plantoread>\n"
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
          << "<manga_num_volumes>0</manga_num_volumes>"
          << "<manga_num_chapters>0</manga_num_chapters>"
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

inline bool writeGzip(const QByteArray& data, const QString& path) {
    QByteArray pathBytes = path.toLocal8Bit();
    gzFile gz = gzopen(pathBytes.constData(), "wb9");
    if (!gz) return false;
    gzwrite(gz, data.constData(), static_cast<unsigned>(data.size()));
    gzclose(gz);
    return true;
}

inline void toMangaBakaJSON(const QList<MangaEntry>& lib, const QString& path) {
    const QString now = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs);
    QJsonArray entries;
    for (const auto& e : lib) {
        const QString mbStatus = MB_STATUS.value(e.status, "plan_to_read");
        QJsonObject entry;
        entry["note"]              = QJsonValue::Null;
        entry["read_link"]         = QJsonValue::Null;
        entry["rating"]            = QJsonValue::Null;
        entry["state"]             = mbStatus;
        entry["priority"]          = 20;
        entry["is_private"]        = false;
        entry["number_of_rereads"] = 0;
        entry["progress_chapter"]  = QJsonValue::Null;
        entry["progress_volume"]   = QJsonValue::Null;
        entry["start_date"]        = QJsonValue::Null;
        entry["finish_date"]       = QJsonValue::Null;
        entry["imported_at"]       = now;
        entry["created_at"]        = now;
        entry["updated_at"]        = now;
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

inline void toAnimeplanetGZ(const QList<MangaEntry>& lib, const QString& path) {
    QByteArray buf;
    QTextStream s(&buf);
    s.setEncoding(QStringConverter::Utf8);
    int totalReading = 0, totalCompleted = 0, totalOnHold = 0;
    int totalDropped = 0, totalPlanToRead = 0;
    for (const auto& e : lib) {
        if (e.status == "reading" || e.status == "re_reading")  ++totalReading;
        else if (e.status == "completed")    ++totalCompleted;
        else if (e.status == "on_hold")      ++totalOnHold;
        else if (e.status == "dropped")      ++totalDropped;
        else if (e.status == "plan_to_read") ++totalPlanToRead;
    }
    s << R"(<?xml version="1.0" encoding="UTF-8"?>)" "\n"
      << "<myanimelist>\n"
      << "  <myinfo>\n"
      << "    <user_id>0</user_id>\n"
      << "    <user_name>mangadex_export</user_name>\n"
      << "    <user_export_type>2</user_export_type>\n"
      << "    <user_total_manga>" << lib.size() << "</user_total_manga>\n"
      << "    <user_total_reading>" << totalReading << "</user_total_reading>\n"
      << "    <user_total_completed>" << totalCompleted << "</user_total_completed>\n"
      << "    <user_total_onhold>" << totalOnHold << "</user_total_onhold>\n"
      << "    <user_total_dropped>" << totalDropped << "</user_total_dropped>\n"
      << "    <user_total_plantoread>" << totalPlanToRead << "</user_total_plantoread>\n"
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
          << "<manga_num_volumes>0</manga_num_volumes>"
          << "<manga_num_chapters>0</manga_num_chapters>"
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