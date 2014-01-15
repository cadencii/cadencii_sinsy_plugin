#pragma once

#include <memory>

namespace sinsy {
class Converter;
}

namespace cadencii {
namespace plugin {

class DictionaryLoader
{
public:
	DictionaryLoader();

	std::shared_ptr<sinsy::Converter>
	makeConverter();

private:
	struct Impl;
	std::shared_ptr<Impl> impl_;
};

}
}
