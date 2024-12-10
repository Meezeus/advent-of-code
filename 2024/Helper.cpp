//==============================================================================
//	Name:
//		Helper.cpp
//
//	Description:
//		Advent of Code 2024 Helper functions and classes
//		(https://adventofcode.com/2024)
//------------------------------------------------------------------------------



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Helper.h"

#include <iostream>
#include <fstream>
#include <format>





namespace Helper
{



//==============================================================================
//		ReadLines - Reads the file corresponding to the given file name and
//		returns its contents as a list of lines.
//------------------------------------------------------------------------------
std::vector<std::string> ReadLines(const std::string& fileName)
{
	std::vector<std::string> lines;

	std::ifstream inputFile(fileName);
	if (inputFile.fail())
	{
		Print("The file {} doesn't exist!", fileName);
		return lines;
	}

	std::string line;
	while (std::getline(inputFile, line))
	{
		lines.push_back(line);
	}

	inputFile.close();

	return lines;
}





//==============================================================================
//		Print - Format args according to the format string and then print the
//		result.
//------------------------------------------------------------------------------
void Print(const std::string& formatString, const auto&... args)
{
	std::cout << std::vformat(formatString, std::make_format_args(args...)) << std::endl;
}



}
