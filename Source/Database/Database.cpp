#include "stdafx.h"
#include "database.h"

/*
* Call back function that returns SQL query result
* db_object, pointer to the Database class instance that called it
* argc, The number of columns in row
* argv, An array of strings representing fields in the row
* az_col_name, An array of strings representing column names
*/
static int statement_callback(void *db_object, int argc, char **argv, char **az_col_name) {
	printf("\ncheckpoint3\n");
	int i;
	Database *this_db = static_cast <Database *> (db_object);
	
	//for (i = 0; i < argc; i++) {
	//	printf("%s = %s\n", az_col_name[i], argv[i] ? argv[i] : "NULL");
	//}
	//printf("\n");

	return 0;
}

/*
 * checks whether database file exists, returns true if the file exists
 */
bool Database::is_exist(){
	return File::Exists(kDbName, this->db_dir_);
}

//Constructors
Database::Database(const std::string &dir) {
	this->db_dir_ = dir;
}

/* 
 * Sets database model file directory
 */
bool Database::SetDirectory(const std::string &dir) {
	bool is_successful = true;
	this->db_dir_ = dir;
	this->db_dir_uri_ = "file:///" + dir;

	return is_successful;
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
	} else {
		std::cerr << "Warning: attempting to connect a database is already connected\n";
	}

	return is_successful;
}

bool Database::CloseConnection() {
	bool is_successful = false;
	int status = sqlite3_close_v2(this->db_);
	if (status == SQLITE_OK) {
		is_successful = true;
	}

	return is_successful;
}

/*
* Deletes database model file specified
* filename: must include file extension
*-----------------------------------------------------------------------------------------------
* !!!CAUTION!!!: use only when necessary, specified database is DELETED and CANNOT be recovered
*/
bool Database::Exterminate() {
	bool is_successful = false;

	if (this->is_connected_ == false) {
		is_successful = File::Destroy(kDbName, this->db_dir_);
	}
	else {
		std::cerr   << "Warning: attempting to exterminate database that is already connected"
					<< ", close the database connection first\n";
	}

	return is_successful;
}

/*
 * Executes a single SQL statement and returns status
 */
bool Database::ExecuteSql(const std::string &statement) {
	bool is_successful = false;
	int status;
	char *z_err_msg = 0;
	Database *this_db = this;

	if ( this->is_connected_ == true) {
		const char* sql = statement.c_str();
		std::cout << "\ncheckpoint\n";
		status = sqlite3_exec(this->db_, sql, statement_callback, (void*)this_db, &z_err_msg);
		if (status == SQLITE_OK) {
			is_successful = true;
		}
		else {
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
	File file(filename,filedir);
	std::string sql;
	file.ReadLine(sql);

	//TODO:
	return is_successful;
}