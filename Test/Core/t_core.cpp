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
		if (database.is_exist() == true) { database.Exterminate(); }

		database.OpenConnection();
		database.ImportSql(sql_file);
		database.CloseConnection();

		WHEN("an activity is added")
		{
			bool creation_is_success = false;
			//TODO: add activity
			THEN("it must be registered by the database successfully")
			{
				REQUIRE(creation_is_success == true);
				AND_WHEN("the activity is deleted")
				{
					bool deletion_is_success = false;
					//TODO: delete activity
					THEN("it must be removed from the database")
					{
						REQUIRE(deletion_is_success);
					}
				}
			} 
		}
		if (database.is_exist() == true) { database.Exterminate(); }
	}
}