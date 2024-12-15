//==============================================================================
//	Name:
//		Solution.cpp
//
//	Description:
//		Advent of Code 2024 Day 4 Part 2
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
		std::cout << Solution::Solve(exampleInput) << ANSIEscapeCodes::ITALIC << " (expected 9)" << ANSIEscapeCodes::RESET << std::endl;
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

	// Iterate over the grid, searching for all occurrences of 'A'.
	for (size_t rowIndex = 0; rowIndex < gHeight; ++rowIndex)
	{
		for (size_t colIndex = 0; colIndex < gWidth; ++colIndex)
		{
			if (grid[rowIndex][colIndex] == 'A' && CheckXMAS(grid, rowIndex, colIndex))
			{
				total += 1;
			}
		}
	}

	Helper::PrintIf(debug,   "------------------------------------------------------------------------------");
	return total;
}





//==============================================================================
//		CheckXMAS - Given a grid and the position of a letter A, checks if that A
//		is the centre of an X-MAS.
//------------------------------------------------------------------------------
bool CheckXMAS(const std::vector<std::vector<char>>& grid, size_t rowIndex, size_t colIndex)
{
	// Check if all positions are within bounds.
	if (!WithinBounds(rowIndex - 1, colIndex - 1) ||
		!WithinBounds(rowIndex - 1, colIndex + 1) ||
		!WithinBounds(rowIndex + 1, colIndex - 1) ||
		!WithinBounds(rowIndex + 1, colIndex + 1))
	{
		return false;
	}

	// Check if the first diagonal is valid.
	bool diagonalOneValid = false;
	if ((grid[rowIndex - 1][colIndex - 1] == 'M' && grid[rowIndex + 1][colIndex + 1] == 'S') ||
		(grid[rowIndex - 1][colIndex - 1] == 'S' && grid[rowIndex + 1][colIndex + 1] == 'M'))
	{
		diagonalOneValid = true;
	}

	// Check if the second diagonal is valid.
	bool diagonalTwoValid = false;
	if ((grid[rowIndex + 1][colIndex - 1] == 'M' && grid[rowIndex - 1][colIndex + 1] == 'S') ||
		(grid[rowIndex + 1][colIndex - 1] == 'S' && grid[rowIndex - 1][colIndex + 1] == 'M'))
	{
		diagonalTwoValid = true;
	}

	// X-MAS if both diagonals are valid.
	return diagonalOneValid && diagonalTwoValid;
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
