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

	bool AddActivity   (const RowResult &row);
	bool DeleteActivity(const RowResult &row);
	bool UpdateActivity(const RowResult &row);

	bool AddListing   (const RowResult &row);
	bool DeleteListing(const RowResult &row);
	bool UpdateListing(const RowResult &row);

	bool AddUserDefinedField(const RowResult &row);
	bool DeleteUserDefinedField(const RowResult &row);
	bool UpdateUserDefinedField(const RowResult &row);

	bool AddUdfEntry(const RowResult &row);
	bool DeleteUdfEntry(const RowResult &row);
	bool UpdateUdfEntry(const RowResult &row);

	Database database_;

private:
	int GenerateUniqueIntId(const std::string &table_name, const std::string &id_name);
	std::string Core::CommaSeparate(std::vector<std::string> &data, const std::string &character="");
	
};
#endif//BACKLOGMANAGER_CORE_CORE_H_ 