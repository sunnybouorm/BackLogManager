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
		database.SetDirectory(dir);
		
		WHEN("A Connection request is made followed by a disconnection request")
		{
			is_connected = database.OpenConnection();
			is_disconnected = database.CloseConnection();
			THEN("A connection and disconnection must be established successfully")
			{
				REQUIRE(is_connected == true);
				REQUIRE(is_disconnected == true);
			}
		}
		WHEN("An extermination request is made")
		{
			is_deleted = database.Exterminate();
			THEN("The database file specified must be deleted")
			{
				REQUIRE(is_deleted == true);
			}
		}
	}
}

SCENARIO("an SQL statement is executed and registered by the system successfully")
{
	GIVEN("A database")
	{
		Database database;
		database.SetDirectory(dir);
		WHEN("an sql statement is executed")
		{
			bool status;
			database.OpenConnection();
			status = database.ExecuteSql("CREATE TABLE Activity(Name VARCHAR(255) PRIMARY KEY);");
			database.CloseConnection();
			database.Exterminate();
			THEN("The command must be registered by the database")
			{
				REQUIRE(status == true );
			}
		}
	}
}

SCENARIO("SQL statements are executed and read for a simple single column table")
{
	GIVEN("A database")
	{
		Database database;
		database.SetDirectory(dir);
		WHEN("sql statements are executed")
		{
			database.OpenConnection();
			database.ExecuteSql("CREATE TABLE Activity(Name VARCHAR(255) PRIMARY KEY);");
			database.ExecuteSql("INSERT INTO Activity (Name) VALUES ('SlowJoe')");
			database.ExecuteSql("SELECT * FROM Activity");
			database.CloseConnection();
			database.Exterminate();
			THEN("The command must be registered by the database")
			{
				REQUIRE(false);
			}
		}
	}
}