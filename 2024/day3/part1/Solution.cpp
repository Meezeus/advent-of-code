//==============================================================================
//	Name:
//		Solution.cpp
//
//	Description:
//		Advent of Code 2024 Day 3 Part 1
//		https://adventofcode.com/2024/day/3
//------------------------------------------------------------------------------



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Solution.h"

#include "Helper.h"

#include <filesystem>
#include <iostream>
#include <regex>





//==============================================================================
//		main
//------------------------------------------------------------------------------
int main()
{
	std::cout << std::endl;

	std::filesystem::path root = std::filesystem::canonical("/proc/self/exe").parent_path();

	std::cout << "Example Solution: ";
	std::vector<std::string> exampleInput = Helper::ReadLines(root / "example.txt");
	if (!exampleInput.empty())
	{
		std::cout << Solution::Solve(exampleInput) << " (expected 161)" << std::endl;
	}

	std::cout << std::endl;

	std::cout << "Input Solution: ";
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
	Helper::PrintIf(debug, "\n//==============================================================================");

	int total = 0;
	std::regex pattern("mul\\((\\d{1,3}),(\\d{1,3})\\)");
	for (std::string line : input)
	{
		for (std::sregex_iterator it(line.begin(), line.end(), pattern); it != std::sregex_iterator(); ++it)
		{
			Helper::PrintIf(debug, "{:12} : {:3} * {:3}", it->str(0), it->str(1), it->str(2));
			total += std::stoi(it->str(1)) * std::stoi(it->str(2));
		}
	}

	Helper::PrintIf(debug, "//------------------------------------------------------------------------------");
	return total;
}



}
