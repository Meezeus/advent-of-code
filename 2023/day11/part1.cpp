//////////////////////////////////////////
// Advent of Code 2023 Day 11 Part 1    //
// https://adventofcode.com/2023/day/11 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

int solution(const std::string fileName);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 374)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);
    int originalRowCount = input.size();
    int originalColCount = input[0].length();

    std::vector<bool> rowIsEmpty(originalRowCount, true);
    std::vector<bool> colIsEmpty(originalColCount, true);
    for (int i1 = 0; i1 < originalRowCount; ++i1) {
        for (int i2 = 0; i2 < originalColCount; ++i2) {
            if (input[i1][i2] != '.') {
                rowIsEmpty[i1] = false;
                colIsEmpty[i2] = false;
            }
        }
    }

    for (int i1 = originalRowCount - 1; i1 >= 0; --i1) {
        if (rowIsEmpty[i1]) {
            input.insert(input.begin() + i1 + 1, std::string(originalRowCount, '.'));
        }
    }
    int newRowCount = input.size();

    for (int i2 = originalColCount - 1; i2 >= 0; --i2) {
        if (colIsEmpty[i2]) {
            for (int i1 = 0; i1 < newRowCount; ++i1) {
                input[i1].insert(i2 + 1, ".");
            }
        }
    }
    int newColCount = input[0].length();

    std::vector<std::pair<int, int>> galaxyPositions;
    for (int i1 = 0; i1 < newRowCount; ++i1) {
        for (int i2 = 0; i2 < newColCount; ++i2) {
            if (input[i1][i2] != '.') {
                galaxyPositions.push_back(std::pair<int, int>(i1, i2));
            }
        }
    }

    int sum = 0;
    for (int i1 = 0; i1 < galaxyPositions.size(); ++i1) {
        for (int i2 = i1 + 1; i2 < galaxyPositions.size(); ++i2) {
            int distance = std::abs(galaxyPositions[i2].first - galaxyPositions[i1].first) + std::abs(galaxyPositions[i2].second - galaxyPositions[i1].second);
            sum += distance;
        }
    }

    return sum;
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
