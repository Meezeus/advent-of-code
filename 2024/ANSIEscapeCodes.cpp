//==============================================================================
//	Name:
//		ANSIEscapeCodes.cpp
//
//	Description:
//		Advent of Code 2024 ANSI escape does for text formatting
//		(https://adventofcode.com/2024)
//------------------------------------------------------------------------------



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include "ANSIEscapeCodes.h"





//==============================================================================
//		main
//------------------------------------------------------------------------------
int main()
{
	ANSIEscapeCodes::ANSIEscapeCodesExamples();
	return 0;
}





namespace ANSIEscapeCodes
{



//==============================================================================
//		ANSIEscapeCodesExamples - Prints an example of all the ANSI escape
//		codes.
//------------------------------------------------------------------------------
void ANSIEscapeCodesExamples()
{
	// Print examples of formatting
	std::cout << BOLD              << "Bold Text"                 << RESET << std::endl;
	std::cout << DIM               << "Dim Text"                  << RESET << std::endl;
	std::cout << ITALIC            << "Italic Text"               << RESET << std::endl;
	std::cout << UNDERLINE         << "Underlined Text"           << RESET << std::endl;
	std::cout << REVERSE           << "Reversed Colors"           << RESET << std::endl;
	std::cout << STRIKETHROUGH     << "Strikethrough Text"        << RESET << std::endl;
	std::cout << std::endl;

	// Print examples of foreground colors
	std::cout << RED               << "Red Text"                  << RESET << std::endl;
	std::cout << GREEN             << "Green Text"                << RESET << std::endl;
	std::cout << YELLOW            << "Yellow Text"               << RESET << std::endl;
	std::cout << BLUE              << "Blue Text"                 << RESET << std::endl;
	std::cout << MAGENTA           << "Magenta Text"              << RESET << std::endl;
	std::cout << CYAN              << "Cyan Text"                 << RESET << std::endl;
	std::cout << WHITE             << "White Text"                << RESET << std::endl;
	std::cout << std::endl;

	// Print examples of bright foreground colors
	std::cout << BRIGHT_BLACK      << "Bright Black Text"         << RESET << std::endl;
	std::cout << BRIGHT_RED        << "Bright Red Text"           << RESET << std::endl;
	std::cout << BRIGHT_GREEN      << "Bright Green Text"         << RESET << std::endl;
	std::cout << BRIGHT_YELLOW     << "Bright Yellow Text"        << RESET << std::endl;
	std::cout << BRIGHT_BLUE       << "Bright Blue Text"          << RESET << std::endl;
	std::cout << BRIGHT_MAGENTA    << "Bright Magenta Text"       << RESET << std::endl;
	std::cout << BRIGHT_CYAN       << "Bright Cyan Text"          << RESET << std::endl;
	std::cout << BRIGHT_WHITE      << "Bright White Text"         << RESET << std::endl;
	std::cout << std::endl;

	// Print examples of background colors
	std::cout << BG_BLACK          << "Black Background"          << RESET << std::endl;
	std::cout << BG_RED            << "Red Background"            << RESET << std::endl;
	std::cout << BG_GREEN          << "Green Background"          << RESET << std::endl;
	std::cout << BG_YELLOW         << "Yellow Background"         << RESET << std::endl;
	std::cout << BG_BLUE           << "Blue Background"           << RESET << std::endl;
	std::cout << BG_MAGENTA        << "Magenta Background"        << RESET << std::endl;
	std::cout << BG_CYAN           << "Cyan Background"           << RESET << std::endl;
	std::cout << BG_WHITE          << "White Background"          << RESET << std::endl;
	std::cout << std::endl;

	// Print examples of bright background colors
	std::cout << BG_BRIGHT_BLACK   << "Bright Black Background"   << RESET << std::endl;
	std::cout << BG_BRIGHT_RED     << "Bright Red Background"     << RESET << std::endl;
	std::cout << BG_BRIGHT_GREEN   << "Bright Green Background"   << RESET << std::endl;
	std::cout << BG_BRIGHT_YELLOW  << "Bright Yellow Background"  << RESET << std::endl;
	std::cout << BG_BRIGHT_BLUE    << "Bright Blue Background"    << RESET << std::endl;
	std::cout << BG_BRIGHT_MAGENTA << "Bright Magenta Background" << RESET << std::endl;
	std::cout << BG_BRIGHT_CYAN    << "Bright Cyan Background"    << RESET << std::endl;
	std::cout << BG_BRIGHT_WHITE   << "Bright White Background"   << RESET << std::endl;
	std::cout << std::endl;
}



}
