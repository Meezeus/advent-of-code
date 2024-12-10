//==============================================================================
//	Name:
//		Solution.cpp
//
//	Description:
//		Advent of Code 2024 Day 2 Part 1
//		https://adventofcode.com/2024/day/2
//------------------------------------------------------------------------------



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Solution.h"

#include "Helper.h"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <optional>





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
		std::cout << Solution::Solve(exampleInput) << " (expected 2)" << std::endl;
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
int Solve(std::vector<std::string> input)
{
	// Convert the input into a more suitable format.
	std::vector<std::vector<int>> reports;
	for (std::string line : input)
	{
		std::vector<int> report;
		std::stringstream stringStream(line);
		while (stringStream.good())
		{
			int number;
			stringStream >> number;
			report.push_back(number);
		}
		reports.push_back(report);
	}

	// Assess each report and count the number of safe reports.
	int safeReportCount = 0;
	for (std::vector<int> report : reports)
	{
		if (IsSafe(report))
		{
			safeReportCount += 1;
		}
	}
	return safeReportCount;
}





//==============================================================================
//		IsSafe - Returns whether the report is considered safe or not.
//------------------------------------------------------------------------------
bool IsSafe(std::vector<int> report)
{
	// Reports with less than 2 levels are considered safe.
	if (report.size() < 2)
	{
		return true;
	}

	// Get the first two levels.
	int previousLevel = report[0];
	int currentLevel  = report[1];

	// The difference in levels must be at least one and at most three to be
	// considered safe.
	int difference = std::abs(currentLevel - previousLevel);
	if (difference < 1 || difference > 3)
	{
		return false;
	}

	// Determine whether the levels are increasing or decreasing.
	bool increasing = currentLevel > previousLevel;

	// Iterate over the remaining levels.
	for (size_t index = 2; index < report.size(); ++index)
	{
		// Update the levels.
		previousLevel = currentLevel;
		currentLevel  = report[index];

		// The difference in levels must be at least one and at most three to be
		// considered safe.
		difference = std::abs(currentLevel - previousLevel);
		if (difference == 0 || difference > 3)
		{
			return false;
		}

		// The levels must be all increasing or all decreasing to be considered
		// safe.
		if (increasing != (currentLevel > previousLevel))
		{
			return false;
		}
	}

	// If all checks have been successful, the report is safe.
	return true;
}



}
