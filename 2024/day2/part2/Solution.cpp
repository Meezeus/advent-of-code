//==============================================================================
//	Name:
//		Solution.cpp
//
//	Description:
//		Advent of Code 2024 Day 2 Part 2
//		https://adventofcode.com/2024/day/2
//------------------------------------------------------------------------------



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Solution.h"

#include "Helper.h"

#include <filesystem>





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
		std::cout << Solution::Solve(exampleInput) << " (expected 4)" << std::endl;
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
		Helper::PrintIf(debug, "Report: {}", report);
		bool safe = IsSafe(debug, report, false);
		Helper::PrintIf(debug, "Report is {}!\n", safe ? "safe" : "unsafe");
		if (safe)
		{
			safeReportCount += 1;
		}
	}

	Helper::PrintIf(debug,   "//------------------------------------------------------------------------------");
	return safeReportCount;
}





//==============================================================================
//		IsSafe - Returns whether the report is considered safe or not.
//------------------------------------------------------------------------------
bool IsSafe(bool debug, const std::vector<int>& report, bool problemDampenerActivated)
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
		// Attempt to activate the Problem Dampener.
		return ActivateProblemDampener(debug, report, problemDampenerActivated, 1);
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
			// Attempt to activate the Problem Dampener.
			return ActivateProblemDampener(debug, report, problemDampenerActivated, index);
		}

		// The levels must be all increasing or all decreasing to be considered
		// safe.
		if (increasing != (currentLevel > previousLevel))
		{
			// Attempt to activate the Problem Dampener.
			return ActivateProblemDampener(debug, report, problemDampenerActivated, index);
		}
	}

	// If all checks have been successful, the report is safe.
	return true;
}





//==============================================================================
//		ActivateProblemDampener - Returns whether the report is considered safe
//		or not after activating the Problem Dampener at the given index.
//------------------------------------------------------------------------------
bool ActivateProblemDampener(bool debug, const std::vector<int>& report, bool& problemDampenerActivated, size_t currentIndex)
{
	// If the Problem Dampener has already been activated, it cannot be used
	// again.
	if (problemDampenerActivated)
	{
		return false;
	}

	// Activate the Problem Dampener.
	Helper::PrintIf(debug, "Activating Problem Dampener at index {}!", currentIndex);
	problemDampenerActivated = true;

	// The report is currently unsafe.
	bool safe = false;

	// A report can be made safe by removing the current level or the previous level.
	std::vector<size_t> indicesToRemove = {currentIndex, currentIndex - 1};

	// Edge case: if the current index is 2, the report can be made safe by
	// removing the first level.
	if (currentIndex == 2)
	{
		indicesToRemove.push_back(0);
	}

	size_t attemptNumber = 0;

	// Check if removing a level will make the report safe.
	while (!safe && attemptNumber < indicesToRemove.size())
	{
		size_t indexToRemove = indicesToRemove[attemptNumber];
		std::vector<int> modifiedReport;

		// If the number of levels seen so far is less than 4, include the
		// report from the beginning.
		if (currentIndex + 1 < 4)
		{
			modifiedReport = report;
			modifiedReport.erase(modifiedReport.begin() + indexToRemove);
		}
		// Otherwise trim the report. The report needs to include two levels
		// before the previous level and the current level (in order to
		// determine whether the levels are increasing or decreasing), but any
		// levels before that are unecessary.
		else
		{
			modifiedReport.assign(report.begin() + currentIndex - 3, report.end());
			int numberOfLevelsTrimmed = report.size() - modifiedReport.size();
			modifiedReport.erase(modifiedReport.begin() + indexToRemove - numberOfLevelsTrimmed);
		}

		// Check the modified report.
		safe = IsSafe(debug, modifiedReport, true);
		Helper::PrintIf(debug && safe, "Problem Dampener succeeded! Removed number {} at index {}", report[indexToRemove], indexToRemove);

		++attemptNumber;
	}

	return safe;
}



}
