#pragma once
#include <fstream>

class File {
private:
	std::string fileName;
	std::string directory;
public:
	std::string test;
	File(const std::string &filename, const std::string &directory);

	static bool exists( const std::string &fileName, const std::string &directory);
	static bool create( const std::string &fileName, const std::string &directory);
	static bool destroy(const std::string &fileName, const std::string &directory);

	bool exists();
	bool create();
	bool destroy();

	void write(const std::string &text);
	std::string read_line();
	void clear();
};