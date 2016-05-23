#include "stdafx.h"
#include "t_db_basic.h"

const std::string dir = "D:\\Development\\Projects\\BacklogManager\\unused_test_directory\\";

SCENARIO("A connection to the database is established")
{
	GIVEN("An initialized set of data")
	{
		bool is_connected    = false;
		bool is_disconnected = false;
		bool is_deleted		 = false;
		Database database;
		database.set_directory(dir);
		
		WHEN("A Connection request is made followed by a disconnection request")
		{
			is_connected = database.open_connection();
			is_disconnected = database.close_connection();
			THEN("A connection and disconnection must be established successfully")
			{
				REQUIRE(is_connected == true);
				REQUIRE(is_disconnected == true);
			}
		}
		WHEN("An extermination request is made")
		{
			is_deleted = database.exterminate();
			THEN("The database file specified must be deleted")
			{
				REQUIRE(is_deleted == true);
			}
		}
	}
}

SCENARIO("an SQL statement is executed")
{
	GIVEN("A database")
	{
		Database database;
		database.set_directory(dir);
		WHEN("an sql statement is executed")
		{
			database.open_connection();
			database.execSQL("CREATE TABLE Activity(Name VARCHAR(255) PRIMARY KEY);");
			database.close_connection();
			database.exterminate();
			THEN("The command must be registered by the database")
			{
				REQUIRE(false);
			}
		}
		database.close_connection();
	}
}