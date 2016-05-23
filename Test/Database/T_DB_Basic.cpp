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

SCENARIO("an SQL text file is read")
{
	Database database;
	database.set_directory(dir);
	database.open_connection();
	database.execSQL("CREATE TABLE Activity(Name VARCHAR(255) PRIMARY KEY);");
	database.exterminate();
	GIVEN("An initialized set of Data")
	{
		WHEN("The file is read")
		{
			THEN("The commands must be executed correctly")
			{
				REQUIRE(false);
			}
		}
	}
}