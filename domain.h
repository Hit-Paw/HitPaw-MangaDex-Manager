#pragma once

#include <QString>
#include <QMap>
#include <QList>
#include "pal.h"

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
    QString artists;
    QString genres;
    QString demographic;
    QString url;
    QString coverUrl;
};

inline const QMap<QString,QString> STATUS_LABELS = {
    {"reading",      "Reading"},
    {"completed",    "Completed"},
    {"on_hold",      "On Hold"},
    {"dropped",      "Dropped"},
    {"plan_to_read", "Plan to Read"},
    {"re_reading",   "Re-reading"},
};

inline const QMap<QString,QString> MAL_STATUS = {
    {"reading",      "Reading"},
    {"completed",    "Completed"},
    {"on_hold",      "On-Hold"},
    {"dropped",      "Dropped"},
    {"plan_to_read", "Plan to Read"},
    {"re_reading",   "Reading"},
};

inline const QMap<QString,QString> MB_STATUS = {
    {"reading",      "reading"},
    {"completed",    "completed"},
    {"on_hold",      "paused"},
    {"dropped",      "dropped"},
    {"plan_to_read", "plan_to_read"},
    {"re_reading",   "rereading"},
};

inline QString statusColor(const QString& status) {
    if (status == "reading")      return Pal::ACCENT;
    if (status == "completed")    return Pal::GREEN;
    if (status == "on_hold")      return "#d29922";
    if (status == "dropped")      return Pal::RED;
    if (status == "re_reading")   return Pal::ACCENT_H;
    return Pal::MUTED;
}