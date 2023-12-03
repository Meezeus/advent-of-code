/////////////////////////////////////////
// Advent of Code 2023 Day 3 Part 2    //
// https://adventofcode.com/2023/day/3 //
/////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>

int solution(std::string fileName);
int calculateGearRatio(const std::vector<std::string>* const inputPtr, int lineNumber, int charNumber);
std::vector<std::string> readLines(std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 467835)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    int sum = 0;
    for (int lineNumber = 0; lineNumber < input.size(); ++lineNumber) {
        for (int charNumber = 0; charNumber < input[lineNumber].length(); ++charNumber) {
            if (input[lineNumber][charNumber] == '*') {
                sum += calculateGearRatio(&input, lineNumber, charNumber);
            }
        }
    }

    return sum;
}

int calculateGearRatio(const std::vector<std::string>* const inputPtr, int lineNumber, int charNumber) {
    std::vector<int> adjacentNumbers = {};
    std::regex pattern("\\d+");
    for (int i = -1; i <= 1; ++i) {
        if (lineNumber + i >= 0 && lineNumber + i < (*inputPtr).size()) {
            std::string line = (*inputPtr)[lineNumber + i];
            std::sregex_iterator iterator(line.begin(), line.end(), pattern);
            std::sregex_iterator end;
            for (; iterator != end; ++iterator) {
                std::smatch match = *iterator;
                int startIndex = match.position();
                int endIndex = startIndex + match.length() - 1;
                if (charNumber >= startIndex - 1 && charNumber <= endIndex + 1) {
                    adjacentNumbers.push_back(std::stoi(match[0]));
                }
            }
        }
    }

    int gearRatio = 0;
    if (adjacentNumbers.size() == 2) {
        gearRatio += adjacentNumbers[0] * adjacentNumbers[1];
    }

    return gearRatio;
}

std::vector<std::string> readLines(std::string fileName) {
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
