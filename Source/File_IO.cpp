#include "stdafx.h"
#include "file_io.h"

File::File(const std::string &filename, const std::string &directory) {
	this->filename_  = filename;
	this->directory_ = directory;
	this->ipos_ = 0;
}

/*
 * Checks whether a specified file Exists in the specified directory
 */
bool File::Exists(const std::string &filename, const std::string &directory) {
	bool is_exist = false;
	std::fstream file;
	const std::string path = directory + filename;
	file.open(path, std::fstream::out | std::fstream::in);//does not Create file

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
 * Creates specified file in specified directory and returns success status
 */
bool File::Create(const std::string &filename, const std::string &directory) {
	bool is_successful = false;
	std::fstream fs;
	const char* fn = nullptr;
	std::string path = directory + filename;
	fn = path.c_str();

	int mode = std::fstream::out;
	if (File::Exists(filename, directory) == false) {
		fs.open(fn, mode);
		fs.close();
		is_successful = true;
	} else {
		std::cerr << "file_io Warning: attempted to create a file that already exists\n";
	}

	return is_successful;
}

/* 
 * Deletes specified file in specified directory and returns success status
 * ----------------------------------------------------------------------------------------------
 * NOTE: 
 * > This function is not secure and may delete the file even if it is in use
 * > filename must contain file extension
 * ----------------------------------------------------------------------------------------------
 */
bool File::Destroy(const std::string &filename, const std::string &directory) {
	bool is_successful = false;

	std::string temp = directory + filename;
	const char* path = (temp).c_str();
	int status = std::remove(path);
	if (status == 0) {
		is_successful = true;
	} else {
		std::cerr << "file_io Warning: failed to destroy file<" << filename << ">, " 
			      << "in directory<" << directory << ">" << "\n";
	}

	return is_successful;
}

bool File::Exists()  { return (File::Exists ( this->filename_, this->directory_)  ); }
bool File::Create()  { return (File::Create ( this->filename_, this->directory_)  ); }
bool File::Destroy() { return (File::Destroy( this->filename_, this->directory_)  ); }

void File::Write(const std::string &text) {
	std::fstream fs;
	int mode = std::fstream::out | std::fstream::app;
	const char* path = nullptr;
	std::string temp = (this->directory_) + (this->filename_);
	path = (temp).c_str();
	
	if (this->Exists() == true) {
		fs.open(path, mode);
		fs << text;
		fs.close();
	} else {
		std::cerr << "file_io Warning: attempted to write to a file that does not exist, "
			      << "filename<"  << this->filename_ << ">, " 
				  << "directory<" << this->directory_ << ">" << "\n";
	}
}

/*
 * Resets io_flags struct to default settings
 */
void File::ResetInputFlags() {
	this->i_flags_.is_bad  = false;
	this->i_flags_.is_eof  = false;
	this->i_flags_.is_good = true;
}

void File::SetInputFlagsGood() {
	this->i_flags_.is_good = true;
	this->i_flags_.is_bad  = false;
	this->i_flags_.is_eof  = false;
}
void File::SetInputFlagsBad() {
	this->i_flags_.is_good = false;
	this->i_flags_.is_bad  = true;
	this->i_flags_.is_eof  = false;
}
void File::SetInputFlagsEof() {
	this->i_flags_.is_good = false;
	this->i_flags_.is_bad  = false;
	this->i_flags_.is_eof  = true;
}

/*
 * reads a line in the current File instance
 * output serves as a container for the text read
 *------------------------------------------------
 * Note:
 * > after ReadLine() is called, the input stream file pointer ipos is updated to where
 *  getline has read. A second call of ReadLine() will continue from the position
 *  determined by ipos.
 */
void File::ReadLine(std::string &output) {
	std::fstream fs;
	int mode = std::fstream::in;
	const char* filename = nullptr;
	std::string path = this->directory_ + this->filename_;
	filename = (path).c_str();

	if ( (this->Exists()) == true) {
		//clear error flags
		fs.clear();
		this->ResetInputFlags();

		fs.open(path, mode);
		fs.seekg(this->ipos_);
		getline(fs, output);

		//check stream status and raise flags as necessary
		if(fs.good()){
			SetInputFlagsGood();
		} else if (fs.bad()) {
			SetInputFlagsBad();
		} else if (fs.eof()) {
			SetInputFlagsEof();
		}

		this->ipos_ = fs.tellg();
		fs.close();
	} else {
		std::cerr   << "file_io Warning: "
					<< "attempted to ReadLine from a file that does not exist, "
					<< "filename<" << this->filename_ << ">, directory<" << this->directory_
					<< ">" << "\n";
	}
}

void File::Clear() {
	this->Destroy();
	this->Create();
}