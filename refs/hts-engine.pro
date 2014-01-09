TEMPLATE = lib
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += staticlib

INCLUDEPATH += $$PWD/hts-engine/src/include

win32:LIBS += -lwinmm

SOURCES += \
    $$PWD/hts-engine/src/lib/HTS_audio.c \
    $$PWD/hts-engine/src/lib/HTS_engine.c \
    $$PWD/hts-engine/src/lib/HTS_gstream.c \
    $$PWD/hts-engine/src/lib/HTS_label.c \
    $$PWD/hts-engine/src/lib/HTS_misc.c \
    $$PWD/hts-engine/src/lib/HTS_model.c \
    $$PWD/hts-engine/src/lib/HTS_pstream.c \
    $$PWD/hts-engine/src/lib/HTS_sstream.c \
    $$PWD/hts-engine/src/lib/HTS_vocoder.c

HEADERS += \
    $$PWD/hts-engine/src/lib/HTS_hidden.h \
    $$PWD/hts-engine/src/include/HTS_engine.h
