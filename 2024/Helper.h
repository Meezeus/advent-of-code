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
#include "ANSIEscapeCodes.h"

#include <csignal>
#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>





//==============================================================================
//		REQUIRE - Crashes the program and prints an error message if the
//		condition evaluates to false.
//------------------------------------------------------------------------------
#define REQUIRE(condition) \
	do \
	{ \
		if (!(condition)) \
		{ \
			std::cout << ANSIEscapeCodes::RED \
					  << "Require failed: " #condition \
					  << "\n\t in file \"" << __FILE__ << "::" << __LINE__ << "\"" \
					  << "\n\t in function \"" << __func__ << "\"" \
					  << ANSIEscapeCodes::RESET << std::endl; \
			__builtin_trap(); \
		} \
	} while (false)





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
void Print(const std::string& formatString, const auto&... args);





//==============================================================================
//		PrintIf - Calls Print with the given arguments, but only if the given
//		condition is true.
//------------------------------------------------------------------------------
void PrintIf(bool condition, const std::string& formatString, const auto&... args);



}





//==============================================================================
//		std::formatter<std::vector<T>, char> - Specialisation of the
//		std::formatter template for formatting vectors.
//------------------------------------------------------------------------------
template <typename T>
struct std::formatter<std::vector<T>, char>;





//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Helper.inl"



#endif // HELPER
