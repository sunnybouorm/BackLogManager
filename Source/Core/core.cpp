#include "../stdafx.h"
#include "core.h"

Core::Core(Database &db) {
	this->database_ = db;
}

bool Core::AddActivity(const std::string &activity_name) {
	bool is_successful = true;
	std::string sql;

	sql = "INSERT INTO Activity VALUES('" + activity_name + "')\n";
	is_successful &= this->database_.ExecuteSql(sql);

	return is_successful;
}

bool Core::DeleteActivity(const std::string &activity_name) {
	bool is_successful = true;
	std::string sql;

	sql = "DELETE FROM Activity WHERE Name='" + activity_name + "'\n";
	is_successful &= this->database_.ExecuteSql(sql);

	return is_successful;
}

//int Core::GenerateLid() {
//	//TODO
//	return 0;
//}
//
//bool Core::AddListing(std::string title, std::string activity_name) {
//	int lid = this->GenerateLid();
//	bool is_successful = true;
//	std::string sql;
//
//	sql = "INSERT INTO Listing VALUES("+ std::to_string(lid) +\
//		"'" + title + "'" + "," + "'" + activity_name + "'" + ")\n";
//	is_successful &= this->database_.ExecuteSql(sql);
//
//	return is_successful;
//}

bool Core::DeleteListing(int lid) {
	bool is_successful = true;
	std::string sql;

	sql = "DELETE FROM Listing WHERE LID=" + std::to_string(lid) + "\n";
	is_successful &= this->database_.ExecuteSql(sql);

	return is_successful;
}