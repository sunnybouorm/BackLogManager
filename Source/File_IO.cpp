#include "stdafx.h"
#include "file_io.h"

File::File(const std::string &filename, const std::string &directory) {
	this->filename_  = filename;
	this->directory_ = directory;
	this->ipos_ = 0;
}

/*
 * Checks whether a specified file exists in the specified directory
 */
bool File::exists(const std::string &filename, const std::string &directory) {
	bool is_exist = false;
	std::fstream file;
	const std::string path = directory + filename;
	file.open(path, std::fstream::out | std::fstream::in);//does not create file

	if (file.is_open()) {
		file.close();
		is_exist = true;
		return is_exist;
	}
	else {
		file.close();
		is_exist = false;
		return is_exist;
	}
}
/*
 * Creates specified file in specified directory and returns status
 */
bool File::create(const std::string &filename, const std::string &directory) {
	bool is_successful = false;
	std::fstream fs;
	const char* fn = nullptr;
	std::string path = directory + filename;

	fn = path.c_str();
	int mode = std::fstream::out;
	if (File::exists(filename, directory) == false) {
		fs.open(fn, mode);
		fs.close();
		is_successful = true;
	}

	return is_successful;
}

/* 
 * Deletes specified file in specified directory and returns status
 * ----------------------------------------------------------------------------------------------
 * NOTE: 
 * > This function is not secure and may delete the file even if it is in use
 * > filename must contain file extension
 * ----------------------------------------------------------------------------------------------
 */
bool File::destroy(const std::string &filename, const std::string &directory) {
	bool is_successful = false;

	std::string temp = directory + filename;
	const char* path = (temp).c_str();
	int status = std::remove(path);
	if (status == 0) {
		is_successful = true;
	}

	return is_successful;
}

bool File::exists()  { return (File::exists ( this->filename_, this->directory_)  ); }
bool File::create()  { return (File::create ( this->filename_, this->directory_)  ); }
bool File::destroy() { return (File::destroy( this->filename_, this->directory_)  ); }

void File::write(const std::string &text) {
	std::fstream fs;
	int mode = std::fstream::out | std::fstream::app;
	const char* path = nullptr;
	std::string temp = (this->directory_) + (this->filename_);
	path = (temp).c_str();
	
	if (this->exists() == true) {
		fs.open(path, mode);
		fs << text;
		fs.close();
	}
}

/*
 * Resets io_flags struct to default settings
 */
void File::reset_i_flags() {
	this->i_flags_.is_bad  = false;
	this->i_flags_.is_eof  = false;
	this->i_flags_.is_good = true;
}

void File::set_i_flags_good() {
	this->i_flags_.is_good = true;
	this->i_flags_.is_bad  = false;
	this->i_flags_.is_eof  = false;
}
void File::set_i_flags_bad() {
	this->i_flags_.is_good = false;
	this->i_flags_.is_bad  = true;
	this->i_flags_.is_eof  = false;
}
void File::set_i_flags_eof() {
	this->i_flags_.is_good = false;
	this->i_flags_.is_bad  = false;
	this->i_flags_.is_eof  = true;
}

/*
 * reads line in current File instance
 * bool* isEoF serves as a container for identifying when end of file is reached
 * returns text read from file in std::string format
 */
void File::read_line(std::string &output) {
	std::fstream fs;
	int mode = std::fstream::in;
	const char* filename = nullptr;
	std::string path = this->directory_ + this->filename_;
	filename = (path).c_str();

	if ( (this->exists()) == true) {
		//clear error flags
		fs.clear();
		this->reset_i_flags();

		fs.open(path, mode);
		fs.seekg(this->ipos_);
		getline(fs, output);

		//check stream status and raise flags where required
		if(fs.good()){
			set_i_flags_good();
		} else if (fs.bad()) {
			set_i_flags_bad();
		} else if (fs.eof()) {
			set_i_flags_eof();
		}

		this->ipos_ = fs.tellg();
		fs.close();
	}
}

void File::clear() {
	this->destroy();
	this->create();
}