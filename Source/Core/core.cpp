#include "../stdafx.h"
#include "core.h"

Core::Core(Database &db) {
	this->database_ = db;
}

bool Core::AddActivity(const std::string &activity_name) {
	bool is_successful = true;
	std::string into_clause, column_names, values, value_clause;

	QueryContainer to_table;
	ColumnContainer column;

	to_table.table_name		= "Activity";
	column.column_name		= "Name";
	column.column_data		= activity_name;
	to_table.columns.push_back(column);

	for (std::vector<std::string>::size_type i = 0; i != to_table.columns.size(); i++) {
		column_names += to_table.columns[i].column_name;

		values += "'";
		values += to_table.columns[i].column_data;
		values += "'";

		if (i != to_table.columns.size() - 1) {
			column_names += ",";
			values += ",";
		}
	}

	into_clause = to_table.table_name;
	into_clause += "(";
	into_clause += column_names;
	into_clause += ")";
	to_table.into_clause = into_clause;

	value_clause = "(";
	value_clause += values;
	value_clause += ")";
	value_clause += ";";
	to_table.value_clause = value_clause;

	is_successful = this->database_.Insert(to_table);

	return is_successful;
}

bool Core::DeleteActivity(const std::string &activity_name) {
	bool is_successful = true;
	std::string where_clause;

	QueryContainer from_table;
	ColumnContainer column;

	from_table.table_name	= "Activity";
	column.column_name		= "Name";
	column.column_data		= activity_name;
	from_table.columns.push_back(column);

	for (std::vector<std::string>::size_type i = 0; i != from_table.columns.size(); i++) {
		where_clause += from_table.columns[i].column_name;
		where_clause += "=";
		where_clause += "'";
		where_clause += from_table.columns[i].column_data;
		where_clause += "'";
		if (i != from_table.columns.size() - 1) { where_clause += " AND "; }
	}
	from_table.where_clause = where_clause;

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
	std::vector<RowResult> raw_result;
	int current_lid, previous_lid;
	previous_lid = 0;
	raw_result = this->database_.read_result_buffer();
	if (raw_result.empty() == true) { return (lid = 1); }
	for (std::vector<RowResult>::iterator i = raw_result.begin(); i != raw_result.end(); ++i) 
	{
		for (std::vector<ColumnContainer>::iterator j = i->begin();\
			j!=i->end() ; ++j) 
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
	std::string into_clause, column_names, values, value_clause;

	QueryContainer to_table;
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

	for (std::vector<std::string>::size_type i = 0; i != to_table.columns.size(); i++) {
		column_names += to_table.columns[i].column_name;

		values += "'";
		values += to_table.columns[i].column_data;
		values += "'";

		if (i != to_table.columns.size() - 1) {
			column_names += ",";
			values += ",";
		}
	}

	into_clause  = to_table.table_name;
	into_clause += "(";
	into_clause += column_names;
	into_clause += ")";
	to_table.into_clause = into_clause;

	value_clause = "(";
	value_clause += values;
	value_clause += ")";
	to_table.value_clause = value_clause;
	
	is_successful &= this->database_.Insert(to_table);

	return is_successful;
}

bool Core::DeleteListing(int lid) {
	bool is_successful = true;
	std::string where_clause;

	QueryContainer from_table;
	ColumnContainer column;

	from_table.table_name	= "Listing";
	column.column_name		= "LID";
	column.column_data		= std::to_string(lid);
	from_table.columns.push_back(column);

	for (std::vector<std::string>::size_type i = 0; i != from_table.columns.size(); i++) {
		where_clause += from_table.columns[i].column_name;
		where_clause += "=";
		where_clause += "'";
		where_clause += from_table.columns[i].column_data;
		where_clause += "'";
		if (i != from_table.columns.size() - 1) { where_clause += " AND "; }
	}
	from_table.where_clause = where_clause;

	is_successful = this->database_.Delete(from_table);

	return is_successful;
}