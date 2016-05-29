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
 * > characters: characters used to enclose the values defaults to an empty string
 * Example psuedo code: 
 *  > data = vector<string> {one, two, three};
 *	> char character = ';
 *  > string result = CommaSeparate(data,character);
 *  > output --> 'one','two','three'
 *-----------------------------------------------------------------------------------------
 * NOTE:
 * > The last element of the output result will not have a comma in front of it.
 */
std::string Core::CommaSeparate(std::vector<std::string> &data, const std::string &characters) 
{
	std::string result, element;
	for (std::vector<std::string>::size_type i = 0; i != data.size(); ++i) {
		element = data[i];
		result += characters;
		result += element;
		result += characters;

		if (i != (data.size() - 1)) { result += ","; }
	}

	return result;
}

/*
 * Inserts a new activity record into the database
 *
 * parameters:
 * > row contains the column data to insert into the new activity record,
 * in this case only an activity name is required no need to worry about activity_ID as it is 
 * automatically generated. Check "Database.h" for more information on RowResult data type
 *
 * psuedo code:
 * > RowResult row = vector< {column_name = "Name", column_data = "[MyactivityName]"} >;
 * > this.AddActivity(row);
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 */
bool Core::AddActivity(const RowResult &row) {
	std::stringstream ss;
	std::string err_msg_1;

	ss << "Core Warning: Attempting to add an activity record "
		<< "with incompatible data\n";
	err_msg_1 = ss.str();

	bool is_successful = false;
	std::string into_clause, value_clause, table_name, col_name_1, col_name_2, activity_name;
	std::vector<std::string> temp_name_vec, temp_val_vec;
	QueryContainer table;

	if (row.begin()->column_name != "Name") { 
		std::cerr << err_msg_1;
		return is_successful = false;
	} else if (row.begin()->column_data.empty() == true) {
		std::cerr << err_msg_1;
		return is_successful = false;
	}
	activity_name = row.begin()->column_data;

	table_name = "Activity";
	col_name_1 = "ActivityID";
	col_name_2 = "Name";

	int activity_id = this->GenerateUniqueIntId(table_name, col_name_1);

	temp_name_vec = { col_name_1, col_name_2 };
	temp_val_vec  = { std::to_string(activity_id), activity_name };

	table.table_name = table_name;

	into_clause  = table.table_name;
	into_clause += "(";
	into_clause += this->CommaSeparate(temp_name_vec);
	into_clause += ")";
	table.into_clause = into_clause;

	value_clause  = "(";
	value_clause += this->CommaSeparate(temp_val_vec, "'");
	value_clause += ")";
	table.value_clause = value_clause;

	is_successful = this->database_.Insert(table);

	return is_successful;
}


/*
* Deletes specified activity record from database
*
* parameters:
* > row contains the column data required to identify the record to delete,
* in this case only an activity ID is required. 
* Check "Database.h" for more information on RowResult data type
*
* psuedo code:
* > RowResult row = vector< {column_name = "ActivityID", column_data = "[MyactivityID]"} >;
* > this.DeleteActivity(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::DeleteActivity(const RowResult &row) {
	std::stringstream ss;
	std::string err_msg_1;

	ss	<< "Core Warning: attempting to delete activity record with "
		<< "incompatible data\n";
	err_msg_1 = ss.str();

	bool is_successful = false;
	std::string where_clause, col_name, col_val, activity_id;
	QueryContainer table;

	if (row.begin()->column_name != "ActivityID") { 
		std::cerr << err_msg_1;
		return is_successful = false; 
	} else if (row.begin()->column_data.empty() == true) { 
		std::cerr << err_msg_1;
		return is_successful = false; 
	}

	activity_id = row.begin()->column_data;
	table.table_name	= "Activity";
	col_name			= "ActivityID";
	col_val				= activity_id;

	where_clause  = col_name;
	where_clause += "=";
	where_clause += "'";
	where_clause += col_val;
	where_clause += "'";
	table.where_clause = where_clause;

	is_successful = this->database_.Delete(table);

	return is_successful;
}

/*
* Updates specified activity record in database
*
* parameters:
* > row contains the column data required to identify the record to update. In this case
* all relevant attributes of the entity Activity are required.
* Check "Database.h" for more information on RowResult data type
*
* psuedo code:
* > RowResult row = vector< 
*	{column_name = "ActivityID",	column_data = "[MyactivityID]"	}
*	{column_name = "Name"		 ,	column_data = "[MyName]"		}>;
* > this.UpdateActivity(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::UpdateActivity(const RowResult &row) {//TODO
	bool is_successful = false;

	std::stringstream ss;
	std::string err_msg_1;

	ss << "Core Warning: attempting to update activity record with "
		<< "incompatible data\n";
	err_msg_1 = ss.str();

	if (row.size() != 2) { std::cerr << err_msg_1; return is_successful = false; }

	std::string activity_id, activity_name;
	for (auto column = row.begin(); column != row.end(); ++column) {
		if (column->column_name == "ActivityID") { activity_id = column->column_data; }
		else if (column->column_name == "Name")  { activity_name = column->column_data; }
		else { std::cerr << err_msg_1;  is_successful = false; };
	}

	if ((activity_id.empty() == true) || (activity_name.empty() == true)) {
		std::cerr << err_msg_1;
		return is_successful = false;
	}

	std::string set_clause, where_clause;
	QueryContainer table;
	table.table_name = "Activity";

	set_clause += "Name=";
	set_clause += "'";
	set_clause += activity_name;
	set_clause += "'";

	table.set_clause = set_clause;

	where_clause  = "ActivityID=";
	set_clause += "'";
	where_clause += activity_id;
	set_clause += "'";

	table.where_clause = where_clause;

	is_successful = this->database_.Update(table);

	return is_successful;
}


/*
* Inserts a new listing record into the database
*
* parameters:
* > row contains the column data to insert into the new listing record,
* in this case only a title and associated activity ID are required, no need to worry about
* the listing ID as it is automatically generated.
* Check "Database.h" for more information on RowResult data type
*
* psuedo code:
* > RowResult row = vector< 
* {column_name = "Title"		, column_data = "[MyTitle]"		} 
* {column_name = "ActivityID"	, column_data = "[MyActivityID]"} >;
* > this.AddActivity(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::AddListing(const RowResult &row) {
	std::string err_msg_1 = "Core Warning: attempting to add listing with incompatible data\n";

	std::string table_name, col_name_1, col_name_2, col_name_3, into_clause, value_clause;
	std::vector<std::string> temp_name_vec, temp_val_vec;
	bool is_successful = false;

	if (row.size() < 2) {std::cerr << err_msg_1; return is_successful = false;}

	std::string title, activity_id;
	for (auto column = row.begin(); column != row.end(); ++column) {
		if (column->column_name == "Title") { title = column->column_data; }
		else if (column->column_name == "ActivityID") { activity_id = column->column_data; }
	}

	if ((title.empty() == true) || (activity_id.empty() == true)) {
		std::cerr << err_msg_1;
		return is_successful = false;
	}

	table_name = "Listing";
	col_name_1 = "LID";
	col_name_2 = "Title";
	col_name_3 = "ActivityID";

	int lid = this->GenerateUniqueIntId(table_name, col_name_1);
	temp_name_vec = {col_name_1, col_name_2, col_name_3};
	temp_val_vec  = {std::to_string(lid), title, activity_id };

	QueryContainer table;

	table.table_name = table_name;

	into_clause  = table.table_name;
	into_clause += "(";
	into_clause += this->CommaSeparate(temp_name_vec);
	into_clause += ")";
	table.into_clause = into_clause;

	value_clause = "(";
	value_clause += this->CommaSeparate(temp_val_vec,"'");
	value_clause += ")";
	table.value_clause = value_clause;
	
	is_successful = this->database_.Insert(table);

	return is_successful;
}

/*
* Deletes specified listing record from database
*
* parameters:
* > row contains the column data required to identify the record to delete,
* in this case only a listing ID is required.
* Check "Database.h" for more information on RowResult data type
*
* psuedo code:
* > RowResult row = vector< {column_name = "LID", column_data = "[MyLID]"} >;
* > this.DeleteActivity(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::DeleteListing(const RowResult &row) {
	std::stringstream ss;
	std::string err_msg_1;

	ss	<< "Core Warning: Attempting to delete a listing record "
		<< "with incompatible data\n";
	err_msg_1 = ss.str();

	bool is_successful = false;
	std::string where_clause, col_name, col_val, lid;
	QueryContainer table;

	if((row.begin()->column_name != "LID") || (row.begin()->column_data.empty() == true)) {
		std::cerr << err_msg_1;
		return is_successful = false;
	}

	lid = row.begin()->column_data;
	table.table_name	= "Listing";
	col_name			= "LID";
	col_val				= lid;

	where_clause += col_name;
	where_clause += "=";
	where_clause += "'";
	where_clause += col_val;
	where_clause += "'";
	table.where_clause = where_clause;

	is_successful = this->database_.Delete(table);

	return is_successful;
}


/*
* Updates specified listing record in database
*
* parameters:
* > row contains the column data required to identify the record to update. In this case
* all relevant attributes of the entity Listing are required.
* Check "Database.h" for more information on RowResult data type
*
* psuedo code:
* > RowResult row = vector<
*	{column_name = "LID"		,	column_data = "[MyLID]"			}
*	{column_name = "Title"		,	column_data = "[MyTitle]		}
*	{column_name = "ActivityID"	,	column_data = "[MyActivityID]"	}>;
* > this.UpdateActivity(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::UpdateListing(const RowResult &row) {
	bool is_successful = false;

	std::stringstream ss;
	std::string err_msg_1;

	ss << "Core Warning: attempting to update Listing record with "
		<< "incompatible data\n";
	err_msg_1 = ss.str();
	
	if (row.size() < 2) { std::cerr<<err_msg_1;  return is_successful = false; }

	std::string lid, title, activity_id;
	for (auto column = row.begin(); column != row.end(); ++column) {
		if (column->column_name == "LID") { lid = column->column_data; }
		else if (column->column_name == "Title") { title = column->column_data; }
		else if (column->column_name == "ActivityID") {activity_id = column->column_data ; }
		else { std::cerr << err_msg_1;  is_successful = false; }; }

	std::string set_clause, where_clause;
	QueryContainer table;
	table.table_name = "Listing";

	if (title.empty() == false) {
		set_clause += "Title=";
		set_clause += "'";
		set_clause += title;
		set_clause += "'";
	}
	if ((activity_id.empty() == false) && (title.empty() == false)) {
		set_clause += ",";
	}
	if (activity_id.empty() == false) {
		set_clause += "ActivityID=";
		set_clause += "'";
		set_clause += activity_id;
		set_clause += "'";
	}
	table.set_clause = set_clause;

	where_clause = "LID=";
	set_clause += "'";
	where_clause += lid;
	set_clause += "'";

	table.where_clause = where_clause;
	is_successful = this->database_.Update(table);

	return is_successful;
}