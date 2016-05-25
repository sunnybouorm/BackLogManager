#pragma once
#include "../Database/database.h"

/*
 * Core application that interfaces with GUI and Database
 */

class Core {

public:
	Core(Database db);
	Database database;

	bool AddActivity(const std::string &activityName);
	bool DeleteActivity(const std::string &activityName);
};