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
#include <sstream>
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





//==============================================================================
//		PrintIf - Calls Print with the given arguments, but only if the given
//		condition is true.
//------------------------------------------------------------------------------
void PrintIf(bool condition, const std::string& formatString, const auto&... args)
{
	if (condition)
	{
		Print(formatString, args...);
	}
}



}





//==============================================================================
//		std::formatter<std::vector<T>, char> - Specialisation of the
//		std::formatter template for formatting vectors.
//------------------------------------------------------------------------------
template <typename T>
struct std::formatter<std::vector<T>, char>
{
	constexpr auto parse(format_parse_context& ctx)
	{
		return ctx.begin(); // No special parsing required.
	}

	template <typename FormatContext>
	auto format(const std::vector<T>& vec, FormatContext& ctx) const
	{
		std::string result = "[";
		for (size_t i = 0; i < vec.size(); ++i)
		{
			if constexpr (std::is_same_v<T, std::string>)
			{
				result += "\"" + vec[i] + "\"";
			}
			else if constexpr (std::is_arithmetic_v<T>)
			{
				result += std::to_string(vec[i]);
			}
			else
			{
				// Use a stringstream for other types.
				std::stringstream ss;
				ss << vec[i];
				result += ss.str();
			}
			if (i != vec.size() - 1)
			{
				result += ", ";
			}
		}
		result += "]";
		return format_to(ctx.out(), "{}", result);
	}
};



#endif // HELPER
