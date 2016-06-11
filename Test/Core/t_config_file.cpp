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
	GIVEN("an initialized config file and core") {
		ConfigFile cfg;
		Database database;
		Core core(database);

		WHEN("the file is cached") {
			bool is_successful = cfg.InitializeCore(core);

			THEN("the cache contents must be valid") {

				REQUIRE(is_successful == true);
			}
		}
	}
}