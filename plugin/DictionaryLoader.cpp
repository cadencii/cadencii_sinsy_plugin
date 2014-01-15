#include <cassert>
#include "DictionaryLoader.hpp"
#include "../src/lib/japanese/JConf.h"
#include "../src/lib/converter/Converter.h"

extern "C" char const* binary_japanese_euc_jp_conf_start;
extern "C" char const* binary_japanese_euc_jp_conf_end;
extern "C" char const* binary_japanese_euc_jp_table_start;
extern "C" char const* binary_japanese_euc_jp_table_end;
extern "C" char const* binary_japanese_macron_start;
extern "C" char const* binary_japanese_macron_end;
extern "C" char const* binary_japanese_shift_jis_conf_start;
extern "C" char const* binary_japanese_shift_jis_conf_end;
extern "C" char const* binary_japanese_shift_jis_table_start;
extern "C" char const* binary_japanese_shift_jis_table_end;
extern "C" char const* binary_japanese_utf_8_conf_start;
extern "C" char const* binary_japanese_utf_8_conf_end;
extern "C" char const* binary_japanese_utf_8_table_start;
extern "C" char const* binary_japanese_utf_8_table_end;
extern "C" char const* binary_japanese_macron_start;
extern "C" char const* binary_japanese_macron_end;

namespace cadencii {
namespace plugin {

struct DictionaryLoader::Impl
{
public:
	Impl()
	{}

	~Impl()
	{}

	std::shared_ptr<sinsy::Converter>
	makeConverter()
	{
		auto result = std::make_shared<sinsy::Converter>();
		
		auto utf8 = makeJapaneseUTF8Conf();
		result->setJapaneseUTF8Conf(utf8.release());

		auto shiftjis = makeJapaneseShiftJISConf();
		result->setJapaneseShiftJISConf(shiftjis.release());

		auto eucjp = makeJapaneseEUCJPConf();
		result->setJapaneseEUCJPConf(eucjp.release());

		return result;
	}

private:
	void initializeStream(std::stringstream & stream, char const* from, char const* to)
	{
		assert(to >= from);
	    std::stringstream temp;
	    size_t const count = (to - from) / sizeof(char);
	    temp.write(from, count);
	    temp.flush();
	    temp.seekg(0);
	    stream.swap(temp);
	}

	std::unique_ptr<sinsy::JConf>
	makeJapaneseUTF8Conf()
	{
		return std::move(makeJapaneseConf(sinsy::ConfManager::UTF_8_STRS,
								  	      binary_japanese_utf_8_table_start, binary_japanese_utf_8_table_end,
										  binary_japanese_utf_8_conf_start, binary_japanese_utf_8_conf_end));
	}

	std::unique_ptr<sinsy::JConf>
	makeJapaneseShiftJISConf()
	{
		return std::move(makeJapaneseConf(sinsy::ConfManager::SHIFT_JIS_STRS,
										  binary_japanese_shift_jis_table_start, binary_japanese_shift_jis_table_end,
										  binary_japanese_shift_jis_conf_start, binary_japanese_shift_jis_conf_end));
	}

	std::unique_ptr<sinsy::JConf>
	makeJapaneseEUCJPConf()
	{
		return std::move(makeJapaneseConf(sinsy::ConfManager::EUC_JP_STRS,
										  binary_japanese_euc_jp_table_start, binary_japanese_euc_jp_table_end,
										  binary_japanese_euc_jp_conf_start, binary_japanese_euc_jp_conf_end));
	}

	std::unique_ptr<sinsy::JConf>
	makeJapaneseConf(std::string const& strs, char const* table_start, char const* table_end, char const* conf_start, char const* conf_end)
	{
        std::unique_ptr<sinsy::JConf> result(new sinsy::JConf(strs));
        std::stringstream table;
        initializeStream(table, table_start, table_end);
        std::stringstream conf;
        initializeStream(conf, conf_start, conf_end);
        std::stringstream macron;
        initializeStream(macron, binary_japanese_macron_start, binary_japanese_macron_end);
        result->read(table, conf, macron);
        return std::move(result);
	}
};

DictionaryLoader::DictionaryLoader()
	: impl_(std::make_shared<Impl>())
{}


std::shared_ptr<sinsy::Converter>
DictionaryLoader::makeConverter()
{
	return impl_->makeConverter();
}

}
}
