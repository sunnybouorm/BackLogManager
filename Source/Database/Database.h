#ifndef BACKLOGMANAGER_DATABASE_DATABASE_H_
#define BACKLOGMANAGER_DATABASE_DATABASE_H_

#include "sqlite3.h"
#include "../File_IO.h"

//Database model file
const std::string kDbName("db.db3");

class Database {
private:
	//directory configuration
	bool is_connected_ = false;
	std::string db_dir_;
	std::string db_dir_uri_;
	const static int default_flags_ = SQLITE_OPEN_URI|SQLITE_OPEN_CREATE|SQLITE_OPEN_READWRITE;
	sqlite3 *db_;

	bool is_exist();//checks if this instance's database file exists

public :
	Database(const std::string &dir="");
	bool IsConnected();
	bool SetDirectory(const std::string &dir);
	bool OpenConnection(const int &flags= default_flags_ );
	bool CloseConnection();
	bool Exterminate();
	bool ImportSql(const std::string &filename, const std::string &filedir);
	bool ExecuteSql(const std::string &statement);
};

#endif // BACKLOGMANAGER_DATABASE_DATABASE_H_