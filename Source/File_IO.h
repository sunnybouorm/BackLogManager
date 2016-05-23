#pragma once
#include <fstream>

typedef struct IO_FLAG_STRUCT {
	bool isEof  = false;
	bool isBad  = false;
	bool isGood = true;
} IO_FLAGS;

class File {
private:
	std::string fileName;
	std::string directory;
	std::istream::streampos ipos; //current file input pointer position
public:
	std::string test;
	IO_FLAGS i_flags;//flags relating to input filestream
	File(const std::string &filename, const std::string &directory);

	static bool exists( const std::string &fileName, const std::string &directory);
	static bool create( const std::string &fileName, const std::string &directory);
	static bool destroy(const std::string &fileName, const std::string &directory);

	bool exists();
	bool create();
	bool destroy();

	void clear_i_flags();

	void write(const std::string &text);
	void read_line(std::string &output);
	void clear();
};