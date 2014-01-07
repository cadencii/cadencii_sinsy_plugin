TEMPLATE = lib
TARGET = sinsy_plugin
CONFIG += plugin
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
INCLUDEPATH += \
    src/lib/converter \
    src/lib/hts_engine_API \
    src/lib/japanese \
    src/lib/label \
    src/lib/score \
    src/lib/temporary \
    src/lib/util \
    src/lib/xml \
    src/include \
    ../ \
    ../cadencii-nt/ \
    refs/hts-engine/src/include

mac:LIBS += -L/usr/local/lib -lHTSEngine

win32 {
    CONFIG(release, debug|release): {
        LIBS += -L$$PWD/refs/hts-engine-build/$$QMAKE_TARGET.arch/release
    } else {
        LIBS += -L$$PWD/refs/hts-engine-build/$$QMAKE_TARGET.arch/debug
    }
}
win32:LIBS += -lhts-engine -lwinmm

SOURCES += \
    src/lib/converter/ConfGroup.cpp \
    src/lib/converter/ConfManager.cpp \
    src/lib/converter/Converter.cpp \
    src/lib/converter/PhonemeInfo.cpp \
    src/lib/converter/UnknownConf.cpp \
    src/lib/converter/util_converter.cpp \
    src/lib/hts_engine_API/HtsEngine.cpp \
    src/lib/hts_engine_API/SynthConditionImpl.cpp \
    src/lib/japanese/JConf.cpp \
    src/lib/label/LabelData.cpp \
    src/lib/label/LabelMaker.cpp \
    src/lib/label/LabelMeasure.cpp \
    src/lib/label/LabelPosition.cpp \
    src/lib/label/LabelStream.cpp \
    src/lib/label/LabelStrings.cpp \
    src/lib/label/NoteGroup.cpp \
    src/lib/label/NoteLabeler.cpp \
    src/lib/label/PhonemeLabeler.cpp \
    src/lib/label/SyllableLabeler.cpp \
    src/lib/score/Beat.cpp \
    src/lib/score/Dynamics.cpp \
    src/lib/score/Key.cpp \
    src/lib/score/Mode.cpp \
    src/lib/score/Note.cpp \
    src/lib/score/Pitch.cpp \
    src/lib/score/ScorePosition.cpp \
    src/lib/score/Slur.cpp \
    src/lib/score/Syllabic.cpp \
    src/lib/score/util_score.cpp \
    src/lib/Sinsy.cpp \
    src/lib/temporary/ScoreDoctor.cpp \
    src/lib/temporary/TempScore.cpp \
    src/lib/util/Configurations.cpp \
    src/lib/util/InputFile.cpp \
    src/lib/util/IStringable.cpp \
    src/lib/util/MacronTable.cpp \
    src/lib/util/MultibyteCharRange.cpp \
    src/lib/util/OutputFile.cpp \
    src/lib/util/PhonemeTable.cpp \
    src/lib/util/StringTokenizer.cpp \
    src/lib/util/util_string.cpp \
    src/lib/xml/XmlData.cpp \
    src/lib/xml/XmlParser.cpp \
    src/lib/xml/XmlReader.cpp \
    src/lib/xml/XmlWriter.cpp \
    src/cadencii_sinsy_plugin.cpp

HEADERS += \
    src/include/sinsy.h \
    src/lib/converter/ConfGroup.h \
    src/lib/converter/ConfManager.h \
    src/lib/converter/Converter.h \
    src/lib/converter/IConf.h \
    src/lib/converter/IConvertable.h \
    src/lib/converter/PhonemeInfo.h \
    src/lib/converter/UnknownConf.h \
    src/lib/converter/util_converter.h \
    src/lib/hts_engine_API/HtsEngine.h \
    src/lib/hts_engine_API/SynthConditionImpl.h \
    src/lib/japanese/JConf.h \
    src/lib/label/ILabelOutput.h \
    src/lib/label/INoteLabel.h \
    src/lib/label/IPhonemeLabel.h \
    src/lib/label/ISyllableLabel.h \
    src/lib/label/LabelData.h \
    src/lib/label/LabelMaker.h \
    src/lib/label/LabelMeasure.h \
    src/lib/label/LabelPosition.h \
    src/lib/label/LabelStream.h \
    src/lib/label/LabelStrings.h \
    src/lib/label/NoteGroup.h \
    src/lib/label/NoteLabeler.h \
    src/lib/label/PhonemeLabeler.h \
    src/lib/label/SyllableLabeler.h \
    src/lib/score/Beat.h \
    src/lib/score/Dynamics.h \
    src/lib/score/IScoreWritable.h \
    src/lib/score/IScoreWriter.h \
    src/lib/score/Key.h \
    src/lib/score/Mode.h \
    src/lib/score/Note.h \
    src/lib/score/Pitch.h \
    src/lib/score/ScorePosition.h \
    src/lib/score/Slur.h \
    src/lib/score/Syllabic.h \
    src/lib/score/util_score.h \
    src/lib/temporary/ScoreDoctor.h \
    src/lib/temporary/TempScore.h \
    src/lib/util/Configurations.h \
    src/lib/util/Deleter.h \
    src/lib/util/ForEachAdapter.h \
    src/lib/util/InputFile.h \
    src/lib/util/IReadableStream.h \
    src/lib/util/IStringable.h \
    src/lib/util/IWritableStream.h \
    src/lib/util/MacronTable.h \
    src/lib/util/MultibyteCharRange.h \
    src/lib/util/OutputFile.h \
    src/lib/util/PhonemeTable.h \
    src/lib/util/StreamException.h \
    src/lib/util/StringTokenizer.h \
    src/lib/util/util_log.h \
    src/lib/util/util_string.h \
    src/lib/util/util_types.h \
    src/lib/util/WritableStrStream.h \
    src/lib/xml/xml_tags.h \
    src/lib/xml/XmlData.h \
    src/lib/xml/XmlParser.h \
    src/lib/xml/XmlReader.h \
    src/lib/xml/XmlWriter.h

