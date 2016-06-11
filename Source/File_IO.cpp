#include "stdafx.h"
#include "file_io.h"

std::istream::streampos operator+= (std::istream::streampos &lhs, int &rhs) {
	return lhs = (lhs + (std::streamoff) rhs);
}

std::istream::streampos operator-= (std::istream::streampos &lhs, int &rhs) {
	return lhs = (lhs - (std::streamoff) rhs);
}

File::File(const std::string &filename, const std::string &directory) {
	this->filename_  = filename;
	this->directory_ = directory;
	this->ipos_ = 0;
}

/*
 * Checks whether a specified file Exists in the specified directory
 */
bool File::Exists(const std::string &filename, const std::string &directory) {
	std::fstream fs;
	bool is_exist = false;
	const std::string path = directory + filename;
	fs.open(path, std::fstream::out | std::fstream::in);//does not Create file

	if (fs.is_open()) {
		fs.close();
		is_exist = true;
		return is_exist;
	}
	else {
		fs.close();
		is_exist = false;
		return is_exist;
	}
}
/*
 * Creates specified file in specified directory and returns success status
 */
bool File::Create(const std::string &filename, const std::string &directory) {
	std::fstream fs;
	bool is_successful = false;
	const char* fn = nullptr;
	std::string path = directory + filename;
	fn = path.c_str();

	int mode = std::fstream::out;
	if (File::Exists(filename, directory) == false) {
		fs.open(fn, mode);
		fs.close();
		is_successful = true;
	} else {
		std::cerr
			<< "File_io Warning: file <"
			<< filename
			<< "> already exists\n";
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
		std::cerr << "File_io Warning: failed to destroy file<" << filename << ">, " 
			      << "in directory<" << directory << ">" << "\n";
	}

	return is_successful;
}

bool File::Exists()  { return (File::Exists ( this->filename_, this->directory_)  ); }
bool File::Create()  { return (File::Create ( this->filename_, this->directory_)  ); }
bool File::Destroy() { return (File::Destroy( this->filename_, this->directory_)  ); }

bool File::Write(const std::string &text) {
	bool is_successful = false;
	std::fstream fs;
	int mode = std::fstream::out | std::fstream::app;
	const char* path = nullptr;
	std::string temp = (this->directory_) + (this->filename_);
	path = (temp).c_str();
	
	if (this->Exists() == true) {
		fs.open(path, mode);
		fs << text;
		fs.close();
		return is_successful = true;
	} else {
		std::cerr << "File_io Warning: attempted to write to a file that does not exist, "
			      << "filename<"  << this->filename_ << ">, " 
				  << "directory<" << this->directory_ << ">" << "\n";
		return is_successful = false;
	}

	return is_successful;
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
 * Reads a character and updates ipos in file object instance
 *
 * returns true if operation is successful
 */
bool File::ReadChar(char &output) {//TODO
	output = NULL;
	bool is_successful = false;
	std::fstream fs;
	int mode = std::fstream::in;
	const char* filename = nullptr;
	std::string path = this->directory_ + this->filename_;
	filename = (path).c_str();

	if ((this->Exists()) == true) {
		//clear error flags
		fs.clear();
		this->ResetInputFlags();

		fs.open(path, mode);
		fs.seekg(this->ipos_);

		char character;
		fs.get(character);

		if (fs.good()) {
			output = character;
			this->ipos_ = fs.tellg();
		}

		if (fs.good()) {
			SetInputFlagsGood();
			is_successful = true;
		}
		else if (fs.bad()) {
			SetInputFlagsBad();
			is_successful = false;
		}
		else if (fs.eof()) {
			SetInputFlagsEof();
			is_successful = true;
		}

		fs.close();
	}
	else {
		std::cerr << "File_io Warning: "
			<< "attempted to ReadChar from a file that does not exist, "
			<< "filename<" << this->filename_ << ">, directory<" << this->directory_
			<< ">" << "\n";
	}

	return is_successful;
}

/*
 * reads current file instance until the specified delimiter is reached
 * output		:	serves as a container for the text read
 * delmiter		:	determines what point to stop reading characters
 * is_inclusive	:	determines whether to include delimiter character(true) or discard it(false)
 *
 * returns true if operation did not raise a bad read flag
 *--------------------------------------------------------------------------------------------------
 * Note:
 * > after ReadToDelimiter() is called, the input stream file pointer ipos is updated to where
 *  the last character was read. A second call of ReadToDelimiter() will continue from the position
 *  determined by ipos_.
 */
bool File::ReadToDelimiter(std::string &output, const char &delimiter, const bool &is_inclusive) {
	output.clear();
	
	bool is_successful = false;
	std::fstream fs;
	int mode = std::fstream::in;
	const char* filename = nullptr;
	std::string path = this->directory_ + this->filename_;
	filename = (path).c_str();

	if ((this->Exists()) == true) {
		//clear error flags
		fs.clear();
		this->ResetInputFlags();

		fs.open(path, mode);
		fs.seekg(this->ipos_);

		char character;
		fs.get(character);
		while (fs.good()) {
			if (character == delimiter) {
				if (is_inclusive == true) {
					if (character != '\r') { output += character; }
				}
				this->ipos_ = fs.tellg();
				break;
			}
			if (character != '\r') { output += character; }
			this->ipos_ = fs.tellg();
			fs.get(character);
		}

		//check stream status and raise flags as necessary
		if (fs.good()) {
			SetInputFlagsGood();
			is_successful = true;
		}
		else if (fs.bad()) {
			SetInputFlagsBad();
			is_successful = false;
		}
		else if (fs.eof()) {
			SetInputFlagsEof();
			is_successful = true;
		}

		fs.close();
	}
	else {
		std::cerr << "File_io Warning: "
			<< "attempted to Read from a file that does not exist, "
			<< "filename<" << this->filename_ << ">, directory<" << this->directory_
			<< ">" << "\n";
	}

	return is_successful;
}

/*
 * reads current file instance until lhs and rhs delimiter are located the returns text in between
 *
 * output		:	serves as a container for the text read
 * lhs delmiter	:	determines what point to start reading characters
 * rhs delmiter	:	determines what point to stop reading characters
 *--------------------------------------------------------------------------------------------------
 * Note:
 * > after ReadBetweenDelimiters() is called, the input stream file pointer ipos is updated to
 * where the last character was read. A second call of ReadBetweenDelimiters() will continue from
 * the position determined by ipos_.
 *
 * returns true if operation did not raise a bad read flag
 */
bool File::ReadBetweenDelimiters(std::string &output, const char &lhs_delimiter, \
	const char &rhs_delimiter)
{
	bool is_successful = false;

	//discard first read where the lhs delimiter is reached
	std::string ignored;
	is_successful = this->ReadToDelimiter(ignored, lhs_delimiter);

	//fetch contents from the current file ipos until rhs delimiter is reached
	is_successful &= this->ReadToDelimiter(output, rhs_delimiter);

	return is_successful;
}

/*
 * reads a line in the current File instance
 * output serves as a container for the text read
 *------------------------------------------------
 * Note:
 * > after ReadLine() is called, the input stream file pointer ipos is updated to where
 *  getline has read. A second call of ReadLine() will continue from the position
 *  determined by ipos.
 *
 * returns true if operation did not raise a bad read flag
 */
bool File::ReadLine(std::string &output) {
	return this->ReadToDelimiter(output, '\n');
}

/*
 * clears a file of its contents by destroying it and recreating it
 */
void File::Clear() {
	this->Destroy();
	this->Create();
}