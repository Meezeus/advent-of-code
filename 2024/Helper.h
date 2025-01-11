//==============================================================================
//	Name:
//		Helper.h
//
//	Description:
//		Advent of Code 2024 Helper functions
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
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>





//==============================================================================
//		REQUIRE - Crashes the program and prints an error message if the
//		condition evaluates to false.
//------------------------------------------------------------------------------
#define GET_REQUIRE(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, NAME, ...) NAME

#define REQUIRE(...) GET_REQUIRE(__VA_ARGS__,    /* _X = NumArgs     */\
                                 REQUIRE_3_To_N, /* _X = NumArgs + 1 */\
                                 REQUIRE_3_To_N, /* _X = NumArgs + 2 */\
                                 REQUIRE_3_To_N, /* _X = NumArgs + 3 */\
                                 REQUIRE_3_To_N, /* _X = NumArgs + 4 */\
                                 REQUIRE_3_To_N, /* _X = NumArgs + 5 */\
                                 REQUIRE_3_To_N, /* _X = NumArgs + 6 */\
                                 REQUIRE_3_To_N, /* _X = NumArgs + 7 */\
                                 REQUIRE_3_To_N, /* _X = NumArgs + 8 */\
                                 REQUIRE_3_To_N, /* _X = NumArgs + 9 */\
                                 REQUIRE_3_To_N, /* _X = NumArgs + 10 */\
                                 REQUIRE_2,      /* _X = NumArgs + 11 */\
                                 REQUIRE_1       /* _X = NumArgs + 12 */)(__VA_ARGS__)

#define REQUIRE_3_To_N(condition, message, ...) \
	do \
	{ \
		if (!(condition)) \
		{ \
			Helper::RequireImpl(#condition, message, __FILE__, __LINE__, __func__, __VA_ARGS__); \
			__builtin_trap(); \
		} \
	} while (false)

#define REQUIRE_2(condition, message) \
	do \
	{ \
		if (!(condition)) \
		{ \
			Helper::RequireImpl(#condition, message, __FILE__, __LINE__, __func__); \
			__builtin_trap(); \
		} \
	} while (false)

#define REQUIRE_1(condition) \
	do \
	{ \
		if (!(condition)) \
		{ \
			Helper::RequireImpl(#condition, "", __FILE__, __LINE__, __func__); \
			__builtin_trap(); \
		} \
	} while (false)





namespace Helper
{



//==============================================================================
//		RequireImpl - Implementation for the REQUIRE macro.
//------------------------------------------------------------------------------
void RequireImpl(const std::string& conditionString,
                 const std::string& message,
                 const char*        file,
                 int                line,
                 const char*        func,
                 const auto&...     args);





//==============================================================================
//		ReadLines - Reads the file corresponding to the given file name and
//		returns its contents as a list of lines.
//------------------------------------------------------------------------------
std::vector<std::string> ReadLines(const std::string& fileName);





//==============================================================================
//		Format - Formats args according to the format string.
//------------------------------------------------------------------------------
std::string Format(const std::string& formatString, const auto&... args);





//==============================================================================
//		Print - Calls Format with the given arguments and then prints the
//		result.
//------------------------------------------------------------------------------
void Print(const std::string& formatString, const auto&... args);





//==============================================================================
//		PrintIf - Calls Print with the given arguments, but only if the given
//		condition is true.
//------------------------------------------------------------------------------
void PrintIf(bool condition, const std::string& formatString, const auto&... args);



} // Helper





//==============================================================================
//		std::formatter<std::set<T>, char> - Specialisation of the std::formatter
//		template for formatting sets.
//------------------------------------------------------------------------------
template <typename T>
struct std::formatter<std::set<T>, char>;





//==============================================================================
//		std::formatter<std::vector<T>, char> - Specialisation of the
//		std::formatter template for formatting vectors.
//------------------------------------------------------------------------------
template <typename T>
struct std::formatter<std::vector<T>, char>;





//==============================================================================
//		std::formatter<std::vector<std::vector<T>>, char> - Specialisation of
//		the std::formatter template for formatting vectors of vectors.
//------------------------------------------------------------------------------
template <typename T>
struct std::formatter<std::vector<std::vector<T>>, char>;





//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Helper.inl"



#endif // HELPER
