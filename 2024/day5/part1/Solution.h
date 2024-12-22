//==============================================================================
//	Name:
//		Solution.h
//
//	Description:
//		Advent of Code 2024 Day 5 Part 1
//		https://adventofcode.com/2024/day/5
//------------------------------------------------------------------------------
#ifndef SOLUTION
#define SOLUTION



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include <string>
#include <vector>





//==============================================================================
//		main
//------------------------------------------------------------------------------
int main();





namespace Solution
{



//==============================================================================
//		Types
//------------------------------------------------------------------------------
typedef std::vector<int> Update;
struct PageOrderingRule
{
	int x;
	int y;
};





//==============================================================================
//		Solve - Returns the solution to the problem for the given input.
//------------------------------------------------------------------------------
int Solve(std::vector<std::string> input, bool debug = false);





//==============================================================================
//		ExtractPageOrderingRule - Extracts a page ordering rule from the given
//		string and returns it.
//------------------------------------------------------------------------------
PageOrderingRule ExtractPageOrderingRule(std::string line);





//==============================================================================
//		ExtractUpdate - Extracts an update from the given string and returns it.
//------------------------------------------------------------------------------
Update ExtractUpdate(std::string line);





//==============================================================================
//		CheckUpdate - Checks whether the given update satisfies the given page
//		ordering rules.
//------------------------------------------------------------------------------
bool CheckUpdate(const Update& update, const std::vector<PageOrderingRule>& rules);



}



#endif // SOLUTION
