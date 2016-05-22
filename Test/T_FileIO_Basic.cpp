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
		file.destroy();
		file.create();

		WHEN("a line of text is written and read")
		{
			std::string output = "test 1 2 3";
			std::string input;
			file.write(output + "\n");
			file.read_line(&input);
			
			THEN("said line of text must match the one initially written")
			{
				REQUIRE(input == output);
			}
			AND_WHEN("the file is cleared of content")
			{
				file.clear();
				std::string input;
				file.read_line(&input);
				THEN("no contents in the file must exist and the file must exist")
				{
					REQUIRE(file.exists() == true);
					REQUIRE(input == "");
				}
			}
		}
		file.destroy();
	}
}

SCENARIO("Multiple lines of text are written and read from a file")
{
	GIVEN("a text file")
	{
		std::string fn = "textFile.txt";
		std::string dir = "";
		File file(fn, dir);
		file.destroy();
		file.create();
		
		WHEN("2 lines of text are written and read")
		{
			std::string output_1 = "123";
			std::string output_2 = "456";
			std::string input_1, input_2;
			file.write(output_1 + "\n");
			file.write(output_2 + "\n");
			file.read_line(&input_1);
			file.read_line(&input_2);

			THEN("the text read must match the text written")
			{
				REQUIRE(input_1 == output_1);
				REQUIRE(input_2 == output_2);
			}
		}
		file.destroy();
	}
}

SCENARIO("A line of text is read and EOF is reached") {
	GIVEN("a text file")
	{
		std::string fn = "textFile.txt";
		std::string dir = "";
		File file(fn, dir);
		file.destroy();
		file.create();
		std::string output = "123";
		std::string input;
		file.write(output);

		WHEN("EOF is reached") {
			file.read_line(&input);
			THEN("Whatever text was before EOF must be read and i_flags must be correct") {
				REQUIRE(input==output);
				REQUIRE(file.i_flags.isEof  == true);
				REQUIRE(file.i_flags.isGood == false);
				REQUIRE(file.i_flags.isBad  == false);
			}
		}
		file.destroy();
	}
}