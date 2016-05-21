#pragma once
#include <fstream>

class File {
public:
	File() {};

	static bool exists( const std::string &fileName, const std::string &directory);
	static bool destroy(const std::string &fileName, const std::string &directory);
};