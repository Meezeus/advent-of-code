//==============================================================================
//	Name:
//		Solution.cpp
//
//	Description:
//		Advent of Code 2024 Day 3 Part 2
//		https://adventofcode.com/2024/day/3
//------------------------------------------------------------------------------



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Solution.h"

#include "ANSIEscapeCodes.h"
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

	std::cout << ANSIEscapeCodes::BOLD << "Example Solution: " << ANSIEscapeCodes::RESET;
	std::vector<std::string> exampleInput = Helper::ReadLines(root / "example.txt");
	if (!exampleInput.empty())
	{
		std::cout << Solution::Solve(exampleInput) << ANSIEscapeCodes::ITALIC << " (expected 48)" << ANSIEscapeCodes::RESET << std::endl;
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

	std::string mulPattern("mul\\((\\d{1,3}),(\\d{1,3})\\)");
	std::string doPattern("do\\(\\)");
	std::string dontPattern("don't\\(\\)");
	std::regex combinedPattern("(" + mulPattern + ")|(" + doPattern + ")|(" + dontPattern + ")");

	int total = 0;
	bool mulEnabled = true;

	for (std::string line : input)
	{
		for (std::sregex_iterator it(line.begin(), line.end(), combinedPattern); it != std::sregex_iterator(); ++it)
		{
			// Debug
			if ((*it)[1].matched) { std::cout << (mulEnabled ? ANSIEscapeCodes::GREEN : ANSIEscapeCodes::RED); }
			Helper::PrintIf(debug && (*it)[0].matched, "Matched {:12} at {}", it->str(0), it->position(0));
			std::cout << ANSIEscapeCodes::RESET;

			// mul(X,Y)
			if ((*it)[1].matched && mulEnabled)
			{
				total += std::stoi(it->str(2)) * std::stoi(it->str(3));
			}
			// do()
			else if ((*it)[4].matched)
			{
				mulEnabled = true;
			}
			// don't()
			else if ((*it)[5].matched)
			{
				mulEnabled = false;
			};
		}
	}

	Helper::PrintIf(debug, "------------------------------------------------------------------------------");
	return total;
}



}
