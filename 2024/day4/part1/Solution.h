//==============================================================================
//	Name:
//		Solution.h
//
//	Description:
//		Advent of Code 2024 Day 4 Part 1
//		https://adventofcode.com/2024/day/4
//------------------------------------------------------------------------------
#ifndef SOLUTION
#define SOLUTION



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include <string>
#include <vector>





//==============================================================================
//		main
//------------------------------------------------------------------------------
int main();





namespace Solution
{





//==============================================================================
//		Types
//------------------------------------------------------------------------------
enum class Direction
{
	North,
	NorthEast,
	East,
	SouthEast,
	South,
	SoutWest,
	West,
	NorthWest,
	NumberOfDirections
};



//==============================================================================
//		Solve - Returns the solution to the problem for the given input.
//------------------------------------------------------------------------------
int Solve(std::vector<std::string> input, bool debug = false);





//==============================================================================
//		FindXMAS - Given a grid and the position of a letter X, finds all
//		occurrences of XMAS that start with that X.
//------------------------------------------------------------------------------
int FindXMAS(std::vector<std::vector<char>>& grid, size_t rowIndex, size_t colIndex);





//==============================================================================
//		FindXMAS - Given a grid and a starting position, checks if the remaining
//		letters of XMAS can be found by following the given direction.
//------------------------------------------------------------------------------
bool FindXMAS(const std::vector<std::vector<char>>& grid,
			  size_t                                rowIndex,
			  size_t                                colIndex,
			  Direction                             direction,
			  std::vector<char>                     remainingLetters);





//==============================================================================
//		WithinBounds - Checks if a given position is within bounds.
//------------------------------------------------------------------------------
bool WithinBounds(size_t rowIndex, size_t colIndex);



}



#endif // SOLUTION
