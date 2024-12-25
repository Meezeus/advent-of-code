//==============================================================================
//	Name:
//		Helper.inl
//
//	Description:
//		Advent of Code 2024 Helper functions
//		(https://adventofcode.com/2024)
//------------------------------------------------------------------------------





namespace Helper
{



//==============================================================================
//		RequireImpl - Implementation for the REQUIRE macro.
//-------------------------------------------------------------------------------
void RequireImpl(const std::string& conditionString,
                 const std::string& message,
                 const char*        file,
                 int                line,
                 const char*        func,
                 const auto&...     args)
{
	std::cout << ANSIEscapeCodes::RED;

	std::cout << ANSIEscapeCodes::BOLD;
	std::cout << "Require failed:";
	std::cout << ANSIEscapeCodes::RESET_BOLD;

	std::cout << ANSIEscapeCodes::RESET_FOREGROUND;
	std::cout << "\n\tREQUIRE(" << conditionString;
	if (!message.empty())
	{
		std::cout << ", \"" << Format(message, args...) << "\"";
	}
	std::cout << ")";
	std::cout << ANSIEscapeCodes::RED;

	std::cout << "\nin function \"" << func << "\",";
	std::cout << "\nin file \"" << file << "::" << line << "\"";

	std::cout << ANSIEscapeCodes::RESET << std::endl;
}





//==============================================================================
//		Format - Formats args according to the format string.
//------------------------------------------------------------------------------
std::string Format(const std::string& formatString, const auto&... args)
{
	return std::vformat(formatString, std::make_format_args(args...));
}





//==============================================================================
//		Print - Calls Format with the given arguments and then prints the
//		result.
//------------------------------------------------------------------------------
void Print(const std::string& formatString, const auto&... args)
{
	std::cout << Format(formatString, args...) << std::endl;
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





//==============================================================================
//		std::formatter<std::vector<std::vector<T>>, char> - Specialisation of
//		the std::formatter template for formatting vectors of vectors.
//------------------------------------------------------------------------------
template <typename T>
struct std::formatter<std::vector<std::vector<T>>, char>
{
	constexpr auto parse(format_parse_context& ctx)
	{
		return ctx.begin(); // No special parsing required.
	}

	template <typename FormatContext>
	auto format(const std::vector<std::vector<T>>& grid, FormatContext& ctx) const
	{
		// Find the maximum width of any element in the grid.
		size_t maxWidth = 0;
		for (size_t rowIndex = 0; rowIndex < grid.size(); ++rowIndex)
		{
			for (size_t colIndex = 0; colIndex < grid[rowIndex].size(); ++colIndex)
			{
				std::ostringstream oss;
				oss << grid[rowIndex][colIndex];
				maxWidth = std::max(maxWidth, oss.str().length());
			}
		}

		// Format the grid row by row with aligned columns.
		std::string result = "[\n";
		for (size_t rowIndex = 0; rowIndex < grid.size(); ++rowIndex)
		{
			result += "    [";
			for (size_t colIndex = 0; colIndex < grid[rowIndex].size(); ++colIndex)
			{
				std::ostringstream oss;
				oss << std::setw(maxWidth) << grid[rowIndex][colIndex];
				result += oss.str();
				if (colIndex != grid[rowIndex].size() - 1)
				{
					result += " ";
				}
			}
			result += "]";
			if (rowIndex != grid.size() - 1)
			{
				result += "\n";
			}
		}
		result += "\n]";
		return std::format_to(ctx.out(), "{}", result);
	}
};
