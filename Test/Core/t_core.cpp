#include "../../Source/stdafx.h"
#include "t_core.h"

std::string db_dir = "D:\\Development\\Projects\\BacklogManager\\Database\\";

SCENARIO("An activity is added and deleted") 
{
	GIVEN("a clean database") 
	{
		std::string sql_filename = "BacklogManager.sql";
		File sql_file(sql_filename, db_dir);

		Database database(db_dir);
		Core core(database);
		if (core.database.is_exist() == true) { core.database.Exterminate(); }

		core.database.OpenConnection();
		core.database.ImportSql(sql_file);
		core.database.CloseConnection();

		WHEN("an activity is added")
		{
			std::string activityName = "Movies";
			bool creation_is_success = false;
			creation_is_success = core.AddActivity(activityName);

			//generate expected result as a basis of comparison
			//-------------------------------------------------
			SqlColumnResult col_res;
			col_res.column_name = "Name";
			col_res.column_data = activityName;
			SqlRowResult row_res;
			row_res.row_result.push_back(col_res);

			std::vector<SqlRowResult> expected;
			expected.push_back(row_res);
			//-------------------------------------------------

			core.database.OpenConnection();
			core.database.ExecuteSql("SELECT * FROM Activity\n");
			std::vector<SqlRowResult> result = core.database.read_result_buffer();
			core.database.CloseConnection();

			REQUIRE( (expected==result) == true );

			THEN("it must be registered by the database correctly")
			{
				REQUIRE(creation_is_success == true);
				AND_WHEN("the activity is deleted")
				{
					bool deletion_is_success = false;
					deletion_is_success = core.DeleteActivity(activityName);

					core.database.OpenConnection();
					core.database.ExecuteSql("SELECT * FROM Activity\n");
					std::vector<SqlRowResult> result = core.database.read_result_buffer();
					core.database.CloseConnection();

					THEN("it must be removed from the database")
					{
						REQUIRE(deletion_is_success == true);
						REQUIRE(result.empty() == true);
					}
				}
			}
		}
		if (core.database.is_exist() == true) { core.database.Exterminate(); }
	}
}