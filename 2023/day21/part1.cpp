//////////////////////////////////////////
// Advent of Code 2023 Day 21 Part 1    //
// https://adventofcode.com/2023/day/21 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>

int solution(const std::string fileName, const int desiredNumberOfSteps);
std::set<std::pair<int, int>> takeStep(const std::vector<std::string>& input, const std::set<std::pair<int, int>>& currentPositions);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution (steps=6): " << solution("example.txt", 6) << " (expected 16)" << std::endl;
    std::cout << "Input Solution (steps=64): " << solution("input.txt", 64) << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName, const int desiredNumberOfSteps) {
    std::vector<std::string> input = readLines(fileName);

    std::pair<int, int> startingPosition;
    for (int rowNumber = 0; rowNumber < input.size(); ++rowNumber) {
        for (int colNumber = 0; colNumber < input[0].length(); ++colNumber) {
            if (input[rowNumber][colNumber] == 'S') {
                startingPosition = {rowNumber, colNumber};
                break;
            }
        }
    }

    std::set<std::pair<int, int>> possiblePositions;
    possiblePositions.insert(startingPosition);

    for (int step = 0; step < desiredNumberOfSteps; ++step) {
        possiblePositions = takeStep(input, possiblePositions);
    }

    return possiblePositions.size();
}

std::set<std::pair<int, int>> takeStep(const std::vector<std::string>& input, const std::set<std::pair<int, int>>& currentPositions) {
    std::set<std::pair<int, int>> newPositions;

    for (const std::pair<int, int>& currentPosition : currentPositions) {
        // Check UP
        if (currentPosition.first > 0 && input[currentPosition.first - 1][currentPosition.second] != '#') {
            newPositions.emplace(currentPosition.first - 1, currentPosition.second);
        }
        // Check RIGHT
        if (currentPosition.second < input[0].length() - 1 && input[currentPosition.first][currentPosition.second + 1] != '#') {
            newPositions.emplace(currentPosition.first, currentPosition.second + 1);
        }
        // Check DOWN
        if (currentPosition.first < input.size() - 1 && input[currentPosition.first + 1][currentPosition.second] != '#') {
            newPositions.emplace(currentPosition.first + 1, currentPosition.second);
        }
        // Check LEFT
        if (currentPosition.second > 0 && input[currentPosition.first][currentPosition.second - 1] != '#') {
            newPositions.emplace(currentPosition.first, currentPosition.second - 1);
        }
    }

    return newPositions;
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
