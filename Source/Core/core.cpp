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

/*
 * Finds and return unused lid integer for a Listing entity table entry to ocuppy
 *-----------------------------------------------------------------------------------
 * NOTE:
 * > lid = 0 is reserved, NEVER occupy it;
 */
int Core::GenerateLid() {
	int lid = 0;
	//fetch lids by ascending order
	this->database_.SqlCommand("SELECT LID FROM Listing ORDER BY LID ASC;");

	//find an unused lid value to occupy
	std::vector<SqlRowResult> raw_result;
	int current_lid, previous_lid;
	previous_lid = 0;
	raw_result = this->database_.read_result_buffer();
	if (raw_result.empty() == true) { return (lid = 1); }
	for (std::vector<SqlRowResult>::iterator i = raw_result.begin(); i != raw_result.end(); ++i) 
	{
		for (std::vector<ColumnContainer>::iterator j = i->row_result.begin();\
			j!=i->row_result.end() ; ++j) 
		{
			current_lid = std::stoi(j->column_data);
			if ((current_lid - previous_lid) > 1) { return (lid = current_lid-1); }//spot found
			current_lid = previous_lid;
		}
	}

	return lid = current_lid++;//there should be an empty spot after the largest lid found
}

bool Core::AddListing(std::string title, std::string activity_name) {
	int lid = this->GenerateLid();
	bool is_successful = true;
	std::string sql;

	TableContainer to_table;
	ColumnContainer column;

	to_table.table_name = "Listing";

	column.column_name = "LID";
	column.column_data = std::to_string(lid);
	to_table.columns.push_back(column);

	column.column_name = "Title";
	column.column_data = title;
	to_table.columns.push_back(column);

	column.column_name = "ActivityName";
	column.column_data = activity_name;
	to_table.columns.push_back(column);
	
	is_successful &= this->database_.Insert(to_table);

	return is_successful;
}

bool Core::DeleteListing(int lid) {
	bool is_successful = true;
	std::string sql;

	TableContainer from_table;
	ColumnContainer column;

	from_table.table_name	= "Listing";
	column.column_name		= "LID";
	column.column_data		= std::to_string(lid);
	from_table.columns.push_back(column);

	is_successful = this->database_.Delete(from_table);

	return is_successful;
}