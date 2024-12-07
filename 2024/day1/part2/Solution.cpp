//==============================================================================
//	Name:
//		Solution.cpp
//
//	Description:
//		Advent of Code 2024 Day 1 Part 2
//		https://adventofcode.com/2024/day/1
//------------------------------------------------------------------------------



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Solution.h"

#include "Helper.h"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <map>





//==============================================================================
//		main
//------------------------------------------------------------------------------
int main()
{
	std::filesystem::path root = std::filesystem::canonical("/proc/self/exe").parent_path();
	std::vector<std::string> exampleInput = Helper::ReadLines(root / "example.txt");
	std::vector<std::string> input        = Helper::ReadLines(root / "input.txt");

	std::cout << std::endl;
	std::cout << "Example Solution: " << Solution::Solve(exampleInput) << " (expected 31)" << std::endl;
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
	std::vector<int> list1;
	std::vector<int> list2;

	for (std::string line : input)
	{
		std::stringstream stringStream(line);
		int num1, num2;
		stringStream >> num1 >> num2;
		list1.push_back(num1);
		list2.push_back(num2);
	}

	std::map<int, int> list2Counts;
	for (int num2 : list2)
	{
		list2Counts[num2] += 1;
	}

	int similarityScore = 0;
	for (int num1 : list1)
	{
		similarityScore += num1 * list2Counts[num1];
	}

	return similarityScore;
}



}
