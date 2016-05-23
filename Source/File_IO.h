#ifndef BACKLOGMANAGER_SOURCE_FILEIO_H_
#define BACKLOGMANAGER_SOURCE_FILEIO_H_

#include <fstream>

typedef struct IO_FLAG_STRUCT {
	bool is_eof  = false;
	bool is_bad  = false;
	bool is_good = true;
} IO_FLAGS;

class File {
private:
	std::string filename_;
	std::string directory_;
	std::istream::streampos ipos_; //current file input pointer position

	void reset_i_flags();//reverts input stream flags to default
	void set_i_flags_good();
	void set_i_flags_bad();
	void set_i_flags_eof();

public:
	std::string test_;
	IO_FLAGS i_flags_;//flags relating to input filestream
	File(const std::string &filename, const std::string &directory);

	static bool exists( const std::string &filename, const std::string &directory);
	static bool create( const std::string &filename, const std::string &directory);
	static bool destroy(const std::string &filename, const std::string &directory);

	bool exists();
	bool create();
	bool destroy();

	void write(const std::string &text);
	void read_line(std::string &output);
	void clear();//clears file by destroying and recreating it
};

#endif // !BACKLOGMANAGER_SOURCE_FILEIO_H_