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

	bool AddActivity   (const RowContainer &row);
	bool DeleteActivity(const RowContainer &row);
	bool UpdateActivity(const RowContainer &row);

	bool AddListing   (const RowContainer &row);
	bool DeleteListing(const RowContainer &row);
	bool UpdateListing(const RowContainer &row);

	bool AddUserDefinedField(const RowContainer &row);
	bool DeleteUserDefinedField(const RowContainer &row);
	bool UpdateUserDefinedField(const RowContainer &row);

	bool AddUdfEntry(const RowContainer &row);
	bool DeleteUdfEntry(const RowContainer &row);
	bool UpdateUdfEntry(const RowContainer &row);



	Database database_;

private:
	int GenerateUniqueIntId(const std::string &table_name, const std::string &id_name);
	std::string Core::CommaSeparate(std::vector<std::string> &data, const std::string &character="");
	
};
#endif//BACKLOGMANAGER_CORE_CORE_H_ 