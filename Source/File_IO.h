#ifndef BACKLOGMANAGER_SOURCE_FILEIO_H_
#define BACKLOGMANAGER_SOURCE_FILEIO_H_

#include <fstream>

std::istream::streampos operator+= (std::istream::streampos &lhs, int &rhs);
std::istream::streampos operator-= (std::istream::streampos &lhs, int &rhs);

typedef struct IoFlagStruct {
	bool is_eof  = false;
	bool is_bad  = false;
	bool is_good = true;
} IFlags;

typedef std::pair<std::istream::streampos, std::istream::streampos> StreamposPair;

class File {

public:
	std::string test_;
	File(const std::string &filename = "", const std::string &directory = "");

	static bool Exists (const std::string &filename, const std::string &directory);
	static bool Create (const std::string &filename, const std::string &directory);
	static bool Destroy(const std::string &filename, const std::string &directory);

	bool Exists();//checks if the file exists
	bool Create();
	bool Destroy();

	std::string get_filename()  { return this->filename_; }
	std::string get_directory() { return this->directory_; }

	void set_filename (const std::string &file_name) {this->filename_  = file_name; }
	void set_directory(const std::string &directory) {this->directory_ = directory; }

	bool Write(const std::string &text, int mode=std::fstream::app);
	bool Write(const std::string &text, const StreamposPair &bracket);
	bool ReadToDelimiter(std::string &output, const char &delimiter = ';', \
		const bool &is_inclusive = false);

	bool ReadBetweenDelimiters(std::string &output, const char &lhs_delimiter,\
		const char &rhs_delimiter);

	bool Read(std::string &output, const StreamposPair &bracket);//read from streampos.first to streampos.second
	bool ReadChar(char &output);
	bool ReadLine(std::string &output);

	std::string to_string();

	void Clear();//clears file by destroying and recreating it

	IFlags get_iflags() { return this->i_flags_; }

	void set_ipos(std::istream::streampos ipos) { this->ipos_ = ipos; }
	std::istream::streampos get_ipos()			{ return this->ipos_; }

	void set_opos(std::ostream::streampos opos) { this->opos_ = opos; }
	std::ostream::streampos get_opos() { return this->opos_; }

	bool SetEofPos();
	std::ostream::streampos get_eof_pos() { return this->eof_pos_; }

private:
	std::string filename_;
	std::string directory_;
	std::istream::streampos ipos_;		//current file input  pointer position
	std::ostream::streampos opos_;		//current file output pointer position
	std::ostream::streampos eof_pos_;	//current file output end of filepointer position

	static const StreamposPair kNullStreamposPair;

	IFlags i_flags_;//flags relating to input filestream

	void ResetInputFlags();//reverts input stream flags to default
	void SetInputFlagsGood();
	void SetInputFlagsBad();
	void SetInputFlagsEof();
};

#endif // !BACKLOGMANAGER_SOURCE_FILEIO_H_