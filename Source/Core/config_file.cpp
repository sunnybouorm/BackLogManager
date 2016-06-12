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

	tag_name		= "schema filename";
	tag_value		= "schema.sql";//TODO change this
	is_successful  &= this->write_tag(tag_name, tag_value);

	tag_name		 = "schema directory";
	tag_value		 = "";//TODO change this
	is_successful	&= this->write_tag(tag_name,tag_value);

	tag_name		= "database directory";
	tag_value		= "";//TODO change this
	is_successful  &= this->write_tag(tag_name, tag_value);

	is_successful  &= this->write_header_end  ("directories");

	return is_successful;
}

bool ConfigFile::Destroy() {
	bool is_successful = false;
	is_successful = this->config_file_.Destroy();
	return is_successful;
}

/*
 * Read tag name from config file and return it, returns null if no tag name is found
 *
 * NOTES:
 * > position in file must be between enclosed by header delimiters
 */
bool ConfigFile::ScanTag(std::string &tag_name) {//TODO
	bool is_successful = false;
	char character;

	do {
		is_successful = this->config_file_.ReadChar(character);

		if (character == '\n') {
			//TODO 
		}

	} while (is_successful == true);

	return is_successful;
}

bool ConfigFile::WriteToHeader(std::string &header_name, std::string &tag_name, std::string &tag_value) {//TODO
	bool is_successful = false;

	std::string cfg_header_name;

	if (this->ScanHeader(cfg_header_name) == false) {
	//TODO error
	}

	if (cfg_header_name == header_name) {}

	return is_successful;
}

/*
 * Read header title from config file and return it, returns null if no header is found
 */
bool ConfigFile::ScanHeader(std::string &header_string) {
	header_string.clear();
	return (this->config_file_.ReadBetweenDelimiters(header_string, '<', '>'));
}

HeaderContainer ParseTags(const std::string &input) {
	std::stringstream ss;
	ss << "Configfile Warning: failed to parse config file";
	std::string err_msg_1 = ss.str();
	ss.str(std::string());

	ss << ", character [";
	std::string err_msg_2 = ss.str();
	ss.str(std::string());

	ss << "] with is_rhs=[";
	std::string err_msg_3 = ss.str();
	ss.str(std::string());

	ss << "] is invalid, ensure config file format is valid";
	std::string err_msg_4 = ss.str();
	ss.str(std::string());

	HeaderContainer header_container;
	if (input.empty() == true) {
		header_container.is_good = true;
		return header_container;
	}

	std::string tag_name;
	std::string tag_value;
	bool is_rhs = false;

	// parse input string for keys and values to map to header_container
	for (auto it = input.begin(); it != input.end(); ++it) {

		if (*it == '<') {//invalid character
			header_container.is_good = false;
			std::cerr << err_msg_1 << err_msg_2 << *it << err_msg_3 << is_rhs << err_msg_4 << "\n";
			return header_container;
		}
		else if (*it == '>') {//invalid character
			std::cerr << err_msg_1 << err_msg_2 << *it << err_msg_3 << is_rhs << err_msg_4 << "\n";
			header_container.is_good = false;
			return header_container;
		}
		else if ( (is_rhs == false) && (*it != '\n') && (*it != '=') ) {
			tag_name += *it;
		}
		else if ((is_rhs == true) && (*it != '\n') && (*it != '=')) {
			tag_value += *it;
		}
		else if ( (*it == '=') && (is_rhs == false) ) {
			is_rhs = true;
		} 
		else if (*it == '\n') {
			header_container.is_good = true;
			if (tag_name.empty() == false) {
				header_container.tag_map.insert(std::make_pair(tag_name, tag_value));
			}

			is_rhs = false;
			tag_name.clear();
			tag_value.clear();
		}
		else {//invalid format
			std::cerr << err_msg_1 << err_msg_2 << *it << err_msg_3 << is_rhs << err_msg_4 << "\n";
			header_container.is_good = false;
			return header_container;
		}
	}

	return header_container;
}

/*
 * Scans through the ConfigFileContainer and checks if all its HeaderContainers
 * have a good signature
 */
bool ConfigFile::CheckCacheIntegrity(const ConfigFileContainer &cache) {
	bool is_valid = false;

	for (auto header = cache.header_map.begin(); header != cache.header_map.end(); ++header) {
		if (header->second.is_good == false) { 
			return is_valid = false; 
		}
	}

	return is_valid = true;
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
	std::stringstream ss;

	ss << "Configfile Warning: ScanAndCache operation failed";
	std::string err_msg_1 = ss.str();
	ss.str(std::string());

	ss << ", header delimiter invalid or does not exist";
	std::string err_msg_2 = ss.str();
	ss.str(std::string());

	ss << ", parse operation failed";
	std::string err_msg_3 = ss.str();
	ss.str(std::string());

	ss << ", ensure config file format is valid";
	std::string err_msg_4 = ss.str();
	ss.str(std::string());

	ss << "Configfile WARNING: Encountered bad read operation when caching config file";
	std::string err_msg_5 = ss.str();
	ss.str(std::string());

	bool is_successful = false;
	std::string header_name, header_name_end;

	//read file until eof is reached
	while (this->config_file_.get_iflags().is_good == true) {

		//find a header
		if (this->ScanHeader(header_name) == false) {
			std::cerr << err_msg_5 << "\n";
			return is_successful = false;
		}
		else if (header_name.empty() == true) {//operation complete, no more headers to process
			break;
		}

		//grab all text inside header and decrement ipos to allow header end delimiter to be found
		std::string buffer;
		if (this->config_file_.ReadToDelimiter(buffer, '<') == false) {
			std::cerr << err_msg_5 << "\n";
			return is_successful = false;
		}

		std::streampos ipos;
		ipos = this->config_file_.get_ipos();
		this->config_file_.set_ipos(ipos-=1);

		HeaderContainer header;
		header = ParseTags(buffer);

		//check parse operation success
		if (header.is_good == false) {
			std::cerr << err_msg_1 << err_msg_3 << err_msg_4 << "\n";
			return is_successful = false;
		}

		//check if header delimiter is valid
		if (this->ScanHeader(header_name_end) == false) {
			std::cerr << err_msg_5 << "\n";
			return is_successful = false;
		}

		if (header_name_end != (header_name + " end")) {
			
			std::cerr << err_msg_1 << err_msg_2 << err_msg_4 << "\n";
			return is_successful = false;
		}

		//Map headers, tags and their associated values to the cache
		if ((header_name.empty() == false) && (header.tag_map.empty() == false)) {
			this->config_cache_.header_map.insert(std::make_pair(header_name, header));
		}
	}

	return is_successful = true;
}