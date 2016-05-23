#ifndef BACKLOGMANAGER_DATABASE_DATABASE_H_
#define BACKLOGMANAGER_DATABASE_DATABASE_H_

#include "sqlite3.h"
#include "../File_IO.h"

//Database model file
const std::string kDbName("db.db3");

typedef struct DatabaseConnectionStatusFlags{
	bool is_connected    = false;
	bool is_disconnected = true;
	bool is_exist        = false;
} DbCsf;

class Database {
private:
	//directory configuration
	std::string db_dir_;
	std::string db_dir_uri_;
	DbCsf db_csf_;
	const static int default_flags_ = SQLITE_OPEN_URI|SQLITE_OPEN_CREATE|SQLITE_OPEN_READWRITE;
	sqlite3 *db_;

	void ResetCsf();
	void SetCsfConnected();
	void SetCsfDisconnected();
	void SetCsfDeleted();

public :
	Database(const std::string &dir="");
	bool SetDirectory(const std::string &dir);
	bool OpenConnection(const int &flags= default_flags_ );
	bool CloseConnection();
	bool Exterminate();
	bool ImportSql(const std::string &filename, const std::string &filedir);
	bool ExecuteSql(const std::string &statement);
};

#endif // BACKLOGMANAGER_DATABASE_DATABASE_H_