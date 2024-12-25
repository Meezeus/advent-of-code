//==============================================================================
//	Name:
//		Helper.cpp
//
//	Description:
//		Advent of Code 2024 Helper functions
//		(https://adventofcode.com/2024)
//------------------------------------------------------------------------------



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Helper.h"

#include "ANSIEscapeCodes.h"

#include <fstream>





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
		std::cout << ANSIEscapeCodes::RED;
		Print("The file {} doesn't exist!", fileName);
		std::cout << ANSIEscapeCodes::RESET;
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



}
