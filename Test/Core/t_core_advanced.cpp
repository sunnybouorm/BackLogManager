#include "../../Source/stdafx.h"
#include "t_core.h"

SCENARIO("duplicate activity names are inserted") {
	GIVEN("a clean database") {
		std::string sql_filename = "BacklogManager.sql";
		File sql_file(sql_filename, kdb_dir);

		Database database(kdb_dir);
		Core core(database);
		if (core.database_.IsConnected() == true) { core.database_.CloseConnection(); }
		if (core.database_.is_exist() == true) { core.database_.Exterminate(); }

		core.database_.OpenConnection();
		core.database_.ImportSql(sql_file);

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