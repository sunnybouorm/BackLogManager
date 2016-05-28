#ifndef BACKLOGMANAGER_CORE_CORE_H_
#define BACKLOGMANAGER_CORE_CORE_H_
#include "../Database/database.h"

/*
 * Core application that interfaces with GUI and Database
 *---------------------------------------------------------------------------------------------
 * NOTE:
 * > Some methods invoked require an open connection to a database Object to function properly,
 * ensure that an open connection is established before invoking any methods that require it.
 */

class Core {

public:
	Core(Database &db);
	
	bool AddActivity(const std::string &activity_name);
	bool DeleteActivity(const std::string &activity_id);

	bool AddListing(const std::string &title, const std::string &activity_id);
	bool DeleteListing(const std::string &lid);
	bool UpdateListing(const std::string &lid);

	Database database_;

private:
	int GenerateUniqueIntId(const std::string &table_name, const std::string &id_name);
	std::string Core::CommaSeparate(std::vector<std::string> &data, const std::string &character="");
	
};
#endif//BACKLOGMANAGER_CORE_CORE_H_ 