#ifndef BACKLOGMANAGER_DATABASE_DATABASE_H_
#define BACKLOGMANAGER_DATABASE_DATABASE_H_

#include "sqlite3.h"
#include "../File_IO.h"

typedef struct SqlColumnResultStruct {//stores a single column element of a returned table
	std::string column_name;
	std::string column_data;
} SqlColumnResult;

typedef struct SqlRowResultStruct {//stores one entire row of a returned table
	std::vector<SqlColumnResult> row_result;
} SqlRowResult;

//inline bool operator==(const SqlRowResult &res1, const SqlRowResult res2);

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
	//std::vector<SqlRowResult> result_buffer_;//processed SQL query result container
	std::vector<SqlRowResult> result_buffer_;

	bool is_exist();//checks if this instance's database file exists

public :

	Database(const std::string &dir = "");
	bool IsConnected();
	bool SetDirectory(const std::string &dir);
	bool OpenConnection(const int &flags= default_flags_ );
	bool CloseConnection();
	bool Exterminate();
	bool ImportSql(const std::string &filename, const std::string &filedir);
	bool ExecuteSql(const std::string &statement);

	void push_to_result_buffer(SqlRowResult value) {this->result_buffer_.push_back(value); }
	void clear_result_buffer() { this->result_buffer_.clear(); }
//	std::vector<SqlColumnResult> read_result_buffer() { return this->result_buffer_; }

	//Tools
	void PrintResultBuffer();
};

#endif // BACKLOGMANAGER_DATABASE_DATABASE_H_