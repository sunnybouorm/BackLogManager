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
	int GenerateUniqueIntId(const std::string &table_name, const std::string &id_name);
	std::string Core::CommaSeparate(std::vector<std::string> &data, const char &character=NULL);

public:
	Core(Database &db);
	Database database_;

	bool AddActivity(const std::string &activity_name);
	bool DeleteActivity(const std::string &activity_id);

	bool AddListing(std::string title, std::string activity_id);
	bool DeleteListing(int lid);
	bool UpdateListing(int lid);
	
};