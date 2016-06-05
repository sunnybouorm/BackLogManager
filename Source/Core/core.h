#ifndef BACKLOGMANAGER_CORE_CORE_H_
#define BACKLOGMANAGER_CORE_CORE_H_
#include "../Database/database.h"


typedef std::multimap<const std::string, const std::string> DbMap;

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

	bool SqlRequest(QueryContainer &query);

	bool AddActivity   (const QueryContainer &query);
	bool DeleteActivity(const RowContainer &row);
	bool UpdateActivity(const RowContainer &row);

	bool AddListing   (const RowContainer &row);
	bool DeleteListing(const RowContainer &row);
	bool UpdateListing(const RowContainer &row);

	bool AddUserDefinedField(const RowContainer &row);
	bool DeleteUserDefinedField(const RowContainer &row);
	bool UpdateUserDefinedField(const RowContainer &row);

	bool AddUdfEntry   (const RowContainer &row);
	bool DeleteUdfEntry(const RowContainer &row);
	bool UpdateUdfEntry(const RowContainer &row);

	bool AddUdfListingM2M(const RowContainer &row);
	bool InsertUdfListingM2M(const RowContainer &row);
	bool DeleteUdfListingM2M(const RowContainer &row);

	Database database_;

private:
	const static std::multimap<const std::string, const std::string> init_db_map();

	int GenerateUniqueIntId(const std::string &table_name, const std::string &id_name);
	std::string Core::CommaSeparate(std::vector<std::string> &data, const std::string &character="");

	const static DbMap kDatabaseMap;
};

#endif//BACKLOGMANAGER_CORE_CORE_H_ 