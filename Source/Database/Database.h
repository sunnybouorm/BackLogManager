#pragma once
#include "sqlite3.h"
#include "../File_IO.h"

//Database model file
const std::string DB_NAME("db.db3");

class Database {
public:
	//application database directory configuration
	//-----------------------------------------------
	static std::string DB_DIR;
	static std::string DB_DIR_URI;
	//-----------------------------------------------
private:
	const static int defaultFlags = SQLITE_OPEN_URI|SQLITE_OPEN_CREATE|SQLITE_OPEN_READWRITE;
	sqlite3 *db;
public :
	Database();
	bool set_directory(const std::string &dir);
	bool open_connection(const int &flags=defaultFlags);
	bool close_connection();
	bool exterminate();
	bool importSQL(const std::string &fileName, const std::string &fileDir);
};