//////////////////////////////////////////
// Advent of Code 2023 Day 13 Part 1    //
// https://adventofcode.com/2023/day/13 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

int solution(const std::string fileName);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 405)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::vector<std::vector<std::vector<char>>> grids;
    std::vector<std::vector<char>> grid;
    for (int i1 = 0; i1 < input.size(); ++i1) {
        if (input[i1].empty()) {
            grids.push_back(grid);
            grid = {};
        }
        else {
            std::vector<char> row;
            for (int i2 = 0; i2 < input[i1].length(); ++i2) {
                row.push_back(input[i1][i2]);
            }
            grid.push_back(row);
        }
    }
    grids.push_back(grid);

    int total = 0;

    for (std::vector<std::vector<char>> grid : grids) {
        for (int mirrorIndex = 0; mirrorIndex < grid.size() - 1; ++mirrorIndex) {
            int maxOffset = std::min(mirrorIndex, static_cast<int>((grid.size() - 1) - (mirrorIndex + 1)));
            bool equal = true;
            for (int offset = 0; offset <= maxOffset; ++offset) {
                std::string top;
                for (char c : grid[mirrorIndex - offset]) {
                    top += c;
                }

                std::string bottom;
                for (char c : grid[mirrorIndex + 1 + offset]) {
                    bottom += c;
                }

                if (top != bottom) {
                    equal = false;
                    break;
                }
            }
            if (equal) {
                total += (mirrorIndex + 1) * 100;
                break;
            }
        }

        for (int mirrorIndex = 0; mirrorIndex < grid[0].size() - 1; ++mirrorIndex) {
            int maxOffset = std::min(mirrorIndex, static_cast<int>((grid[0].size() - 1) - (mirrorIndex + 1)));
            bool equal = true;
            for (int offset = 0; offset <= maxOffset; ++offset) {
                std::string left;
                for (std::vector<char> row : grid) {
                    left += row[mirrorIndex - offset];
                }

                std::string right;
                for (std::vector<char> row : grid) {
                    right += row[mirrorIndex + 1 + offset];
                }

                if (left != right) {
                    equal = false;
                    break;
                }
            }
            if (equal) {
                total += mirrorIndex + 1;
                break;
            }
        }
    }

    return total;
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
