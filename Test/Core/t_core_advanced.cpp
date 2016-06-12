#include "../../Source/stdafx.h"
#include "t_core.h"

SCENARIO("duplicate activity names are inserted") {
	GIVEN("a clean database") {
		Core core;
		core.database_.OpenConnection();

		WHEN("two activities with the same Name attribute are added") {
			QueryContainer	query;
			std::string column_name, column_data, table_name, activity_name, activity_id;
			RowContainer	row;

			bool creation_is_success = false;

			//record 1
			table_name		= "Activity";
			activity_name	= "Movies";
			activity_id		= "1";

			column_name = "Name";
			column_data = activity_name;
			row[column_name] = column_data;

			query.table_name = table_name;
			query.columns = row;
			query.request = INSERT;

			creation_is_success = core.SqlRequest(query);
			REQUIRE(creation_is_success == true);

			//record 2
			table_name		= "Activity";
			activity_name	= "Movies";
			activity_id		= "2";

			column_name		 = "Name";
			column_data		 = activity_name;
			row[column_name] = column_data;

			query.table_name = table_name;
			query.columns = row;
			query.request = INSERT;

			std::streambuf* old = std::cerr.rdbuf(0);
			creation_is_success = core.SqlRequest(query);
			std::cerr.rdbuf(old);

			row.clear();

			THEN("SQL unique constraint violation error must be triggered") {
				REQUIRE(creation_is_success == false);
			}
		}
		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
		if (core.database_.is_exist() == true) { core.database_.Exterminate(); }
	}
}

SCENARIO("duplicate listing titles are inserted") {
	GIVEN("a clean database") {
		Core core;
		core.database_.OpenConnection();
		

		WHEN("two listing with the same title attribute are added under the same activity") {
			QueryContainer	query;
			std::string column_name, column_data, table_name, title;
			std::string activity_name, activity_id, lid;
			RowContainer	row;

			bool creation_is_success = false;

			//Activity record 1
			table_name		 = "Activity";
			activity_name	 = "Movies";

			column_name		 = "Name";
			column_data		 = activity_name;
			row[column_name] = column_data;

			query.table_name = table_name;
			query.columns	 = row;
			query.request	 = INSERT;
			row.clear();

			creation_is_success = core.SqlRequest(query);
			REQUIRE(creation_is_success == true);

			//Listing record 1
			table_name			= "Listing";
			title				= "Lord of ring";
			activity_id			= "1";
			lid					= "1";

			column_name			= "Title";
			column_data			= title;
			row[column_name]	= column_data;

			column_name			= "ActivityID";
			column_data			= activity_id;
			row[column_name]	= column_data;

			query.table_name	= table_name;
			query.columns		= row;
			query.request		= INSERT;
			row.clear();

			creation_is_success = core.SqlRequest(query);
			REQUIRE(creation_is_success == true);

			//Listing record 2
			table_name			= "Listing";
			title				= "Lord of ring";
			activity_id			= "1";
			lid					= "2";

			column_name			= "Title";
			column_data			= title;
			row[column_name]	= column_data;

			column_name			= "ActivityID";
			column_data			= activity_id;
			row[column_name]	= column_data;

			query.table_name	= table_name;
			query.columns		= row;
			query.request		= INSERT;

			std::streambuf* old = std::cerr.rdbuf(0);
			creation_is_success = core.SqlRequest(query);
			std::cerr.rdbuf(old);
			row.clear();

			THEN("SQL unique constraint violation error must be triggered") {
				REQUIRE(creation_is_success == false);
			}
		}

		WHEN("two listing with the same title attribute are added under different activities") {
			QueryContainer	query;
			std::string column_name, column_data, table_name, title;
			std::string activity_name, activity_id, lid;
			RowContainer	row;

			bool creation_is_success = false;

			//Activity record 1
			table_name			= "Activity";
			activity_name		= "Movies";

			column_name			= "Name";
			column_data			= activity_name;
			row[column_name]	= column_data;

			query.table_name	= table_name;
			query.columns		= row;
			query.request		= INSERT;
			row.clear();

			creation_is_success = core.SqlRequest(query);
			REQUIRE(creation_is_success == true);

			//Activity record 2
			table_name			= "Activity";
			activity_name		= "TV shows";

			column_name			= "Name";
			column_data			= activity_name;
			row[column_name]	= column_data;

			query.table_name	= table_name;
			query.columns		= row;
			query.request		= INSERT;
			row.clear();

			creation_is_success = core.SqlRequest(query);
			REQUIRE(creation_is_success == true);

			//record 1
			table_name			= "Listing";
			title				= "Lord of ring";
			activity_id			= "1";
			lid					= "1";

			column_name			= "Title";
			column_data			= title;
			row[column_name]	= column_data;

			column_name			= "ActivityID";
			column_data			= activity_id;
			row[column_name]	= column_data;

			query.table_name	= table_name;
			query.columns		= row;
			query.request		= INSERT;

			creation_is_success = core.SqlRequest(query);
			REQUIRE(creation_is_success == true);

			//record 2
			table_name			= "Listing";
			title				= "Lord of ring";
			activity_id			= "2";
			lid					= "2";

			column_name			= "Title";
			column_data			= title;
			row[column_name]	= column_data;

			column_name			= "ActivityID";
			column_data			= activity_id;
			row[column_name]	= column_data;

			query.table_name	= table_name;
			query.columns		= row;
			query.request		= INSERT;

			creation_is_success = core.SqlRequest(query);

			row.clear();

			THEN("SQL unique constraint violation must not trigger") {
				REQUIRE(creation_is_success == true);
			}
		}
		if (core.database_.IsConnected()	== true) { core.database_.CloseConnection();	}
		if (core.database_.is_exist()		== true) { core.database_.Exterminate();		}
	}
}