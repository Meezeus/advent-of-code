//////////////////////////////////////////
// Advent of Code 2023 Day 11 Part 2    //
// https://adventofcode.com/2023/day/11 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

long long solution(const std::string fileName, int multiplier);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution (multiplier=2): " << solution("example.txt", 2) << " (expected 374)" << std::endl;
    std::cout << "Example Solution (multiplier=10): " << solution("example.txt", 10) << " (expected 1030)" << std::endl;
    std::cout << "Example Solution (multiplier=100): " << solution("example.txt", 100) << " (expected 8410)" << std::endl;
    std::cout << "Input Solution (multiplier=1000000): " << solution("input.txt", 1000000) << std::endl;
    std::cout << std::endl;
}

long long solution(const std::string fileName, int multiplier) {
    std::vector<std::string> input = readLines(fileName);

    std::vector<bool> rowIsEmpty(input.size(), true);
    std::vector<bool> colIsEmpty(input[0].length(), true);
    for (int i1 = 0; i1 < input.size(); ++i1) {
        for (int i2 = 0; i2 < input[0].length(); ++i2) {
            if (input[i1][i2] != '.') {
                rowIsEmpty[i1] = false;
                colIsEmpty[i2] = false;
            }
        }
    }

    std::vector<std::pair<int, int>> galaxyPositions;
    for (int i1 = 0; i1 < input.size(); ++i1) {
        for (int i2 = 0; i2 < input[i1].length(); ++i2) {
            if (input[i1][i2] != '.') {
                galaxyPositions.push_back(std::pair<int, int>(i1, i2));
            }
        }
    }

    long long sum = 0;
    for (int galaxyIndex1 = 0; galaxyIndex1 < galaxyPositions.size(); ++galaxyIndex1) {
        for (int galaxyIndex2 = galaxyIndex1 + 1; galaxyIndex2 < galaxyPositions.size(); ++galaxyIndex2) {
            long long distance = 0;

            int smallX = std::min(galaxyPositions[galaxyIndex1].first, galaxyPositions[galaxyIndex2].first);
            int bigX = std::max(galaxyPositions[galaxyIndex1].first, galaxyPositions[galaxyIndex2].first);
            for (int xIndex = smallX + 1; xIndex <= bigX; ++xIndex) {
                if (rowIsEmpty[xIndex]) {
                    distance += multiplier;
                }
                else {
                    distance += 1;
                }
            }

            int smallY = std::min(galaxyPositions[galaxyIndex1].second, galaxyPositions[galaxyIndex2].second);
            int bigY = std::max(galaxyPositions[galaxyIndex1].second, galaxyPositions[galaxyIndex2].second);
            for (int yIndex = smallY + 1; yIndex <= bigY; ++yIndex) {
                if (colIsEmpty[yIndex]) {
                    distance += multiplier;
                }
                else {
                    distance += 1;
                }
            }

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
