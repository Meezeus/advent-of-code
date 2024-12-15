//==============================================================================
//	Name:
//		Solution.h
//
//	Description:
//		Advent of Code 2024 Day 4 Part 2
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
//		Solve - Returns the solution to the problem for the given input.
//------------------------------------------------------------------------------
int Solve(std::vector<std::string> input, bool debug = false);





//==============================================================================
//		CheckXMAS - Given a grid and the position of a letter A, checks if that A
//		is the centre of an X-MAS.
//------------------------------------------------------------------------------
bool CheckXMAS(const std::vector<std::vector<char>>& grid, size_t rowIndex, size_t colIndex);





//==============================================================================
//		WithinBounds - Checks if a given position is within bounds.
//------------------------------------------------------------------------------
bool WithinBounds(size_t rowIndex, size_t colIndex);



}



#endif // SOLUTION
