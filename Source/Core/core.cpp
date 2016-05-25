#include "../stdafx.h"
#include "core.h"

Core::Core(Database db) {
	this->database = db;
}

bool Core::AddActivity(const std::string &activityName) {
	bool is_successful = true;
	std::string sql;

	is_successful &= this->database.OpenConnection();
	sql = "INSERT INTO Activity VALUES('" + activityName + "')\n";
	is_successful &= this->database.ExecuteSql(sql);
	is_successful &= this->database.CloseConnection();

	return is_successful;
}

bool Core::DeleteActivity(const std::string &activityName) {
	bool is_successful = true;
	std::string sql;

	is_successful &= this->database.OpenConnection();
	sql = "DELETE FROM Activity WHERE Name='" + activityName + "'\n";
	is_successful &= this->database.ExecuteSql(sql);
	is_successful &= this->database.CloseConnection();

	return is_successful;
}