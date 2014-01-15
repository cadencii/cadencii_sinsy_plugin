#pragma once

#include <memory>

namespace sinsy {
class Converter;
}

namespace cadencii {
namespace plugin {

class ConverterFactory
{
public:
	ConverterFactory();

	std::shared_ptr<sinsy::Converter>
	makeConverter();

private:
	struct Impl;
	std::shared_ptr<Impl> impl_;
};

}
}
