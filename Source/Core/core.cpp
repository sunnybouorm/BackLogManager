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
		{ "Activity", "ActivityID" },	//automatically generated on insert

		{ "Listing"	, "LID" },			//automatically generated on insert

		{ "UserDefinedField", "UDFID" },//automatically generated on insert

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
*	RowContainer
*   [ {column_name_0, column_data_0}, {column_name_1, column_data_1}, ...]
* > After
* pair[ vector<string>, vector<string> ]
* [ {column_name_0,column_name_1,...} , {column_data_0,column_data_1,...} ]
*/
std::pair<std::vector<std::string>, std::vector<std::string>> row_2_vect(const RowContainer &row) {
	std::pair<std::vector<std::string>, std::vector<std::string>> result;

	for (auto column = row.begin(); column != row.end(); ++column) {
		result.first.push_back (column->first);
		result.second.push_back(column->second);
	}

	return result;
}

bool Core::InitializeFromConfigFile() {//TODO
	std::stringstream ss;

	ss << "Core Warning: Failed to initialize from config file";
	std::string err_msg_1 = ss.str();
	ss.str(std::string());

	ss << ", bad read encountered";
	std::string err_msg_2 = ss.str();
	ss.str(std::string());

	bool is_successful = false;

	if (this->cfg_.ScanAndCache() == false) { 
		std::cerr << err_msg_1 << err_msg_2 << "\n";
		return is_successful = false;
	}

	//set database directory
	std::string db_dir = this->cfg_.get_tag_value("directories", "database directory");
	this->database_.SetDirectory(db_dir);
	this->database_.OpenConnection();

	//set schema directory and import the schema
	std::string schema_fn, schema_dir;
	schema_dir	= this->cfg_.get_tag_value("directories", "schema directory");
	schema_fn	= this->cfg_.get_tag_value("directories", "schema filename");
		
	this->database_.ImportSql(schema_fn, schema_dir);
	this->database_.CloseConnection();

	return  is_successful;
}

Core::Core() {
	this->InitializeFromConfigFile();
	this->database_.OpenConnection();
}

Core::~Core() {
	this->database_.CloseConnection();
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
bool Core::SqlRequest(QueryContainer &query) {
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

		//iterate through predefined mapped columns specific to the table name requested in query
		for (auto db_col = table_range.first; db_col != table_range.second; ++db_col) {
			if (db_col->second == q_col->first) {
				is_exist = true;
				break;
			}
		}
		if (is_exist == false) {
			std::cerr << err_msg << ", column <" << q_col->first << "> is invalid\n";
			return is_successful = false;
		}
		else { is_exist = false; }
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

		std::string key_name, key_data;
		key_name = "ActivityID";

		int activity_id = this->GenerateUniqueIntId(query.table_name, key_name);
		key_data = std::to_string(activity_id);
		query.columns[key_name] = key_data;
		is_successful = this->AddActivity(query);

	} 
	else if ((query.table_name == "Activity") && (query.request == UPDATE)) {
		is_successful = this->UpdateActivity(query);

	} 
	else if ((query.table_name == "Activity") && (query.request == DELETE)) {
		is_successful = this->DeleteActivity(query);
	}


	else if ((query.table_name == "Listing") && (query.request == INSERT)) {
		std::string key_name, key_data;

		key_name = "LID";
		int lid = this->GenerateUniqueIntId(query.table_name, key_name);
		key_data = std::to_string(lid);
		query.columns[key_name] = key_data;
		is_successful = this->AddListing(query);

	} 
	else if ((query.table_name == "Listing") && (query.request == UPDATE)) {
		is_successful = this->UpdateListing(query);

	} 
	else if ((query.table_name == "Listing") && (query.request == DELETE)) {
		is_successful = this->DeleteListing(query);
	}


	else if( (query.table_name == "UserDefinedField") && (query.request == INSERT) ){
		std::string key_name, key_data;

		key_name = "UDFID";
		int udfid = this->GenerateUniqueIntId(query.table_name, key_name);
		key_data = std::to_string(udfid);
		query.columns[key_name] = key_data;
		is_successful = this->AddUserDefinedField(query);
	}
	else if( (query.table_name == "UserDefinedField") && (query.request == UPDATE) ){
		is_successful = this->UpdateUserDefinedField(query);
	}
	else if( (query.table_name == "UserDefinedField") && (query.request == DELETE) ){
		is_successful = this->DeleteUserDefinedField(query);
	}


	else if ( (query.table_name == "UDFentry") && (query.request == INSERT) ){
		is_successful = this->AddUdfEntry(query);
	}
	else if ( (query.table_name == "UDFentry") && (query.request == UPDATE) ) {
		is_successful = this->UpdateUdfEntry(query);
	}
	else if ( (query.table_name == "UDFentry") && (query.request == DELETE) ) {
		is_successful = this->DeleteUdfEntry(query);
	}

	
	else if ((query.table_name == "Listing_UDFentry") && (query.request == INSERT)) {
		is_successful = this->AddUdfListingM2M(query);
	}
	else if ((query.table_name == "Listing_UDFentry") && (query.request == UPDATE)) {
		is_successful = this->UpdateUdfListingM2M(query);
	}
	else if ((query.table_name == "Listing_UDFentry") && (query.request == DELETE)) {
		is_successful = this->DeleteUdfListingM2M(query);
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
	for (TableContainer::iterator row = raw_result.begin(); row != raw_result.end(); ++row)
	{
		auto column = row->begin();
		current_id = std::stoi(column->second);
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
* Deletes a record from a table as specified by the query struct
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

	auto last_col = query.search_params.end();
	--last_col;
	for (auto col = query.search_params.begin(); col != query.search_params.end(); ++col) {
		col_name = col->first;
		col_val	 = col->second;

		where_clause = col_name;
		where_clause += "=";
		where_clause += "'";
		where_clause += col_val;
		where_clause += "'";

		if (col != last_col) { where_clause += " AND "; };
	}
	query.where_clause = where_clause;

	is_successful = this->database_.Delete(query);

	return is_successful;
}


/*
* Updates a record in a table as specified by the query struct
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::Update(QueryContainer &query) {
	bool is_successful = false;
	std::string set_clause, where_clause;

	std::stringstream ss;
	ss.str(std::string());
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
		set_clause += column->first;
		set_clause += "=";
		set_clause += "'";
		set_clause += column->second;
		set_clause += "'";

		if (column != last_col) { set_clause += ","; }
	}
	query.set_clause = set_clause;

	last_col = query.search_params.end();
	--last_col;
	for (auto key = query.search_params.begin(); key != query.search_params.end(); ++key) {
		where_clause += key->first;
		where_clause += "=";
		where_clause += "'";
		where_clause += key->second;
		where_clause += "'";

		if (key != last_col) { where_clause += " AND "; }
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

/*
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 */
bool Core::AddUdfEntry(QueryContainer &query) {
	bool is_successful = false;
	is_successful = this->Insert(query);

	return is_successful;
}

/*
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 */
bool Core::DeleteUdfEntry(QueryContainer &query) {
	bool is_successful = false;
	is_successful = this->Delete(query);

	return is_successful;
}

/*
 *-------------------------------------------------------------
 * Notes:
 * > Requires an open connection to database
 */
bool Core::UpdateUdfEntry(QueryContainer &query) {
	bool is_successful = false;
	is_successful = this->Update(query);

	return is_successful;
}


/*
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::AddUdfListingM2M(QueryContainer &query) {
	bool is_successful = false;
	is_successful = this->Insert(query);

	return is_successful;
}

/*
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::DeleteUdfListingM2M(QueryContainer &query) {
	bool is_successful = false;
	is_successful = this->Delete(query);

	return is_successful;
}

/*
*-------------------------------------------------------------
* Notes:
* > Requires an open connection to database
*/
bool Core::UpdateUdfListingM2M(QueryContainer &query) {
	bool is_successful = false;
	is_successful = this->Update(query);

	return is_successful;
}