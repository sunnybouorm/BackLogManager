#include "stdafx.h"
#include "Database.h"

//static string class member declarations
std::string Database::DB_DIR;
std::string Database::DB_DIR_URI;

//Constructors
Database::Database() {
	//TODO: Any required initialization routines
}

/* 
 * Sets database model file directory
 */
bool Database::set_directory(const std::string &dir) {
	bool isSuccessful = true;
	this->DB_DIR = dir;
	this->DB_DIR_URI = "file:///" + dir;

	return isSuccessful;
}

/*
 * Connects to a given database model file, creates a new database file if file specified
 * does not exist
 * flags  : flags relating to file open operation
 *			refer to https://www.sqlite.org/c3ref/c_open_autoproxy.html,
 *			https://www.sqlite.org/c3ref/open.html
 *-----------------------------------------------------------------------------------------------
 * NOTE: 
 * > ensure every open_connection() is followed by a corresponding close_connection()
 *	before a second open_connection() request is issued
 *-----------------------------------------------------------------------------------------------
 */
bool Database::open_connection(const int &flags) {
	bool isSuccessful = false;
	std::string path = this->DB_DIR_URI + DB_NAME;
	const char *filename = path.c_str();
	const char *zVfs = nullptr ;

	int status = sqlite3_open_v2(filename, &(this->db), flags, zVfs);
	if (status == 0) {
		isSuccessful = true;
	}
	else {
		std::cerr << "failed to establish connection to database: %s\n", sqlite3_errmsg(this->db);
		std::cerr.flush();
	}
	
return		isSuccessful;
}

bool Database::close_connection() {
	bool isSuccessful = false;
	int status = sqlite3_close_v2(this->db);
	if (status == SQLITE_OK) {
		isSuccessful = true;
	} else {
		std::cerr << "failed to close connection to database: %s\n", sqlite3_errmsg(this->db);
		std::cerr.flush();
	}
	return isSuccessful;
}

/*
* Deletes database model file specified
* fileName: must include file extension
*-----------------------------------------------------------------------------------------------
* !!!CAUTION!!!: use only when necessary, specified database is DELETED and CANNOT be recovered
*/
bool Database::exterminate() {
	bool isSuccessful = false;
	isSuccessful = File::destroy(DB_NAME, this->DB_DIR);

	if (isSuccessful == false) {
		std::cerr << "failed to exterminate database: " << sqlite3_errmsg(this->db);
		std::cerr << "\n";
		std::cerr.flush();
	}

	return isSuccessful;
}

/*
 * Reads a text file containing SQL syntax and processes it line by line until EoF or an error
 * is raised
 */
bool Database::importSQL(const std::string &fileName, const std::string &fileDir)
{
	bool isSuccessful = false;
	File file(fileName, fileDir);

	this->open_connection();
	std::string SQL_line;
	//while (SQL_line != ) {
	//	SQL_line = file.read_line();
	//}
	this->close_connection();

	return isSuccessful;
}