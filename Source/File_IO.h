#ifndef BACKLOGMANAGER_SOURCE_FILEIO_H_
#define BACKLOGMANAGER_SOURCE_FILEIO_H_

#include <fstream>

typedef struct IoFlagStruct {
	bool is_eof  = false;
	bool is_bad  = false;
	bool is_good = true;
} IoFlags;

class File {
private:
	std::string filename_;
	std::string directory_;
	std::istream::streampos ipos_; //current file input pointer position

	void ResetInputFlags();//reverts input stream flags to default
	void SetInputFlagsGood();
	void SetInputFlagsBad();
	void SetInputFlagsEof();

public:
	std::string test_;
	IoFlags i_flags_;//flags relating to input filestream
	File(const std::string &filename, const std::string &directory);

	static bool Exists( const std::string &filename, const std::string &directory);
	static bool Create( const std::string &filename, const std::string &directory);
	static bool Destroy(const std::string &filename, const std::string &directory);

	bool Exists();//checks if the file exists
	bool Create();
	bool Destroy();

	void Write(const std::string &text);
	void ReadLine(std::string &output);
	void Clear();//clears file by destroying and recreating it
};

#endif // !BACKLOGMANAGER_SOURCE_FILEIO_H_