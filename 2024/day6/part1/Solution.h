//==============================================================================
//	Name:
//		Solution.h
//
//	Description:
//		Advent of Code 2024 Day 6 Part 1
//		https://adventofcode.com/2024/day/6
//------------------------------------------------------------------------------
#ifndef SOLUTION
#define SOLUTION



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Grid.h"

#include <string>
#include <vector>





//==============================================================================
//		main
//------------------------------------------------------------------------------
int main();





namespace Solution
{



//==============================================================================
//		Map
//------------------------------------------------------------------------------
class Map : public Grid<char>
{
public:
	                                        Map(size_t height, size_t width);
	                                        Map(const std::vector<std::vector<char>>& grid);
	                                        Map(std::vector<std::vector<char>>&& grid);
};





//==============================================================================
//		Solve - Returns the solution to the problem for the given input.
//------------------------------------------------------------------------------
int Solve(std::vector<std::string> input, bool debug = false);





//==============================================================================
//		Move - Simulates the given guard's movement in the given map. Returns
//		true if the guard remains on the map and false if they leave it.
//------------------------------------------------------------------------------
bool Move(Map& map, GridPosition& guard, bool debug);



} // Solution



#endif // SOLUTION
