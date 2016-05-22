#include "stdafx.h"
#include "File_IO.h"

File::File(const std::string &fileName, const std::string &directory) {
	this->fileName  = fileName;
	this->directory = directory;
}

/*
 * Checks whether a specified file exists in the specified directory
 */
bool File::exists(const std::string &fileName, const std::string &directory) {
	bool isExist = false;
	std::fstream file;
	const std::string path = directory + fileName;
	file.open(path, std::fstream::out | std::fstream::in);//does not create file

	if (file.is_open()) {
		file.close();
		isExist = true;
		return isExist;
	}
	else {
		file.close();
		isExist = false;
		return isExist;
	}
}
/*
 * Creates specified file in specified directory and returns status
 */
bool File::create(const std::string &fileName, const std::string &directory) {
	bool isSuccessful = false;
	std::fstream fs;
	const char* fn = nullptr;
	std::string path = directory + fileName;

	fn = path.c_str();
	int mode = std::fstream::out;
	if (File::exists(fileName, directory) == false) {
		fs.open(fn, mode);
		fs.close();
		isSuccessful = true;
	}

	return isSuccessful;
}

/* 
 * Deletes specified file in specified directory and returns status
 * ----------------------------------------------------------------------------------------------
 * NOTE: 
 * > This function is not secure and may delete the file even if it is in use
 * > fileName must contain file extension
 * ----------------------------------------------------------------------------------------------
 */
bool File::destroy(const std::string &fileName, const std::string &directory) {
	bool isSuccessful = false;

	std::string temp = directory + fileName;
	const char* path = (temp).c_str();
	int status = std::remove(path);
	if (status == 0) {
		isSuccessful = true;
	}

	return isSuccessful;
}

bool File::exists()  { return File::exists( this->fileName, this->directory); }
bool File::create()  { return File::create( this->fileName, this->directory); }
bool File::destroy() { return File::destroy(this->fileName, this->directory); }

void File::write(const std::string &text) {
	std::fstream fs;
	int mode = std::fstream::out | std::fstream::app;
	const char* filename = nullptr;
	std::string path = this->directory + this->fileName;
	filename = (path).c_str();
	
	if (this->exists() == true) {
		fs.open(fileName, mode);
		fs << text;
		fs.close();
	}
}

std::string File::read_line() {
	std::string text;

	std::fstream fs;
	int mode = std::fstream::in;
	const char* filename = nullptr;
	std::string path = this->directory + this->fileName;
	filename = (path).c_str();

	fs.open(path,mode);
	getline(fs,text);
	fs.close();

	return text;
}

void File::clear() {
	this->destroy();
	this->create();
}