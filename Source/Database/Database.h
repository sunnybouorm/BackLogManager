#ifndef BACKLOGMANAGER_DATABASE_DATABASE_H_
#define BACKLOGMANAGER_DATABASE_DATABASE_H_

#include "sqlite3.h"
#include "../File_IO.h"

typedef struct ColumnStruct {//stores a single column element
	std::string column_name;
	std::string column_data;
} ColumnContainer;

typedef struct TableStruct {
	std::string table_name;
	std::vector<ColumnContainer> columns;
} TableContainer;

typedef struct SqlRowResultStruct {//stores one entire row of a returned table
	std::vector<ColumnContainer> row_result;
} SqlRowResult;

bool operator==(const SqlRowResult &res1, const SqlRowResult &res2);
bool operator!=(const SqlRowResult &res1, const SqlRowResult &res2);
bool operator==(const std::vector<SqlRowResult> &res1, const std::vector<SqlRowResult> &res2);
bool operator!=(const std::vector<SqlRowResult> &res1, const std::vector<SqlRowResult> &res2);

//Database model file
const std::string kDbName("db.db3");

class Database {

private:
	friend static int StatementCallback(void *db_object, int count, char **data, char **az_col_name);

	//directory configuration
	bool is_connected_ = false;
	std::string db_dir_;
	std::string db_dir_uri_;
	const static int default_flags_ = SQLITE_OPEN_URI|SQLITE_OPEN_CREATE|SQLITE_OPEN_READWRITE;
	sqlite3 *db_;
	std::vector<SqlRowResult> result_buffer_;
	File db_file_;

	void push_to_result_buffer(SqlRowResult value) { this->result_buffer_.push_back(value); }
	void clear_result_buffer() { this->result_buffer_.clear(); }

public :
	Database();
	Database(const std::string &directory);

	void SetDirectory(const std::string &directory);

	bool IsConnected();
	bool OpenConnection(const int &flags= default_flags_ );
	bool CloseConnection();
	bool Exterminate();
	bool ImportSql(const std::string &filename, const std::string &filedir);
	bool ImportSql(File file);
	bool SqlCommand(const std::string &statement);

	bool Insert(const TableContainer &table);
	bool Delete(const TableContainer &table);
	bool Update(const TableContainer &table);

	std::vector<SqlRowResult> read_result_buffer() { return this->result_buffer_; }

	bool is_exist();//checks if this instance's database file exists

	//Tools
	void PrintResultBuffer();
};

#endif // BACKLOGMANAGER_DATABASE_DATABASE_H_