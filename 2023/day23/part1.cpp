//////////////////////////////////////////
// Advent of Code 2023 Day 23 Part 1    //
// https://adventofcode.com/2023/day/23 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>

int solution(const std::string fileName);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 94)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::set<std::pair<int, int>> lastPositions;
    std::set<std::pair<int, int>> currentPositions;
    currentPositions.insert({0, 1});

    int iterationNumber = 0;
    while(!currentPositions.empty()) {
        iterationNumber++;
        std::set<std::pair<int, int>> newPositions;

        for (const std::pair<int, int>& currentPosition : currentPositions) {
            std::pair<int, int> newPosition;
            // Check UP
            newPosition = {currentPosition.first - 1, currentPosition.second};
            if (std::find(lastPositions.begin(), lastPositions.end(), newPosition) == lastPositions.end() && newPosition.first >= 0 && input[newPosition.first][newPosition.second] != '#' && input[newPosition.first][newPosition.second] != 'v') {
                newPositions.insert(newPosition);
            }
            // Check RIGHT
            newPosition = {currentPosition.first, currentPosition.second + 1};
            if (std::find(lastPositions.begin(), lastPositions.end(), newPosition) == lastPositions.end() && newPosition.second <= input[0].length() - 1 && input[newPosition.first][newPosition.second] != '#' && input[newPosition.first][newPosition.second] != '<') {
                newPositions.insert(newPosition);
            }
            // Check DOWN
            newPosition = {currentPosition.first + 1, currentPosition.second};
            if (std::find(lastPositions.begin(), lastPositions.end(), newPosition) == lastPositions.end() && newPosition.first <= input.size() - 1 && input[newPosition.first][newPosition.second] != '#' && input[newPosition.first][newPosition.second] != '^') {
                newPositions.insert(newPosition);
            }
            // Check LEFT
            newPosition = {currentPosition.first, currentPosition.second - 1};
            if (std::find(lastPositions.begin(), lastPositions.end(), newPosition) == lastPositions.end() && newPosition.second >= 0 && input[newPosition.first][newPosition.second] != '#' && input[newPosition.first][newPosition.second] != '>') {
                newPositions.insert(newPosition);
            }
        }

        lastPositions = std::move(currentPositions);
        currentPositions = std::move(newPositions);
    }

    return iterationNumber - 1;
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
