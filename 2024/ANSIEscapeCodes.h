//==============================================================================
//	Name:
//		ANSIEscapeCodes.h
//
//	Description:
//		Advent of Code 2024 ANSI escape does for text formatting
//		(https://adventofcode.com/2024)
//------------------------------------------------------------------------------
#ifndef ANSI_ESCAPE_CODES
#define ANSI_ESCAPE_CODES



//==============================================================================
//		Includes
//------------------------------------------------------------------------------
#include <iostream>
#include <string>





namespace ANSIEscapeCodes
{



//==============================================================================
//		Constants
//------------------------------------------------------------------------------
const std::string RESET               = "\033[0m";

// Text formatting
const std::string BOLD                = "\033[1m";
const std::string DIM                 = "\033[2m";
const std::string ITALIC              = "\033[3m";
const std::string UNDERLINE           = "\033[4m";
const std::string REVERSE             = "\033[7m";
const std::string STRIKETHROUGH       = "\033[9m";

const std::string RESET_BOLD          = "\033[22m";
const std::string RESET_DIM           = "\033[22m";
const std::string RESET_ITALIC        = "\033[23m";
const std::string RESET_UNDERLINE     = "\033[24m";
const std::string RESET_REVERSE       = "\033[27m";
const std::string RESET_STRIKETHROUGH = "\033[29m";

// Foreground colors
const std::string BLACK               = "\033[30m";
const std::string RED                 = "\033[31m";
const std::string GREEN               = "\033[32m";
const std::string YELLOW              = "\033[33m";
const std::string BLUE                = "\033[34m";
const std::string MAGENTA             = "\033[35m";
const std::string CYAN                = "\033[36m";
const std::string WHITE               = "\033[37m";

const std::string BRIGHT_BLACK        = "\033[90m";
const std::string BRIGHT_RED          = "\033[91m";
const std::string BRIGHT_GREEN        = "\033[92m";
const std::string BRIGHT_YELLOW       = "\033[93m";
const std::string BRIGHT_BLUE         = "\033[94m";
const std::string BRIGHT_MAGENTA      = "\033[95m";
const std::string BRIGHT_CYAN         = "\033[96m";
const std::string BRIGHT_WHITE        = "\033[97m";

const std::string RESET_FOREGROUND    = "\033[39m";

// Background colors
const std::string BG_BLACK            = "\033[40m";
const std::string BG_RED              = "\033[41m";
const std::string BG_GREEN            = "\033[42m";
const std::string BG_YELLOW           = "\033[43m";
const std::string BG_BLUE             = "\033[44m";
const std::string BG_MAGENTA          = "\033[45m";
const std::string BG_CYAN             = "\033[46m";
const std::string BG_WHITE            = "\033[47m";

const std::string BG_BRIGHT_BLACK     = "\033[100m";
const std::string BG_BRIGHT_RED       = "\033[101m";
const std::string BG_BRIGHT_GREEN     = "\033[102m";
const std::string BG_BRIGHT_YELLOW    = "\033[103m";
const std::string BG_BRIGHT_BLUE      = "\033[104m";
const std::string BG_BRIGHT_MAGENTA   = "\033[105m";
const std::string BG_BRIGHT_CYAN      = "\033[106m";
const std::string BG_BRIGHT_WHITE     = "\033[107m";

const std::string RESET_BACKGROUND    = "\033[49m";





//==============================================================================
//		ANSIEscapeCodesExamples - Prints an example of all the ANSI escape
//		codes.
//------------------------------------------------------------------------------
void ANSIEscapeCodesExamples();



}



#endif // ANSI_ESCAPE_CODES
