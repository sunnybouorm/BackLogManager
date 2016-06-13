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
	this->config_file_.set_filename (ConfigFile::kfile_name_);
	this->config_file_.set_directory(ConfigFile::kfile_dir_);
	this->config_file_.Create();

	std::string tag_name, tag_value;

	//Insert default parameters
	is_successful    = this->write_header_start("directories");

	tag_name		 = "schema filename";
	tag_value		 = "schema.sql";//TODO change this
	is_successful   &= this->write_tag(tag_name, tag_value);

	tag_name		 = "schema directory";
	tag_value		 = "";//TODO change this
	is_successful	&= this->write_tag(tag_name,tag_value);

	tag_name		 = "database directory";
	tag_value		 = "";//TODO change this
	is_successful   &= this->write_tag(tag_name, tag_value);

	is_successful   &= this->write_header_end  ("directories");

	return is_successful;
}

bool ConfigFile::Destroy() {
	bool is_successful = false;
	is_successful = this->config_file_.Destroy();
	return is_successful;
}

/*
 * Search for tag name in the config file and set file pointers accordingly.
 * If tag_name specified is found then streampositions enclosing the desired tag is stored in bracket.
 * Otherwise ipos and opos are set to the last valid tag position near <header_name end> delimiter.
 *
 * returns true if no errors are encountered
 *---------------------------------------------------------------------------------------------------------------------
 * NOTES:
 * > Ensure position in file is enclosed by header delimiters and begins at the start of the first tag just after
 * <header_name> delimiter
 */
bool ConfigFile::EncloseTag(const std::string &tag_name, StreamposPair &bracket) {//TODO
	std::string line_buffer, tag_name_buffer;

	bool is_successful	= false;
	char ignore;

	is_successful  = this->config_file_.ReadChar(ignore);//discard two redundant newline characters
	is_successful &= this->config_file_.ReadChar(ignore);
	while (this->config_file_.get_iflags().is_good) {

		bracket.first  = this->config_file_.get_ipos();
		is_successful &= this->config_file_.ReadLine(line_buffer);
		bracket.second = this->config_file_.get_ipos();

		if (line_buffer.empty() == true) { //reached end of header, tag_name not found
			return is_successful; 
		}

		//check if desired tag is reached
		for (auto it = line_buffer.begin(); it != line_buffer.end(); ++it) {
			if (*it == '=') {
				if (tag_name == tag_name_buffer) {//tag_name found
					return is_successful;
				}
				tag_name_buffer.clear();
			}
			else {
				tag_name_buffer += *it;
			}
		}
	}

	return is_successful = false;
}

/*
 * Writes tag with tag_name and associated tag_value to the specified header in config file
 *
 * returns true if operation successful
 */
bool ConfigFile::WriteToHeader(std::string &header_name, std::string &tag_name, std::string &tag_value) {
	std::stringstream ss;

	ss << "Configfile Warning: failed to write tag to header";
	std::string err_msg_1 = ss.str();
	ss.str(std::string());

	ss << ", file format invalid";
	std::string err_msg_2 = ss.str();
	ss.str(std::string());

	bool is_successful = false;

	std::string cfg_header_name;
	std::string tag = tag_name + "=" + tag_value + "\n\n";
	StreamposPair bracket;

	//scan for matching header_name
	while(this->config_file_.get_iflags().is_good) {
		if (this->ScanHeader(cfg_header_name) == false) {
			std::cerr << err_msg_1 << "\n";

			return is_successful = false;
		}

		if (cfg_header_name == header_name) {
			//locate tag
			is_successful = this->EncloseTag(tag_name, bracket);

			if (is_successful == false) {
				std::cerr << err_msg_1 << err_msg_2 << "\n";

				return is_successful = false;
			}

			//write tag to desired location
			is_successful &= this->config_file_.Write(tag, bracket);

			return is_successful;
		}
	}

	//specified header name not found, append a new header name along with specified tag
	is_successful  = this->write_header_start(header_name);
	is_successful &= this->write_tag(tag_name, tag_value);
	is_successful &= this->write_header_end(header_name);

	return is_successful;
}

/*
 * Read header title from config file and store it in header_string, returns true if no errors are encountered
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