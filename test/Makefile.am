ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = \
    -I $(abs_srcdir)/../refs/cadencii_synth_api \
    -I $(abs_srcdir)/../src \
    -I $(abs_srcdir)/../src/include \
    -I $(abs_srcdir)/../src/lib/converter \
    -I $(abs_srcdir)/../src/lib/hts_engine_API \
    -I $(abs_srcdir)/../src/lib/japanese \
    -I $(abs_srcdir)/../src/lib/label \
    -I $(abs_srcdir)/../src/lib/score \
    -I $(abs_srcdir)/../src/lib/temporary \
    -I $(abs_srcdir)/../src/lib/util \
    -I $(abs_srcdir)/../src/lib/xml \
    -I $(abs_srcdir)/../refs/gtest/include \
    -I $(abs_srcdir)/../refs/gtest \
    -DGTEST_HAS_TR1_TUPLE=0

bin_PROGRAMS = cadencii_sinsy_plugin_test

cadencii_sinsy_plugin_test_SOURCES = \
    $(abs_srcdir)/../refs/gtest/src/gtest-all.cc \
    $(abs_srcdir)/../refs/gtest/src/gtest_main.cc \
    $(abs_srcdir)/SinsySessionTest.cpp \
    $(abs_srcdir)/SinsySingingSynthesizerTest.cpp \
    $(abs_srcdir)/../plugin/SinsySession.cpp \
    $(abs_srcdir)/../plugin/SinsySession.hpp \
    $(abs_srcdir)/../plugin/SinsySingingSynthesizer.cpp \
    $(abs_srcdir)/../plugin/SinsySingingSynthesizer.hpp \
    $(abs_srcdir)/../plugin/DictionaryLoader.cpp \
    $(abs_srcdir)/../plugin/DictionaryLoader.hpp \
	$(abs_srcdir)/../src/lib/Sinsy.cpp \
	$(abs_srcdir)/../src/lib/converter/ConfGroup.cpp \
	$(abs_srcdir)/../src/lib/converter/ConfGroup.h \
	$(abs_srcdir)/../src/lib/converter/ConfManager.cpp \
	$(abs_srcdir)/../src/lib/converter/ConfManager.h \
	$(abs_srcdir)/../src/lib/converter/Converter.cpp \
	$(abs_srcdir)/../src/lib/converter/Converter.h \
	$(abs_srcdir)/../src/lib/converter/IConf.h \
	$(abs_srcdir)/../src/lib/converter/IConvertable.h \
	$(abs_srcdir)/../src/lib/converter/PhonemeInfo.cpp \
	$(abs_srcdir)/../src/lib/converter/PhonemeInfo.h \
	$(abs_srcdir)/../src/lib/converter/UnknownConf.cpp \
	$(abs_srcdir)/../src/lib/converter/UnknownConf.h \
	$(abs_srcdir)/../src/lib/converter/util_converter.cpp \
	$(abs_srcdir)/../src/lib/converter/util_converter.h \
	$(abs_srcdir)/../src/lib/hts_engine_API/HtsEngine.cpp \
	$(abs_srcdir)/../src/lib/hts_engine_API/HtsEngine.h \
	$(abs_srcdir)/../src/lib/hts_engine_API/SynthConditionImpl.cpp \
	$(abs_srcdir)/../src/lib/hts_engine_API/SynthConditionImpl.h \
	$(abs_srcdir)/../src/lib/japanese/JConf.cpp \
	$(abs_srcdir)/../src/lib/japanese/JConf.h \
	$(abs_srcdir)/../src/lib/label/ILabelOutput.h \
	$(abs_srcdir)/../src/lib/label/INoteLabel.h \
	$(abs_srcdir)/../src/lib/label/IPhonemeLabel.h \
	$(abs_srcdir)/../src/lib/label/ISyllableLabel.h \
	$(abs_srcdir)/../src/lib/label/LabelData.cpp \
	$(abs_srcdir)/../src/lib/label/LabelData.h \
	$(abs_srcdir)/../src/lib/label/LabelMaker.cpp \
	$(abs_srcdir)/../src/lib/label/LabelMaker.h \
	$(abs_srcdir)/../src/lib/label/LabelMeasure.cpp \
	$(abs_srcdir)/../src/lib/label/LabelMeasure.h \
	$(abs_srcdir)/../src/lib/label/LabelPosition.cpp \
	$(abs_srcdir)/../src/lib/label/LabelPosition.h \
	$(abs_srcdir)/../src/lib/label/LabelStream.cpp \
	$(abs_srcdir)/../src/lib/label/LabelStream.h \
	$(abs_srcdir)/../src/lib/label/LabelStrings.cpp \
	$(abs_srcdir)/../src/lib/label/LabelStrings.h \
	$(abs_srcdir)/../src/lib/label/NoteGroup.cpp \
	$(abs_srcdir)/../src/lib/label/NoteGroup.h \
	$(abs_srcdir)/../src/lib/label/NoteLabeler.cpp \
	$(abs_srcdir)/../src/lib/label/NoteLabeler.h \
	$(abs_srcdir)/../src/lib/label/PhonemeLabeler.cpp \
	$(abs_srcdir)/../src/lib/label/PhonemeLabeler.h \
	$(abs_srcdir)/../src/lib/label/SyllableLabeler.cpp \
	$(abs_srcdir)/../src/lib/label/SyllableLabeler.h \
	$(abs_srcdir)/../src/lib/score/Beat.cpp \
	$(abs_srcdir)/../src/lib/score/Beat.h \
	$(abs_srcdir)/../src/lib/score/Dynamics.cpp \
	$(abs_srcdir)/../src/lib/score/Dynamics.h \
	$(abs_srcdir)/../src/lib/score/IScoreWritable.h \
	$(abs_srcdir)/../src/lib/score/IScoreWriter.h \
	$(abs_srcdir)/../src/lib/score/Key.cpp \
	$(abs_srcdir)/../src/lib/score/Key.h \
	$(abs_srcdir)/../src/lib/score/Mode.cpp \
	$(abs_srcdir)/../src/lib/score/Mode.h \
	$(abs_srcdir)/../src/lib/score/Note.cpp \
	$(abs_srcdir)/../src/lib/score/Note.h \
	$(abs_srcdir)/../src/lib/score/Pitch.cpp \
	$(abs_srcdir)/../src/lib/score/Pitch.h \
	$(abs_srcdir)/../src/lib/score/ScorePosition.cpp \
	$(abs_srcdir)/../src/lib/score/ScorePosition.h \
	$(abs_srcdir)/../src/lib/score/Slur.cpp \
	$(abs_srcdir)/../src/lib/score/Slur.h \
	$(abs_srcdir)/../src/lib/score/Syllabic.cpp \
	$(abs_srcdir)/../src/lib/score/Syllabic.h \
	$(abs_srcdir)/../src/lib/score/util_score.cpp \
	$(abs_srcdir)/../src/lib/score/util_score.h \
	$(abs_srcdir)/../src/lib/temporary/ScoreDoctor.cpp \
	$(abs_srcdir)/../src/lib/temporary/ScoreDoctor.h \
	$(abs_srcdir)/../src/lib/temporary/TempScore.cpp \
	$(abs_srcdir)/../src/lib/temporary/TempScore.h \
	$(abs_srcdir)/../src/lib/util/Configurations.cpp \
	$(abs_srcdir)/../src/lib/util/Configurations.h \
	$(abs_srcdir)/../src/lib/util/Deleter.h \
	$(abs_srcdir)/../src/lib/util/ForEachAdapter.h \
	$(abs_srcdir)/../src/lib/util/IReadableStream.h \
	$(abs_srcdir)/../src/lib/util/IStringable.cpp \
	$(abs_srcdir)/../src/lib/util/IStringable.h \
	$(abs_srcdir)/../src/lib/util/IWritableStream.h \
	$(abs_srcdir)/../src/lib/util/InputFile.cpp \
	$(abs_srcdir)/../src/lib/util/InputFile.h \
	$(abs_srcdir)/../src/lib/util/MacronTable.cpp \
	$(abs_srcdir)/../src/lib/util/MacronTable.h \
	$(abs_srcdir)/../src/lib/util/MultibyteCharRange.cpp \
	$(abs_srcdir)/../src/lib/util/MultibyteCharRange.h \
	$(abs_srcdir)/../src/lib/util/OutputFile.cpp \
	$(abs_srcdir)/../src/lib/util/OutputFile.h \
	$(abs_srcdir)/../src/lib/util/PhonemeTable.cpp \
	$(abs_srcdir)/../src/lib/util/PhonemeTable.h \
	$(abs_srcdir)/../src/lib/util/StreamException.h \
	$(abs_srcdir)/../src/lib/util/StringTokenizer.cpp \
	$(abs_srcdir)/../src/lib/util/StringTokenizer.h \
	$(abs_srcdir)/../src/lib/util/WritableStrStream.h \
	$(abs_srcdir)/../src/lib/util/util_log.h \
	$(abs_srcdir)/../src/lib/util/util_string.cpp \
	$(abs_srcdir)/../src/lib/util/util_string.h \
	$(abs_srcdir)/../src/lib/util/util_types.h \
	$(abs_srcdir)/../src/lib/xml/XmlData.cpp \
	$(abs_srcdir)/../src/lib/xml/XmlData.h \
	$(abs_srcdir)/../src/lib/xml/XmlParser.cpp \
	$(abs_srcdir)/../src/lib/xml/XmlParser.h \
	$(abs_srcdir)/../src/lib/xml/XmlReader.cpp \
	$(abs_srcdir)/../src/lib/xml/XmlReader.h \
	$(abs_srcdir)/../src/lib/xml/XmlWriter.cpp \
	$(abs_srcdir)/../src/lib/xml/XmlWriter.h \
	$(abs_srcdir)/../src/lib/xml/xml_tags.h \
	$(abs_srcdir)/../src/dic/japanese.euc_jp.conf.cpp \
	$(abs_srcdir)/../src/dic/japanese.euc_jp.table.cpp \
	$(abs_srcdir)/../src/dic/japanese.macron.cpp \
	$(abs_srcdir)/../src/dic/japanese.shift_jis.conf.cpp \
	$(abs_srcdir)/../src/dic/japanese.shift_jis.table.cpp \
	$(abs_srcdir)/../src/dic/japanese.utf_8.conf.cpp \
	$(abs_srcdir)/../src/dic/japanese.utf_8.table.cpp

$(abs_srcdir)/../src/dic/japanese.euc_jp.conf.cpp: $(abs_srcdir)/../src/dic/japanese.euc_jp.conf
	cd $(abs_srcdir)/../tool && make
	cd $(abs_srcdir)/../src/dic && $(abs_srcdir)/../tool/file_to_source $^ $@

$(abs_srcdir)/../src/dic/japanese.euc_jp.table.cpp: $(abs_srcdir)/../src/dic/japanese.euc_jp.table
	cd $(abs_srcdir)/../tool && make
	cd $(abs_srcdir)/../src/dic && $(abs_srcdir)/../tool/file_to_source $^ $@

$(abs_srcdir)/../src/dic/japanese.macron.cpp: $(abs_srcdir)/../src/dic/japanese.macron
	cd $(abs_srcdir)/../tool && make
	cd $(abs_srcdir)/../src/dic && $(abs_srcdir)/../tool/file_to_source $^ $@

$(abs_srcdir)/../src/dic/japanese.shift_jis.conf.cpp: $(abs_srcdir)/../src/dic/japanese.shift_jis.conf
	cd $(abs_srcdir)/../tool && make
	cd $(abs_srcdir)/../src/dic && $(abs_srcdir)/../tool/file_to_source $^ $@

$(abs_srcdir)/../src/dic/japanese.shift_jis.table.cpp: $(abs_srcdir)/../src/dic/japanese.shift_jis.table
	cd $(abs_srcdir)/../tool && make
	cd $(abs_srcdir)/../src/dic && $(abs_srcdir)/../tool/file_to_source $^ $@

$(abs_srcdir)/../src/dic/japanese.utf_8.conf.cpp: $(abs_srcdir)/../src/dic/japanese.utf_8.conf
	cd $(abs_srcdir)/../tool && make
	cd $(abs_srcdir)/../src/dic && $(abs_srcdir)/../tool/file_to_source $^ $@

$(abs_srcdir)/../src/dic/japanese.utf_8.table.cpp: $(abs_srcdir)/../src/dic/japanese.utf_8.table
	cd $(abs_srcdir)/../tool && make
	cd $(abs_srcdir)/../src/dic && $(abs_srcdir)/../tool/file_to_source $^ $@

clean-generic:
	rm -f $(abs_srcdir)/../src/dic/japanese.euc_jp.conf.cpp
	rm -f $(abs_srcdir)/../src/dic/japanese.euc_jp.table.cpp
	rm -f $(abs_srcdir)/../src/dic/japanese.macron.cpp
	rm -f $(abs_srcdir)/../src/dic/japanese.shift_jis.conf.cpp
	rm -f $(abs_srcdir)/../src/dic/japanese.shift_jis.table.cpp
	rm -f $(abs_srcdir)/../src/dic/japanese.utf_8.conf.cpp
	rm -f $(abs_srcdir)/../src/dic/japanese.utf_8.table.cpp
