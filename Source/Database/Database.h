#ifndef BACKLOGMANAGER_DATABASE_DATABASE_H_
#define BACKLOGMANAGER_DATABASE_DATABASE_H_

#include "sqlite3.h"
#include "../File_IO.h"

//Database model file
const std::string db_name("db.db3");

typedef struct DATABASE_CONNECTION_STATUS_FLAGS{
	bool is_connected    = false;
	bool is_disconnected = true;
	bool is_exist        = false;
} DB_CSF;

class Database {
private:
	//directory configuration
	std::string db_dir_;
	std::string db_dir_uri_;
	DB_CSF db_csf_;
	const static int default_flags_ = SQLITE_OPEN_URI|SQLITE_OPEN_CREATE|SQLITE_OPEN_READWRITE;
	sqlite3 *db_;

	void reset_csf();
	void set_csf_connected();
	void set_csf_disconnected();
	void set_csf_deleted();

public :
	Database(const std::string &dir="");
	bool set_directory(const std::string &dir);
	bool open_connection(const int &flags= default_flags_ );
	bool close_connection();
	bool exterminate();
	bool importSQL(const std::string &filename, const std::string &filedir);
	bool execSQL(const std::string &statement);
};

#endif // BACKLOGMANAGER_DATABASE_DATABASE_H_