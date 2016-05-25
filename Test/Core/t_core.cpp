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

		database.OpenConnection();
		database.ImportSql(sql_file);
		database.CloseConnection();

		WHEN("an activity is added")
		{
			THEN("it must be registered by the database successfully")
			{
				REQUIRE(false);
			} AND_WHEN("the activity is deleted")
			{
				THEN("it must be removed from the database")
				{
					REQUIRE(false);
				}
			}
		}
	}
}