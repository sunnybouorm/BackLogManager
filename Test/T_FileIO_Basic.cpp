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

SCENARIO("A line of text is written and read from a file")
{
	GIVEN("A text file")
	{
		std::string fn  = "textFile.txt";
		std::string dir = "";
		File::create(fn,dir);
		WHEN("a line of text is written and read")
		{
			THEN("said line of text must match the one initially written")
			{
				REQUIRE(false);
			}
		}
	}
}