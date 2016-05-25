#pragma once
#include "../Database/database.h"

/*
 * Core application that interfaces with GUI and Database
 *---------------------------------------------------------------------------------------------
 * NOTE:
 * > Some methods invoked require an open connection to a database Object to function properly,
 * ensure that an open connection is established before invoking any methods that require it.
 */

class Core {
private:

public:
	Core(Database &db);
	Database database_;

	bool SqlInsert(std::string table_name);

	bool AddActivity(const std::string &activity_name);
	bool DeleteActivity(const std::string &activity_name);

	int  GenerateLid();//finds valid primary key for creating a new Listing entry
	bool AddListing(std::string title, std::string activity_name);
	bool DeleteListing(int lid);
};