#include "../../Source/stdafx.h"
#include "t_core.h"

std::string db_dir = "D:\\Development\\Projects\\BacklogManager\\Database\\";

SCENARIO("A single activity is added, updated and deleted") 
{
	GIVEN("a clean database") 
	{
		std::string sql_filename = "BacklogManager.sql";
		File sql_file(sql_filename, db_dir);

		Database database(db_dir);
		Core core(database);
		if (core.database_.IsConnected()	== true) { core.database_.CloseConnection(); }
		if (core.database_.is_exist()		== true) { core.database_.Exterminate(); }

		core.database_.OpenConnection();
		core.database_.ImportSql(sql_file);

		WHEN("an activity is added")
		{
			QueryContainer	query;
			ColumnContainer column;
			RowContainer	row;
			RowContainer	search_params;

			bool creation_is_success = false;
			
			std::string table_name	  = "Activity";
			std::string activity_name = "Movies";
			std::string activity_id	  = "1";

			column.column_name		  = "Name";
			column.column_data		  = activity_name;
			row.clear();
			row.push_back(column);

			query.table_name = table_name;
			query.columns	 = row;
			query.request	 = INSERT;

			creation_is_success = core.SqlRequest(query);

			//generate expected result as a basis of comparison
			//-------------------------------------------------
			ColumnContainer col_res;
			RowContainer row_res;

			col_res.column_name = "ActivityID";
			col_res.column_data = activity_id;
			row_res.push_back(col_res);

			col_res.column_name = "Name";
			col_res.column_data = activity_name;
			row_res.push_back(col_res);

			TableContainer expected;
			expected.push_back(row_res);
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
					ColumnContainer column;

					core.database_.SqlCommand("SELECT ActivityID FROM Activity");
					result = core.database_.read_result_buffer();
					REQUIRE(result.begin()->begin()->column_name == "ActivityID");
					
					activity_id = result.begin()->begin()->column_data;
					column.column_name = "ActivityID";
					column.column_data = activity_id;
					search_params.clear();
					search_params.push_back(column);

					row.clear();
					column.column_name = "ActivityID";
					column.column_data = activity_id;
					row.push_back(column);

					column.column_name = "Name";
					column.column_data = "TV";
					row.push_back(column);
					
					query.table_name	= table_name;
					query.columns		= row;
					query.search_params = search_params;
					query.request		= UPDATE;

					update_is_successful = core.SqlRequest(query);

					expected.clear();
					expected.push_back(row);

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
					std::string activity_id = result.begin()->begin()->column_data;
					column.column_name = "ActivityID";
					column.column_data = activity_id;
					row.clear();
					row.push_back(column);

					query.columns.clear();
					query.table_name	= table_name;
					query.search_params	= row;
					query.request		= DELETE;

					deletion_is_success = core.SqlRequest(query);

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
		std::string sql_filename = "BacklogManager.sql";
		File sql_file(sql_filename, db_dir);

		Database database(db_dir);
		Core core(database);
		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
		if (core.database_.is_exist()	 == true) { core.database_.Exterminate(); }

		core.database_.OpenConnection();
		core.database_.ImportSql(sql_file);

		WHEN("multiple activities are added")
		{
			QueryContainer query;
			ColumnContainer column;
			RowContainer row;
			bool creation_is_success = false;

			column.column_name = "Name";

			std::string table_name = "Activity";

			std::string activity_name1 = "Games";
			column.column_data = activity_name1;
			row.clear();
			row.push_back(column);

			query.table_name = table_name;
			query.columns = row;
			query.request = INSERT;
			creation_is_success = core.SqlRequest(query);

			std::string activity_name2 = "Music";
			column.column_data = activity_name2;
			row.clear();
			row.push_back(column);

			query.columns = row;
			query.request = INSERT;
			creation_is_success = core.SqlRequest(query);

			std::string activity_name3 = "Movies";
			column.column_data = activity_name3;
			row.clear();
			row.push_back(column);

			query.columns = row;
			query.request = INSERT;
			creation_is_success = core.SqlRequest(query);

			//generate expected result as a basis of comparison
			//-------------------------------------------------
			ColumnContainer col_res;
			RowContainer row_res;
			TableContainer expected;

			col_res.column_name = "Name";

			col_res.column_data = activity_name1;
			row_res.push_back(col_res);
			expected.push_back(row_res);
			row_res.clear();

			col_res.column_data = activity_name2;
			row_res.push_back(col_res);
			expected.push_back(row_res);
			row_res.clear();

			col_res.column_data = activity_name3;
			row_res.push_back(col_res);
			expected.push_back(row_res);
			//-------------------------------------------------

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
					activity_id_1 = result.begin()->begin()->column_data;

					sql = "SELECT ActivityID FROM Activity WHERE Name='Music';";
					REQUIRE(core.database_.SqlCommand(sql) == true);
					result = core.database_.read_result_buffer();
					activity_id_2 = result.begin()->begin()->column_data;

					sql = "SELECT ActivityID FROM Activity WHERE Name='Movies';";
					REQUIRE(core.database_.SqlCommand(sql) == true);
					result = core.database_.read_result_buffer();
					activity_id_3 = result.begin()->begin()->column_data;

					column.column_name = "ActivityID";
					column.column_data = activity_id_1;
					row.clear();
					row.push_back(column);

					query.search_params = row;
					query.request = DELETE;
					deletion_is_success = core.SqlRequest(query);

					column.column_data = activity_id_2;
					row.clear();
					row.push_back(column);

					query.search_params = row;
					query.request = DELETE;
					deletion_is_success = core.SqlRequest(query);

					column.column_data = activity_id_3;
					row.clear();
					row.push_back(column);

					query.search_params = row;
					query.request = DELETE;
					deletion_is_success = core.SqlRequest(query);

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
					activity_id_1 = result.begin()->begin()->column_data;

					sql = "SELECT ActivityID FROM Activity WHERE Name='Movies';";
					REQUIRE(core.database_.SqlCommand(sql) == true);
					result = core.database_.read_result_buffer();
					activity_id_2 = result.begin()->begin()->column_data;

					column.column_name = "ActivityID";
					column.column_data = activity_id_1;
					row.clear();
					row.push_back(column);

					query.search_params = row;
					query.request = DELETE;
					deletion_is_success = core.SqlRequest(query);

					column.column_data = activity_id_2;
					row.clear();
					row.push_back(column);

					query.search_params = row;
					query.request = DELETE;
					deletion_is_success = core.SqlRequest(query);

					core.database_.SqlCommand("SELECT Name FROM Activity\n");
					TableContainer result = core.database_.read_result_buffer();

					expected.pop_back();
					expected.erase(expected.begin());
					THEN("only the specified activities must be deleted")
					{
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
		std::string sql_filename = "BacklogManager.sql";
		File sql_file(sql_filename, db_dir);

		Database database(db_dir);
		Core core(database);
		if (core.database_.IsConnected()	== true) { core.database_.CloseConnection(); }
		if (core.database_.is_exist()		== true) { core.database_.Exterminate(); }

		core.database_.OpenConnection();
		core.database_.ImportSql(sql_file);

		WHEN("a listing is added")
		{
			std::string table_name = "Listing";
			QueryContainer query;
			ColumnContainer column;
			RowContainer row;
			row.clear();
			bool creation_is_success = false;

			std::string title		= "The Great Escape";
			std::string activity_id = "1";

			column.column_name = "Title";
			column.column_data = title;
			row.push_back(column);

			column.column_name = "ActivityID";
			column.column_data = activity_id;
			row.push_back(column);

			query.table_name	= table_name;
			query.columns		= row;
			query.request		= INSERT;

			creation_is_success	=  core.SqlRequest(query);

			//generate expected result as a basis of comparison
			//-------------------------------------------------
			ColumnContainer col_res;
			RowContainer row_res;

			col_res.column_name = "Title";
			col_res.column_data = title;
			row_res.push_back(col_res);

			col_res.column_name = "ActivityID";
			col_res.column_data = activity_id;
			row_res.push_back(col_res);

			TableContainer expected;
			expected.push_back(row_res);
			//-------------------------------------------------

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
					std::string lid = result.begin()->begin()->column_data;
					column.column_name = "LID";
					column.column_data = lid;
					row.clear();
					row.push_back(column);

					query.table_name	= table_name;
					query.search_params = row;
					query.request		= DELETE;

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

//SCENARIO("Multiple listings are added and deleted")
//{
//	GIVEN("a clean database")
//	{
//		std::string sql_filename = "BacklogManager.sql";
//		File sql_file(sql_filename, db_dir);
//
//		Database database(db_dir);
//		Core core(database);
//		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
//		if (core.database_.is_exist()    == true) { core.database_.Exterminate(); }
//
//		core.database_.OpenConnection();
//		core.database_.ImportSql(sql_file);
//
//		WHEN("multiple listings are added")
//		{
//			ColumnContainer column;
//			RowContainer row;
//			row.clear();
//			bool creation_is_success = false;
//
//			std::string title1, title2, title3, activity_id_1, activity_id_2, activity_id_3;
//
//			title1 = "The Great Escape";
//			activity_id_1 = "1";
//
//			column.column_name = "Title";
//			column.column_data = title1;
//			row.push_back(column);
//
//			column.column_name = "ActivityID";
//			column.column_data = activity_id_1;
//			row.push_back(column);
//
//			creation_is_success = core.AddListing(row);
//			row.clear();
//
//			title2 = "Drackula Souls 3";
//			activity_id_2 = "2";
//
//			column.column_name = "Title";
//			column.column_data = title2;
//			row.push_back(column);
//
//			column.column_name = "ActivityID";
//			column.column_data = activity_id_2;
//			row.push_back(column);
//
//			creation_is_success &= core.AddListing(row);
//			row.clear();
//
//			title3 = "Ghost Husslers";
//			activity_id_3 = "3";
//
//			column.column_name = "Title";
//			column.column_data = title3;
//			row.push_back(column);
//
//			column.column_name = "ActivityID";
//			column.column_data = activity_id_3;
//			row.push_back(column);
//
//			creation_is_success &= core.AddListing(row);
//
//			//generate expected result as a basis of comparison
//			//-------------------------------------------------
//			ColumnContainer col_res1, col_res2;
//			RowContainer row_res;
//			TableContainer expected;
//
//			col_res1.column_name = "Title";
//			col_res2.column_name = "ActivityID";
//
//			col_res1.column_data = title1;
//			row_res.push_back(col_res1);
//			col_res2.column_data = activity_id_1;
//			row_res.push_back(col_res2);
//			expected.push_back(row_res);
//			row_res.clear();
//
//			col_res1.column_data = title2;
//			row_res.push_back(col_res1);
//			col_res2.column_data = activity_id_2;
//			row_res.push_back(col_res2);
//			expected.push_back(row_res);
//			row_res.clear();
//
//			col_res1.column_data = title3;
//			row_res.push_back(col_res1);
//			col_res2.column_data = activity_id_3;
//			row_res.push_back(col_res2);
//			expected.push_back(row_res);
//			//-------------------------------------------------
//
//			core.database_.SqlCommand("SELECT Title,ActivityID FROM Listing\n");
//			TableContainer result = core.database_.read_result_buffer();
//
//			THEN("the changes must be regisitered by the database correctly")
//			{
//				REQUIRE(creation_is_success  == true);
//				REQUIRE(expected == result);
//
//				AND_WHEN("all listings are deleted")
//				{
//					bool is_deleted = true;
//
//					core.database_.SqlCommand("SELECT LID FROM Listing ORDER BY LID ASC");
//					result = core.database_.read_result_buffer();
//					std::string lid;
//					for (auto i = result.begin(); i != result.end(); ++i) {
//						lid = i->begin()->column_data;
//
//						row.clear();
//						column.column_name = "LID";
//						column.column_data = lid;
//						row.push_back(column);
//						is_deleted &= core.DeleteListing(row);
//					}
//					THEN("all listing records must seize to exist")
//					{
//						core.database_.SqlCommand("SELECT * FROM Listing");
//						result = core.database_.read_result_buffer();
//
//						REQUIRE(is_deleted == true);
//						REQUIRE(result.empty() == true);
//					}
//				}
//
//				AND_WHEN("specific listings are deleted")
//				{
//					bool is_deleted = true;
//					std::string lid;
//					row.clear();
//
//					core.database_.SqlCommand("SELECT LID FROM Listing WHERE ActivityID= '1';");
//					result = core.database_.read_result_buffer();
//					lid = result.begin()->begin()->column_data;
//					column.column_name = "LID";
//					column.column_data = lid;
//					row.push_back(column);
//					REQUIRE(core.DeleteListing(row) == true);
//					row.clear();
//
//					core.database_.SqlCommand("SELECT LID FROM Listing WHERE ActivityID= '3';");
//					result = core.database_.read_result_buffer();
//					lid = result.begin()->begin()->column_data;
//					column.column_name = "LID";
//					column.column_data = lid;
//					row.push_back(column);
//					REQUIRE(core.DeleteListing(row) == true);
//					row.clear();
//
//					core.database_.SqlCommand("SELECT ActivityID FROM Listing");
//					result = core.database_.read_result_buffer();
//
//					THEN("only the specified listings must be deleted")
//					{
//						REQUIRE(result.size() == 1);
//						REQUIRE(result.begin()->begin()->column_data == "2");
//					}
//				}
//			}
//		}
//		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
//		if (core.database_.is_exist()	 == true) { core.database_.Exterminate(); }
//	}
//}
//
//SCENARIO("A single listing is updated")
//{
//	GIVEN("a database with a single listing")
//	{
//		std::string sql_filename = "BacklogManager.sql";
//		File sql_file(sql_filename, db_dir);
//
//		Database database(db_dir);
//		Core core(database);
//		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
//		if (core.database_.is_exist()    == true) { core.database_.Exterminate(); }
//
//		core.database_.OpenConnection();
//		core.database_.ImportSql(sql_file);
//
//		ColumnContainer column;
//		RowContainer row;
//		row.clear();
//
//		std::string title = "The Great Escape";
//		std::string activity_id = "1";
//
//		column.column_name = "Title";
//		column.column_data = title;
//		row.push_back(column);
//
//		column.column_name = "ActivityID";
//		column.column_data = activity_id;
//		row.push_back(column);
//
//		REQUIRE(core.AddListing(row));
//		
//
//		WHEN("the listing is updated")
//		{
//			bool update_is_successful = false;
//			TableContainer result, expected;
//
//			REQUIRE(core.database_.SqlCommand("SELECT LID FROM Listing WHERE Title='The Great Escape'"));
//			result = core.database_.read_result_buffer();
//			REQUIRE(result.begin()->begin()->column_name == "LID");
//			std::string lid = result.begin()->begin()->column_data;
//
//			row.clear();
//			column.column_name = "LID";
//			column.column_data = lid;
//			row.push_back(column);
//
//			column.column_name = "Title";
//			column.column_data = "The Terrible Escape";
//			row.push_back(column);
//
//			update_is_successful = core.UpdateListing(row);
//			expected.push_back(row);
//
//			THEN("the changes must be registered correctly by the database")
//			{
//				core.database_.SqlCommand("SELECT LID,Title FROM Listing WHERE Title='The Terrible Escape'");
//				result = core.database_.read_result_buffer();
//				REQUIRE(update_is_successful == true);
//				REQUIRE(expected == result);
//			}
//		}
//		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
//		if (core.database_.is_exist() == true) { core.database_.Exterminate(); }
//	}
//}
//
//SCENARIO("a single user defined field is added, updated and deleted") {
//	GIVEN("a clean database")
//	{
//		std::string sql_filename = "BacklogManager.sql";
//		File sql_file(sql_filename, db_dir);
//
//		Database database(db_dir);
//		Core core(database);
//		if (core.database_.IsConnected()	== true) { core.database_.CloseConnection(); }
//		if (core.database_.is_exist()		== true) { core.database_.Exterminate(); }
//
//		core.database_.OpenConnection();
//		core.database_.ImportSql(sql_file);
//
//		WHEN("a user defined field is added")
//		{
//			bool is_added_successfully = false;
//			ColumnContainer column;
//			RowContainer row;
//			row.clear();
//
//			TableContainer result, expected;
//
//			std::string name, data_type, description, activity_id, sql;
//
//			name		= "Genre";
//			data_type	= "string";
//			description = "";
//			activity_id = "1";
//
//			column.column_name = "Name";
//			column.column_data = name;
//			row.push_back(column);
//
//			column.column_name = "DataType";
//			column.column_data = data_type;
//			row.push_back(column);
//
//			column.column_name = "Description";
//			column.column_data = description;
//			row.push_back(column);
//
//			column.column_name = "ActivityID";
//			column.column_data = activity_id;
//			row.push_back(column);
//
//			is_added_successfully = core.AddUserDefinedField(row);
//
//			expected.clear();
//			expected.push_back(row);
//
//			THEN("the change must be registered by the database correctly")
//			{
//				sql = "SELECT UDFID FROM UserDefinedField";
//				REQUIRE(core.database_.SqlCommand(sql) == true);
//				result = core.database_.read_result_buffer();
//				REQUIRE(result.begin()->size() == 1);
//
//				std::string  udfid = result.begin()->begin()->column_data;
//
//				sql = "SELECT Name,Datatype,Description,ActivityID FROM UserDefinedField";
//				REQUIRE(core.database_.SqlCommand(sql) == true);
//				result = core.database_.read_result_buffer();
//				REQUIRE(is_added_successfully == true);
//				REQUIRE(result == expected);
//
//				AND_WHEN("the user defined field is updated")
//				{
//					bool is_updated_successfully = false;
//					name		= "Genres";
//					data_type	= "int";
//					description = "some description";
//					activity_id = "2";
//
//					row.clear();
//
//					column.column_name = "UDFID";
//					column.column_data = udfid;
//					row.push_back(column);
//
//					column.column_name = "Name";
//					column.column_data = name;
//					row.push_back(column);
//
//					column.column_name = "DataType";
//					column.column_data = data_type;
//					row.push_back(column);
//
//					column.column_name = "Description";
//					column.column_data = description;
//					row.push_back(column);
//
//					column.column_name = "ActivityID";
//					column.column_data = activity_id;
//					row.push_back(column);
//
//					is_updated_successfully = core.UpdateUserDefinedField(row);
//
//					expected.clear();
//					expected.push_back(row);
//
//					THEN("the change must be registered by the database")
//					{
//						sql = "SELECT * FROM UserDefinedField";
//						REQUIRE(core.database_.SqlCommand(sql) == true);
//						result = core.database_.read_result_buffer();
//						REQUIRE(is_updated_successfully == true);
//						REQUIRE(result == expected);
//
//						AND_WHEN("the user defined field is deleted")
//						{
//							bool is_deleted_successfully = false;
//							is_deleted_successfully = core.DeleteUserDefinedField(row);
//
//							THEN("it must seize to exist")
//							{
//								REQUIRE(is_deleted_successfully == true);
//								sql = "SELECT * FROM UserDefinedField";
//								REQUIRE(core.database_.SqlCommand(sql) == true);
//								result = core.database_.read_result_buffer();
//								REQUIRE(result.empty() == true);
//							}
//						}
//					}
//				}
//			}
//		}
//		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
//		if (core.database_.is_exist() == true) { core.database_.Exterminate(); }
//	}
//}
//
//SCENARIO("a single user defined field data entry is added, updated, and deleted")
//{
//	GIVEN("a clean database")
//	{
//		std::string sql_filename = "BacklogManager.sql";
//		File sql_file(sql_filename, db_dir);
//
//		Database database(db_dir);
//		Core core(database);
//		if (core.database_.IsConnected()	== true) { core.database_.CloseConnection(); }
//		if (core.database_.is_exist()		== true) { core.database_.Exterminate(); }
//
//		core.database_.OpenConnection();
//		core.database_.ImportSql(sql_file);
//
//		WHEN("a user defined field data entry is added")
//		{
//			bool is_added_successfully = false;
//			ColumnContainer column;
//			RowContainer row;
//			row.clear();
//
//			TableContainer result, expected;
//			std::string udfid, data, sql;
//
//			udfid		= "1";
//			data		= "Comedy";
//
//			column.column_name = "Data";
//			column.column_data = data;
//			row.push_back(column);
//
//			column.column_name = "UDFID";
//			column.column_data = udfid;
//			row.push_back(column);
//
//			is_added_successfully = core.AddUdfEntry(row);
//
//			expected.clear();
//			expected.push_back(row);
//
//			THEN("the change must be registered by the database")
//			{
//				sql = "SELECT * FROM UDFentry";
//				REQUIRE(core.database_.SqlCommand(sql) == true);
//				result = core.database_.read_result_buffer();
//				REQUIRE(result.size() == 1);
//				REQUIRE(is_added_successfully == true);
//				REQUIRE(result == expected);
//
//				AND_WHEN("the data entry is updated")
//				{
//					bool is_updated_successfully = false;
//
//					udfid = "1";
//					data  = "Action";
//					row.clear();
//
//					column.column_name = "Data";
//					column.column_data = data;
//					row.push_back(column);
//
//					column.column_name = "UDFID";
//					column.column_data = udfid;
//					row.push_back(column);
//
//					is_updated_successfully = core.UpdateUdfEntry(row);
//
//					expected.clear();
//					expected.push_back(row);
//
//					THEN("the change must be registered by the database")
//					{
//						sql = "SELECT * FROM UDFentry";
//						REQUIRE(core.database_.SqlCommand(sql) == true);
//						result = core.database_.read_result_buffer();
//						REQUIRE(is_updated_successfully == true);
//						REQUIRE(result == expected);
//
//						AND_WHEN("the data entry is deleted")
//						{
//							bool is_deleted_successfully = false;
//							is_deleted_successfully = core.DeleteUdfEntry(row);
//
//							THEN("it must seize to exist")
//							{
//								REQUIRE(is_deleted_successfully == true);
//								sql = "SELECT * FROM UDFentry";
//								REQUIRE(core.database_.SqlCommand(sql) == true);
//								result = core.database_.read_result_buffer();
//								REQUIRE(result.empty() == true);
//							}
//						}
//					}
//				}
//			}
//		}
//		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
//		if (core.database_.is_exist() == true) { core.database_.Exterminate(); }
//	}
//
//}