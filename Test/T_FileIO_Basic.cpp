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
		File file(fn,dir);

		WHEN("a line of text is written and read")
		{
			file.create();
			std::string output = "test 1 2 3";
			std::string input;
			file.write(output + "\n");
			input = file.read_line();
			
			THEN("said line of text must match the one initially written")
			{
				REQUIRE(input == output);
			}
		}
		AND_WHEN("the file is cleared of content")
		{
			file.clear();
			std::string input;
			input = file.read_line();
			THEN("no contents in the file must exist and the file must exist")
			{
				REQUIRE(file.exists() == true);
				REQUIRE(input == "");
			}
			file.destroy();
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