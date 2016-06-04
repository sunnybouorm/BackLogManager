#include "../stdafx.h"
#include "core.h"



const DbMap Core::init_db_map() {

	const std::multimap<const std::string, const std::string> DbMapContainer = {
		{ "Activity", "ActivityID"	},
		{ "Activity", "Name"		},

		{ "Listing"	, "LID"			},
		{ "Listing"	, "Title"		},
		{ "Listing"	, "ActivityID"	},

		{ "UserDefinedField", "UDFID"		},
		{ "UserDefinedField", "Name"		},
		{ "UserDefinedField", "DataType"	},
		{ "UserDefinedField", "Description" },
		{ "UserDefinedField", "ActivityID"	},

		{ "UDFentry", "Data"	},
		{ "UDFentry", "UDFID"	},

		{ "Listing_UDFentry", "LID"			},
		{ "Listing_UDFentry", "UDFID"		},
		{ "Listing_UDFentry", "EntryData"	}
	};

	return DbMapContainer;
}

const DbMap Core::kDatabaseMap = Core::init_db_map();

Core::Core(Database &db) {
	this->database_ = db;
}

/*
* Finds and returns unused integer ID for a specified query entry to occupy
*-----------------------------------------------------------------------------------
* NOTE:
* > id = 0 is reserved, NEVER occupy it;
*/
int Core::GenerateUniqueIntId(const std::string &table_name, const std::string &id_name) {
	int id = 0;
	//fetch occupied ids by ascending order
	TableContainer raw_result;
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
	for (std::vector<RowContainer>::iterator i = raw_result.begin(); i != raw_result.end(); ++i)
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
 * parameters.
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
 * automatically generated. Check "Database.h" for more information on RowContainer data type
 *
 * psuedo code:
 * > RowContainer row = vector< {column_name = "Name", column_data = "[MyactivityName]"} >;
 * > this.AddActivity(row);
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 */
bool Core::AddActivity(const RowContainer &row) {
	std::stringstream ss;
	std::string err_msg_1;

	ss << "Core Warning: Attempting to add an activity record "
		<< "with incompatible data\n";
	err_msg_1 = ss.str();

	bool is_successful = false;
	std::string into_clause, value_clause, table_name, col_name_1, col_name_2, activity_name;
	std::vector<std::string> temp_name_vec, temp_val_vec;
	QueryContainer query;

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

	query.table_name = table_name;

	into_clause  = query.table_name;
	into_clause += "(";
	into_clause += this->CommaSeparate(temp_name_vec);
	into_clause += ")";
	query.into_clause = into_clause;

	value_clause  = "(";
	value_clause += this->CommaSeparate(temp_val_vec, "'");
	value_clause += ")";
	query.value_clause = value_clause;

	is_successful = this->database_.Insert(query);

	return is_successful;
}


/*
* Deletes specified activity record from database
*
* parameters:
* > row contains the column data required to identify the record to delete,
* in this case only an activity ID is required. 
* Check "Database.h" for more information on RowContainer data type
*
* psuedo code:
* > RowContainer row = vector< {column_name = "ActivityID", column_data = "[MyactivityID]"} >;
* > this.DeleteActivity(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::DeleteActivity(const RowContainer &row) {
	std::stringstream ss;
	std::string err_msg_1;

	ss	<< "Core Warning: attempting to delete activity record with "
		<< "incompatible data\n";
	err_msg_1 = ss.str();

	bool is_successful = false;
	std::string where_clause, col_name, col_val, activity_id;
	QueryContainer query;

	if (row.begin()->column_name != "ActivityID") { 
		std::cerr << err_msg_1;
		return is_successful = false; 
	} else if (row.begin()->column_data.empty() == true) { 
		std::cerr << err_msg_1;
		return is_successful = false; 
	}

	activity_id = row.begin()->column_data;
	query.table_name	= "Activity";
	col_name			= "ActivityID";
	col_val				= activity_id;

	where_clause  = col_name;
	where_clause += "=";
	where_clause += "'";
	where_clause += col_val;
	where_clause += "'";
	query.where_clause = where_clause;

	is_successful = this->database_.Delete(query);

	return is_successful;
}

/*
* Updates specified activity record in database
*
* parameters:
* > row contains the column data required to identify the record to update. In this case
* the primary key ActivityID is compulsory, the other attributes are optional as long as a minimum 
* of a single optional attribute is updated.
* Check "Database.h" for more information on RowContainer data type
*
* psuedo code:
* > RowContainer row = vector< 
*	{column_name = "ActivityID",	column_data = "[MyactivityID]"	}
*	{column_name = "Name"		 ,	column_data = "[MyName]"		}>;
* > this.UpdateActivity(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::UpdateActivity(const RowContainer &row) {
	bool is_successful = false;

	std::stringstream ss;
	std::string err_msg_1;

	ss << "Core Warning: attempting to update activity record with "
		<< "incompatible data\n";
	err_msg_1 = ss.str();

	if (row.size() < 2) { std::cerr << err_msg_1; return is_successful = false; }

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
	QueryContainer query;
	query.table_name = "Activity";

	set_clause += "Name=";
	set_clause += "'";
	set_clause += activity_name;
	set_clause += "'";

	query.set_clause = set_clause;

	where_clause  = "ActivityID=";
	set_clause += "'";
	where_clause += activity_id;
	set_clause += "'";

	query.where_clause = where_clause;

	is_successful = this->database_.Update(query);

	return is_successful;
}


/*
* Inserts a new listing record into the database
*
* parameters:
* > row contains the column data to insert into the new listing record,
* in this case only a title and associated activity ID are required, no need to worry about
* the listing ID as it is automatically generated.
* Check "Database.h" for more information on RowContainer data type
*
* psuedo code:
* > RowContainer row = vector< 
* {column_name = "Title"		, column_data = "[MyTitle]"		} 
* {column_name = "ActivityID"	, column_data = "[MyActivityID]"} >;
* > this.AddActivity(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::AddListing(const RowContainer &row) {
	std::string err_msg_1 = "Core Warning: attempting to add listing with incompatible data\n";

	std::string table_name, col_name_1, col_name_2, col_name_3, into_clause, value_clause;
	std::vector<std::string> temp_name_vec, temp_val_vec;
	bool is_successful = false;

	if (row.size() != 2) {
		std::cerr << err_msg_1 << ", size <" << row.size() << "> invalid\n";
		return is_successful = false;
	}

	std::string title, activity_id;
	for (auto column = row.begin(); column != row.end(); ++column) {
		if (column->column_name == "Title") { title = column->column_data; }
		else if (column->column_name == "ActivityID") { activity_id = column->column_data; }
	}

	if ((title.empty() == true) || (activity_id.empty() == true)) {
		std::cerr << err_msg_1 << ", activity or title attributes NULL\n";
		return is_successful = false;
	}

	table_name = "Listing";
	col_name_1 = "LID";
	col_name_2 = "Title";
	col_name_3 = "ActivityID";

	int lid = this->GenerateUniqueIntId(table_name, col_name_1);
	temp_name_vec = {col_name_1, col_name_2, col_name_3};
	temp_val_vec  = {std::to_string(lid), title, activity_id };

	QueryContainer query;

	query.table_name = table_name;

	into_clause  = query.table_name;
	into_clause += "(";
	into_clause += this->CommaSeparate(temp_name_vec);
	into_clause += ")";
	query.into_clause = into_clause;

	value_clause = "(";
	value_clause += this->CommaSeparate(temp_val_vec,"'");
	value_clause += ")";
	query.value_clause = value_clause;
	
	is_successful = this->database_.Insert(query);

	return is_successful;
}

/*
* Deletes specified listing record from database
*
* parameters:
* > row contains the column data required to identify the record to delete,
* in this case only a listing ID is required.
* Check "Database.h" for more information on RowContainer data type
*
* psuedo code:
* > RowContainer row = vector< {column_name = "LID", column_data = "[MyLID]"} >;
* > this.DeleteActivity(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::DeleteListing(const RowContainer &row) {
	std::stringstream ss;
	std::string err_msg_1;

	ss	<< "Core Warning: Attempting to delete a listing record "
		<< "with incompatible data\n";
	err_msg_1 = ss.str();

	bool is_successful = false;
	std::string where_clause, col_name, col_val, lid;
	QueryContainer query;

	if((row.begin()->column_name != "LID") || (row.begin()->column_data.empty() == true)) {
		std::cerr << err_msg_1;
		return is_successful = false;
	}

	lid = row.begin()->column_data;
	query.table_name	= "Listing";
	col_name			= "LID";
	col_val				= lid;

	where_clause += col_name;
	where_clause += "=";
	where_clause += "'";
	where_clause += col_val;
	where_clause += "'";
	query.where_clause = where_clause;

	is_successful = this->database_.Delete(query);

	return is_successful;
}


/*
* Updates specified listing record in database
*
* parameters:
* > row contains the column data required to identify the record to update. In this case
* the primary key LID is compulsory, the other attributes are optional as long as a minimum 
* of a single optional attribute is updated.
* Check "Database.h" for more information on RowContainer data type
*
* psuedo code:
* > RowContainer row = vector<
*	{column_name = "LID"		,	column_data = "[MyLID]"			}
*	{column_name = "Title"		,	column_data = "[MyTitle]		}
*	{column_name = "ActivityID"	,	column_data = "[MyActivityID]"	}>;
* > this.UpdateActivity(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::UpdateListing(const RowContainer &row) {
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
		else { std::cerr << err_msg_1;  is_successful = false; }; 
	}

	if (lid.empty() == true) { std::cerr << err_msg_1; return is_successful = false; }

	std::string set_clause, where_clause;
	QueryContainer query;
	query.table_name = "Listing";

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
	query.set_clause = set_clause;

	where_clause = "LID=";
	set_clause += "'";
	where_clause += lid;
	set_clause += "'";

	query.where_clause = where_clause;
	is_successful = this->database_.Update(query);

	return is_successful;
}


/*
* Inserts a new UserDefinedField record into the database
*
* parameters:
* > row contains the column data to insert into the new record,
* In this case the primary key UDFID is not compulsory, as it is automatically generated.
* Check "Database.h" for more information on RowContainer data type
*
* psuedo code:
* > RowContainer row = vector<
* {column_name = "UDFID"		, column_data = "[MyUDFID]"			}
* {column_name = "Name"			, column_data = "[MyName]"			}
* {column_name = "DataType"		, column_data = "[MyDataType]"		} 
* {column_name = "Description"	, column_data = "[MyDescription]"	} 
* {column_name = "ActivityID"	, column_data = "[MyActivityID]"	} >;
* > this.AddUserDefinedField(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
* > The attribute Datatype has three options, {"string","int","bool"}
*/
bool Core::AddUserDefinedField(const RowContainer &row) {
	std::stringstream ss;
	std::string err_msg_1;

	ss	<< "Core Warning: attempting to add UserDefinedField record with "
		<< "incompatible data\n";
	err_msg_1 = ss.str();

	bool is_successful = false;
	std::string into_clause, value_clause;
	std::string udfid,name,data_type,description,activity_id;
	
	if (row.size() != 4) { 
		std::cerr << err_msg_1 << ", size <"<< row.size() << "> invalid\n"; 
		return is_successful = false; 
	}
	for (auto column = row.begin(); column != row.end(); ++column) {
			 if (column->column_name == "Name")			{ name		 = column->column_data; }
		else if (column->column_name == "DataType")		{ data_type  = column->column_data; }
		else if (column->column_name == "Description")	{description = column->column_data; }
		else if (column->column_name == "ActivityID")	{activity_id = column->column_data; }
		else { 
			std::cerr<<err_msg_1 << ", attribute <" << column->column_name << "> invalid\n";
			return is_successful = false;
		}
	}

	std::string table_name, col_name_1, col_name_2, col_name_3, col_name_4, col_name_5;
	std::vector<std::string> temp_name_vec, temp_val_vec;

	table_name = "UserDefinedField";
	col_name_1 = "UDFID";
	col_name_2 = "Name";
	col_name_3 = "DataType";
	col_name_4 = "Description";
	col_name_5 = "ActivityID";

	udfid = std::to_string(this->GenerateUniqueIntId(table_name, col_name_1));
	temp_name_vec = { col_name_1, col_name_2, col_name_3, col_name_4, col_name_5};
	temp_val_vec  = { udfid, name, data_type, description, activity_id };

	QueryContainer query;

	query.table_name = table_name;

	into_clause = query.table_name;
	into_clause += "(";
	into_clause += this->CommaSeparate(temp_name_vec);
	into_clause += ")";
	query.into_clause = into_clause;

	value_clause = "(";
	value_clause += this->CommaSeparate(temp_val_vec, "'");
	value_clause += ")";
	query.value_clause = value_clause;

	is_successful = this->database_.Insert(query);

	return is_successful;
}


/*
* Deletes specified activity record from database
*
* parameters:
* > row contains the column data required to identify the record to delete,
* in this case only a UDFID is required.
* Check "Database.h" for more information on RowContainer data type
*
* psuedo code:
* > RowContainer row = vector< {column_name = "UDFID", column_data = "[MyUDFID]"} >;
* > this.DeleteActivity(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::DeleteUserDefinedField(const RowContainer &row) {
	std::stringstream ss;
	std::string err_msg_1;

	ss << "Core Warning: Attempting to delete a UDF record "
		<< "with incompatible data\n";
	err_msg_1 = ss.str();

	bool is_successful = false;
	std::string where_clause, col_name, col_val, udfid;
	QueryContainer query;

	if ((row.begin()->column_name != "UDFID") || (row.begin()->column_data.empty() == true)) {
		std::cerr << err_msg_1;
		return is_successful = false;
	}

	udfid = row.begin()->column_data;
	query.table_name = "UserDefinedField";
	col_name = "UDFID";
	col_val = udfid;

	where_clause += col_name;
	where_clause += "=";
	where_clause += "'";
	where_clause += col_val;
	where_clause += "'";
	query.where_clause = where_clause;

	is_successful = this->database_.Delete(query);

	return is_successful;
}

/*
* Updates specified UDF record in database
*
* parameters:
* > row contains the column data required to identify the record to update. In this case
* the primary key UDFID is compulsory, the other attributes are optional as long as a minimum
* of a single optional attribute is updated.
* Check "Database.h" for more information on RowContainer data type
*
* psuedo code:
* > RowContainer row = vector<
*	{column_name = "UDFID"			,	column_data = "[MyUDFID]"		}
*	{column_name = "Name"			,	column_data = "[MyName]		}
*	{column_name = "DataType"		,	column_data = "[MyDataType]"	} 
*	{column_name = "Description"	,	column_data = "[MyDescription]"	} 
*	{column_name = "ActivityID"		,	column_data = "[MyActivityID]"	} >;
* > this.UpdateActivity(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::UpdateUserDefinedField(const RowContainer &row) {
	bool is_successful = false;

	std::stringstream ss;
	std::string err_msg_1;

	ss << "Core Warning: attempting to update UDF record with "
		<< "incompatible data\n";
	err_msg_1 = ss.str();

	if (row.size() < 2) { 
		std::cerr << err_msg_1 << ", row size <" << row.size() <<"> is invalid\n";
		return is_successful = false; 
	}

	std::string udfid, name, description, data_type, activity_id;
	for (auto column = row.begin(); column != row.end(); ++column) {
			 if (column->column_name == "UDFID")		{ udfid = column->column_data;			}
		else if (column->column_name == "Name")			{ name = column->column_data;			}
		else if (column->column_name == "DataType")		{ data_type = column->column_data;		}
		else if (column->column_name == "Description")	{ description = column->column_data;	}
		else if (column->column_name == "ActivityID")	{ activity_id = column->column_data;	}
		else { 
			std::cerr << err_msg_1 << ", attribute <" << column->column_name 
					  << "> is invalid\n";  
			is_successful = false; 
		}
	}

	if (udfid.empty() == true) { 
		std::cerr << err_msg_1 << "primary key UDFID cannot be null\n";
		return is_successful = false; 
	}

	std::string set_clause, where_clause;
	QueryContainer query;
	bool is_next = false;
	query.table_name = "UserDefinedField";

	if (name.empty() == false) {
		set_clause += "Name=";
		set_clause += "'";
		set_clause += name;
		set_clause += "'";
		is_next = true;
	}
	if (data_type.empty() == false) {
		if (is_next == true) {
			set_clause += ",";
		}
		set_clause += "DataType=";
		set_clause += "'";
		set_clause += data_type;
		set_clause += "'";
		is_next = true;
	}
	if (description.empty() == false) {
		if (is_next == true) {
			set_clause += ",";
		}
		set_clause += "Description=";
		set_clause += "'";
		set_clause += description;
		set_clause += "'";
		is_next = true;
	}
	if (activity_id.empty() == false) {
		if (is_next == true) {
			set_clause += ",";
		}
		set_clause += "ActivityID=";
		set_clause += "'";
		set_clause += activity_id;
		set_clause += "'";
	}	
	query.set_clause = set_clause;

	where_clause = "UDFID=";
	set_clause += "'";
	where_clause += udfid;
	set_clause += "'";

	query.where_clause = where_clause;
	is_successful = this->database_.Update(query);

	return is_successful;
}

/*
* Inserts a new UDFentry record into the database
*
* parameters:
* > row contains the column data to insert into the new record,
* In this case the primary key UDFID is not compulsory, as it is automatically generated.
* Check "Database.h" for more information on RowContainer data type
*
* psuedo code:
* > RowContainer row = vector<
* {column_name = "Data"	, column_data = "[MyData]"	} >;
* > this.AddUserDefinedField(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::AddUdfEntry(const RowContainer &row) {
	std::stringstream ss;
	std::string err_msg_1;

	ss << "Core Warning: attempting to add UDFentry record with "
		<< "incompatible data\n";
	err_msg_1 = ss.str();

	bool is_successful = false;
	std::string into_clause, value_clause;
	std::string udfid, data;

	if (row.size() != 2) { std::cerr << err_msg_1; return is_successful = false; }
	for (auto column = row.begin(); column != row.end(); ++column) {
			 if (column->column_name == "UDFID") { udfid = column->column_data; }
		else if (column->column_name == "Data" ) { data  = column->column_data; }
		else { 
			std::cerr << err_msg_1 << ", column < " << column->column_name <<"> is invalid\n";
			return is_successful = false; 
		}
	}
	if ((udfid.empty() == true) || (data.empty() == true)) {
		std::cerr << err_msg_1 << ", data and UDFID cannot be NULL\n";
		return is_successful = false; 
	}

	std::string table_name, col_name_1, col_name_2;
	std::vector<std::string> temp_name_vec, temp_val_vec;

	table_name = "UDFentry";
	col_name_1 = "UDFID";
	col_name_2 = "Data";

	temp_name_vec = { col_name_1, col_name_2};
	temp_val_vec  = { udfid		, data};

	QueryContainer query;

	query.table_name = table_name;

	into_clause = query.table_name;
	into_clause += "(";
	into_clause += this->CommaSeparate(temp_name_vec);
	into_clause += ")";
	query.into_clause = into_clause;

	value_clause = "(";
	value_clause += this->CommaSeparate(temp_val_vec, "'");
	value_clause += ")";
	query.value_clause = value_clause;

	is_successful = this->database_.Insert(query);

	return is_successful;
}

/*
* Deletes specified UDFentry record from database
*
* parameters:
* > row contains the column data required to identify the record to delete,
* in this case Data and associated UDFID are required to uniquely identify the record.
* Check "Database.h" for more information on RowContainer data type
*
* psuedo code:
* > RowContainer row = vector< {column_name = "UDFID", column_data = "[MyUDFID]"} 
*							{column_name = "Data" , column_data = "[MyData]"} >;
* > this.DeleteActivity(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::DeleteUdfEntry(const RowContainer &row) {
	std::stringstream ss;
	std::string err_msg_1;

	ss << "Core Warning: Attempting to delete a UDFentry record "
		<< "with incompatible data\n";
	err_msg_1 = ss.str();

	bool is_successful = false;
	std::string where_clause, col_name, col_val, udfid, data;
	QueryContainer query;

	if (row.size() != 2) { std::cerr << err_msg_1; return is_successful = false; }
	for (auto column = row.begin(); column != row.end(); ++column) {
			 if(column->column_name == "Data")	{ data	= column->column_data;	}
		else if(column->column_name == "UDFID") { udfid = column->column_data;	}
		else { std::cerr << err_msg_1;  return is_successful = false; }
	}
	if ((udfid.empty() == true) || (data.empty() == true)) { 
		std::cerr << err_msg_1; 
		return is_successful = false; 
	}

	query.table_name = "UDFentry";

	col_name = "UDFID";
	col_val	 = udfid;
	where_clause += col_name;
	where_clause += "=";
	where_clause += "'";
	where_clause += col_val;
	where_clause += "'";

	where_clause += " AND ";

	col_name = "Data";
	col_val	 = data;
	where_clause += col_name;
	where_clause += "=";
	where_clause += "'";
	where_clause += col_val;
	where_clause += "'";

	query.where_clause = where_clause;
	is_successful = this->database_.Delete(query);

	return is_successful;
}

/*
* Replaces specified UDFentry record in database
*
* parameters:
* > row contains the column data required to identify the record to update. 
* in this case Data and associated UDFID are required to uniquely identify the record.
* Check "Database.h" for more information on RowContainer data type
*
* psuedo code:
* > RowContainer row = vector<
*	{column_name = "UDFID"		,	column_data = "[MyUDFID]"	}
*	{column_name = "Data"		,	column_data = "[MyName]		} >;
* > this.UpdateActivity(row);
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::UpdateUdfEntry(const RowContainer &row) {
	bool is_successful = false;

	std::stringstream ss;
	std::string err_msg_1;

	ss << "Core Warning: attempting to update UDFentry record with "
		<< "incompatible data\n";
	err_msg_1 = ss.str();

	if (row.size() != 2) {
		std::cerr << err_msg_1 << ", row size <" << row.size() << "> is invalid\n";
		return is_successful = false;
	}

	std::string udfid, data;
	for (auto column = row.begin(); column != row.end(); ++column) {
		if (column->column_name		 == "UDFID") { udfid = column->column_data; }
		else if (column->column_name == "Data")  { data = column->column_data;  }
		else {
			std::cerr << err_msg_1 << ", attribute <" << column->column_name
				<< "> is invalid\n";
			is_successful = false;
		}
	}

	if ( (udfid.empty() == true) || (data.empty() == true)) {
		std::cerr << err_msg_1 << "primary key UDFID and attribute data cannot be NULL\n";
		return is_successful = false;
	}

	std::string set_clause, where_clause;
	QueryContainer query;

	query.table_name = "UDFentry";

	set_clause += "Data=";
	set_clause += "'";
	set_clause += data;
	set_clause += "'";
	query.set_clause = set_clause;

	where_clause = "UDFID=";
	set_clause += "'";
	where_clause += udfid;
	set_clause += "'";

	query.where_clause = where_clause;
	is_successful = this->database_.Update(query);

	return is_successful;
}

bool AddUdfListingM2M(const RowContainer &row);
bool InsertUdfListingM2M(const RowContainer &row);
bool DeleteUdfListingM2M(const RowContainer &row);

//bool Insert(QueryContainer &query) {//TODO
//
//
//
//
//		 if (query.table_name == "Activity"){}
//	else if (query.table_name == "Listing") {}
//	else if (query.table_name == "UserDefinedField") {}
//	else if (query.table_name == "UDFentry") {}
//	else if (query.table_name == "Listing_UDFentry") {}
//	else {}
//}
//
//
//bool Delete(QueryContainer &query);
//bool Update(QueryContainer &query);