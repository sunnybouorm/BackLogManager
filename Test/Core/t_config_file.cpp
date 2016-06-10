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