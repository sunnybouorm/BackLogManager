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
#include "core.h"

typedef std::map<std::string, std::string>	StringMap;// key = tag_name, value= tag_value

typedef struct HeaderContainerStruct
{
	StringMap tag_map;
	
} HeaderContainer;

typedef std::pair<std::string, HeaderContainer> HeaderMapPair;
typedef std::map <std::string, HeaderContainer> HeaderMap;// key = header_name

typedef struct ConfigFileContainerStruct
{
	HeaderMap header_map;

} ConfigFileContainer;

class ConfigFile {

public:
	ConfigFile();

	bool Create ();
	bool Destroy();
	bool Exists () { return this->config_file_.Exists(); }

	bool InitializeFromConfigFile(Core &core);

private:
	static const std::string kfile_name_;
	static const std::string kfile_dir_;

	File config_file_;
	ConfigFileContainer config_cache_;

	bool write_header_start	(const std::string &header_name);
	bool write_header_end	(const std::string &header_name);
	bool write_tag(const std::string& tag_name, const std::string &value);

	std::string ScanHeader();
	bool ScanAndCache();//scans config file and stores result in config_cache_
};