include(../cadencii_sinsy_plugin.pro)

TEMPLATE = app
CONFIG -= plugin
CONFIG += console

INCLUDEPATH += \
    $$PWD/../refs/gtest/include \
    $$PWD/../refs/gtest \
    $$PWD/../src \

DEFINES += GTEST_HAS_TR1_TUPLE=0

mac {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib
}

SOURCES += \
    $$PWD/../refs/gtest/src/gtest-all.cc \
    $$PWD/../refs/gtest/src/gtest_main.cc \
    $$PWD/SinsySessionTest.cpp \

HEADERS += \
    $$PWD/stub/ScoreProviderStub.hpp \
    $$PWD/stub/ScoreWritableStub.hpp \
