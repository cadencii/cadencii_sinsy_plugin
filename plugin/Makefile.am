AM_CPPFLAGS = \
    -I $(abs_srcdir)/../refs/cadencii_synth_api \
    -I $(abs_srcdir)/../src \
    -I $(abs_srcdir)/../src/lib/score \
    -I $(abs_srcdir)/../src/lib/util \
    -I $(abs_srcdir)/../src/lib/converter \
    -I $(abs_srcdir)/../src/lib/temporary \
    -I $(abs_srcdir)/../src/lib/hts_engine_API

lib_LTLIBRARIES = libcadencii_sinsy_plugin.la

libcadencii_sinsy_plugin_la_SOURCES = \
    $(abs_srcdir)/main.cpp \
    $(abs_srcdir)/SinsySession.cpp \
    $(abs_srcdir)/SinsySession.hpp \
    $(abs_srcdir)/SinsySingingSynthesizer.cpp \
    $(abs_srcdir)/SinsySingingSynthesizer.hpp \
    $(abs_srcdir)/DictionaryLoader.cpp \
    $(abs_srcdir)/DictionaryLoader.hpp \
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
