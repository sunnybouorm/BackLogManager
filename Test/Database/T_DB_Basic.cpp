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

//SCENARIO("SQL statements are executed and read for a simple single column table")
//{
//	GIVEN("A database")
//	{
//		Database database;
//		database.SetDirectory(dir);
//		WHEN("sql statements are executed")
//		{
//			database.OpenConnection();
//			database.ExecuteSql("CREATE TABLE PERSON(ID INT PRIMARY KEY, Name VARCHAR(255), Surname VARCHAR(255));");
//			database.ExecuteSql("INSERT INTO PERSON (ID,Name,Surname) VALUES (1,'Sam','Fish')");
//			database.ExecuteSql("INSERT INTO PERSON (ID,Name,Surname) VALUES (2,'Jan','Ganer')");
//			database.ExecuteSql("INSERT INTO PERSON (ID,Name,Surname) VALUES (3,'Joe','Slow')");
//			database.ExecuteSql("INSERT INTO PERSON (ID,Name,Surname) VALUES (4,'Tek','snap')");
//			database.ExecuteSql("INSERT INTO PERSON (ID,Name,Surname) VALUES (5,'Xia','Xiao')");
//			database.ExecuteSql("INSERT INTO PERSON (ID,Name,Surname) VALUES (6,'Dre','Bad')");
//			database.ExecuteSql("SELECT * FROM PERSON");
//			database.CloseConnection();
//			database.Exterminate();
//			THEN("The command must be registered by the database")
//			{
//				REQUIRE(false);
//			}
//		}
//	}
//}

SCENARIO("SQL statements are executed and read for a multiple column table with multiple values")
{
	GIVEN("A database")
	{
		Database database;
		database.SetDirectory(dir);

		SqlColumnResult temp;
		std::string temp_str_insert, temp_str_cols, temp_str_data;
		SqlRowResult expected, result;
		std::vector<std::string> insert_statements;
		std::string table_name = "PERSON";

		std::string col_names[]					   =			{ "ID"	, "Name", "Surname"	};
		int cols = col_names->size();
		std::vector<std::vector<std::string>> data =	{		{ "1"	, "'Sam'"	, "'Fish'"	},
																{ "2"	, "'Jan'"	, "'Ganer'"	}
														};
		std::vector<std::string> d_row;
		for (std::vector<std::vector<std::string>>::size_type row = 0; row != data.size(); row++){
			d_row = data[row];
			temp_str_data = "";

			for (int col = 0; col <= col_names->size(); col++) {
				temp.column_name = col_names[col];
				temp.column_data = d_row[col];
				expected.row_result.push_back(temp);

				if ( (row == 0)) { temp_str_cols += col_names[col]; }
				if ( (row == 0) && (col != col_names->size())){temp_str_cols += ",";	}

				temp_str_data += d_row[col];
				if (col != col_names->size()) { temp_str_data += ","; }

			}
			temp_str_insert = "INSERT INTO " + table_name + "(" + temp_str_cols + ") " +\
				"VALUES(" + temp_str_data + ")";
			insert_statements.push_back(temp_str_insert);
		}
		std::cout << "statement: <" << insert_statements[0] << ">\n";

		WHEN("sql statements are executed")
		{
			database.OpenConnection();
			database.ExecuteSql("CREATE TABLE PERSON(ID INT PRIMARY KEY, Name VARCHAR(255), Surname VARCHAR(255));");
			database.ExecuteSql(insert_statements[0]);
			database.ExecuteSql(insert_statements[1]);
			database.ExecuteSql("SELECT * FROM PERSON");
			database.CloseConnection();
			database.Exterminate();
			result = *database.read_result_buffer().begin();
			THEN("The commands must be registered by the database correctly")
			{
				if ((result == expected)	  == false) { database.PrintResultBuffer(); }
				REQUIRE( (result==expected)   == true);
			}
		}

		//WHEN("sql statements are executed")
		//{
		//	database.OpenConnection();
		//	database.ExecuteSql("CREATE TABLE PERSON(ID INT PRIMARY KEY, Name VARCHAR(255), Surname VARCHAR(255));");
		//	database.ExecuteSql("INSERT INTO PERSON (ID,Name,Surname) VALUES (1,'Sam','Fish')");
		//	database.ExecuteSql("INSERT INTO PERSON (ID,Name,Surname) VALUES (2,'Jan','Ganer')");
		//	database.ExecuteSql("SELECT * FROM PERSON");
		//	database.CloseConnection();
		//	database.Exterminate();
		//	result = *database.read_result_buffer().begin();
		//	THEN("The commands must be registered by the database correctly")
		//	{
		//		REQUIRE(person1 == result1);//TODO
		//		REQUIRE(person2 == result2);
		//	}
		//}
	}
}