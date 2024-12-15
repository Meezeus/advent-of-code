//==============================================================================
//	Name:
//		Solution.cpp
//
//	Description:
//		Advent of Code 2024 Day 4 Part 1
//		https://adventofcode.com/2024/day/4
//------------------------------------------------------------------------------



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Solution.h"

#include "ANSIEscapeCodes.h"
#include "Helper.h"

#include <filesystem>





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
		std::cout << Solution::Solve(exampleInput) << ANSIEscapeCodes::ITALIC << " (expected 18)" << ANSIEscapeCodes::RESET << std::endl;
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
//		Globals
//------------------------------------------------------------------------------
size_t gHeight = 0;
size_t gWidth  = 0;



//==============================================================================
//		Solve - Returns the solution to the problem for the given input.
//------------------------------------------------------------------------------
int Solve(std::vector<std::string> input, bool debug/* = false*/)
{
	Helper::PrintIf(debug, "\n==============================================================================");

	// Convert the input into a grid.
	std::vector<std::vector<char>> grid;
	for (std::string line : input)
	{
		std::vector<char> row;
		for (char c : line)
		{
			row.push_back(c);
		}
		grid.push_back(row);
	}
	gHeight = grid.size();
	gWidth  = grid[0].size();

	int total = 0;

	// Iterate over the grid, searching for all occurrences of 'X'.
	for (size_t rowIndex = 0; rowIndex < gHeight; ++rowIndex)
	{
		for (size_t colIndex = 0; colIndex < gWidth; ++colIndex)
		{
			if (grid[rowIndex][colIndex] == 'X')
			{
				// Find all possible XMAS starting with this X.
				total += FindXMAS(grid, rowIndex, colIndex);
			}
		}
	}

	Helper::PrintIf(debug,   "------------------------------------------------------------------------------");
	return total;
}





//==============================================================================
//		FindXMAS - Given a grid and the position of a letter X, finds all
//		occurrences of XMAS that start with that X.
//------------------------------------------------------------------------------
int FindXMAS(std::vector<std::vector<char>>& grid, size_t rowIndex, size_t colIndex)
{
	int total = 0;

	// Check for XMAS in all directions.
	for (int directionNumber = 0;
		 directionNumber < static_cast<int>(Direction::NumberOfDirections);
		 ++directionNumber)
	{
		Direction direction = static_cast<Direction>(directionNumber);
		if (FindXMAS(grid, rowIndex, colIndex, direction, {'M', 'A', 'S'}))
		{
			total += 1;
		}
	}

	return total;
}





//==============================================================================
//		FindXMAS - Given a grid and a starting position, checks if the remaining
//		letters of XMAS can be found by following the given direction.
//------------------------------------------------------------------------------
bool FindXMAS(const std::vector<std::vector<char>>& grid,
			  size_t                                rowIndex,
			  size_t                                colIndex,
			  Direction                             direction,
			  std::vector<char>                     remainingLetters)
{
	// If no letters left, we have found XMAS!
	if (remainingLetters.empty())
	{
		return true;
	}

	// Move in the given direction.
	switch (direction)
	{
		case Direction::North:     rowIndex -= 1;                break;
		case Direction::NorthEast: rowIndex -= 1; colIndex += 1; break;
		case Direction::East:                     colIndex += 1; break;
		case Direction::SouthEast: rowIndex += 1; colIndex += 1; break;
		case Direction::South:     rowIndex += 1;                break;
		case Direction::SoutWest:  rowIndex += 1; colIndex -= 1; break;
		case Direction::West:                     colIndex -= 1; break;
		case Direction::NorthWest: rowIndex -= 1; colIndex -= 1; break;
		default: REQUIRE(false);                                 break;
	}

	// Check if the new position is within bounds and if it contains the next
	// letter.
	if (WithinBounds(rowIndex, colIndex) && grid[rowIndex][colIndex] == remainingLetters[0])
	{
		// Search for the remaining letters.
		remainingLetters.erase(remainingLetters.begin());
		return FindXMAS(grid, rowIndex, colIndex, direction, remainingLetters);
	}
	else
	{
		return false;
	}
}





//==============================================================================
//		WithinBounds - Checks if a given position is within bounds.
//------------------------------------------------------------------------------
bool WithinBounds(size_t rowIndex, size_t colIndex)
{
	if (rowIndex >= gHeight || colIndex >= gWidth)
	{
		return false;
	}
	else
	{
		return true;
	}
}



}
