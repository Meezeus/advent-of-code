//////////////////////////////////////////
// Advent of Code 2023 Day 14 Part 2    //
// https://adventofcode.com/2023/day/14 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

enum Direction { NORTH, WEST, SOUTH, EAST };

int solution(const std::string fileName);
void spin(std::vector<std::string>& input);
void slideRock(std::vector<std::string>& input, const int rockRow, const int rockCol, const int direction);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 64)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);
    int desiredSpinNumber = 1000000000;

    std::vector<std::string> gridStringHistory;
    int currentSpinNumber = 0;
    int cycleStartNumber;
    int cycleLength;

    bool cycleFound = false;
    while (!cycleFound) {
        spin(input);
        ++currentSpinNumber;

        std::string gridString;
        for (std::string line : input) {
            gridString += line;
        }

        std::vector<std::string>::iterator iterator = std::find(gridStringHistory.begin(), gridStringHistory.end(), gridString);
        if (iterator != gridStringHistory.end()) {
            cycleFound = true;
            cycleStartNumber = (iterator - gridStringHistory.begin()) + 1;
            cycleLength = currentSpinNumber - cycleStartNumber;
        }

        gridStringHistory.push_back(gridString);
    }

    int desiredCycleNumber = ((desiredSpinNumber - cycleStartNumber) % cycleLength) + cycleStartNumber;
    std::string gridString = gridStringHistory[desiredCycleNumber - 1];

    int rowIndex = 0;
    int colIndex = 0;
    int maxColIndex = input[0].length() - 1;
    for (int charIndex = 0; charIndex < gridString.length(); ++charIndex) {
        input[rowIndex][colIndex] = gridString[charIndex];

        ++colIndex;
        if (colIndex > maxColIndex) {
            colIndex = 0;
            ++rowIndex;
        }
    }

    int total = 0;
    for (int rockRow = 0; rockRow < input.size(); ++rockRow) {
            for (int rockCol = 0; rockCol < input[0].length(); ++rockCol) {
                if (input[rockRow][rockCol] == 'O') {
                    total += input.size() - rockRow;
                }
            }
        }
    return total;
}

void spin(std::vector<std::string>& input) {
    for (int rockRow = 1; rockRow < input.size(); ++rockRow) {
        for (int rockCol = 0; rockCol < input[0].length(); ++rockCol) {
            if (input[rockRow][rockCol] == 'O') {
                slideRock(input, rockRow, rockCol, NORTH);
            }
        }
    }

    for (int rockCol = 1; rockCol < input[0].length(); ++rockCol) {
        for (int rockRow = 0; rockRow < input.size(); ++rockRow) {
            if (input[rockRow][rockCol] == 'O') {
                slideRock(input, rockRow, rockCol, WEST);
            }
        }
    }

    for (int rockRow = input.size() - 2; rockRow >= 0; --rockRow) {
        for (int rockCol = 0; rockCol < input[0].length(); ++rockCol) {
            if (input[rockRow][rockCol] == 'O') {
                slideRock(input, rockRow, rockCol, SOUTH);
            }
        }
    }

    for (int rockCol = input[0].length() - 2; rockCol >= 0; --rockCol) {
        for (int rockRow = 0; rockRow < input.size(); ++rockRow) {
            if (input[rockRow][rockCol] == 'O') {
                slideRock(input, rockRow, rockCol, EAST);
            }
        }
    }
}

void slideRock(std::vector<std::string>& input, const int rockRow, const int rockCol, const int direction) {
    int rowOffset = 0;
    int colOffset = 0;
    switch (direction) {
        case NORTH:
            rowOffset = -1;
            break;
        case WEST:
            colOffset = -1;
            break;
        case SOUTH:
            rowOffset = 1;
            break;
        case EAST:
            colOffset = 1;
            break;
    }

    if (rockRow + rowOffset >= 0 && rockRow + rowOffset < input.size() &&
        rockCol + colOffset >= 0 && rockCol + colOffset < input[0].length() &&
        input[rockRow + rowOffset][rockCol + colOffset] == '.')
    {
        input[rockRow][rockCol] = '.';
        input[rockRow + rowOffset][rockCol + colOffset] = 'O';
        slideRock(input, rockRow + rowOffset, rockCol + colOffset, direction);
    }
}

std::vector<std::string> readLines(const std::string fileName) {
    std::vector<std::string> lines;
    std::ifstream inputFile(fileName);

    if (inputFile.fail()) {
        std::cout << "The file doesn't exist you dummy!" << std::endl;
    }
    else {
        std::string line;
        while (std::getline(inputFile, line)) {
            lines.push_back(line);
        }
        inputFile.close();
    }

    return lines;
}
