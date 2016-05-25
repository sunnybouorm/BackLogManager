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

			THEN("it must be registered by the database successfully")
			{
				REQUIRE(creation_is_success == true);
				AND_WHEN("the activity is deleted")
				{
					bool deletion_is_success = false;
					deletion_is_success = core.DeleteActivity(activityName);
					THEN("it must be removed from the database")
					{
						REQUIRE(deletion_is_success == true);
					}
				}
			}
		}
		if (core.database.is_exist() == true) { core.database.Exterminate(); }
	}
}