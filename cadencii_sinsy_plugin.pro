TEMPLATE = lib
TARGET = sinsy_plugin
CONFIG += plugin
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
INCLUDEPATH += \
    $$PWD/src \
    $$PWD/src/lib/converter \
    $$PWD/src/lib/hts_engine_API \
    $$PWD/src/lib/japanese \
    $$PWD/src/lib/label \
    $$PWD/src/lib/score \
    $$PWD/src/lib/temporary \
    $$PWD/src/lib/util \
    $$PWD/src/lib/xml \
    $$PWD/src/include \
    $$PWD/../ \
    $$PWD/../cadencii-nt/ \
    $$PWD/refs/hts-engine/src/include

LIBS += -lHTSEngine

CONFIG(x86_64): {
    ARCH = x64
} else {
    ARCH = x86
}

debug: {
    LIBS += -L$$PWD/refs/hts-engine-build/$$ARCH/debug
} else {
    LIBS += -L$$PWD/refs/hts-engine-build/$$ARCH/release
}

win32: LIBS += -lwinmm

SOURCES += \
    $$PWD/src/lib/converter/ConfGroup.cpp \
    $$PWD/src/lib/converter/ConfManager.cpp \
    $$PWD/src/lib/converter/Converter.cpp \
    $$PWD/src/lib/converter/PhonemeInfo.cpp \
    $$PWD/src/lib/converter/UnknownConf.cpp \
    $$PWD/src/lib/converter/util_converter.cpp \
    $$PWD/src/lib/hts_engine_API/HtsEngine.cpp \
    $$PWD/src/lib/hts_engine_API/SynthConditionImpl.cpp \
    $$PWD/src/lib/japanese/JConf.cpp \
    $$PWD/src/lib/label/LabelData.cpp \
    $$PWD/src/lib/label/LabelMaker.cpp \
    $$PWD/src/lib/label/LabelMeasure.cpp \
    $$PWD/src/lib/label/LabelPosition.cpp \
    $$PWD/src/lib/label/LabelStream.cpp \
    $$PWD/src/lib/label/LabelStrings.cpp \
    $$PWD/src/lib/label/NoteGroup.cpp \
    $$PWD/src/lib/label/NoteLabeler.cpp \
    $$PWD/src/lib/label/PhonemeLabeler.cpp \
    $$PWD/src/lib/label/SyllableLabeler.cpp \
    $$PWD/src/lib/score/Beat.cpp \
    $$PWD/src/lib/score/Dynamics.cpp \
    $$PWD/src/lib/score/Key.cpp \
    $$PWD/src/lib/score/Mode.cpp \
    $$PWD/src/lib/score/Note.cpp \
    $$PWD/src/lib/score/Pitch.cpp \
    $$PWD/src/lib/score/ScorePosition.cpp \
    $$PWD/src/lib/score/Slur.cpp \
    $$PWD/src/lib/score/Syllabic.cpp \
    $$PWD/src/lib/score/util_score.cpp \
    $$PWD/src/lib/Sinsy.cpp \
    $$PWD/src/lib/temporary/ScoreDoctor.cpp \
    $$PWD/src/lib/temporary/TempScore.cpp \
    $$PWD/src/lib/util/Configurations.cpp \
    $$PWD/src/lib/util/InputFile.cpp \
    $$PWD/src/lib/util/IStringable.cpp \
    $$PWD/src/lib/util/MacronTable.cpp \
    $$PWD/src/lib/util/MultibyteCharRange.cpp \
    $$PWD/src/lib/util/OutputFile.cpp \
    $$PWD/src/lib/util/PhonemeTable.cpp \
    $$PWD/src/lib/util/StringTokenizer.cpp \
    $$PWD/src/lib/util/util_string.cpp \
    $$PWD/src/lib/xml/XmlData.cpp \
    $$PWD/src/lib/xml/XmlParser.cpp \
    $$PWD/src/lib/xml/XmlReader.cpp \
    $$PWD/src/lib/xml/XmlWriter.cpp \
    $$PWD/plugin/SinsySession.cpp \
    $$PWD/plugin/SinsySingingSynthesizer.cpp \
    $$PWD/plugin/main.cpp \

HEADERS += \
    $$PWD/src/include/sinsy.h \
    $$PWD/src/lib/converter/ConfGroup.h \
    $$PWD/src/lib/converter/ConfManager.h \
    $$PWD/src/lib/converter/Converter.h \
    $$PWD/src/lib/converter/IConf.h \
    $$PWD/src/lib/converter/IConvertable.h \
    $$PWD/src/lib/converter/PhonemeInfo.h \
    $$PWD/src/lib/converter/UnknownConf.h \
    $$PWD/src/lib/converter/util_converter.h \
    $$PWD/src/lib/hts_engine_API/HtsEngine.h \
    $$PWD/src/lib/hts_engine_API/SynthConditionImpl.h \
    $$PWD/src/lib/japanese/JConf.h \
    $$PWD/src/lib/label/ILabelOutput.h \
    $$PWD/src/lib/label/INoteLabel.h \
    $$PWD/src/lib/label/IPhonemeLabel.h \
    $$PWD/src/lib/label/ISyllableLabel.h \
    $$PWD/src/lib/label/LabelData.h \
    $$PWD/src/lib/label/LabelMaker.h \
    $$PWD/src/lib/label/LabelMeasure.h \
    $$PWD/src/lib/label/LabelPosition.h \
    $$PWD/src/lib/label/LabelStream.h \
    $$PWD/src/lib/label/LabelStrings.h \
    $$PWD/src/lib/label/NoteGroup.h \
    $$PWD/src/lib/label/NoteLabeler.h \
    $$PWD/src/lib/label/PhonemeLabeler.h \
    $$PWD/src/lib/label/SyllableLabeler.h \
    $$PWD/src/lib/score/Beat.h \
    $$PWD/src/lib/score/Dynamics.h \
    $$PWD/src/lib/score/IScoreWritable.h \
    $$PWD/src/lib/score/IScoreWriter.h \
    $$PWD/src/lib/score/Key.h \
    $$PWD/src/lib/score/Mode.h \
    $$PWD/src/lib/score/Note.h \
    $$PWD/src/lib/score/Pitch.h \
    $$PWD/src/lib/score/ScorePosition.h \
    $$PWD/src/lib/score/Slur.h \
    $$PWD/src/lib/score/Syllabic.h \
    $$PWD/src/lib/score/util_score.h \
    $$PWD/src/lib/temporary/ScoreDoctor.h \
    $$PWD/src/lib/temporary/TempScore.h \
    $$PWD/src/lib/util/Configurations.h \
    $$PWD/src/lib/util/Deleter.h \
    $$PWD/src/lib/util/ForEachAdapter.h \
    $$PWD/src/lib/util/InputFile.h \
    $$PWD/src/lib/util/IReadableStream.h \
    $$PWD/src/lib/util/IStringable.h \
    $$PWD/src/lib/util/IWritableStream.h \
    $$PWD/src/lib/util/MacronTable.h \
    $$PWD/src/lib/util/MultibyteCharRange.h \
    $$PWD/src/lib/util/OutputFile.h \
    $$PWD/src/lib/util/PhonemeTable.h \
    $$PWD/src/lib/util/StreamException.h \
    $$PWD/src/lib/util/StringTokenizer.h \
    $$PWD/src/lib/util/util_log.h \
    $$PWD/src/lib/util/util_string.h \
    $$PWD/src/lib/util/util_types.h \
    $$PWD/src/lib/util/WritableStrStream.h \
    $$PWD/src/lib/xml/xml_tags.h \
    $$PWD/src/lib/xml/XmlData.h \
    $$PWD/src/lib/xml/XmlParser.h \
    $$PWD/src/lib/xml/XmlReader.h \
    $$PWD/src/lib/xml/XmlWriter.h \
    $$PWD/plugin/SinsySession.hpp \
    $$PWD/plugin/SinsySingingSynthesizer.hpp \
