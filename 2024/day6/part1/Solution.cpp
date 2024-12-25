//==============================================================================
//	Name:
//		Solution.cpp
//
//	Description:
//		Advent of Code 2024 Day 6 Part 1
//		https://adventofcode.com/2024/day/6
//------------------------------------------------------------------------------



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Solution.h"

#include "ANSIEscapeCodes.h"
#include "Helper.h"

#include <filesystem>
#include <format>





//==============================================================================
//		main
//------------------------------------------------------------------------------
int main()
{
	std::cout << std::endl;

	std::filesystem::path root = std::filesystem::canonical("/proc/self/exe").parent_path();

	std::cout << ANSIEscapeCodes::BOLD << "Example Solution: " << ANSIEscapeCodes::RESET;
	std::vector<std::string> exampleInput = Helper::ReadLines(root / "example.txt");
	if (!exampleInput.empty())
	{
		std::cout << Solution::Solve(exampleInput, true) << ANSIEscapeCodes::ITALIC << " (expected 41)" << ANSIEscapeCodes::RESET << std::endl;
	}

	std::cout << std::endl;

	std::cout << ANSIEscapeCodes::BOLD << "Input Solution: " << ANSIEscapeCodes::RESET;
	std::vector<std::string> input = Helper::ReadLines(root / "input.txt");
	if (!input.empty())
	{
		std::cout << Solution::Solve(input) << std::endl;
	}

	std::cout << std::endl;
}





namespace Solution
{



//==============================================================================
//		Map::Map(size_t height, size_t width);
//------------------------------------------------------------------------------
Map::Map(size_t height, size_t width)
	: Grid(height, width, '.')
{}





//==============================================================================
//		Map::Map(const std::vector<std::vector<char>>& grid)
//------------------------------------------------------------------------------
Map::Map(const std::vector<std::vector<char>>& grid)
	: Grid(grid)
{}





//==============================================================================
//		Map::Map(std::vector<std::vector<char>>&& grid)
//------------------------------------------------------------------------------
Map::Map(std::vector<std::vector<char>>&& grid)
	: Grid(grid)
{}





//==============================================================================
//		Solve - Returns the solution to the problem for the given input.
//------------------------------------------------------------------------------
int Solve(std::vector<std::string> input, bool debug/* = false*/)
{
	Helper::PrintIf(debug, "\n==============================================================================");

	// Turn the input into a map.
	Map map(input.size(), input[0].length());
	GridPosition guard;
	for (size_t lineIndex = 0; lineIndex < input.size(); ++lineIndex)
	{
		for (size_t charIndex = 0; charIndex < input[0].length(); ++charIndex)
		{
			char c = input[lineIndex][charIndex];
			map[lineIndex][charIndex] = c;
			if (c == '^' || c == '>' || c == 'v' || c == '<')
			{
				guard = {static_cast<int>(lineIndex), static_cast<int>(charIndex)};
			}
		}
	}

	// Simulate the guard's movement until they leave the map.
	Helper::PrintIf(debug, "\n{}", map);
	while (Move(map, guard, debug));
	Helper::PrintIf(debug, "\n{}", map);

	// Count the number of unique positions.
	int total = 0;
	for (size_t rowIndex = 0; rowIndex < map.GetHeight(); ++rowIndex)
	{
		for (size_t colIndex = 0; colIndex < map.GetWidth(); ++colIndex)
		{
			if (map[rowIndex][colIndex] == 'X')
			{
				total += 1;
			}
		}
	}

	Helper::PrintIf(debug,   "------------------------------------------------------------------------------");
	return total;
}





//==============================================================================
//		Move - Simulates the given guard's movement in the given map. Returns
//		true if the guard remains on the map and false if they leave it.
//------------------------------------------------------------------------------
bool Move(Map& map, GridPosition& guard, bool debug)
{
	char currentDirection = map[guard];

	// Figure out the new position/direction based on the current position and
	// direction.
	GridPosition newPosition;
	char         newDirection;
	if (currentDirection == '^')
	{
		newPosition  = {guard.rowIndex - 1, guard.colIndex};
		newDirection = '>';
	}
	else if (currentDirection == '>')
	{
		newPosition  = {guard.rowIndex, guard.colIndex + 1};
		newDirection = 'v';
	}
	else if (currentDirection == 'v')
	{
		newPosition  = {guard.rowIndex + 1, guard.colIndex};
		newDirection = '<';
	}
	else if (currentDirection == '<')
	{
		newPosition  = {guard.rowIndex, guard.colIndex - 1};
		newDirection = '^';
	}
	else
	{
		REQUIRE(false, "Invalid direction!");
	}

	// If the new position is not within bounds, the guard will leave the map
	// this movement.
	if (!map.WithinBounds(newPosition))
	{
		// Mark the current position.
		map[guard] = 'X';
		return false;
	}
	// If the new position is not an obstacle, the guard will move forward.
	else if (map[newPosition] != '#')
	{
		// Move the guard.
		map[guard] = 'X';
		guard = newPosition;
		map[guard] = currentDirection;
	}
	// Otherwise the guard will rotate 90 degrees clockwise.
	else
	{
		// Rotate the guard
		map[guard] = newDirection;
		Helper::PrintIf(debug, "\n{}", map);
	}

	return true;
}



} // Solution





//==============================================================================
//		std::formatter<Solution::Map, char> - Specialisation of the
//		std::formatter template for maps.
//------------------------------------------------------------------------------
template<>
struct std::formatter<Solution::Map, char>
{
	constexpr auto parse(format_parse_context& ctx)
	{
		return ctx.begin(); // No special parsing required.
	}

	template <typename FormatContext>
	auto format(const Solution::Map& map, FormatContext& ctx) const
	{
		std::string result;
		for (size_t rowIndex = 0; rowIndex < map.GetHeight(); ++rowIndex)
		{
			for (size_t colIndex = 0; colIndex < map.GetWidth(); ++colIndex)
			{
				char c = map[rowIndex][colIndex];
				if (c == '#')
				{
					result += ANSIEscapeCodes::RED;
					result += c;
					result += ANSIEscapeCodes::RESET;
				}
				else if (c == 'X')
				{
					result += ANSIEscapeCodes::BLUE;
					result += c;
					result += ANSIEscapeCodes::RESET;
				}
				else if (c == '^' || c == '>' || c == 'v' || c == '<')
				{
					result += ANSIEscapeCodes::BOLD;
					result += ANSIEscapeCodes::GREEN;
					result += c;
					result += ANSIEscapeCodes::RESET;
				}
				else
				{
					result += c;
				}
				if (colIndex != map.GetWidth() - 1)
				{
					result += " ";
				}
			}
			if (rowIndex != map.GetHeight() - 1)
			{
				result += "\n";
			}
		}
		return std::format_to(ctx.out(), "{}", result);
	}
};
