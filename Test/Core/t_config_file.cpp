#include "../../Source/stdafx.h"
#include "t_config_file.h"

SCENARIO("a config file is created and destroyed") {
	GIVEN("an initialized set of data") {
		
		WHEN("a config file is initialized and not destroyed") {
			ConfigFile file;
			THEN("it must exist") {
				REQUIRE(file.Exists() == true);
			}
		}

		AND_WHEN("the config file is initialized when the file already exists") {
			ConfigFile file;
			THEN("it must exist") {
				REQUIRE(file.Exists() == true);

				AND_WHEN("the file is destroyed") {
					file.Destroy();
					THEN("it must seize to exists") {
						REQUIRE(file.Exists() == false);
					}
				}
			}
		}
	}
}

SCENARIO("a fresh config file is cached") {
	GIVEN("an initialized config file") {
		ConfigFile cfg;
		cfg.Destroy();
		cfg.Create();

		WHEN("the file is cached") {
			bool is_successful = false;
			is_successful = cfg.ScanAndCache();

			THEN("the cache contents must be valid") {
				REQUIRE(is_successful == true);
				REQUIRE(cfg.get_cache().header_map.count("directories") == 1);

				REQUIRE(cfg.get_cache().header_map.at("directories").tag_map.count("schema filename") == 1);
				REQUIRE(cfg.get_cache().header_map.at("directories").tag_map.at("schema filename") == "schema.sql");

				REQUIRE(cfg.get_cache().header_map.at("directories").tag_map.count("schema directory")	== 1);
				REQUIRE(cfg.get_cache().header_map.at("directories").tag_map.at("schema directory")		== "");

				REQUIRE(cfg.get_cache().header_map.at("directories").tag_map.at("database directory")		== "");
				REQUIRE(cfg.get_cache().header_map.at("directories").tag_map.count("database directory")	== 1);

				AND_WHEN("the file is destroyed") {
					cfg.Destroy();
					THEN("it must seize to exists") {
						REQUIRE(cfg.Exists() == false);
					}
				}
			}
		}
	}
}

SCENARIO("a fresh config file is written to and cached") {
	GIVEN("an initialized config file") {
		ConfigFile cfg;
		cfg.Destroy();
		cfg.Create();

		std::string header_name, tag_name, tag_value;

		WHEN("the config file is written to and cached") {
			header_name = "directories";
			tag_name	= "test tag";
			tag_value	= "test value";

			REQUIRE(cfg.WriteToHeader(header_name, tag_name, tag_value) == true);

			std::string result = cfg.config_file_.to_string();
			std::string expected;
			expected += "\n";
			expected += "<directories>";
			expected += "\n\n";
			expected += "schema filename=schema.sql\n";
			expected += "schema directory=\n";
			expected += "database directory=\n";
			expected += "test tag=test value\n";
			expected += "<directories end>";
			expected += "\n";

			THEN("the data must be mapped correctly") {
				REQUIRE(result==expected);
			}
		}
	}
}