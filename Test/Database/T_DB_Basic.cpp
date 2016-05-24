#include "../../Source/stdafx.h"
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

//SCENARIO("an SQL statement is executed and registered by the system successfully")
//{
//	GIVEN("A database")
//	{
//		Database database;
//		database.SetDirectory(dir);
//		WHEN("an sql statement is executed")
//		{
//			bool status;
//			database.OpenConnection();
//			status = database.ExecuteSql("CREATE TABLE Activity(Name VARCHAR(255) PRIMARY KEY);");
//			database.CloseConnection();
//			database.Exterminate();
//			THEN("The command must be registered by the database")
//			{
//				REQUIRE(status == true );
//			}
//		}
//	}
//}

SCENARIO("SQL statements are executed and read for a simple single column table")
{
	GIVEN("A database")
	{
		Database database;
		database.SetDirectory(dir);
		WHEN("sql statements are executed")
		{
			database.OpenConnection();
			database.ExecuteSql("CREATE TABLE PERSON(ID INT PRIMARY KEY, Name VARCHAR(255), Surname VARCHAR(255));");
			database.ExecuteSql("INSERT INTO PERSON (ID,Name,Surname) VALUES (1,'Sam','Fish')");
			database.ExecuteSql("INSERT INTO PERSON (ID,Name,Surname) VALUES (2,'Jan','Ganer')");
			database.ExecuteSql("INSERT INTO PERSON (ID,Name,Surname) VALUES (3,'Joe','Slow')");
			database.ExecuteSql("INSERT INTO PERSON (ID,Name,Surname) VALUES (4,'Tek','snap')");
			database.ExecuteSql("INSERT INTO PERSON (ID,Name,Surname) VALUES (5,'Xia','Xiao')");
			database.ExecuteSql("INSERT INTO PERSON (ID,Name,Surname) VALUES (6,'Dre','Bad')");
			database.ExecuteSql("SELECT * FROM PERSON");
			database.CloseConnection();
			database.Exterminate();
			THEN("The command must be registered by the database")
			{
				REQUIRE(false);
			}
		}
	}
}