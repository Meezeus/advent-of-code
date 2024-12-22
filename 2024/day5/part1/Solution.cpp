//==============================================================================
//	Name:
//		Solution.cpp
//
//	Description:
//		Advent of Code 2024 Day 5 Part 1
//		https://adventofcode.com/2024/day/5
//------------------------------------------------------------------------------



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Solution.h"

#include "ANSIEscapeCodes.h"
#include "Helper.h"

#include <filesystem>
#include <map>





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
		std::cout << Solution::Solve(exampleInput) << ANSIEscapeCodes::ITALIC << " (expected 143)" << ANSIEscapeCodes::RESET << std::endl;
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

	int total = 0;
	std::vector<PageOrderingRule> rules;
	std::vector<Update> correctUpdates;

	int section = 0;
	for (std::string line : input)
	{
		// An empty line means we are entering a new section.
		if (line.empty())
		{
			++section;
		}
		// Section 0 contains the page ordering rules.
		else if (section == 0)
		{
			rules.push_back(ExtractPageOrderingRule(line));
		}
		// Section 1 contains the updates.
		else if (section == 1)
		{
			Update update = ExtractUpdate(line);
			if (CheckUpdate(update, rules))
			{
				correctUpdates.push_back(update);
			}
		}
		// There should be no other sections.
		else
		{
			REQUIRE(false, "Invalid section: {}", section);
		}
	}

	for (const Update& update : correctUpdates)
	{
		total += update[update.size() / 2];
	}

	Helper::PrintIf(debug,   "------------------------------------------------------------------------------");
	return total;
}





//==============================================================================
//		ExtractPageOrderingRule - Extracts a page ordering rule from the given
//		string and returns it.
//------------------------------------------------------------------------------
PageOrderingRule ExtractPageOrderingRule(std::string line)
{
	std::istringstream iss(line);
	std::string token;

	std::getline(iss, token, '|');
	int num1 = std::stoi(token);

	std::getline(iss, token);
	int num2 = std::stoi(token);

	return {num1, num2};
}





//==============================================================================
//		ExtractUpdate - Extracts an update from the given string and returns it.
//------------------------------------------------------------------------------
Update ExtractUpdate(std::string line)
{
	Update update;

	std::istringstream iss(line);
	std::string token;
	while (std::getline(iss, token, ','))
	{
		int num = std::stoi(token);
		update.push_back(num);
	}

	return update;
}





//==============================================================================
//		CheckUpdate - Checks whether the given update satisfies the given page
//		ordering rules.
//------------------------------------------------------------------------------
bool CheckUpdate(const Update& update, const std::vector<PageOrderingRule>& rules)
{
	// Record which pages are in the update and their indices.
	std::map<int, bool>   pageNumberSeen;
	std::map<int, size_t> pageNumberToIndex;
	for (size_t index = 0; index < update.size(); ++index)
	{
		int pageNumber = update[index];
		pageNumberSeen   [pageNumber] = true;
		pageNumberToIndex[pageNumber] = index;
	}

	// Check whether each page ordering rule is satisfied.
	bool correct = true;
	for (const PageOrderingRule& rule : rules)
	{
		// If either of the page numbers from the rule have not been seen in the
		// update, the rule does not apply.
		if (!pageNumberSeen[rule.x] || !pageNumberSeen[rule.y])
		{
			continue;
		}

		// If the page ordering rule is not satisfied, the update is incorrect.
		if (!(pageNumberToIndex[rule.x] < pageNumberToIndex[rule.y]))
		{
			correct = false;
			break;
		}
	}

	return correct;
}



}
