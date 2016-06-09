#include "../Source/stdafx.h"
#include "t_utility.h"

void PrintTableResult(TableContainer data) {

	int row_count = 0;
	std::cout
		<< "----------------------\n"
		<< "TableContainer contents:\n"
		<< "----------------------\n";
	if (data.empty() == false) {
		for (auto row = data.begin(); row != data.end(); row++) {
			std::cout << "row#<" << std::to_string(row_count) << ">\n";
			row_count++;
			for (auto col = row->begin(); col != row->end(); ++col) {
				std::cout
					<< "<" << col->first << ">"
					<< "<" << col->second << ">" << "\n";
			}
			std::cout << "\n";
		}
	}
	else {
		std::cout << "result buffer empty\n";
	}

	std::cout
		<< "______________________\n"
		<< "TableContainer END"
		<< "\n\n";

}