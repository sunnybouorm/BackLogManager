#ifndef BACKLOGMANAGER_CORE_CORE_H_
#define BACKLOGMANAGER_CORE_CORE_H_
#include "../Database/database.h"
#include "config_file.h"


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
	Core();
	~Core();
	bool SqlRequest(QueryContainer &query);
	Database database_;
	ConfigFile cfg_;

	bool InitializeFromConfigFile();

private:
	const static std::multimap<const std::string, const std::string> init_db_map();
	const static std::multimap<const std::string, const std::string> init_superkey_map();

	const static DbMap kDatabaseMap_;
	const static DbMap kSuperkeyMap_; // unique identifiers of each entity

	bool Insert(QueryContainer &query);
	bool Delete(QueryContainer &query);
	bool Update(QueryContainer &query);

	bool AddActivity	(QueryContainer	&query);
	bool DeleteActivity	(QueryContainer	&query);
	bool UpdateActivity	(QueryContainer	&query);

	bool AddListing		(QueryContainer	&query);
	bool DeleteListing	(QueryContainer	&query);
	bool UpdateListing	(QueryContainer	&query);

	bool AddUserDefinedField	(QueryContainer	&query);
	bool DeleteUserDefinedField	(QueryContainer	&query);
	bool UpdateUserDefinedField	(QueryContainer	&query);

	bool AddUdfEntry	(QueryContainer	&query);
	bool DeleteUdfEntry	(QueryContainer	&query);
	bool UpdateUdfEntry	(QueryContainer	&query);

	bool AddUdfListingM2M	(QueryContainer	&query);
	bool UpdateUdfListingM2M(QueryContainer	&query);
	bool DeleteUdfListingM2M(QueryContainer	&query);

	int GenerateUniqueIntId(const std::string &table_name, const std::string &id_name);
	std::string Core::CommaSeparate(std::vector<std::string> &data, \
		const std::string &character = "");
	bool insert_params_check(QueryContainer query, std::string err_msg);
};

#endif//BACKLOGMANAGER_CORE_CORE_H_ 