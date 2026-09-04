QT       += core gui network widgets
CONFIG   += c++17
TARGET    = MangaDexExporter
TEMPLATE  = app

SOURCES  += main.cpp
HEADERS  += pal.h domain.h export.h secure_store.h
RESOURCES += icons.qrc

win32: RC_ICONS = hitpaw.ico

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

win32:  LIBS += -lz
unix:   LIBS += -lz