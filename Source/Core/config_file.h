/*
 * The configuration file's primarily function is to initialize the program on startup
 *
 * File Format:
 *
---CONFIG FILE START---
\n<[header_name]>\n\n

[tag_name_1]=[value]\n
...
[tag_name_n]=[value]\n

\n<[header_name] end>\n
---CONFIG FILE END---

 */
#pragma once
#include "../stdafx.h"
#include "../File_IO.h"

typedef std::map<std::string, std::string>	StringMap;// key = tag_name, value= tag_value

typedef struct HeaderContainerStruct
{
	bool is_good = false;//data integrity check
	StringMap tag_map;
	
} HeaderContainer;

typedef std::pair<std::string, HeaderContainer> HeaderMapPair;
typedef std::map <std::string, HeaderContainer> HeaderMap;// key = header_name

typedef struct ConfigFileContainerStruct
{
	bool is_good = false;//data integrity check
	HeaderMap header_map;

} ConfigFileContainer;

class ConfigFile {

public:
	File config_file_;

	static bool ConfigFile::CheckCacheIntegrity(const ConfigFileContainer &cache);

	ConfigFile();

	bool Create ();
	bool Destroy();
	bool Exists () { return this->config_file_.Exists(); }

	bool WriteToHeader(std::string &header_name, std::string &tag_name, std::string &tag_value);

	bool ScanAndCache();//scans config file and stores result in config_cache_

	ConfigFileContainer get_cache() { return this->config_cache_; }

	std::string get_tag_value(const std::string &header_name, const std::string &tag_name) {
		return this->config_cache_.header_map.at(header_name).tag_map.at(tag_name);
	}

private:
	static const std::string kfile_name_;
	static const std::string kfile_dir_;

	ConfigFileContainer config_cache_;

	bool write_header_start	(const std::string &header_name);
	bool write_header_end	(const std::string &header_name);
	bool write_tag(const std::string& tag_name, const std::string &value);

	bool EncloseTag(const std::string &tag_name, StreamposPair &bracket);
	bool ScanHeader(std::string &header_string);
};