#ifndef BACKLOGMANAGER_DATABASE_DATABASE_H_
#define BACKLOGMANAGER_DATABASE_DATABASE_H_

#include "sqlite3.h"
#include "../File_IO.h"

typedef struct ColumnStruct {//stores a single column element
	std::string column_name;
	std::string column_data;
} ColumnContainer;

typedef std::vector <ColumnContainer> RowContainer;
typedef std::vector <RowContainer> TableContainer;

typedef struct QueryStruct {//used to generate an SQL query
	std::string table_name;
	std::vector<ColumnContainer> columns;
	std::vector<std::string> primary_keys;

	std::string select_clause;
	std::string value_clause;
	std::string set_clause;
	std::string where_clause;
	std::string order_by_clause;
	std::string into_clause;

} QueryContainer;

bool operator==(const RowContainer &res1, const RowContainer &res2);
bool operator!=(const RowContainer &res1, const RowContainer &res2);
bool operator==(const std::vector<RowContainer> &res1, const std::vector<RowContainer> &res2);
bool operator!=(const std::vector<RowContainer> &res1, const std::vector<RowContainer> &res2);

//Database model file
const std::string kDbName("db.db3");

class Database {

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

	bool Insert(const QueryContainer &table);
	bool Delete(const QueryContainer &table);
	bool Update(const QueryContainer &table);

	std::vector<RowContainer> read_result_buffer() { return this->result_buffer_; }
	bool is_exist();//checks if this instance's database file exists

	//Diagnostic
	void PrintResultBuffer();

private:
	friend static int StatementCallback(void *db_object, int count, char **data, char **az_col_name);

	void push_to_result_buffer(RowContainer value) { this->result_buffer_.push_back(value); }
	void clear_result_buffer() { this->result_buffer_.clear(); }

	//directory configuration
	bool is_connected_ = false;
	std::string db_dir_;
	std::string db_dir_uri_;
	const static int default_flags_ = SQLITE_OPEN_URI | SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE;
	sqlite3 *db_;
	TableContainer result_buffer_;
	File db_file_;
};

#endif // BACKLOGMANAGER_DATABASE_DATABASE_H_