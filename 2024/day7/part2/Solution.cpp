//==============================================================================
//	Name:
//		Solution.cpp
//
//	Description:
//		Advent of Code 2024 Day 7 Part 2
//		https://adventofcode.com/2024/day/7
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
		std::cout << Solution::Solve(exampleInput) << ANSIEscapeCodes::ITALIC << " (expected 11387)" << ANSIEscapeCodes::RESET << std::endl;
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
long Solve(std::vector<std::string> input, bool debug/* = false*/)
{
	Helper::PrintIf(debug, "\n==============================================================================");

	// Convert the input into equations.
	std::vector<Equation> equations;
	for (std::string line : input)
	{
		std::istringstream iss(line);
		std::string token;

		// Get the test value.
		std::getline(iss, token, ':');
		long testValue = std::stol(token);

		// Consume the space after the colon.
		iss.get();

		// Get the operands.
		std::vector<long> operands;
		while (std::getline(iss, token, ' '))
		{
			operands.push_back(std::stol(token));
		}

		// Save the equation.
		equations.push_back({testValue, operands});
	}

	// For each equation, check if it can be made true. If so, add its test
	// value to the total.
	long total = 0;
	for (const Equation& equation : equations)
	{
		if (CanBeMadeTrue(equation))
		{
			total += equation.testValue;
		}
	}

	Helper::PrintIf(debug,   "------------------------------------------------------------------------------");
	return total;
}





//==============================================================================
//		CanBeMadeTrue - Checks whether the given equation can be made true.
//------------------------------------------------------------------------------
bool CanBeMadeTrue(const Equation& equation)
{
	// If there is just one operand left, check if it equals the test value.
	if (equation.operands.size() == 1)
	{
		return equation.testValue == equation.operands[0];
	}

	// Otherwise, there is more work to be done. Get the relevant values from
	// the equation.
	long testValue = equation.testValue;
	long operand1  = equation.operands[0];
	long operand2  = equation.operands[1];

	// Use the plus operator to combine the first two operands. Create a new
	// operands list for this operator.
	long newOperandPlus = operand1 + operand2;
	std::vector<long> newOperandsPlus = {newOperandPlus};
	newOperandsPlus.insert(newOperandsPlus.end(), equation.operands.begin() + 2, equation.operands.end());

	// Use the times operator to combine the first two operands. Create a new
	// operands list for this operator.
	long newOperandMultiply = operand1 * operand2;
	std::vector<long> newOperandsMultiply = {newOperandMultiply};
	newOperandsMultiply.insert(newOperandsMultiply.end(), equation.operands.begin() + 2, equation.operands.end());

	// Use the concatenation operator to combine the first two operands. Create
	// a new operands list for this operator.
	long newOperandConcatenation = std::stol(std::to_string(operand1) + std::to_string(operand2));
	std::vector<long> newOperandsConcatenation = {newOperandConcatenation};
	newOperandsConcatenation.insert(newOperandsConcatenation.end(), equation.operands.begin() + 2, equation.operands.end());

	// Check if the equation can be made true using any of the operators.
	return CanBeMadeTrue({testValue, newOperandsPlus})     ||
	       CanBeMadeTrue({testValue, newOperandsMultiply}) ||
	       CanBeMadeTrue({testValue, newOperandsConcatenation});
}



} // Solution
