//==============================================================================
//	Name:
//		Helper.h
//
//	Description:
//		Advent of Code 2024 Helper functions and classes
//		(https://adventofcode.com/2024)
//------------------------------------------------------------------------------
#ifndef HELPER
#define HELPER



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include <format>
#include <iostream>
#include <string>
#include <vector>





namespace Helper
{



//==============================================================================
//		ReadLines - Reads the file corresponding to the given file name and
//		returns its contents as a list of lines.
//------------------------------------------------------------------------------
std::vector<std::string> ReadLines(const std::string& fileName);





//==============================================================================
//		Print - Format args according to the format string and then print the
//		result.
//------------------------------------------------------------------------------
void Print(const std::string& formatString, const auto&... args)
{
	std::cout << std::vformat(formatString, std::make_format_args(args...)) << std::endl;
}



}



#endif // HELPER
