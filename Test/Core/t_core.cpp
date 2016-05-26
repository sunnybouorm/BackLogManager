#include "../../Source/stdafx.h"
#include "t_core.h"

std::string db_dir = "D:\\Development\\Projects\\BacklogManager\\Database\\";

SCENARIO("An activity is added and deleted") 
{
	GIVEN("a clean database_") 
	{
		std::string sql_filename = "BacklogManager.sql";
		File sql_file(sql_filename, db_dir);

		Database database_(db_dir);
		Core core(database_);
		if (core.database_.IsConnected()	== true) { core.database_.CloseConnection(); }
		if (core.database_.is_exist()		== true) { core.database_.Exterminate(); }

		core.database_.OpenConnection();
		core.database_.ImportSql(sql_file);

		WHEN("an activity is added")
		{
			std::string activity_name = "Movies";
			bool creation_is_success = false;
			creation_is_success = core.AddActivity(activity_name);

			//generate expected result as a basis of comparison
			//-------------------------------------------------
			ColumnContainer col_res;
			RowResult row_res;

			col_res.column_name = "Name";
			col_res.column_data = activity_name;
			row_res.push_back(col_res);

			TableResult expected;
			expected.push_back(row_res);
			//-------------------------------------------------

			core.database_.SqlCommand("SELECT * FROM Activity\n");
			TableResult result = core.database_.read_result_buffer();

			THEN("it must be registered by the database_ correctly")
			{
				REQUIRE((expected == result) == true);
				REQUIRE(creation_is_success == true);
				AND_WHEN("the activity is deleted")
				{
					bool deletion_is_success = false;
					deletion_is_success = core.DeleteActivity(activity_name);

					core.database_.SqlCommand("SELECT * FROM Activity\n");
					TableResult result = core.database_.read_result_buffer();

					THEN("it must be removed from the database_")
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

		Database database_(db_dir);
		Core core(database_);
		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
		if (core.database_.is_exist() == true) { core.database_.Exterminate(); }

		core.database_.OpenConnection();
		core.database_.ImportSql(sql_file);

		WHEN("multiple activities are added")
		{

			bool creation_is_success = false;

			std::string activity_name1 = "Games";
			creation_is_success = core.AddActivity(activity_name1);

			std::string activity_name2 = "Music";
			creation_is_success &= core.AddActivity(activity_name2);

			std::string activity_name3 = "Movies";
			creation_is_success &= core.AddActivity(activity_name3);

			//generate expected result as a basis of comparison
			//-------------------------------------------------
			ColumnContainer col_res;
			RowResult row_res;
			TableResult expected;

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

			core.database_.SqlCommand("SELECT * FROM Activity\n");
			TableResult result = core.database_.read_result_buffer();

			THEN("the changes must be regisitered by the database correctly")
			{
				REQUIRE(creation_is_success == true);
				REQUIRE(result == expected);

				AND_WHEN("all activities are deleted")
				{
					bool deletion_is_success = false;
					deletion_is_success  = core.DeleteActivity(activity_name1);
					deletion_is_success &= core.DeleteActivity(activity_name2);
					deletion_is_success &= core.DeleteActivity(activity_name3);

					core.database_.SqlCommand("SELECT * FROM Activity\n");
					TableResult result = core.database_.read_result_buffer();
					THEN("all activity records must seize to exist")
					{
						REQUIRE(deletion_is_success == true);
						REQUIRE(result.empty() == true);
					}
				}

				AND_WHEN("specific activities are deleted")
				{
					bool deletion_is_success = false;
					deletion_is_success  = core.DeleteActivity(activity_name1);
					deletion_is_success &= core.DeleteActivity(activity_name3);

					core.database_.SqlCommand("SELECT * FROM Activity\n");
					TableResult result = core.database_.read_result_buffer();

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

SCENARIO("A listing is added and deleted")
{
	GIVEN("a clean database_")
	{
		std::string sql_filename = "BacklogManager.sql";
		File sql_file(sql_filename, db_dir);

		Database database_(db_dir);
		Core core(database_);
		if (core.database_.IsConnected()	== true) { core.database_.CloseConnection(); }
		if (core.database_.is_exist()		== true) { core.database_.Exterminate(); }

		core.database_.OpenConnection();
		core.database_.ImportSql(sql_file);

		WHEN("a listing is added")
		{
			bool creation_is_success = false;

			std::string title = "The Great Escape";
			std::string activity_name = "Sports";
			creation_is_success =  core.AddListing(title, activity_name);

			//generate expected result as a basis of comparison
			//-------------------------------------------------
			ColumnContainer col_res;
			RowResult row_res;

			col_res.column_name = "Title";
			col_res.column_data = title;
			row_res.push_back(col_res);

			col_res.column_name = "ActivityName";
			col_res.column_data = activity_name;
			row_res.push_back(col_res);

			TableResult expected;
			expected.push_back(row_res);
			//-------------------------------------------------

			core.database_.SqlCommand("SELECT Title,ActivityName FROM Listing\n");
			TableResult result = core.database_.read_result_buffer();

			THEN("it must be registered by the database_ correctly")
			{
				REQUIRE((expected == result) == true);
				REQUIRE(creation_is_success  == true);
				AND_WHEN("the activity is deleted")
				{
					bool deletion_is_success = false;

					core.database_.SqlCommand("SELECT LID FROM Listing\n");
					result = core.database_.read_result_buffer();
					int lid = std::stoi(result.begin()->begin()->column_data);
					deletion_is_success = core.DeleteListing(lid);

					core.database_.SqlCommand("SELECT Title,ActivityName FROM Listing\n");
					result = core.database_.read_result_buffer();

					THEN("it must be removed from the database_")
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
	GIVEN("a clean database")
	{
		WHEN("multiple listings are added")
		{
			THEN("the changes must be regisitered by the database correctly")
			{
				REQUIRE(false);//TODO

				AND_WHEN("all listings are deleted")
				{
					THEN("all listing records must seize to exist")
					{
						REQUIRE(false);//TODO
					}
				}

				AND_WHEN("all specified listings are deleted")
				{
					THEN("only the specified listings must be deleted")
					{
						REQUIRE(false);//TODO
					}
				}
			}
		}
	}
}

SCENARIO("A single listing is updated")
{
	GIVEN("a database with a single listing")
	{
		WHEN("the listing is updated")
		{
			bool is_successful = false;
			THEN("the changes must be registered correctly by the database")
			{
				REQUIRE(is_successful == true);
			}
		}
	}
}