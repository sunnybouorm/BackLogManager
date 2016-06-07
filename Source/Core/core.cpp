#include "../stdafx.h"
#include "core.h"



const DbMap Core::init_db_map() {

	const std::multimap<const std::string, const std::string> DbMapContainer = {
		{ "Activity", "ActivityID"			},	//automatically generated on insert
		{ "Activity", "Name"				},

		{ "Listing"	, "LID"					},	//automatically generated on insert
		{ "Listing"	, "Title"				},
		{ "Listing"	, "ActivityID"			},

		{ "UserDefinedField", "UDFID"		},	//automatically generated on insert
		{ "UserDefinedField", "Name"		},
		{ "UserDefinedField", "DataType"	},
		{ "UserDefinedField", "Description" },
		{ "UserDefinedField", "ActivityID"	},

		{ "UDFentry", "Data"				},
		{ "UDFentry", "UDFID"				},

		{ "Listing_UDFentry", "LID"			},
		{ "Listing_UDFentry", "UDFID"		},
		{ "Listing_UDFentry", "EntryData"	}
	};

	return DbMapContainer;
}

const DbMap Core::kDatabaseMap_ = Core::init_db_map();

const DbMap Core::init_superkey_map() {

	const std::multimap<const std::string, const std::string> DbMapContainer = {
		{ "Activity", "ActivityID" },

		{ "Listing"	, "LID" },

		{ "UserDefinedField", "UDFID" },

		{ "UDFentry", "Data" },
		{ "UDFentry", "UDFID" },

		{ "Listing_UDFentry", "LID" },
		{ "Listing_UDFentry", "UDFID" },
		{ "Listing_UDFentry", "EntryData" }
	};

	return DbMapContainer;
}

const DbMap Core::kSuperkeyMap_ = Core::init_superkey_map();

/*
* Extracts columns from a Rowcontainer object into a different container format
* > Before:
*	vector<ColumnContainer>
*   [ {column_name_0, column_data_0}, {column_name_1, column_data_1}, ...]
* > After
* pair[ vector<string>, vector<string> ]
* [ {column_name_0,column_name_1,...} , {column_data_0,column_data_1,...} ]
*/
std::pair<std::vector<std::string>, std::vector<std::string>> row_2_vect(const RowContainer &row) {
	std::pair<std::vector<std::string>, std::vector<std::string>> result;

	for (auto column = row.begin(); column != row.end(); ++column) {
		result.first.push_back (column->column_name);
		result.second.push_back(column->column_data);
	}

	return result;
}

Core::Core(Database &db) {
	this->database_ = db;
}

/* 
 * Performs a check on columns in query to ensure they are valid and returns true if 
 * valid
 */
bool Core::insert_params_check(QueryContainer query, std::string err_msg) {
	bool is_valid = false;

	return is_valid;
}

/*
 * Selects an appropriate function to handle the SQL query given, (Insert/Delete/Update)
 *
 * parameters:
 * > query: contains all data required to construct an SQL query
 *-------------------------------------------------------------
 * Usage:
 * > Insert request:
 *   query elements required: {table_name,columns,request}
 *   do not include automatically generated primary keys in columns
 * > Delete request:
 *   query elements required: {table_name,search_params,request}
 * > Update request:
 *   query elements required: {table_name,search_params,columns,request}
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 * > Unique IDs are automatically generated when inserting new records, ensure
 *   automatically generated IDs are not included when making an insert request.
 */
bool Core::SqlRequest(QueryContainer &query) {//TODO
	bool is_successful = false;

	std::stringstream ss;
	std::string err_msg;

	ss << "Core Warning: Attempting to make an SQL request with incompatible data";
	err_msg = ss.str();

	//check if table specififed exists in database
	auto table_iter = kDatabaseMap_.find(query.table_name);
	if (table_iter == kDatabaseMap_.end()) { 
		std::cerr << err_msg << ", table <" << query.table_name << "> is invalid\n";
		return is_successful = false;
	}

	//check if query columns specified exist in database
	bool is_exist = false;
	auto table_range = kDatabaseMap_.equal_range(query.table_name);
	for (auto q_col = query.columns.begin(); q_col != query.columns.end(); ++q_col) {
		for (auto db_col = table_range.first; db_col != table_range.second; ++db_col) {
			if (db_col->second == q_col->column_name) { 
				is_exist = true; 
				break;
			}
		}
		if (is_exist == false) {
			std::cerr << err_msg << ", column <" << q_col->column_name << "> is invalid\n";
			return is_successful = false;
		} else {is_exist = false;}
	}

	//check if search params valid where applicable
	if ( (query.request == UPDATE) || (query.request == DELETE) ) {
		if (query.search_params.empty()==true) {
			std::cout	<< err_msg
						<< ", search params cannot be empty when updating or deleting records\n";

			return is_successful = false;
		}
	}

	//select relavent function to handle request
	if ( (query.table_name == "Activity") && (query.request == INSERT) ) {
		std::string key_name = "ActivityID";
		ColumnContainer column;

		int activity_id = this->GenerateUniqueIntId(query.table_name, key_name);
		column.column_name = key_name;
		column.column_data = std::to_string(activity_id);
		query.columns.push_back(column);
		is_successful = this->AddActivity(query);

	} else if ((query.table_name == "Activity") && (query.request == UPDATE)) {
		is_successful = this->UpdateActivity(query);

	} else if ((query.table_name == "Activity") && (query.request == DELETE)) {
		is_successful = this->DeleteActivity(query);
	}


	else if ((query.table_name == "Listing") && (query.request == INSERT)) {
		std::string key_name = "LID";
		ColumnContainer column;

		int lid = this->GenerateUniqueIntId(query.table_name, key_name);
		column.column_name = key_name;
		column.column_data = std::to_string(lid);
		query.columns.push_back(column);
		is_successful = this->AddListing(query);

	} else if ((query.table_name == "Listing") && (query.request == UPDATE)) {
		is_successful = this->UpdateListing(query);

	} else if ((query.table_name == "Listing") && (query.request == DELETE)) {
		is_successful = this->DeleteListing(query);
	}

	else if( (query.table_name == "UserDefinedField") && (query.request == INSERT) ){
		std::string key_name = "UDFID";
		ColumnContainer column;

		int udfid = this->GenerateUniqueIntId(query.table_name, key_name);
		column.column_name = key_name;
		column.column_data = std::to_string(udfid);
		query.columns.push_back(column);
		is_successful = this->AddUserDefinedField(query);
	}
	else if( (query.table_name == "UserDefinedField") && (query.request == UPDATE) ){
		is_successful = this->UpdateUserDefinedField(query);
	}
	else if( (query.table_name == "UserDefinedField") && (query.request == DELETE) ){
		is_successful = this->DeleteUserDefinedField(query);
	}


	else if (query.table_name == "") {

	}
	else if (query.table_name == "") {

	}
	else if (query.table_name == "") {

	}


	else if (query.table_name == "") {

	}
	else if (query.table_name == "") {

	}
	else if (query.table_name == "") {

	}
	else {
		std::cerr << err_msg << ", failed to find SQL operation requested\n" ;
	}

	return is_successful;
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
 * Inserts a new record into a table as specified by the query struct
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 */
bool Core::Insert(QueryContainer &query) {
	bool is_successful = false;
	std::pair< std::vector <std::string>, std::vector <std::string> > temp;

	//error check
	std::stringstream ss;
	ss	<< "Core Warning: Invalid parameters passed to Insert operation"
		<< ", rowsize <" << query.columns.size() << "> is invalid.";
	std::string err_msg = ss.str();
	if (query.columns.size() != kDatabaseMap_.count(query.table_name)) {
		std::cerr << err_msg;
		return is_successful = false;
	}

	temp = row_2_vect(query.columns);
	std::vector <std::string> temp_name_vec = temp.first;
	std::vector <std::string> temp_val_vec = temp.second;

	std::string into_clause;
	into_clause = query.table_name;
	into_clause += "(";
	into_clause += this->CommaSeparate(temp_name_vec);
	into_clause += ")";
	query.into_clause = into_clause;

	std::string value_clause;
	value_clause = "(";
	value_clause += this->CommaSeparate(temp_val_vec, "'");
	value_clause += ")";
	query.value_clause = value_clause;

	is_successful = this->database_.Insert(query);

	return is_successful;
}

/*
* Deletes records from a table as specified by the query struct
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::Delete(QueryContainer &query) {
	bool is_successful = false;
	std::string where_clause, col_name, col_val;

	std::stringstream ss;
	ss << "Core Warning: Invalid parameters passed to Delete operation";
	std::string err_msg = ss.str();

	//error check
	if (query.search_params.size() !=  kSuperkeyMap_.count(query.table_name) )
	{
		std::cerr << err_msg
			<< ", primary keys size<" << query.search_params.size()
			<< "> is invalid"
			<< "\n";

		return is_successful = false;

	}

	col_name = query.search_params.begin()->column_name;
	col_val = query.search_params.begin()->column_data;

	where_clause = col_name;
	where_clause += "=";
	where_clause += "'";
	where_clause += col_val;
	where_clause += "'";
	query.where_clause = where_clause;

	is_successful = this->database_.Delete(query);

	return is_successful;
}


/*
* Updates records in a table as specified by the query struct
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::Update(QueryContainer &query) {
	bool is_successful = false;
	std::string set_clause, where_clause;

	std::stringstream ss;
	ss.clear();
	ss << "Core Warning: Invalid parameters passed to Update operation";
	std::string err_msg = ss.str();

	//error check
	if  (query.columns.size() > kDatabaseMap_.count(query.table_name) && \
		(query.columns.empty() == true))
	{
		std::cerr << err_msg << ", rowsize <" << query.columns.size() << "> is invalid.\n";

		return is_successful = false;
	}

	auto last_col = query.columns.end();
	--last_col;
	for (auto column = query.columns.begin(); column != query.columns.end(); ++column) {
		set_clause += column->column_name;
		set_clause += "=";
		set_clause += "'";
		set_clause += column->column_data;
		set_clause += "'";

		if (column != last_col) { set_clause += ","; }
	}
	query.set_clause = set_clause;

	last_col = query.search_params.end();
	--last_col;
	for (auto key = query.search_params.begin(); key != query.search_params.end(); ++key) {
		where_clause += key->column_name;
		where_clause += "=";
		set_clause += "'";
		where_clause += key->column_data;
		set_clause += "'";

		if (key != last_col) { set_clause += " AND "; }
	}

	query.where_clause = where_clause;
	is_successful = this->database_.Update(query);

	return is_successful;
}

/*
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 */
bool Core::AddActivity(QueryContainer &query) {
	bool is_successful = false;
	is_successful = this->Insert(query);

	return is_successful;
}


/*
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 */
bool Core::DeleteActivity(QueryContainer &query) {
	bool is_successful = false;
	is_successful = this->Delete(query);

	return is_successful;
}

/*
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 */
bool Core::UpdateActivity(QueryContainer &query) {
	bool is_successful = false;
	is_successful = this->Update(query);

	return is_successful;
}

/*
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 */
bool Core::AddListing(QueryContainer &query) {
	bool is_successful = false;
	is_successful = this->Insert(query);

	return is_successful;
}

/*
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 */
bool Core::DeleteListing(QueryContainer &query) {
	bool is_successful = false;
	is_successful = this->Delete(query);

	return is_successful;
}

/*
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 */
bool Core::UpdateListing(QueryContainer &query) {
	bool is_successful = false;
	is_successful = this->Update(query);

	return is_successful;
}


/*
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 * > The attribute Datatype has three options, {"string","int","bool"}
 */
bool Core::AddUserDefinedField(QueryContainer &query) {
	bool is_successful = false;
	is_successful = this->Insert(query);

	return is_successful;
}


/*
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 */
bool Core::DeleteUserDefinedField(QueryContainer &query) {
	bool is_successful = false;
	is_successful = this->Delete(query);

	return is_successful;
}

/*
 * Updates specified UDF record in database
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 */
bool Core::UpdateUserDefinedField(QueryContainer &query) {
	bool is_successful = false;
	is_successful = this->Update(query);

	return is_successful;
}

///*
//* Inserts a new UDFentry record into the database
//*
//* parameters:
//* > row contains the column data to insert into the new record,
//* In this case the primary key UDFID is not compulsory, as it is automatically generated.
//* Check "Database.h" for more information on RowContainer data type
//*
//* psuedo code:
//* > RowContainer row = vector<
//* {column_name = "Data"	, column_data = "[MyData]"	} >;
//* > this.AddUserDefinedField(row);
//*-------------------------------------------------------------
//* Notes:
//* > Requires an open connection to database
//*/
//bool Core::AddUdfEntry(const RowContainer &row) {
//	std::stringstream ss;
//	std::string err_msg;
//
//	ss << "Core Warning: attempting to add UDFentry record with "
//		<< "incompatible data\n";
//	err_msg = ss.str();
//
//	bool is_successful = false;
//	std::string into_clause, value_clause;
//	std::string udfid, data;
//
//	if (row.size() != 2) { std::cerr << err_msg; return is_successful = false; }
//	for (auto column = row.begin(); column != row.end(); ++column) {
//			 if (column->column_name == "UDFID") { udfid = column->column_data; }
//		else if (column->column_name == "Data" ) { data  = column->column_data; }
//		else { 
//			std::cerr << err_msg << ", column < " << column->column_name <<"> is invalid\n";
//			return is_successful = false; 
//		}
//	}
//	if ((udfid.empty() == true) || (data.empty() == true)) {
//		std::cerr << err_msg << ", data and UDFID cannot be NULL\n";
//		return is_successful = false; 
//	}
//
//	std::string table_name, col_name_1, col_name_2;
//	std::vector<std::string> temp_name_vec, temp_val_vec;
//
//	table_name = "UDFentry";
//	col_name_1 = "UDFID";
//	col_name_2 = "Data";
//
//	temp_name_vec = { col_name_1, col_name_2};
//	temp_val_vec  = { udfid		, data};
//
//	QueryContainer query;
//
//	query.table_name = table_name;
//
//	into_clause = query.table_name;
//	into_clause += "(";
//	into_clause += this->CommaSeparate(temp_name_vec);
//	into_clause += ")";
//	query.into_clause = into_clause;
//
//	value_clause = "(";
//	value_clause += this->CommaSeparate(temp_val_vec, "'");
//	value_clause += ")";
//	query.value_clause = value_clause;
//
//	is_successful = this->database_.Insert(query);
//
//	return is_successful;
//}
//
///*
//* Deletes specified UDFentry record from database
//*
//* parameters:
//* > row contains the column data required to identify the record to delete,
//* in this case Data and associated UDFID are required to uniquely identify the record.
//* Check "Database.h" for more information on RowContainer data type
//*
//* psuedo code:
//* > RowContainer row = vector< {column_name = "UDFID", column_data = "[MyUDFID]"} 
//*							{column_name = "Data" , column_data = "[MyData]"} >;
//* > this.DeleteActivity(row);
//*-------------------------------------------------------------
//* Notes:
//* > Requires an open connection to database
//*/
//bool Core::DeleteUdfEntry(const RowContainer &row) {
//	std::stringstream ss;
//	std::string err_msg;
//
//	ss << "Core Warning: Attempting to delete a UDFentry record "
//		<< "with incompatible data\n";
//	err_msg = ss.str();
//
//	bool is_successful = false;
//	std::string where_clause, col_name, col_val, udfid, data;
//	QueryContainer query;
//
//	if (row.size() != 2) { std::cerr << err_msg; return is_successful = false; }
//	for (auto column = row.begin(); column != row.end(); ++column) {
//			 if(column->column_name == "Data")	{ data	= column->column_data;	}
//		else if(column->column_name == "UDFID") { udfid = column->column_data;	}
//		else { std::cerr << err_msg;  return is_successful = false; }
//	}
//	if ((udfid.empty() == true) || (data.empty() == true)) { 
//		std::cerr << err_msg; 
//		return is_successful = false; 
//	}
//
//	query.table_name = "UDFentry";
//
//	col_name = "UDFID";
//	col_val	 = udfid;
//	where_clause += col_name;
//	where_clause += "=";
//	where_clause += "'";
//	where_clause += col_val;
//	where_clause += "'";
//
//	where_clause += " AND ";
//
//	col_name = "Data";
//	col_val	 = data;
//	where_clause += col_name;
//	where_clause += "=";
//	where_clause += "'";
//	where_clause += col_val;
//	where_clause += "'";
//
//	query.where_clause = where_clause;
//	is_successful = this->database_.Delete(query);
//
//	return is_successful;
//}
//
///*
//* Replaces specified UDFentry record in database
//*
//* parameters:
//* > row contains the column data required to identify the record to update. 
//* in this case Data and associated UDFID are required to uniquely identify the record.
//* Check "Database.h" for more information on RowContainer data type
//*
//* psuedo code:
//* > RowContainer row = vector<
//*	{column_name = "UDFID"		,	column_data = "[MyUDFID]"	}
//*	{column_name = "Data"		,	column_data = "[MyName]		} >;
//* > this.UpdateActivity(row);
//*-------------------------------------------------------------
//* Notes:
//* > Requires an open connection to database
//*/
//bool Core::UpdateUdfEntry(const RowContainer &row) {
//	bool is_successful = false;
//
//	std::stringstream ss;
//	std::string err_msg;
//
//	ss << "Core Warning: attempting to update UDFentry record with "
//		<< "incompatible data\n";
//	err_msg = ss.str();
//
//	if (row.size() != 2) {
//		std::cerr << err_msg << ", row size <" << row.size() << "> is invalid\n";
//		return is_successful = false;
//	}
//
//	std::string udfid, data;
//	for (auto column = row.begin(); column != row.end(); ++column) {
//		if (column->column_name		 == "UDFID") { udfid = column->column_data; }
//		else if (column->column_name == "Data")  { data = column->column_data;  }
//		else {
//			std::cerr << err_msg << ", attribute <" << column->column_name
//				<< "> is invalid\n";
//			is_successful = false;
//		}
//	}
//
//	if ( (udfid.empty() == true) || (data.empty() == true)) {
//		std::cerr << err_msg << "primary key UDFID and attribute data cannot be NULL\n";
//		return is_successful = false;
//	}
//
//	std::string set_clause, where_clause;
//	QueryContainer query;
//
//	query.table_name = "UDFentry";
//
//	set_clause += "Data=";
//	set_clause += "'";
//	set_clause += data;
//	set_clause += "'";
//	query.set_clause = set_clause;
//
//	where_clause = "UDFID=";
//	set_clause += "'";
//	where_clause += udfid;
//	set_clause += "'";
//
//	query.where_clause = where_clause;
//	is_successful = this->database_.Update(query);
//
//	return is_successful;
//}
//
//bool AddUdfListingM2M(const RowContainer &row);
//bool InsertUdfListingM2M(const RowContainer &row);
//bool DeleteUdfListingM2M(const RowContainer &row);

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