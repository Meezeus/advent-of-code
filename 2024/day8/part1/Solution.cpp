//==============================================================================
//	Name:
//		Solution.cpp
//
//	Description:
//		Advent of Code 2024 Day 8 Part 1
//		https://adventofcode.com/2024/day/8
//------------------------------------------------------------------------------



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Solution.h"

#include "ANSIEscapeCodes.h"
#include "Grid.h"
#include "Helper.h"

#include <filesystem>
#include <map>
#include <set>





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
		std::cout << Solution::Solve(exampleInput, true) << ANSIEscapeCodes::ITALIC << " (expected 14)" << ANSIEscapeCodes::RESET << std::endl;
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
//		Solve - Returns the solution to the problem for the given input.
//------------------------------------------------------------------------------
int Solve(std::vector<std::string> input, bool debug/* = false*/)
{
	Helper::PrintIf(debug, "\n==============================================================================");

	// Convert the input into a grid.
	Grid grid(input.size(), input[0].length(), '.');
	for (size_t rowIndex = 0; rowIndex < input.size(); ++rowIndex)
	{
		for (size_t colIndex = 0; colIndex < input[0].length(); ++colIndex)
		{
			grid[rowIndex][colIndex] = input[rowIndex][colIndex];
		}
	}

	// Find all the different frequencies and the positions of their antennas.
	std::set<char> frequencies;
	std::map<char, std::vector<GridPosition>> antennaPositions;
	for (size_t rowIndex = 0; rowIndex < grid.GetHeight(); ++rowIndex)
	{
		for (size_t colIndex = 0; colIndex < grid.GetWidth(); ++colIndex)
		{
			char c = grid[rowIndex][colIndex];
			if (c != '.')
			{
				frequencies.insert(c);
				antennaPositions[c].push_back({rowIndex, colIndex});
			}
		}
	}

	// For each frequency...
	for (char frequency : frequencies)
	{
		// ...and for each pair of antenna positions of that frequency...
		for (GridPosition antenna1 : antennaPositions[frequency])
		{
			for (GridPosition antenna2 : antennaPositions[frequency])
			{
				// ...if the antenna positions are different...
				if (antenna1 != antenna2)
				{
					// ... calculate the delta from antenna1 to antenna2.
					int rowDelta = antenna2.rowIndex - antenna1.rowIndex;
					int colDelta = antenna2.colIndex - antenna1.colIndex;

					// The antinode is at the position antenna2 + delta.
					GridPosition antinode = {antenna2.rowIndex + rowDelta, antenna2.colIndex + colDelta};

					// If the antinode is within the grid, mark it.
					if (grid.WithinBounds(antinode))
					{
						grid[antinode] = '#';
					}
				}

			}
		}
	}
	Helper::PrintIf(debug, "{}", grid);

	// Count the number of unique antinode positions.
	int total = 0;
	for (size_t rowIndex = 0; rowIndex < grid.GetHeight(); ++rowIndex)
	{
		for (size_t colIndex = 0; colIndex < grid.GetWidth(); ++colIndex)
		{
			if (grid[rowIndex][colIndex] == '#')
			{
				total += 1;
			}
		}
	}

	Helper::PrintIf(debug,   "------------------------------------------------------------------------------");
	return total;
}



} // Solution
