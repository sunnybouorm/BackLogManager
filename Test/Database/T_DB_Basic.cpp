#include "stdafx.h"
#include "T_DB_Basic.h"

const std::string dir = "D:\\Development\\Projects\\BacklogManager\\unused_test_directory\\";

SCENARIO("A connection to the database is established")
{
	GIVEN("An initialized set of data")
	{
		bool isConnected    = false;
		bool isDisconnected = false;
		bool isDeleted		= false;
		Database database;
		database.set_directory(dir);
		
		WHEN("A Connection request is made followed by a disconnection request")
		{
			isConnected = database.open_connection();
			isDisconnected = database.close_connection();
			THEN("A connection and disconnection must be established successfully")
			{
				REQUIRE(isConnected == true);
				REQUIRE(isDisconnected == true);
			}
		}
		WHEN("An extermination request is made")
		{
			isDeleted = database.exterminate();
			THEN("The database file specified must be deleted")
			{
				REQUIRE(isDeleted == true);
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