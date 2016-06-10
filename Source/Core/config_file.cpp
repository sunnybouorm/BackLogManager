#include "config_file.h"

const std::string ConfigFile::kfile_name_ = "config.ini";
const std::string ConfigFile::kfile_dir_  = "";

ConfigFile::ConfigFile() {
	this->config_file_.set_filename (this->kfile_name_);
	this->config_file_.set_directory(this->kfile_dir_);

	if (this->config_file_.Exists() == false) {
		this->Create();
	}
}

bool ConfigFile::write_header_start(const std::string &header_name) {
	bool is_successful = false;

	is_successful &= this->config_file_.Write("\n");
	is_successful  = this->config_file_.Write("<");
	is_successful &= this->config_file_.Write(header_name);
	is_successful &= this->config_file_.Write(">");
	is_successful &= this->config_file_.Write("\n\n");

	return is_successful;
}

bool ConfigFile::write_header_end(const std::string &header_name) {
	bool is_successful = false;

	is_successful &= this->config_file_.Write("\n");
	is_successful = this->config_file_.Write("<");
	is_successful &= this->config_file_.Write(header_name);
	is_successful &= this->config_file_.Write(" end");
	is_successful &= this->config_file_.Write(">");
	is_successful &= this->config_file_.Write("\n");

	return is_successful;
}

bool ConfigFile::write_tag(const std::string& tag_name, const std::string &value) {
	bool is_successful = false;

	is_successful  = this->config_file_.Write(tag_name);
	is_successful &= this->config_file_.Write("=");
	is_successful &= this->config_file_.Write(value);
	is_successful &= this->config_file_.Write("\n");

	return is_successful;
}

bool ConfigFile::Create() {
	bool is_successful = false;
	this->config_file_.set_filename(ConfigFile::kfile_name_);
	this->config_file_.set_directory(ConfigFile::kfile_dir_);
	this->config_file_.Create();

	std::string tag_name, tag_value;

	//Insert default parameters
	is_successful  = this->write_header_start("directories");

	tag_name  = "schema directory";
	tag_value = "";
	is_successful &= this->write_tag(tag_name,tag_value);

	tag_name = "database directory";
	tag_value = "";
	is_successful &= this->write_tag(tag_name, tag_value);

	is_successful &= this->write_header_end  ("directories");

	return is_successful;
}

bool ConfigFile::Destroy() {
	bool is_successful = false;
	is_successful = this->config_file_.Destroy();
	return is_successful;
}

/*
 * Read header title from file and return it, returns null if no header is found
 */
std::string ConfigFile::ScanHeader() {
	std::string header_string;
	this->config_file_.ReadBetweenDelimiters(header_string, '<', '>');

	return header_string;
}

HeaderContainer ParseTags(const std::string &input) {
	HeaderContainer header_container;
	if (input.empty() == true) {
		return header_container;
	}

	//TODO

	return header_container;
}

/* 
 * Parses config file and stores a copy of its contents in config_cache_
 * ----------------------------------------------------------------------
 * Notes:
 * > Keep in mind that the config file is meant for initializing the program at startup and is 
 * not intended to be read after the program is initialized. If this is not the case then ensure 
 * config_cache_ is updated after any changes to the config file are made.
 */
bool ConfigFile::ScanAndCache() {
	bool is_successful = false;

	ConfigFileContainer cache;

	//read file until eof is reached
	while (this->config_file_.get_iflags().is_good == true) {
		std::string header_name;
		while ( (header_name.empty() == false) && 
				(this->config_file_.get_iflags().is_good == true) ) 
		{
			header_name = this->ScanHeader();
		}
		//grab all text inside header and decrement ipos to allow header end delimiter to be found
		std::string buffer;
		this->config_file_.ReadToDelimiter(buffer, '<');
		std::streampos ipos;
		ipos = this->config_file_.get_ipos();
		this->config_file_.set_ipos(ipos-=1);

		HeaderContainer header;
		header = ParseTags(buffer);

		if ((header_name.empty() == false) && (header.tag_map.empty() == false)) {
			cache.header_map.insert(std::make_pair(header_name, header));
		}
	}
	if (this->config_file_.get_iflags().is_bad == true) { 
		std::cerr 
			<< "Configfile WARNING: Encountered bad read operation when caching config file"
			<<"\n";
		return is_successful = false; 
	}

	return is_successful;
}

bool ConfigFile::InitializeFromConfigFile(Core &core) {
	bool is_successful = false;

	return is_successful;
}