//==============================================================================
//	Name:
//		Solution.cpp
//
//	Description:
//		Advent of Code 2024 Day 6 Part 2
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
		std::cout << Solution::Solve(exampleInput, true) << ANSIEscapeCodes::ITALIC << " (expected 6)" << ANSIEscapeCodes::RESET << std::endl;
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





//==============================================================================position
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

	// Turn the input into a map and get the guard's starting position.
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

	// Keep track of the movement history.
	MovementHistory history;
	// Keep track of valid obstacle positions.
	ValidLocations validObstacles;
	// Find all valid obstacle positions.
	FindValidObstaclePositions(map, guard, history, validObstacles, debug);

	// Count the number of valid obstacle positions.
	int total = 0;
	for (size_t rowIndex = 0; rowIndex < map.GetHeight(); ++rowIndex)
	{
		for (size_t colIndex = 0; colIndex < map.GetWidth(); ++colIndex)
		{
			GridPosition position = {static_cast<int>(rowIndex), static_cast<int>(colIndex)};
			if (validObstacles[position])
			{
				total += 1;
			}
		}
	}

	Helper::PrintIf(debug,   "------------------------------------------------------------------------------");
	return total;
}





//==============================================================================
//		FindValidObstaclePositions - Finds and records all valid obstacle
//		positions that will cause the guard to enter a loop.
//------------------------------------------------------------------------------
void FindValidObstaclePositions(Map&             map,
                                GridPosition&    guard,
                                MovementHistory& history,
                                ValidLocations&  validObstacles,
                                bool             debug)
{
	// Print the starting map.
	Helper::PrintIf(debug, "\n{}", map);

	// Create copies of the parameters.
	Map                 mapCopy = map;
	GridPosition      guardCopy = guard;
	MovementHistory historyCopy = history;

	// Check if the next position is a valid position to place an obstacle.
	GridPosition nextPosition = GetNextPosition(mapCopy, guardCopy);
	if (mapCopy.WithinBounds(nextPosition) &&
	    mapCopy[nextPosition] != '#'       &&
	    mapCopy[nextPosition] != 'X'       &&
	    !validObstacles[nextPosition])
	{
		Helper::PrintIf(debug, "\nAttempting to create a loop by placing an obstacle at {}", nextPosition);

		// Place the obstacle.
		mapCopy[nextPosition] = '@';

		// Simulate guard movement until a loop is found or the guard leaves the
		// map.
		bool loopFound = false;
		while (!loopFound && Move(mapCopy, guardCopy, historyCopy, debug))
		{
			// Check for a loop.
			if (historyCopy[{guardCopy, mapCopy[guardCopy]}])
			{
				loopFound = true;
				validObstacles[nextPosition] = true;
			}
		}

		// Print the map after simulating guard movement in the map copy. Then
		// print the results.
		Helper::PrintIf(debug, "\n{}", mapCopy);
		if (loopFound)
		{
			Helper::PrintIf(debug, "\n{}Found a loop!{}",  ANSIEscapeCodes::GREEN, ANSIEscapeCodes::RESET);
		}
		else
		{
			Helper::PrintIf(debug, "\n{}No loop found!{}", ANSIEscapeCodes::RED,   ANSIEscapeCodes::RESET);
		}
	}
	// Otherwise skip trying to place an obstacle.
	else
	{
		Helper::PrintIf(debug, "\nThe next location is not a valid obstacle location!");
	}

	// Move one step in the original map and check if the guard has left the
	// map.
	bool guardLeft = !Move(map, guard, history, false);

	// If we are still on the map, keep searching for valid obstacles locations.
	if (!guardLeft)
	{
		FindValidObstaclePositions(map, guard, history, validObstacles, debug);
	}
}





//==============================================================================
//		Move - Simulates the given guard's movement in the given map. Records
//		the movement history. Returns true if the guard remains on the map and
//		false if they leave it.
//------------------------------------------------------------------------------
bool Move(Map& map, GridPosition& guard, MovementHistory& history, bool debug)
{
	// Get the current direction.
	char currentDirection = map[guard];

	// Record the current position and direction in the history.
	history[{guard, currentDirection}] = true;

	// Figure out the new position and direction.
	GridPosition nextPosition  = GetNextPosition (map, guard);
	char         nextDirection = GetNextDirection(currentDirection);

	// If the new position is not within bounds, the guard will leave the map
	// this movement.
	if (!map.WithinBounds(nextPosition))
	{
		// Mark the current position.
		map[guard] = 'X';
		return false;
	}
	// If the new position is not an obstacle, the guard will move forward.
	else if (map[nextPosition] != '#' && map[nextPosition] != '@')
	{
		// Move the guard.
		map[guard] = 'X';
		guard = nextPosition;
		map[guard] = currentDirection;
	}
	// Otherwise the guard will change directions.
	else
	{
		// Rotate the guard
		map[guard] = nextDirection;
		Helper::PrintIf(debug, "\n{}", map);
	}

	return true;
}





//==============================================================================
//		GetNextPosition - Returns the position in front of the guard.
//------------------------------------------------------------------------------
GridPosition GetNextPosition(const Map& map, const GridPosition& guard)
{
	GridPosition nextPosition;

	char currentDirection = map[guard];
	if (currentDirection == '^')
	{
		nextPosition  = {guard.rowIndex - 1, guard.colIndex};
	}
	else if (currentDirection == '>')
	{
		nextPosition  = {guard.rowIndex, guard.colIndex + 1};
	}
	else if (currentDirection == 'v')
	{
		nextPosition  = {guard.rowIndex + 1, guard.colIndex};
	}
	else if (currentDirection == '<')
	{
		nextPosition  = {guard.rowIndex, guard.colIndex - 1};
	}
	else
	{
		REQUIRE(false, "Invalid direction {}!", currentDirection);
	}

	return nextPosition;
}





//==============================================================================
//		GetNextDirection - Returns the direction the guard will face after
//		rotating 90 degrees clockwise from the current direction.
//------------------------------------------------------------------------------
char GetNextDirection(char currentDirection)
{
	char nextDirection;

	if (currentDirection == '^')
	{
		nextDirection = '>';
	}
	else if (currentDirection == '>')
	{
		nextDirection = 'v';
	}
	else if (currentDirection == 'v')
	{
		nextDirection = '<';
	}
	else if (currentDirection == '<')
	{
		nextDirection = '^';
	}
	else
	{
		REQUIRE(false, "Invalid direction {}!", currentDirection);
	}

	return nextDirection;
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
				if (c == '@')
				{
					result += ANSIEscapeCodes::YELLOW;
					result += '#';
					result += ANSIEscapeCodes::RESET;
				}
				else if (c == '#')
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
