#pragma once
// domain.h — core types extracted from main.cpp:120 for modularization.
// No credentials here. Pure data structures. Include pal.h for statusColor.
//
// v3.7 additions: reading progress tracking fields (chaptersRead, totalChapters,
// lastChapterRead). Persisted per-manga in QSettings ("progress/{id}"). Defaults
// preserve v3.6 compatibility — old code that constructs MangaEntry{} is unaffected.

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

    // v3.7 — reading progress (defaults = "no progress tracked")
    int     chaptersRead    = 0;      // how many chapters the user has read
    int     totalChapters   = 0;      // total chapters in the manga (0 = unknown)
    int     lastChapterRead = 0;      // last chapter number read (for "continue reading")
    QString lastReadAt;               // ISO timestamp of last progress update

    // Convenience: progress as 0..100 (returns -1 if total unknown)
    int progressPercent() const {
        if (totalChapters <= 0) return -1;
        return qBound(0, chaptersRead * 100 / totalChapters, 100);
    }
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
    // Ember palette: warm status tones (amber/sand/gold) instead of cold yellow.
    if (status == "reading")      return Pal::ACCENT;    // orange — active
    if (status == "completed")    return Pal::GREEN;     // leaf
    if (status == "on_hold")      return Pal::SAND;      // warm sand
    if (status == "dropped")      return Pal::RED;       // coral
    if (status == "re_reading")   return Pal::GOLD;      // gold
    if (status == "plan_to_read") return Pal::MUTED;     // queued — quiet grey
    return Pal::MUTED;
}
