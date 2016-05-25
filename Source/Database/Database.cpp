#include "../stdafx.h"
#include "database.h"

/*overloaded operators*/
//------------------------------------------------------------------------------------------------
bool operator==(const SqlRowResult &res1, const SqlRowResult &res2) {
	if (res1.row_result.size() != res2.row_result.size()) {return false; }
	else {
		for (std::vector<SqlColumnResult>::size_type i = 0; i!= res1.row_result.size() ; i++){

			if (res1.row_result[i].column_data != res2.row_result[i].column_data) { return false; }
			if (res1.row_result[i].column_name != res2.row_result[i].column_name) { return false; }
		}
	}
	return true;
}
bool operator!=(const SqlRowResult &res1, const SqlRowResult &res2) {
	if (res1 == res2) { return false; }
	else { return true; }
}

bool operator==(const std::vector<SqlRowResult> &res1, const std::vector<SqlRowResult> &res2) {
	if (res1.size() != res2.size()) { return false; }
	else {
		for (std::vector<SqlRowResult>::size_type i = 0; i != res1.size(); i++) {
			if (res1[i] != res2[i]) { return false; }
		}
	}
}
bool operator!=(const std::vector<SqlRowResult> &res1, const std::vector<SqlRowResult> &res2) {
	if (res1 == res2) { return false; }
	else { return true; }
}
//------------------------------------------------------------------------------------------------

/*
* Call back function that returns SQL query result
* db_object	:	pointer to the Database class instance that called it
* count		:	The number of columns in the table
* data		:	An array of strings representing fields in the row
* col_names	:	An array of strings representing column names
*/
static int StatementCallback(void *db_object, int count, char **data, char **az_col_name) {
	int i;
	Database *this_db = static_cast <Database *> (db_object);
	SqlColumnResult col_res;
	SqlRowResult row_res;
	for (i = 0; i < count; i++) {
		col_res.column_name = az_col_name[i];
		col_res.column_data = data[i] ? data[i] : "NULL";// if data[i] then data[i] else "NULL"
		row_res.row_result.push_back(col_res);
	}
	//append row result data to database instance that requested the callback
	this_db->push_to_result_buffer(row_res);

	return 0;
}

//Constructors
Database::Database() {
	this->SetDirectory("");
}

Database::Database(const std::string &directory) {
	this->SetDirectory(directory);
}

/*
 * Set the directory of the database file
 */
void Database::SetDirectory(const std::string &directory) {
	this->db_dir_ = directory;
	this->db_dir_uri_ = "file:///" + directory;
	File file(kDbName, directory);
	this->db_file = file;
}

/*
* checks whether database file exists, returns true if the file exists
*/
bool Database::is_exist() {
	return File::Exists(kDbName, this->db_dir_);
}

/*
* checks database connection status, returns true if connected
*/
bool Database::IsConnected() {
	return this->is_connected_;
}

/*
 * Connects to a given database model file, creates a new database file if file specified
 * does not exist
 * flags  : flags relating to file open operation
 *			refer to https://www.sqlite.org/c3ref/c_open_autoproxy.html,
 *			https://www.sqlite.org/c3ref/open.html
 *-----------------------------------------------------------------------------------------------
 * NOTE: 
 * > ensure every OpenConnection() is followed by a corresponding CloseConnection()
 *	before a second OpenConnection() request is issued
 *-----------------------------------------------------------------------------------------------
 */
bool Database::OpenConnection(const int &flags) {
	bool is_successful = false;
	std::string path = this->db_dir_uri_ + kDbName;
	const char *filename = path.c_str();
	const char *zvfs = nullptr ;

	if (this->is_connected_ == false) {

		int status = sqlite3_open_v2(filename, &(this->db_), flags, zvfs);
		if (status == 0) {
			is_successful      = true;
			this->is_connected_ = true;
		}
	}

	return is_successful;
}

bool Database::CloseConnection() {
	bool is_successful = false;
	int status = sqlite3_close_v2(this->db_);
	if (status == SQLITE_OK) {
		is_successful       = true;
		this->is_connected_ = false;
	}

	return is_successful;
}

/*
* Deletes database model file specified
*-----------------------------------------------------------------------------------------------
* !!!CAUTION!!!: use only when necessary, specified database is DELETED and CANNOT be recovered
*/
bool Database::Exterminate() {
	bool is_successful = false;

	if (this->is_connected_ == false) {
		is_successful = File::Destroy(kDbName, this->db_dir_);
	}
	else {
		std::cerr   << "Database Warning: attempting to exterminate database that is already" 
			        << "connected, close the database connection first\n";
	}

	return is_successful;
}

/*
 * Executes a single SQL statement and returns status
 * NOTE:
 * ---------------------------------------------------
 * > The sql result_buffer_ is cleared with every call of this function,
 *		hence any data from result_buffer_ required by user must be processed before making
 *		another call to ExecuteSql()
 */
bool Database::ExecuteSql(const std::string &statement) {
	bool is_successful = false;
	int status;
	char *z_err_msg = 0;
	Database *this_db = this;

	//clear the result_buffer_ prior to StatementCallback() call
	this->clear_result_buffer();

	if ( this->is_connected_ == true) {
		const char* sql = statement.c_str();

		status = sqlite3_exec(this->db_, sql, StatementCallback, (void*)this_db, &z_err_msg);
		if (status == SQLITE_OK) {
			is_successful = true;
		} else {
			is_successful = false;
			std::cerr << "SQL error: " << z_err_msg << "\n";
		}
	}

	return is_successful;
}

/*
 * Reads a text file containing SQL syntax and processes it line by line until EoF or an error
 * is raised
 */
bool Database::ImportSql(const std::string &filename, const std::string &filedir)
{
	bool is_successful = false;
	bool status = false;
	File file(filename, filedir);
	std::string sql;

	while (file.i_flags_.is_good == true) {//loop until EoF reached or bad file operation
		file.ReadLine(sql);
		status = this->ExecuteSql(sql);
		if (status == false) { exit; }
	}

	//if sql execution fails or a bad file read operation then return failure
	if ( (status = false) || (file.i_flags_.is_bad == true) ) {
		is_successful = false; 
		std::cerr << "Database Warning: failed to import SQL file\n";
	} else { is_successful = true; }

	return is_successful;
}

bool Database::ImportSql(File file) {
	bool is_successful = false;
	is_successful = this->ImportSql(file.get_filename(),file.get_directory());

	return is_successful;
}

/*
 * Processes and displays the resulting table of the latest sql query
 */
void Database::PrintResultBuffer() {
	std::cout
		<< "----------------------\n"
		<< "SQL query result:\n"
		<< "Database<" << this->db_dir_uri_ << ">" << "\n"
		<< "----------------------\n";
	if (this->result_buffer_.empty() == false) {
		for (auto row = result_buffer_.begin(); row != result_buffer_.end(); row++) {
			for (auto col = row->row_result.begin(); col != row->row_result.end(); col++) {
				std::cout
					<< "<" << col->column_name	<< ">"
					<< "<" << col->column_data	<< ">"	<< "\n";
			}
			std::cout << "\n";
		}
	}
	else {
		std::cout << "result buffer empty\n";
	}

	std::cout
		<< "______________________\n"
		<< "QUERY END"
		<< "\n\n";
}