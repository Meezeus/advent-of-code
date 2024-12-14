//==============================================================================
//	Name:
//		Solution.h
//
//	Description:
//		Advent of Code 2024 Day 2 Part 2
//		https://adventofcode.com/2024/day/2
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
//		IsSafe - Returns whether the report is considered safe or not.
//------------------------------------------------------------------------------
bool IsSafe(bool debug, const std::vector<int>& report, bool problemDampenerActivated);





//==============================================================================
//		ActivateProblemDampener - Returns whether the report is considered safe
//		or not after activating the Problem Dampener at the given index.
//------------------------------------------------------------------------------
bool ActivateProblemDampener(bool debug, const std::vector<int>& report, bool& problemDampenerActivated, size_t currentIndex);



}



#endif // SOLUTION
