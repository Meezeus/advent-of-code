//////////////////////////////////////////
// Advent of Code 2023 Day 14 Part 1    //
// https://adventofcode.com/2023/day/14 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

int solution(const std::string fileName);
void slideRock(std::vector<std::string>& input, int rockRow, int rockCol);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 136)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    for (int rockRow = 1; rockRow < input.size(); ++rockRow) {
        for (int rockCol = 0; rockCol < input[rockRow].length(); ++rockCol) {
            if (input[rockRow][rockCol] == 'O') {
                slideRock(input, rockRow, rockCol);
            }
        }
    }

    int total = 0;
    for (int rockRow = 0; rockRow < input.size(); ++rockRow) {
            for (int rockCol = 0; rockCol < input[rockRow].length(); ++rockCol) {
                if (input[rockRow][rockCol] == 'O') {
                    total += input.size() - rockRow;
                }
            }
        }
    return total;
}

void slideRock(std::vector<std::string>& input, int rockRow, int rockCol) {
    if (rockRow > 0 && input[rockRow - 1][rockCol] == '.') {
        input[rockRow][rockCol] = '.';
        input[rockRow - 1][rockCol] = 'O';
        slideRock(input, rockRow - 1, rockCol);
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
