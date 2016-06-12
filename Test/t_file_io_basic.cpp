/*
 * Unit test for File object
 */
#include "../Source/stdafx.h"
#include "t_file_io_basic.h"

SCENARIO("A file is created and destroyed using static members") 
{
	GIVEN("An initialized set of data")
	{
		const std::string filename = "test_file.txt";
		const std::string dir = "";

		WHEN("A file is created then destroyed")
		{
			bool is_created = false;
			bool is_destroyed = false;
			std::fstream fs;
			File::Create(filename, dir);
			is_created = File::Exists(filename, dir);
			File::Destroy(filename, dir);
			is_destroyed = File::Exists(filename, dir);

			THEN("the result of checking if the file exists must correlate")
			{
				REQUIRE(is_created   == true);
				REQUIRE(is_destroyed == false);
			} 
		}
	}
}

SCENARIO("A file is created and destroyed using non-static members")
{
	GIVEN("An initialized set of data")
	{
		const std::string filename = "test_file.txt";
		const std::string dir = "";
		File file(filename,dir);
		if (file.Exists() == true) { file.Destroy(); }

		WHEN("A file is created then destroyed")
		{
			bool is_created   = false;
			bool is_destroyed = false;
			file.Create();
			is_created   = file.Exists();
			file.Destroy();
			is_destroyed = file.Exists();

			THEN("the result of checking if the file exists must correlate")
			{
				REQUIRE(is_created   == true);
				REQUIRE(is_destroyed == false);
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
		if(file.Exists() == true){ file.Destroy(); }
		file.Create();

		WHEN("a line of text is written and read")
		{
			std::string output = "test 1 2 3";
			std::string input;
			file.Write(output + "\n");
			file.ReadLine(input);
			REQUIRE(file.get_iflags().is_bad == false);
			
			THEN("said line of text must match the one initially written")
			{
				REQUIRE(input == output);
			}
			AND_WHEN("the file is cleared of content")
			{
				file.Clear();
				std::string input;
				file.ReadLine(input);
				REQUIRE(file.get_iflags().is_bad == false);

				THEN("no contents in the file must exist and the file must exist")
				{
					REQUIRE(file.Exists() == true);
					REQUIRE(input == "");
				}
			}
		}
		file.Destroy();
	}
}

SCENARIO("Multiple lines of text are written and read from a file")
{
	GIVEN("a text file")
	{
		std::string fn = "textFile.txt";
		std::string dir = "";
		File file(fn, dir);
		if (file.Exists() == true) { file.Destroy(); }
		
		
		WHEN("2 lines of text are written and read")
		{
			file.Create();
			std::string output_1 = "123";
			std::string output_2 = "456";
			std::string input_1, input_2;
			file.Write(output_1 + "\n");
			file.Write(output_2 + "\n");

			file.ReadLine(input_1);
			REQUIRE(file.get_iflags().is_bad == false);

			file.ReadLine(input_2);
			REQUIRE(file.get_iflags().is_bad == false);

			THEN("the text read must match the text written")
			{
				REQUIRE(input_1 == output_1);
				REQUIRE(input_2 == output_2);

				file.Destroy();
			}
		}
	}
}

SCENARIO("A line of text is read and EOF is reached") {
	GIVEN("a text file")
	{
		std::string fn = "textFile.txt";
		std::string dir = "";
		File file(fn, dir);
		if (file.Exists() == true) { file.Destroy(); }

		WHEN("EOF is reached") 
		{
			file.Create();
			std::string output = "123";
			std::string input;
			file.Write(output);
			file.ReadLine(input);
			THEN("Whatever text was before EOF must be read and i_flags must be correct")
			{
				REQUIRE(input==output);
				REQUIRE(file.get_iflags().is_eof  == true);
				REQUIRE(file.get_iflags().is_good == false);
				REQUIRE(file.get_iflags().is_bad  == false);
				AND_WHEN("another line is read without reaching EOF")
				{
					file.Write(output + "\n");
					file.ReadLine(input);
					REQUIRE(file.get_iflags().is_bad == false);
					THEN("the text and i_flags must be correct")
					{
						REQUIRE(input==output);
						REQUIRE(file.get_iflags().is_eof  == false);
						REQUIRE(file.get_iflags().is_good == true);
						REQUIRE(file.get_iflags().is_bad  == false);
					}
				}
			}
		}
		file.Destroy();
	}
}