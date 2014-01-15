#include <libgen.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>

static std::string get_basename(std::string const& file_path)
{
	std::unique_ptr<char> buffer;
	buffer.reset(new char[file_path.size()]);
	std::copy(file_path.begin(), file_path.end(), buffer.get());
	char * result = basename(buffer.get());
	if (result) {
		return std::string(result);
	} else {
		return std::string("a");
	}
}

static std::string transform_invalid_symbolchar(std::string const& file_path)
{
	std::string result = file_path;
	std::transform(result.begin(), result.end(), result.begin(), [](char c) {
		if (std::isalnum(c)) {
			return c;
		} else {
			return '_';
		}
	});
	return result;
}

int main(int argc, char *argv[])
{
	if (argc < 3) {
		return 1;
	}
	std::string input(argv[1]);
	std::string output(argv[2]);
	std::ofstream fout(output.c_str(), std::ios::out);
	std::string symbol_name = transform_invalid_symbolchar(get_basename(input));

	fout << "extern \"C\" {" << std::endl;

	fout << "static char s[] = {" << std::endl;
	size_t size = 0;
	FILE * fin = fopen(input.c_str(), "rb");
	while (!feof(fin)) {
		int const c = fgetc(fin);
		if (c == EOF) {
			break;
		}
		++size;
		fout << "(char)0x" << std::hex << c << std::dec << ", ";
	}
	fclose(fin);
	fout << "(char)0 };" << std::endl;

	fout << "extern char const* binary_" << symbol_name << "_start = (char const*)s;" << std::endl;
	fout << "extern char const* binary_" << symbol_name << "_end = (char const*)s + " << size << ";" << std::endl;

	fout << "} // extern \"C\"" << std::endl;

	return 0;
}
