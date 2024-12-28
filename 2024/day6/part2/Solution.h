//==============================================================================
//	Name:
//		Solution.h
//
//	Description:
//		Advent of Code 2024 Day 6 Part 2
//		https://adventofcode.com/2024/day/6
//------------------------------------------------------------------------------
#ifndef SOLUTION
#define SOLUTION



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Grid.h"

#include <string>
#include <map>
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
typedef std::map<std::pair<GridPosition, char>, bool> MovementHistory;
typedef std::map<GridPosition, bool>                  ValidLocations;





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
//		FindValidObstaclePositions - Finds and records all valid obstacle
//		positions that will cause the guard to enter a loop.
//------------------------------------------------------------------------------
void FindValidObstaclePositions(Map&             map,
                                GridPosition&    guard,
                                MovementHistory& history,
                                ValidLocations&  validObstacles,
                                bool             debug);





//==============================================================================
//		Move - Simulates the given guard's movement in the given map. Records
//		the movement history. Returns true if the guard remains on the map and
//		false if they leave it.
//------------------------------------------------------------------------------
bool Move(Map& map, GridPosition& guard, MovementHistory& history, bool debug);





//==============================================================================
//		GetNextPosition - Returns the position in front of the guard.
//------------------------------------------------------------------------------
GridPosition GetNextPosition(const Map& map, const GridPosition& guard);





//==============================================================================
//		GetNextDirection - Returns the direction the guard will face after
//		rotating 90 degrees clockwise from the current direction.
//------------------------------------------------------------------------------
char GetNextDirection(char currentDirection);



} // Solution



#endif // SOLUTION
