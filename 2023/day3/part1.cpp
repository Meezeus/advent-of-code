/////////////////////////////////////////
// Advent of Code 2023 Day 3 Part 1    //
// https://adventofcode.com/2023/day/3 //
/////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>

int solution(std::string fileName);
bool isPartNumber(std::string previousLine, std::string currentLine, std::string nextLine, int startIndex, int endIndex);
std::vector<std::string> readLines(std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 4361)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::string previousLine;
    std::string currentLine;
    std::string nextLine;
    std::vector<int> partNumbers = {};
    for (int lineNumber = 0; lineNumber < input.size(); ++lineNumber) {
        previousLine = (lineNumber - 1 < 0) ? "" : input[lineNumber - 1];
        currentLine = input[lineNumber];
        nextLine = (lineNumber + 1 >= input.size()) ? "" : input[lineNumber + 1];

        for (int startIndex = 0; startIndex < currentLine.length(); ++startIndex) {
            if (std::isdigit(currentLine[startIndex])) {
                int endIndex = startIndex;
                while (std::isdigit(currentLine[endIndex+1])) {
                    endIndex += 1;
                }

                if (isPartNumber(previousLine, currentLine, nextLine, startIndex, endIndex)) {
                    partNumbers.push_back(std::stoi(currentLine.substr(startIndex, endIndex-startIndex+1)));
                }

                startIndex = endIndex;
            }
        }
    }

    int sum = 0;
    for (int number : partNumbers) {
        sum += number;
    }

    return sum;
}

bool isPartNumber(std::string previousLine, std::string currentLine, std::string nextLine, int startIndex, int endIndex) {
    bool foundSymbol = false;
    for (int i = startIndex - 1; i <= endIndex + 1; ++i) {
        if (i < 0 || i >= currentLine.length()) {
            continue;
        }

        if (!previousLine.empty() && previousLine[i] != '.' && !(std::isdigit(previousLine[i]))) {
            foundSymbol = true;
        }
        if (currentLine[i] != '.' && !(std::isdigit(currentLine[i]))) {
            foundSymbol = true;
        }
        if (!nextLine.empty() && nextLine[i] != '.' && !(std::isdigit(nextLine[i]))) {
            foundSymbol = true;
        }
    }
    return foundSymbol;
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
