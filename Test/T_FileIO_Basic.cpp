#include "stdafx.h"
#include "T_FileIO_Basic.h"

SCENARIO("A file is created and destroyed") 
{
	GIVEN("An initialized set of data")
	{
		const std::string filename = "testFile.txt";
		const std::string dir = "";

		bool isExist = false;
		WHEN("A file is created")
		{
			File::create(filename, dir);
			THEN("It must exist")
			{
				isExist = File::exists(filename, dir);
				REQUIRE(isExist == true);
			}
		}

		WHEN("A file is removed")
		{
			isExist = true;//to ensure function is setting to false
			File::destroy(filename, dir);
			THEN("It must seize to exist")
			{
				isExist = File::exists(filename, dir);
				REQUIRE(isExist == false);
			}
		}
	}
}