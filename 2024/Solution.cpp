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

#include "ANSIEscapeCodes.h"
#include "Helper.h"

#include <filesystem>





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
		std::cout << Solution::Solve(exampleInput) << ANSIEscapeCodes::ITALIC << " (expected X)" << ANSIEscapeCodes::RESET << std::endl;
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

	Helper::PrintIf(debug,   "------------------------------------------------------------------------------");
	return 0;
}



} // Solution
