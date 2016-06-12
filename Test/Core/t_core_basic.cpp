#include "../../Source/stdafx.h"
#include "t_core.h"

SCENARIO("A single activity is added, updated and deleted") 
{
	GIVEN("a clean database") 
	{
		Core core;
		core.database_.OpenConnection();

		WHEN("an activity is added")
		{
			QueryContainer	query;
			std::string column_name, column_data;
			RowContainer	row;
			RowContainer	search_params;

			bool creation_is_success = false;
			
			std::string table_name	  = "Activity";
			std::string activity_name = "Movies";
			std::string activity_id	  = "1";

			column_name		 = "Name";
			column_data		 = activity_name;
			
			row[column_name] = column_data;

			query.table_name = table_name;
			query.columns	 = row;
			query.request	 = INSERT;			

			creation_is_success = core.SqlRequest(query);
			row.clear();

			//generate expected result as a basis of comparison
			//-------------------------------------------------
			RowContainer row_res;

			column_name = "ActivityID";
			column_data = activity_id;
			row_res[column_name] = column_data;

			column_name = "Name";
			column_data = activity_name;
			row_res[column_name] = column_data;

			TableContainer expected;
			expected.push_back(row_res);
			row_res.clear();
			//-------------------------------------------------

			core.database_.SqlCommand("SELECT * FROM Activity\n");
			TableContainer result = core.database_.read_result_buffer();

			THEN("it must be registered by the database correctly")
			{
				REQUIRE(creation_is_success == true);
				REQUIRE(expected == result);

				AND_WHEN("the activity is updated")
				{
					bool update_is_successful = false;
					std::string activity_id;
					std::string column_name, column_data;

					core.database_.SqlCommand("SELECT ActivityID FROM Activity");
					result = core.database_.read_result_buffer();
					REQUIRE(result.begin()->count("ActivityID") == 1);
					
					activity_id = result.begin()->at("ActivityID");
					column_name = "ActivityID";
					column_data = activity_id;
					search_params.clear();
					search_params[column_name] = column_data;
			
					column_name = "ActivityID";
					column_data = activity_id;
					row[column_name] = column_data;

					column_name = "Name";
					column_data = "TV";
					row[column_name] = column_data;
					
					query.table_name	= table_name;
					query.columns		= row;
					query.search_params = search_params;
					query.request		= UPDATE;

					update_is_successful = core.SqlRequest(query);

					expected.clear();
					expected.push_back(row);
					row.clear();

					core.database_.SqlCommand("SELECT * FROM Activity");
					result = core.database_.read_result_buffer();

					THEN("it must be registed by the database successfully")
					{
						REQUIRE(update_is_successful == true);
						REQUIRE(result == expected);
					}
				}

				AND_WHEN("the activity is deleted")
				{
					bool deletion_is_success = false;
					core.database_.SqlCommand("SELECT ActivityID FROM Activity WHERE Name='Movies';");
					result = core.database_.read_result_buffer();
					std::string activity_id = result.begin()->at("ActivityID");
					column_name = "ActivityID";
					column_data = activity_id;
					row[column_name] = column_data;

					query.columns.clear();
					query.table_name	= table_name;
					query.search_params	= row;
					query.request		= DELETE;

					deletion_is_success = core.SqlRequest(query);
					row.clear();

					core.database_.SqlCommand("SELECT * FROM Activity;");
					TableContainer result = core.database_.read_result_buffer();

					THEN("it must be removed from the database")
					{
						REQUIRE(deletion_is_success == true);
						REQUIRE(result.empty() == true);
					}
				}
			}
		}
		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
		if (core.database_.is_exist()	 == true) { core.database_.Exterminate(); }
	}
}

SCENARIO("Multiple activities are added and deleted")
{
	GIVEN("a clean database")
	{
		Core core;
		core.database_.OpenConnection();
		

		WHEN("multiple activities are added")
		{
			QueryContainer query;
			std::string column_name, column_data;
			RowContainer row;
			bool creation_is_success = false;
			TableContainer expected;

			column_name = "Name";

			std::string table_name = "Activity";

			std::string activity_name1 = "Games";
			column_data = activity_name1;
			
			row[column_name] = column_data;

			query.table_name = table_name;
			query.columns = row;
			query.request = INSERT;
			creation_is_success = core.SqlRequest(query);
			expected.push_back(row);

			std::string activity_name2 = "Music";
			column_data = activity_name2;
			
			row[column_name] = column_data;

			query.columns = row;
			query.request = INSERT;
			creation_is_success &= core.SqlRequest(query);
			expected.push_back(row);

			std::string activity_name3 = "Movies";
			column_data = activity_name3;
			
			row[column_name] = column_data;

			query.columns = row;
			query.request = INSERT;
			creation_is_success &= core.SqlRequest(query);
			expected.push_back(row);

			row.clear();

			core.database_.SqlCommand("SELECT Name FROM Activity\n");
			TableContainer result = core.database_.read_result_buffer();

			THEN("the changes must be regisitered by the database correctly")
			{
				REQUIRE(creation_is_success == true);
				REQUIRE(result == expected);

				AND_WHEN("all activities are deleted")
				{
					bool deletion_is_success = false;
					std::string activity_id_1, activity_id_2, activity_id_3;
					std::string sql;

					sql = "SELECT ActivityID FROM Activity WHERE Name='Games';";
					REQUIRE(core.database_.SqlCommand(sql) == true);
					result = core.database_.read_result_buffer();
					activity_id_1 = result.begin()->at("ActivityID");

					sql = "SELECT ActivityID FROM Activity WHERE Name='Music';";
					REQUIRE(core.database_.SqlCommand(sql) == true);
					result = core.database_.read_result_buffer();
					activity_id_2 = result.begin()->at("ActivityID");

					sql = "SELECT ActivityID FROM Activity WHERE Name='Movies';";
					REQUIRE(core.database_.SqlCommand(sql) == true);
					result = core.database_.read_result_buffer();
					activity_id_3 = result.begin()->at("ActivityID");

					column_name = "ActivityID";
					column_data = activity_id_1;
					row[column_name] = column_data;

					query.search_params = row;
					query.request = DELETE;
					deletion_is_success = core.SqlRequest(query);

					column_data = activity_id_2;
					row[column_name] = column_data;

					query.search_params = row;
					query.request = DELETE;
					deletion_is_success &= core.SqlRequest(query);

					column_data = activity_id_3;
					row[column_name] = column_data;

					query.search_params = row;
					query.request = DELETE;
					deletion_is_success &= core.SqlRequest(query);
					row.clear();

					core.database_.SqlCommand("SELECT Name FROM Activity\n");
					TableContainer result = core.database_.read_result_buffer();
					THEN("all activity records must seize to exist")
					{
						REQUIRE(deletion_is_success == true);
						REQUIRE(result.empty() == true);
					}
				}

				AND_WHEN("specific activities are deleted")
				{
					bool deletion_is_success = false;
					std::string activity_id_1, activity_id_2;
					std::string sql;

					sql = "SELECT ActivityID FROM Activity WHERE Name='Games';";
					REQUIRE(core.database_.SqlCommand(sql) == true);
					result = core.database_.read_result_buffer();
					activity_id_1 = result.begin()->at("ActivityID");

					sql = "SELECT ActivityID FROM Activity WHERE Name='Movies';";
					REQUIRE(core.database_.SqlCommand(sql) == true);
					result = core.database_.read_result_buffer();
					activity_id_2 = result.begin()->at("ActivityID");

					column_name = "ActivityID";
					column_data = activity_id_1;
					row[column_name] = column_data;

					query.search_params = row;
					query.request = DELETE;
					deletion_is_success = core.SqlRequest(query);

					column_data = activity_id_2;
					row[column_name] = column_data;

					query.search_params = row;
					query.request = DELETE;
					deletion_is_success &= core.SqlRequest(query);
					row.clear();

					core.database_.SqlCommand("SELECT Name FROM Activity\n");
					TableContainer result = core.database_.read_result_buffer();

					expected.pop_back();
					expected.erase(expected.begin());
					THEN("only the specified activities must be deleted")
					{
						REQUIRE(deletion_is_success == true);
						REQUIRE(result.empty() == false);
						REQUIRE(result == expected);
					}
				}
			}
		}
		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
		if (core.database_.is_exist()	 == true) { core.database_.Exterminate(); }
	}
}

SCENARIO("A single listing is added and deleted")
{
	GIVEN("a clean database")
	{
		Core core;
		core.database_.OpenConnection();

		QueryContainer	query;
		std::string column_name, column_data, table_name, title;
		std::string activity_name, activity_id, lid;
		RowContainer	row;

		//Activity record 1
		table_name			= "Activity";
		activity_name		= "Movies";
		
		column_name			= "Name";
		column_data			= activity_name;
		row[column_name]	= column_data;

		query.table_name	= table_name;
		query.columns		= row;
		query.request		= INSERT;
		row.clear();

		REQUIRE(core.SqlRequest(query) == true);

		WHEN("a listing is added")
		{
			table_name = "Listing";
			
			bool creation_is_success = false;

			std::string title		= "The Great Escape";
			std::string activity_id = "1";

			column_name = "Title";
			column_data = title;
			row[column_name] = column_data;

			column_name = "ActivityID";
			column_data = activity_id;
			row[column_name] = column_data;

			query.table_name	= table_name;
			query.columns		= row;
			query.request		= INSERT;

			creation_is_success	=  core.SqlRequest(query);

			TableContainer expected;
			expected.push_back(row);
			row.clear();

			core.database_.SqlCommand("SELECT Title,ActivityID FROM Listing;");
			TableContainer result = core.database_.read_result_buffer();

			THEN("it must be registered by the database correctly")
			{
				REQUIRE(creation_is_success == true);
				REQUIRE(result == expected);
				AND_WHEN("the listing is deleted")
				{
					bool deletion_is_success = false;

					core.database_.SqlCommand("SELECT LID FROM Listing\n");
					result = core.database_.read_result_buffer();
					std::string lid = result.begin()->at("LID");
					column_name = "LID";
					column_data = lid;
					
					row[column_name] = column_data;

					query.table_name	= table_name;
					query.search_params = row;
					query.request		= DELETE;
					row.clear();

					deletion_is_success = core.SqlRequest(query);
					REQUIRE(deletion_is_success == true);

					core.database_.SqlCommand("SELECT * FROM Listing\n");
					result = core.database_.read_result_buffer();

					THEN("it must be removed from the database")
					{
						REQUIRE(deletion_is_success == true);
						REQUIRE(result.empty() == true);
					}
				}
			}
		}
		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
		if (core.database_.is_exist()	 == true) { core.database_.Exterminate(); }
	}
}

SCENARIO("Multiple listings are added and deleted")
{
	GIVEN("a database with multiple activity records")
	{
		Core core;
		core.database_.OpenConnection();
		
		QueryContainer	query;
		std::string column_name, column_data, table_name, title;
		std::string activity_name, activity_id, lid;
		RowContainer	row;

		//Activity record 1
		table_name = "Activity";
		activity_name = "Movies";

		column_name = "Name";
		column_data = activity_name;
		row[column_name] = column_data;

		query.table_name = table_name;
		query.columns = row;
		query.request = INSERT;
		row.clear();
		REQUIRE(core.SqlRequest(query) == true);

		//Activity record 2
		table_name = "Activity";
		activity_name = "Games";

		column_name = "Name";
		column_data = activity_name;
		row[column_name] = column_data;

		query.table_name = table_name;
		query.columns = row;
		query.request = INSERT;
		row.clear();

		REQUIRE(core.SqlRequest(query) == true);

		//Activity record 3
		table_name = "Activity";
		activity_name = "TV Shows";

		column_name = "Name";
		column_data = activity_name;
		row[column_name] = column_data;

		query.table_name = table_name;
		query.columns = row;
		query.request = INSERT;
		row.clear();

		REQUIRE(core.SqlRequest(query) == true);


		WHEN("multiple listings are added")
		{
			QueryContainer query;
			std::string column_name, column_data;
			RowContainer row;
			TableContainer expected;
			
			bool creation_is_success = false;

			std::string title1, title2, title3, activity_id_1, activity_id_2, activity_id_3;

			std::string table_name = "Listing";
			query.table_name = table_name;

			title1 = "The Great Escape";
			activity_id_1 = "1";

			column_name = "Title";
			column_data = title1;
			row[column_name] = column_data;

			column_name = "ActivityID";
			column_data = activity_id_1;
			row[column_name] = column_data;

			query.columns = row;
			query.request = INSERT;
			creation_is_success = core.SqlRequest(query);
			expected.push_back(row);

			title2 = "Drackula Souls 3";
			activity_id_2 = "2";

			column_name = "Title";
			column_data = title2;
			row[column_name] = column_data;

			column_name = "ActivityID";
			column_data = activity_id_2;
			row[column_name] = column_data;

			query.columns = row;
			query.request = INSERT;
			creation_is_success &= core.SqlRequest(query);
			expected.push_back(row);			

			title3 = "Ghost Husslers";
			activity_id_3 = "3";

			column_name = "Title";
			column_data = title3;
			row[column_name] = column_data;

			column_name = "ActivityID";
			column_data = activity_id_3;
			row[column_name] = column_data;

			query.columns = row;
			query.request = INSERT;
			creation_is_success &= core.SqlRequest(query);
			expected.push_back(row);

			row.clear();

			core.database_.SqlCommand("SELECT Title,ActivityID FROM Listing\n");
			TableContainer result = core.database_.read_result_buffer();

			THEN("the changes must be regisitered by the database correctly")
			{
				REQUIRE(creation_is_success  == true);
				REQUIRE(expected == result);

				AND_WHEN("all listings are deleted")
				{
					bool is_deleted = true;

					core.database_.SqlCommand("SELECT LID FROM Listing ORDER BY LID ASC");
					result = core.database_.read_result_buffer();
					std::string lid;
					for (auto i = result.begin(); i != result.end(); ++i) {
						lid = i->at("LID");

						column_name = "LID";
						column_data = lid;
						row[column_name] = column_data;

						query.search_params = row;
						query.request = DELETE;
						is_deleted	 &= core.SqlRequest(query);
						row.clear();
						
					}
					THEN("all listing records must seize to exist")
					{
						core.database_.SqlCommand("SELECT * FROM Listing");
						result = core.database_.read_result_buffer();

						REQUIRE(is_deleted == true);
						REQUIRE(result.empty() == true);
					}
				}

				AND_WHEN("specific listings are deleted")
				{
					bool is_deleted = true;
					std::string lid;
					

					core.database_.SqlCommand("SELECT LID FROM Listing WHERE ActivityID= '1';");
					result = core.database_.read_result_buffer();
					lid = result.begin()->at("LID");
					column_name = "LID";
					column_data = lid;
					row[column_name] = column_data;

					query.search_params = row;
					query.request = DELETE;
					is_deleted &= core.SqlRequest(query);
					REQUIRE(is_deleted == true);
					

					core.database_.SqlCommand("SELECT LID FROM Listing WHERE ActivityID= '3';");
					result = core.database_.read_result_buffer();
					lid = result.begin()->at("LID");
					column_name = "LID";
					column_data = lid;
					row[column_name] = column_data;

					query.search_params = row;
					query.request = DELETE;
					is_deleted &= core.SqlRequest(query);
					row.clear();
					REQUIRE(is_deleted == true);
					

					core.database_.SqlCommand("SELECT ActivityID FROM Listing");
					result = core.database_.read_result_buffer();

					THEN("only the specified listings must be deleted")
					{
						REQUIRE(result.size() == 1);
						REQUIRE(result.begin()->at("ActivityID") == "2");
					}
				}
			}
		}
		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
		if (core.database_.is_exist()	 == true) { core.database_.Exterminate(); }
	}
}

SCENARIO("A single listing is updated")
{
	GIVEN("a database with a single activity record and a single listing record")
	{
		Core core;
		core.database_.OpenConnection();

		QueryContainer	query;
		std::string column_name, column_data, table_name, title;
		std::string activity_name, activity_id, lid;
		RowContainer	row;

		//Activity record
		table_name			= "Activity";
		activity_name		= "Movies";

		column_name			= "Name";
		column_data			= activity_name;
		row[column_name]	= column_data;

		query.table_name	= table_name;
		query.columns		= row;
		query.request		= INSERT;
		row.clear();
		REQUIRE(core.SqlRequest(query) == true);

		//Listing record
		table_name = "Listing";
		query.table_name = table_name;
		title = "The Great Escape";
		activity_id = "1";

		column_name = "Title";
		column_data = title;
		row[column_name] = column_data;

		column_name = "ActivityID";
		column_data = activity_id;
		row[column_name] = column_data;

		query.columns = row;
		query.request = INSERT;
		row.clear();

		REQUIRE(core.SqlRequest(query) == true);

		WHEN("the listing is updated")
		{
			bool update_is_successful = false;
			TableContainer result, expected;

			REQUIRE(core.database_.SqlCommand("SELECT LID FROM Listing WHERE Title='The Great Escape'"));
			result = core.database_.read_result_buffer();
			REQUIRE(result.begin()->count("LID") == 1);
			std::string lid = result.begin()->at("LID");

			column_name = "LID";
			column_data = lid;
			row[column_name] = column_data;
			query.search_params = row;
			
			column_name = "Title";
			column_data = "The Terrible Escape";
			row[column_name] = column_data;

			query.columns = row;
			query.request = UPDATE;
			update_is_successful = core.SqlRequest(query);
			expected.push_back(row);
			row.clear();

			THEN("the changes must be registered correctly by the database")
			{
				core.database_.SqlCommand("SELECT LID,Title FROM Listing WHERE Title='The Terrible Escape'");
				result = core.database_.read_result_buffer();

				REQUIRE(update_is_successful == true);
				REQUIRE(result == expected);
			}
		}
		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
		if (core.database_.is_exist() == true) { core.database_.Exterminate(); }
	}
}

SCENARIO("a single user defined field is added, updated and deleted") {
	GIVEN("a database with a two activity records")
	{
		Core core;
		core.database_.OpenConnection();

		QueryContainer	query;
		std::string column_name, column_data, table_name, title;
		std::string activity_name, activity_id, lid;
		RowContainer	row;

		//Activity record 1
		table_name			= "Activity";
		activity_name		= "Movies";

		column_name			= "Name";
		column_data			= activity_name;
		row[column_name]	= column_data;

		query.table_name	= table_name;
		query.columns		= row;
		query.request		= INSERT;
		row.clear();
		REQUIRE(core.SqlRequest(query) == true);

		//Activity record 2
		table_name			= "Activity";
		activity_name		= "TV shows";

		column_name			= "Name";
		column_data			= activity_name;
		row[column_name]	= column_data;

		query.table_name	= table_name;
		query.columns		= row;
		query.request		= INSERT;
		row.clear();
		REQUIRE(core.SqlRequest(query) == true);

		WHEN("a user defined field is added")
		{
			bool is_added_successfully = false;	

			std::string table_name = "UserDefinedField";
			query.table_name = table_name;

			TableContainer result, expected;

			std::string name, data_type, description, activity_id, sql;

			name					= "Genre";
			data_type				= "string";
			description				= "";
			activity_id				= "1";

			column_name				= "Name";
			column_data				= name;
			row[column_name]		= column_data;

			column_name				= "DataType";
			column_data				= data_type;
			row[column_name]		= column_data;

			column_name				= "Description";
			column_data				= description;
			row[column_name]		= column_data;

			column_name				= "ActivityID";
			column_data				= activity_id;
			row[column_name]		= column_data;

			query.columns			= row;
			query.request			= INSERT;
			is_added_successfully	= core.SqlRequest(query);

			expected.clear();
			expected.push_back(row);
			row.clear();

			THEN("the change must be registered by the database correctly")
			{
				sql = "SELECT UDFID FROM UserDefinedField";
				REQUIRE(core.database_.SqlCommand(sql) == true);
				result = core.database_.read_result_buffer();
				REQUIRE(result.begin()->size() == 1);

				std::string  udfid = result.begin()->at("UDFID");

				sql = "SELECT Name,Datatype,Description,ActivityID FROM UserDefinedField";
				REQUIRE(core.database_.SqlCommand(sql) == true);
				result = core.database_.read_result_buffer();
				REQUIRE(is_added_successfully == true);
				REQUIRE(result == expected);

				AND_WHEN("the user defined field is updated")
				{
					bool is_updated_successfully = false;
					name		= "Genres";
					data_type	= "int";
					description = "some description";
					activity_id = "2";

					column_name = "UDFID";
					column_data = udfid;
					row[column_name] = column_data;
					query.search_params = row;			

					column_name = "Name";
					column_data = name;
					row[column_name] = column_data;

					column_name = "DataType";
					column_data = data_type;
					row[column_name] = column_data;

					column_name = "Description";
					column_data = description;
					row[column_name] = column_data;

					column_name = "ActivityID";
					column_data = activity_id;
					row[column_name] = column_data;

					query.columns = row;
					query.request = UPDATE;
					is_updated_successfully = core.SqlRequest(query);

					expected.clear();
					expected.push_back(row);
					row.clear();

					THEN("the change must be registered by the database")
					{
						sql = "SELECT * FROM UserDefinedField";
						REQUIRE(core.database_.SqlCommand(sql) == true);
						result = core.database_.read_result_buffer();

						REQUIRE(is_updated_successfully == true);
						REQUIRE(result == expected);

						AND_WHEN("the user defined field is deleted")
						{
							bool is_deleted_successfully = false;

							column_name = "UDFID";
							column_data = udfid;
							row[column_name] = column_data;
							query.search_params = row;
							query.request = DELETE;
							row.clear();
							is_deleted_successfully = core.SqlRequest(query);

							THEN("it must seize to exist")
							{
								REQUIRE(is_deleted_successfully == true);
								sql = "SELECT * FROM UserDefinedField";
								REQUIRE(core.database_.SqlCommand(sql) == true);
								result = core.database_.read_result_buffer();
								REQUIRE(result.empty() == true);
							}
						}
					}
				}
			}
		}
		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
		if (core.database_.is_exist() == true) { core.database_.Exterminate(); }
	}
}

SCENARIO("a single user defined field data entry is added, updated, and deleted")
{
	GIVEN("a database with a single activity and UDF record")
	{
		Core core;
		core.database_.OpenConnection();

		QueryContainer	query;
		std::string column_name, column_data, table_name, title;
		std::string activity_name, activity_id, lid, name, data_type, description;
		RowContainer	row;

		//Activity record 1
		table_name = "Activity";
		activity_name = "Movies";

		column_name = "Name";
		column_data = activity_name;
		row[column_name] = column_data;

		query.table_name = table_name;
		query.columns = row;
		query.request = INSERT;
		REQUIRE(core.SqlRequest(query) == true);
		row.clear();

		//UDF record
		table_name			= "UserDefinedField";
		name				= "Genre";
		data_type			= "string";
		description			= "";
		activity_id			= "1";

		column_name			= "Name";
		column_data			= name;
		row[column_name]	= column_data;

		column_name			= "DataType";
		column_data			= data_type;
		row[column_name]	= column_data;

		column_name			= "Description";
		column_data			= description;
		row[column_name]	= column_data;

		column_name			= "ActivityID";
		column_data			= activity_id;
		row[column_name]	= column_data;

		query.table_name	= table_name;
		query.columns		= row;
		query.request		= INSERT;

		REQUIRE(core.SqlRequest(query) == true);
		row.clear();

		WHEN("a user defined field data entry is added")
		{
			bool is_added_successfully = false;

			table_name = "UDFentry";
			query.table_name = table_name;

			TableContainer result, expected;
			std::string udfid, data, sql;

			udfid					= "1";
			data					= "Comedy";

			column_name				= "Data";
			column_data				= data;
			row[column_name]		= column_data;

			column_name				= "UDFID";
			column_data				= udfid;
			row[column_name]		= column_data;

			query.table_name		= table_name;
			query.columns			= row;
			query.request			= INSERT;
			is_added_successfully	= core.SqlRequest(query);

			expected.clear();
			expected.push_back(row);

			THEN("the change must be registered by the database")
			{
				sql = "SELECT * FROM UDFentry";
				REQUIRE(core.database_.SqlCommand(sql) == true);
				result = core.database_.read_result_buffer();
				REQUIRE(result.size() == 1);
				REQUIRE(is_added_successfully == true);
				REQUIRE(result == expected);

				AND_WHEN("the data entry is updated")
				{
					bool is_updated_successfully = false;

					query.search_params = row;
					row.clear();

					udfid = "1";
					data  = "Action";

					column_name = "Data";
					column_data = data;
					row[column_name] = column_data;

					column_name = "UDFID";
					column_data = udfid;
					row[column_name] = column_data;
					query.columns		= row;
					query.request = UPDATE;
					
					is_updated_successfully = core.SqlRequest(query);

					expected.clear();
					expected.push_back(row);

					THEN("the change must be registered by the database")
					{
						sql = "SELECT * FROM UDFentry";
						REQUIRE(core.database_.SqlCommand(sql) == true);
						result = core.database_.read_result_buffer();
						REQUIRE(is_updated_successfully == true);
						REQUIRE(result == expected);

						AND_WHEN("the data entry is deleted")
						{
							bool is_deleted_successfully = false;
							query.search_params = row;
							query.request = DELETE;
							row.clear();

							is_deleted_successfully = core.SqlRequest(query);

							THEN("it must seize to exist")
							{
								REQUIRE(is_deleted_successfully == true);
								sql = "SELECT * FROM UDFentry";
								REQUIRE(core.database_.SqlCommand(sql) == true);
								result = core.database_.read_result_buffer();
								REQUIRE(result.empty() == true);
							}
						}
					}
				}
			}
		}
		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
		if (core.database_.is_exist() == true) { core.database_.Exterminate(); }
	}

}

SCENARIO("A single Listing_UDFentry many to many releationship is added updated and deleted") {
	GIVEN("a database with Activity,UDF,Listing, and Entry records inserted") {
		Core core;
		core.database_.OpenConnection();

		QueryContainer	query;
		std::string column_name, column_data, table_name, title, data, udfid;
		std::string activity_name, activity_id, lid, name, data_type, description;
		RowContainer row;

		//Activity record 1
		table_name			= "Activity";
		query.table_name	= table_name;
		activity_name		= "Movies";

		column_name			= "Name";
		column_data			= activity_name;
		row[column_name]	= column_data;

		query.columns		= row;
		query.request		= INSERT;
		REQUIRE(core.SqlRequest(query) == true);
		row.clear();

		//Activity record 2
		table_name			= "Activity";
		query.table_name	= table_name;
		activity_name		= "TV Shows";

		column_name			= "Name";
		column_data			= activity_name;
		row[column_name]	= column_data;

		query.columns		= row;
		query.request		= INSERT;
		REQUIRE(core.SqlRequest(query) == true);
		row.clear();

		//UDF record 1
		table_name			= "UserDefinedField";
		query.table_name = table_name;
		name				= "Genre";
		data_type			= "string";
		description			= "";
		activity_id			= "1";

		column_name			= "Name";
		column_data			= name;
		row[column_name]	= column_data;

		column_name			= "DataType";
		column_data			= data_type;
		row[column_name]	= column_data;

		column_name			= "Description";
		column_data			= description;
		row[column_name]	= column_data;

		column_name			= "ActivityID";
		column_data			= activity_id;
		row[column_name]	= column_data;

		query.columns		= row;
		query.request		= INSERT;

		REQUIRE(core.SqlRequest(query) == true);
		row.clear();

		//UDF record 2
		table_name			= "UserDefinedField";
		query.table_name	= table_name;
		name				= "Genre";
		data_type			= "string";
		description			= "";
		activity_id			= "2";

		column_name			= "Name";
		column_data			= name;
		row[column_name]	= column_data;

		column_name			= "DataType";
		column_data			= data_type;
		row[column_name]	= column_data;

		column_name			= "Description";
		column_data			= description;
		row[column_name]	= column_data;

		column_name			= "ActivityID";
		column_data			= activity_id;
		row[column_name]	= column_data;

		query.columns		= row;
		query.request		= INSERT;

		REQUIRE(core.SqlRequest(query) == true);
		row.clear();

		//Listing record 1
		table_name			= "Listing";
		query.table_name	= table_name;
		title				= "The Great Escape";
		activity_id			= "1";

		column_name			= "Title";
		column_data			= title;
		row[column_name]	= column_data;

		column_name			= "ActivityID";
		column_data			= activity_id;
		row[column_name]	= column_data;

		query.columns = row;
		query.request = INSERT;
		row.clear();

		REQUIRE(core.SqlRequest(query) == true);

		//Listing record 2
		table_name			= "Listing";
		query.table_name	= table_name;
		title				= "The Great Escape";
		activity_id			= "2";

		column_name			= "Title";
		column_data			= title;
		row[column_name]	= column_data;

		column_name			= "ActivityID";
		column_data			= activity_id;
		row[column_name]	= column_data;

		query.columns		= row;
		query.request		= INSERT;
		row.clear();

		REQUIRE(core.SqlRequest(query) == true);

		//UDFentry record 1
		table_name			= "UDFentry";
		query.table_name	= table_name;
		data				= "comedy";
		udfid				= "1";

		column_name			= "Data";
		column_data			= data;
		row[column_name]	= column_data;

		column_name			= "UDFID";
		column_data			= udfid;
		row[column_name]	= column_data;

		query.columns		= row;
		query.request		= INSERT;
		row.clear();

		REQUIRE(core.SqlRequest(query) == true);

		//UDFentry record 2
		table_name				= "UDFentry";
		query.table_name		= table_name;
		data					= "Action";
		udfid					= "2";

		column_name				= "Data";
		column_data				= data;
		row[column_name]		= column_data;

		column_name				= "UDFID";
		column_data				= udfid;
		row[column_name]		= column_data;

		query.columns			= row;
		query.request			= INSERT;
		row.clear();

		REQUIRE(core.SqlRequest(query) == true);

		WHEN("a new Listing_UDFentry record is added") {
			bool is_added_successfully = false;

			table_name				= "Listing_UDFentry";
			query.table_name		= table_name;

			column_name				= "LID";
			column_data				= "1";
			row[column_name]		= column_data;

			column_name				= "UDFID";
			column_data				= "1";
			row[column_name]		= column_data;

			column_name				= "EntryData";
			column_data				= "comedy";
			row[column_name]		= column_data;

			query.columns			= row;
			query.request			= INSERT;
			is_added_successfully	= core.SqlRequest(query);

			TableContainer result, expected;
			std::string sql;

			expected.push_back(row);

			sql = "SELECT * FROM Listing_UDFentry";
			REQUIRE(core.database_.SqlCommand(sql) == true);
			result = core.database_.read_result_buffer();

			THEN("it must be registered by the database correctly") {
				REQUIRE(is_added_successfully == true);
				REQUIRE(result == expected);

				AND_WHEN("a new Listing_UDFentry record is updated") {
					bool is_updated_successfuly = false;

					query.search_params = row;
					row.clear();
					
					column_name = "LID";
					column_data = "2";
					row[column_name] = column_data;

					column_name = "UDFID";
					column_data = "2";
					row[column_name] = column_data;

					column_name = "EntryData";
					column_data = "Action";
					row[column_name] = column_data;

					query.columns = row;
					query.request = UPDATE;
					is_updated_successfuly = core.SqlRequest(query);

					expected.clear();
					expected.push_back(row);

					sql = "SELECT * FROM Listing_UDFentry";
					REQUIRE(core.database_.SqlCommand(sql) == true);
					result = core.database_.read_result_buffer();

					THEN("the changes must be registered by the database correctly") {
						REQUIRE(is_updated_successfuly == true);
						REQUIRE(result == expected);

						AND_WHEN("the record is deleted") {
							bool is_deleted_successfully = false;

							query.search_params = row;
							query.columns.clear();
							query.request = DELETE;
							is_deleted_successfully = core.SqlRequest(query);
							row.clear();

							sql = "SELECT * FROM Listing_UDFentry";
							REQUIRE(core.database_.SqlCommand(sql) == true);
							result = core.database_.read_result_buffer();

							THEN("it must not exist in the database") {
								REQUIRE(is_deleted_successfully == true);
								REQUIRE(result.empty() == true);
							}
						}
					}
				}
			}
		}
		if (core.database_.IsConnected() == true)	{ core.database_.CloseConnection(); }
		if (core.database_.is_exist() == true)		{ core.database_.Exterminate();		}
	}

}