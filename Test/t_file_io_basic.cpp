#include "stdafx.h"
#include "t_file_io_basic.h"

SCENARIO("A file is created and destroyed using static members") 
{
	GIVEN("An initialized set of data")
	{
		const std::string filename = "test_file.txt";
		const std::string dir = "D:\\Development\\Projects\\BacklogManager\\unused_test_directory\\";

		bool is_exist = false;
		WHEN("A file is created")
		{
			File::Create(filename, dir);
			THEN("It must exist")
			{
				is_exist = File::Exists(filename, dir);
				REQUIRE(is_exist == true);
			} AND_WHEN("the file is removed")
			{
				is_exist = true;//to ensure function is setting to false
				File::Destroy(filename, dir);
				THEN("It must seize to exist")
				{
					is_exist = File::Exists(filename, dir);
					REQUIRE(is_exist == false);
				}
			}
		}
	}
}

SCENARIO("A file is created and destroyed using non-static members")
{
	GIVEN("An initialized set of data")
	{
		const std::string filename = "test_file.txt";
		const std::string dir = "D:\\Development\\Projects\\BacklogManager\\unused_test_directory\\";
		File file(filename,dir);

		bool is_exist = false;
		WHEN("A file is created")
		{
			file.Create();
			THEN("It must exist")
			{
				is_exist = File::Exists(filename, dir);
				REQUIRE(is_exist == true);
			} AND_WHEN("A file is removed")
			{
				is_exist = true;//to ensure function is setting to false
				file.Destroy();
				THEN("It must seize to exist")
				{
					is_exist = File::Exists(filename, dir);
					REQUIRE(is_exist == false);
				}
			}
		}
	}
}

SCENARIO("A line of text is written and read from a file")
{
	GIVEN("A text file")
	{
		std::string fn  = "textFile.txt";
		std::string dir = "D:\\Development\\Projects\\BacklogManager\\unused_test_directory\\";
		File file(fn,dir);
		file.Destroy();
		file.Create();

		WHEN("a line of text is written and read")
		{
			std::string output = "test 1 2 3";
			std::string input;
			file.Write(output + "\n");
			file.ReadLine(input);
			REQUIRE(file.i_flags_.is_bad == false);
			
			THEN("said line of text must match the one initially written")
			{
				REQUIRE(input == output);
			}
			AND_WHEN("the file is cleared of content")
			{
				file.Clear();
				std::string input;
				file.ReadLine(input);
				REQUIRE(file.i_flags_.is_bad == false);

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
		std::string dir = "D:\\Development\\Projects\\BacklogManager\\unused_test_directory\\";
		File file(fn, dir);
		file.Destroy();
		file.Create();
		
		WHEN("2 lines of text are written and read")
		{
			std::string output_1 = "123";
			std::string output_2 = "456";
			std::string input_1, input_2;
			file.Write(output_1 + "\n");
			file.Write(output_2 + "\n");

			file.ReadLine(input_1);
			REQUIRE(file.i_flags_.is_bad == false);

			file.ReadLine(input_2);
			REQUIRE(file.i_flags_.is_bad == false);

			THEN("the text read must match the text written")
			{
				REQUIRE(input_1 == output_1);
				REQUIRE(input_2 == output_2);
			}
		}
		file.Destroy();
	}
}

SCENARIO("A line of text is read and EOF is reached") {
	GIVEN("a text file")
	{
		std::string fn = "textFile.txt";
		std::string dir = "D:\\Development\\Projects\\BacklogManager\\unused_test_directory\\";
		File file(fn, dir);
		file.Destroy();
		file.Create();
		std::string output = "123";
		std::string input;
		file.Write(output);

		WHEN("EOF is reached") 
		{
			file.ReadLine(input);
			REQUIRE(file.i_flags_.is_bad == false);
			THEN("Whatever text was before EOF must be read and i_flags must be correct")
			{
				REQUIRE(input==output);
				REQUIRE(file.i_flags_.is_eof  == true);
				REQUIRE(file.i_flags_.is_good == false);
				REQUIRE(file.i_flags_.is_bad  == false);
				AND_WHEN("another line is read without reaching EOF")
				{
					file.Write(output + "\n");
					file.ReadLine(input);
					REQUIRE(file.i_flags_.is_bad == false);
					THEN("the text and i_flags must be correct")
					{
						REQUIRE(input==output);
						REQUIRE(file.i_flags_.is_eof  == false);
						REQUIRE(file.i_flags_.is_good == true);
						REQUIRE(file.i_flags_.is_bad  == false);
					}
				}
			}
		}
		file.Destroy();
	}
}