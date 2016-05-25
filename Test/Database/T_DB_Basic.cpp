/*
 * Unit test for Database object
 */

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
		Database database(dir);
		if (database.IsConnected()	== true) { database.CloseConnection(); }
		if (database.is_exist()		== true) { database.Exterminate(); }
		
		WHEN("A Connection request is made followed by a disconnection request")
		{
			is_connected = database.OpenConnection();
			is_disconnected = database.CloseConnection();
			THEN("A connection and disconnection must be established successfully")
			{
				REQUIRE(is_connected == true);
				REQUIRE(is_disconnected == true);

				AND_WHEN("An extermination request is made")
				{
					is_deleted = database.Exterminate();
					THEN("The database file specified must be deleted")
					{
						REQUIRE(is_deleted == true);
					}
				}
			}
		}
		if (database.IsConnected()	== true) { database.CloseConnection(); }
		if (database.is_exist()		== true) { database.Exterminate(); }
	}
}

SCENARIO("an SQL statement is executed and registered by the system successfully")
{
	GIVEN("A database")
	{
		Database database(dir);
		if (database.IsConnected()	== true) { database.CloseConnection(); }
		if (database.is_exist()		== true) { database.Exterminate(); }

		WHEN("an sql statement is executed")
		{
			bool status;
			database.OpenConnection();
			status = database.SqlCommand("CREATE TABLE Activity(Name VARCHAR(255) PRIMARY KEY);");
			database.CloseConnection();
			database.Exterminate();
			THEN("The command must be registered by the database")
			{
				REQUIRE(status == true );
			}
		}
		if (database.IsConnected()	== true) { database.CloseConnection(); }
		if (database.is_exist()		== true) { database.Exterminate(); }
	}
}

SCENARIO("SQL statements are executed and read for a multiple column table with multiple values")
{
	GIVEN("A database with some data entries inserted")
	{
		Database database(dir);
		if (database.IsConnected()	== true) { database.CloseConnection(); }
		if (database.is_exist()		== true) { database.Exterminate(); }

		ColumnContainer temp_CR;
		SqlRowResult    temp_RR;
		std::string temp_str_insert, temp_str_cols, temp_str_data;
		std::vector<SqlRowResult> expected, result;
		std::vector<std::string> insert_statements;
		std::string table_name = "PERSON";

		std::vector<std::string> col_names		   =			{ "ID"	, "Name", "Surname"	};
		std::vector<std::vector<std::string>> data =	{		{ "1"	, "Sam"	, "Fish"	},
																{ "2"	, "Jan"	, "Ganer"	},
																{ "3"	, "Jay"	, "Ganer"	},
																{ "4"	, "Dre"	, "Flint"	}
														};
		//TODO: this method of testing is too convoluted and messy, simplify it
		/*
		 * emulate database to generate an expected value to serve as a basis of comparison 
		 * with the actual database this code also generates strings for insert statements for
		 * convenience
		 */
		//-----------------------------------------------------------------------------------------
		std::vector<std::string> d_row;
		for (std::vector<std::vector<std::string>>::size_type row = 0; row != data.size(); row++){
			d_row = data[row];
			temp_str_data = "";
			temp_RR.row_result.clear();

			for (std::vector<std::string>::size_type col = 0; col != col_names.size(); col++) {
				temp_CR.column_name = col_names[col];
				temp_CR.column_data = d_row[col];
				temp_RR.row_result.push_back(temp_CR);

				if ( (row == 0)) { temp_str_cols += col_names[col]; }
				if ( (row == 0) && (col != col_names.size()-1)){temp_str_cols += ",";	}

				temp_str_data += "'" + d_row[col] + "'";
				if (col != col_names.size()-1) { temp_str_data += ","; }

			}
			expected.push_back(temp_RR);

			temp_str_insert = "INSERT INTO " + table_name + "(" + temp_str_cols + ") " +\
				"VALUES(" + temp_str_data + ")";
			insert_statements.push_back(temp_str_insert);
		//-----------------------------------------------------------------------------------------
		}

		WHEN("sql statements are executed")
		{
			database.OpenConnection();
			database.SqlCommand("CREATE TABLE PERSON(ID INT PRIMARY KEY, Name VARCHAR(255), Surname VARCHAR(255));");
			database.SqlCommand(insert_statements[0]);
			database.SqlCommand(insert_statements[1]);
			database.SqlCommand(insert_statements[2]);
			database.SqlCommand(insert_statements[3]);
			database.SqlCommand("SELECT * FROM PERSON");
			database.CloseConnection();
			database.Exterminate();
			result = database.read_result_buffer();
			THEN("The commands must be registered by the database correctly")
			{
				//additional REQUIRE clauses added to ensure overloaded operators function properly
				REQUIRE( (expected != result)   == false);
				REQUIRE( (expected != expected) == false);
				REQUIRE(expected == expected);
				REQUIRE(result	 == result);
				REQUIRE(expected == result);
				REQUIRE(result	 == expected);
			}
		}
		if (database.IsConnected()	== true) { database.CloseConnection(); }
		if (database.is_exist()		== true) { database.Exterminate(); }
	}
}

SCENARIO("An SQL text file is imported into database") 
{
	GIVEN("A database and SQL text file")
	{
		Database database(dir);
		if (database.IsConnected()	== true) { database.CloseConnection(); }
		if (database.is_exist()		== true) { database.Exterminate(); }
		bool status = false;

		std::string filename = "sql_test_file.sql";
		File sql_file(filename, dir);
		sql_file.Create();
		std::string sql_container;

		//SQL text file contents
		//----------------------------------------------------------------------------------------
		sql_container  = "CREATE TABLE Activity(Name VARCHAR(255) PRIMARY KEY);\n";

		sql_container += "CREATE TABLE Listing(LID INT PRIMARY KEY, Title VARCHAR(255),";
		sql_container += " ActivityName VARCHAR(255), FOREIGN KEY(ActivityName) REFERENCES"; 
		sql_container += " Activity(Name));\n";

		sql_container += "CREATE TABLE UserDefinedField(Name VARCHAR(255) PRIMARY KEY,DataType";
		sql_container += " VARCHAR(255),Description VARCHAR(255));\n";

		sql_container += "CREATE TABLE UDFentry(Data BLOB, UDFname VARCHAR(255),";
		sql_container += " FOREIGN KEY(UDFname) REFERENCES UserDefinedField(Name));\n";

		sql_container += "CREATE TABLE Listing_UDF(LID INT, UDFname VARCHAR(255),";
		sql_container += " FOREIGN KEY(LID) REFERENCES Listing(LID), FOREIGN KEY(UDFname)";
		sql_container += " REFERENCES UserDefinedField(Name));\n";

		sql_container += "CREATE TABLE Listing_UDFentry(LID INT, UDFname VARCHAR(255),";
		sql_container += " EntryData BLOB, FOREIGN KEY(LID) REFERENCES Listing(LID),"; 
		sql_container += " FOREIGN KEY(UDFname) REFERENCES UserDefinedField(Name),";
		sql_container += " FOREIGN KEY(EntryData) REFERENCES UDFentry(Data))\n";
		//----------------------------------------------------------------------------------------
		sql_file.Write(sql_container);

		WHEN("An SQL text file is imported") 
		{
			database.OpenConnection();
			status = database.ImportSql(sql_file);
			sql_file.Destroy();
			database.CloseConnection();
			database.Exterminate();
			THEN("The operation must register as successful")
			{
				REQUIRE(status == true);
			}
		}
		if (database.IsConnected()	== true) { database.CloseConnection(); }
		if (database.is_exist()		== true) { database.Exterminate(); }
	}

}