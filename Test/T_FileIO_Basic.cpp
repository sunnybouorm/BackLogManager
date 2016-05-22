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
		File file(fn,dir);
		WHEN("a line of text is written and read")
		{
			bool writeSuccess = false;
			bool readSuccess  = false;

			THEN("said line of text must match the one initially written")
			{
				REQUIRE(writeSuccess == true);
				REQUIRE(readSuccess == true);
			}
		}
		WHEN("the file is cleared")
		{
			bool clearSuccess = false;
			THEN("no contents in the file must exist")
			{
				REQUIRE(clearSuccess == true);
			}
		}
	}
}

SCENARIO("Multiple lines of text are written and read from a file")
{
	GIVEN("a text file")
	{
		WHEN("2 lines of text are written and read")
		{
			THEN("the text read must match the text written")
			{
				REQUIRE(false);
			}
		}
	}
}