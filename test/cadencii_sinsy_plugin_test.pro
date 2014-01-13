include(../cadencii_sinsy_plugin.pro)

TEMPLATE = app
CONFIG -= plugin
CONFIG += console

INCLUDEPATH += \
    $$PWD/../refs/gtest/include \
    $$PWD/../refs/gtest \
    $$PWD/../src \

DEFINES += GTEST_HAS_TR1_TUPLE=0

SOURCES += \
    $$PWD/../refs/gtest/src/gtest-all.cc \
    $$PWD/../refs/gtest/src/gtest_main.cc \
    $$PWD/SinsySessionTest.cpp \
    $$PWD/SinsySingingSynthesizerTest.cpp \

HEADERS += \
    $$PWD/stub/ScoreProviderStub.hpp \
    $$PWD/stub/ScoreWritableStub.hpp \
