//==============================================================================
//	Name:
//		Solution.cpp
//
//	Description:
//		Advent of Code 2024 Day X Part X
//		https://adventofcode.com/2024/day/X
//------------------------------------------------------------------------------



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Solution.h"

#include "Helper.h"

#include <filesystem>
#include <iostream>





//==============================================================================
//		main
//------------------------------------------------------------------------------
int main()
{
	std::filesystem::path root = std::filesystem::canonical("/proc/self/exe").parent_path();
	std::vector<std::string> exampleInput = Helper::ReadLines(root / "example.txt");
	std::vector<std::string> input        = Helper::ReadLines(root / "input.txt");

	std::cout << std::endl;
	std::cout << "Example Solution: " << Solution::Solve(exampleInput) << " (expected X)" << std::endl;
	std::cout << "Input Solution: "   << Solution::Solve(input)                           << std::endl;
	std::cout << std::endl;
}





namespace Solution
{



//==============================================================================
//		Solve - Returns the solution to the problem for the given input.
//------------------------------------------------------------------------------
int Solve(std::vector<std::string> input)
{
}



}
