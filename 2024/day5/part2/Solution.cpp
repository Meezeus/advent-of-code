//==============================================================================
//	Name:
//		Solution.cpp
//
//	Description:
//		Advent of Code 2024 Day 5 Part 2
//		https://adventofcode.com/2024/day/5
//------------------------------------------------------------------------------



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "Solution.h"

#include "ANSIEscapeCodes.h"
#include "Helper.h"

#include <algorithm>
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
		std::cout << Solution::Solve(exampleInput, true) << ANSIEscapeCodes::ITALIC << " (expected 123)" << ANSIEscapeCodes::RESET << std::endl;
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

	std::vector<PageOrderingRule> rules;
	std::vector<Update> incorrectUpdates;

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
			if (!CheckUpdate(update, rules))
			{
				incorrectUpdates.push_back(update);
			}
		}
		// There should be no other sections.
		else
		{
			REQUIRE(false, "Invalid section: {}", section);
		}
	}

	int total = 0;

	for (const Update& update : incorrectUpdates)
	{
		Update correctedUpdate = CorrectUpdate(update, rules, debug);
		total += correctedUpdate[correctedUpdate.size() / 2];
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
		if (!pageNumberSeen[rule.lhs] || !pageNumberSeen[rule.rhs])
		{
			continue;
		}

		// If the page ordering rule is not satisfied, the update is incorrect.
		if (!(pageNumberToIndex[rule.lhs] < pageNumberToIndex[rule.rhs]))
		{
			correct = false;
			break;
		}
	}

	return correct;
}





//==============================================================================
//		CorrectUpdate - Returns a corrected copy of the given incorrect update,
//		such that it satisfies the given page ordering rules.
//------------------------------------------------------------------------------
Update CorrectUpdate(const Update& incorrectUpdate, const std::vector<PageOrderingRule>& rules, bool debug)
{
	Helper::PrintIf(debug, "\nIncorrect update: {}{}{}", ANSIEscapeCodes::RED, incorrectUpdate, ANSIEscapeCodes::RESET);

	// Record which pages are in the update.
	std::map<int, bool>   pageNumberSeen;
	for (int pageNumber : incorrectUpdate)
	{
		pageNumberSeen[pageNumber] = true;
	}

	// Find the first page number to insert. This is the page number which
	// doesn't show up on the RHS of any relevant page ordering rules.
	Update::const_iterator firstPageIt = incorrectUpdate.begin();
	for (; firstPageIt != incorrectUpdate.end(); ++firstPageIt)
	{
		int validFirstNumber = true;
		for (const PageOrderingRule& rule : rules)
		{
			// If either of the page numbers from the rule have not been seen in the
			// update, the rule does not apply.
			if (!pageNumberSeen[rule.lhs] || !pageNumberSeen[rule.rhs])
			{
				continue;
			}

			// The page number cannot show up on the RHS.
			if (rule.rhs == *firstPageIt)
			{
				validFirstNumber = false;
			}
		}

		// If the page number is a valid first page number, break.
		if (validFirstNumber)
		{
			break;
		}
	}
	REQUIRE(firstPageIt != incorrectUpdate.end(), "Could not find a valid first page!");

	// Create the corrected update and add the first page.
	Update correctedUpdate;
	correctedUpdate.push_back(*firstPageIt);

	// Add the remaining pages to the corrected update, making sure to satisfy
	// all relevant rules.
	for (Update::const_iterator pageIt = incorrectUpdate.begin(); pageIt != incorrectUpdate.end(); ++pageIt)
	{
		// Skip the page if it's the first page.
		if (pageIt == firstPageIt)
		{
			continue;
		}

		Helper::PrintIf(debug, "\n\tCorrected update: {}{}{}, current page: {}{}{}",
			ANSIEscapeCodes::GREEN, correctedUpdate, ANSIEscapeCodes::RESET,
			ANSIEscapeCodes::GREEN, *pageIt,         ANSIEscapeCodes::RESET);

		// The insertion iterator starts off as small as possible.
		Update::iterator insertionIt = correctedUpdate.begin();

		// Ensure that the insertion iterator will satisfy all relevant rule.
		for (const PageOrderingRule& rule : rules)
		{
			// If either of the page numbers from the rule have not been seen in the
			// update, the rule does not apply.
			if (!pageNumberSeen[rule.lhs] || !pageNumberSeen[rule.rhs])
			{
				continue;
			}

			// If the current page is the RHS of the rule...
			if (rule.rhs == *pageIt)
			{
				// ...and the LHS of the rule is in the corrected update...
				if (Update::iterator lhsIt = std::find(correctedUpdate.begin(), correctedUpdate.end(), rule.lhs); lhsIt != correctedUpdate.end())
				{
					// ...update the insertion iterator to ensure that it will
					// satisfy the rule.
					insertionIt = std::max(insertionIt, lhsIt);
				}
				Helper::PrintIf(debug, "\tRelevant rule: {}|{} - insertion iterator is now {}", rule.lhs, rule.rhs, (insertionIt - correctedUpdate.begin()));
			}
		}

		// Insert the page into the corrected update.
		Helper::PrintIf(debug, "\tInserting the current page after element {} with index {}", *insertionIt, (insertionIt - correctedUpdate.begin()));
		correctedUpdate.insert(insertionIt + 1, *pageIt);
	}

	Helper::PrintIf(debug, "\nCorrected update: {}{}{}", ANSIEscapeCodes::GREEN, correctedUpdate, ANSIEscapeCodes::RESET);
	return correctedUpdate;
}



}
