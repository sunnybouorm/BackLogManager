#include "../stdafx.h"
#include "core.h"

Core::Core(Database &db) {
	this->database_ = db;
}

bool Core::AddActivity(const std::string &activity_name) {
	bool is_successful = true;
	std::string sql;

	TableContainer to_table;
	ColumnContainer column;

	to_table.table_name		= "Activity";
	column.column_name		= "Name";
	column.column_data		= activity_name;
	to_table.columns.push_back(column);

	is_successful = this->database_.Insert(to_table);

	return is_successful;
}

bool Core::DeleteActivity(const std::string &activity_name) {
	bool is_successful = true;
	std::string sql;

	TableContainer from_table;
	ColumnContainer column;

	from_table.table_name	= "Activity";
	column.column_name		= "Name";
	column.column_data		= activity_name;
	from_table.columns.push_back(column);

	is_successful = this->database_.Delete(from_table);

	return is_successful;
}

//int Core::GenerateLid() {
//	//TODO
//	return 0;
//}
//
//bool Core::AddListing(std::string title, std::string activity_name) {
//	int lid = this->GenerateLid();
//	bool is_successful = true;
//	std::string sql;
//
//	sql = "INSERT INTO Listing VALUES("+ std::to_string(lid) +\
//		"'" + title + "'" + "," + "'" + activity_name + "'" + ")\n";
//	is_successful &= this->database_.ExecuteSql(sql);
//
//	return is_successful;
//}

bool Core::DeleteListing(int lid) {
	bool is_successful = true;

	TableContainer table;
	table.table_name = "Listing";
	table.columns[0].column_name = "LID";
	table.columns[0].column_data = std::to_string(lid);

	is_successful = this->database_.Insert(table);

	return is_successful;
}