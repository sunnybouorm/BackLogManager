#include "../stdafx.h"
#include "core.h"

Core::Core(Database &db) {
	this->database_ = db;
}

/*
* Finds and return unused integer ID for a specified table entry to occupy
*-----------------------------------------------------------------------------------
* NOTE:
* > id = 0 is reserved, NEVER occupy it;
*/
int Core::GenerateUniqueIntId(const std::string &table_name, const std::string &id_name) {
	int id = 0;
	//fetch occupied ids by ascending order
	TableResult raw_result;
	std::string sql;
	sql  = "SELECT ";
	sql += id_name;
	sql += " FROM ";
	sql += table_name;
	sql += " ORDER BY ";
	sql += id_name;
	sql += " ASC;";

	this->database_.SqlCommand(sql);
	raw_result = this->database_.read_result_buffer();

	//find an unused id value to occupy
	int current_id, previous_id;
	previous_id = 0;
	if (raw_result.empty() == true) { return (id = 1); }
	for (std::vector<RowResult>::iterator i = raw_result.begin(); i != raw_result.end(); ++i)
	{
		auto j = i->begin();
		current_id = std::stoi(j->column_data);
		if ((current_id - previous_id) > 1) { return (id = current_id - 1); }//spot found
		previous_id = current_id;
	}

	return id = (current_id + 1);//there should be an empty spot after the largest id found
}

/*
 * Sepperate given data with commas and enclose by character if specified
 * parameters
 * > data: values to sepperate with commas
 * > character: character used to enclose the values defaults to NULL
 * Example psuedo code: 
 *  > data = vector<string> {one, two, three};
 *	> char character = ';
 *  > string result = CommaSeparate(data,character);
 *  > output --> 'one','two','three'
 *-----------------------------------------------------------------------------------------
 * NOTE:
 * > The last element of the output result will not have a comma in front of it.
 */
std::string Core::CommaSeparate(std::vector<std::string> &data, const char &character) 
{
	std::string result, element;
	for (std::vector<std::string>::size_type i = 0; i != data.size(); ++i) {
		element = data[i];
		result += character;
		result += element;
		result += character;

		if (i != (data.size() - 1)) { result += ","; }
	}

	return result;
}

bool Core::AddActivity(const std::string &activity_name) {
	bool is_successful = false;
	std::string into_clause, value_clause, table_name, col_name_1, col_name_2;
	std::vector<std::string> temp_name_vec, temp_val_vec;

	QueryContainer to_table;

	table_name	= "Activity";
	col_name_1	= "ActivityID";
	col_name_2	= "Name";

	int activity_id = this->GenerateUniqueIntId(table_name, col_name_1);

	temp_name_vec	= {col_name_1, col_name_2};
	temp_val_vec	= {std::to_string(activity_id), activity_name};

	to_table.table_name	= table_name;

	into_clause = to_table.table_name;
	into_clause += "(";
	into_clause += this->CommaSeparate(temp_name_vec);
	into_clause += ")";
	to_table.into_clause = into_clause;

	value_clause = "(";
	value_clause += this->CommaSeparate(temp_val_vec, '\'');
	value_clause += ")";
	value_clause += ";";
	to_table.value_clause = value_clause;

	is_successful = this->database_.Insert(to_table);

	return is_successful;
}

bool Core::DeleteActivity(const std::string &activity_id) {
	bool is_successful = false;
	std::string where_clause, col_name, col_val;

	QueryContainer from_table;

	from_table.table_name	= "Activity";
	col_name				= "ActivityID";
	col_val					= activity_id;

	where_clause += col_name;
	where_clause += "=";
	where_clause += "'";
	where_clause += col_val;
	where_clause += "'";
	from_table.where_clause = where_clause;

	is_successful = this->database_.Delete(from_table);

	return is_successful;
}

bool Core::AddListing(std::string title, std::string activity_id) {
	std::string table_name, col_name_1, col_name_2, col_name_3, into_clause, value_clause;
	std::vector<std::string> temp_name_vec, temp_val_vec;
	bool is_successful = false;

	table_name = "Listing";
	col_name_1 = "LID";
	col_name_2 = "Title";
	col_name_3 = "ActivityID";

	int activity_id = this->GenerateUniqueIntId(table_name, col_name_1);
	temp_name_vec = {col_name_1, col_name_2, col_name_3};
	temp_val_vec  = {activity_id, title, activity_name };

	QueryContainer to_table;

	to_table.table_name = table_name;

	into_clause  = to_table.table_name;
	into_clause += "(";
	into_clause += this->CommaSeparate(temp_name_vec);
	into_clause += ")";
	to_table.into_clause = into_clause;

	value_clause = "(";
	value_clause += this->CommaSeparate(temp_val_vec,'\'');
	value_clause += ")";
	to_table.value_clause = value_clause;
	
	is_successful = this->database_.Insert(to_table);

	return is_successful;
}

bool Core::DeleteListing(int lid) {
	bool is_successful = false;
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

bool Core::UpdateListing(int lid) {
	return false;
}