QT       += core gui network widgets
CONFIG   += c++17
TARGET    = MangaDexExporter
TEMPLATE  = app

SOURCES  += main.cpp
RESOURCES += icons.qrc

win32: RC_ICONS = hitpaw.ico

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# zlib for gzip export (Anime-Planet)
win32:  LIBS += -lz
unix:   LIBS += -lz
