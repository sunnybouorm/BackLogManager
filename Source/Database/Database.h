#ifndef BACKLOGMANAGER_DATABASE_DATABASE_H_
#define BACKLOGMANAGER_DATABASE_DATABASE_H_

#include "sqlite3.h"
#include "../File_IO.h"

//Database model file
const std::string DB_NAME("db.db3");

typedef struct DATABASE_CONNECTION_STATUS_FLAGS{
	bool isConnected    = false;
	bool isDisconnected = true;
	bool isExist        = false;
} DB_CSF;

class Database {
private:
	//directory configuration
	std::string DB_DIR;
	std::string DB_DIR_URI;
	DB_CSF db_csf;
	const static int defaultFlags = SQLITE_OPEN_URI|SQLITE_OPEN_CREATE|SQLITE_OPEN_READWRITE;
	sqlite3 *db;

	void reset_csf();
	void set_csf_connected();
	void set_csf_disconnected();
	void set_csf_deleted();

public :
	Database(const std::string &dir="");
	bool set_directory(const std::string &dir);
	bool open_connection(const int &flags=defaultFlags);
	bool close_connection();
	bool exterminate();
	bool importSQL(const std::string &fileName, const std::string &fileDir);
	bool execSQL(const std::string &statement);
};

#endif // BACKLOGMANAGER_DATABASE_DATABASE_H_