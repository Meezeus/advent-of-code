//////////////////////////////////////////
// Advent of Code 2023 Day 16 Part 1    //
// https://adventofcode.com/2023/day/16 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

enum Direction { RIGHT, DOWN, LEFT, UP };

int solution(const std::string fileName);
void travel(const int rowIndex, const int colIndex, const Direction direction, const std::vector<std::string>& input, std::vector<std::vector<char>>& energyGrid, std::vector<std::string>& history);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 46)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);
    std::vector<std::vector<char>> energyGrid = std::vector<std::vector<char>>(input.size(), std::vector<char>(input[0].length(), '.'));
    std::vector<std::string> history;

    travel(0, 0, RIGHT, input, energyGrid, history);

    int total = 0;
    for (int i1 = 0; i1 < energyGrid.size(); ++i1) {
        for (int i2 = 0; i2 < energyGrid[0].size(); ++i2) {
            if (energyGrid[i1][i2] == '#') {
                total += 1;
            }
        }
    }
    return total;
}

void travel(const int rowIndex, const int colIndex, const Direction direction, const std::vector<std::string>& input, std::vector<std::vector<char>>& energyGrid, std::vector<std::string>& history) {
    if (rowIndex < 0 || rowIndex >= input.size() || colIndex < 0 || colIndex >= input[0].length()) {
        return;
    }

    std::string historyString = std::to_string(rowIndex) + " " + std::to_string(colIndex) + " " + std::to_string(direction);
    if (std::find(history.begin(), history.end(), historyString) != history.end()) {
        return;
    }
    history.push_back(historyString);

    energyGrid[rowIndex][colIndex] = '#';

    int rowOffset = 0;
    int colOffset = 0;
    switch (direction) {
        case RIGHT:
            colOffset = 1;
            break;
        case DOWN:
            rowOffset = 1;
            break;
        case LEFT:
            colOffset = -1;
            break;
        case UP:
            rowOffset = -1;
            break;
    }

    char tile = input[rowIndex][colIndex];
    switch (tile) {
        case '.':
            travel(rowIndex + rowOffset, colIndex + colOffset, direction, input, energyGrid, history);
            break;
        case '|':
            if (direction == LEFT || direction == RIGHT) {
                travel(rowIndex - 1, colIndex, UP, input, energyGrid, history);
                travel(rowIndex + 1, colIndex, DOWN, input, energyGrid, history);
            }
            else if (direction == UP || direction == DOWN){
                travel(rowIndex + rowOffset, colIndex + colOffset, direction, input, energyGrid, history);
            }
            break;
        case '-':
            if (direction == UP || direction == DOWN) {
                travel(rowIndex, colIndex - 1, LEFT, input, energyGrid, history);
                travel(rowIndex, colIndex + 1, RIGHT, input, energyGrid, history);
            }
            else if (direction == LEFT || direction == RIGHT){
                travel(rowIndex + rowOffset, colIndex + colOffset, direction, input, energyGrid, history);
            }
            break;
        case '/':
            if (direction == UP) {
                travel(rowIndex, colIndex + 1, RIGHT, input, energyGrid, history);
            }
            else if (direction == RIGHT) {
                travel(rowIndex - 1, colIndex, UP, input, energyGrid, history);
            }
            else if (direction == DOWN) {
                travel(rowIndex, colIndex - 1, LEFT, input, energyGrid, history);
            }
            else if (direction == LEFT) {
                travel(rowIndex + 1, colIndex, DOWN, input, energyGrid, history);
            }
            break;
        case '\\':
            if (direction == UP) {
                travel(rowIndex, colIndex - 1, LEFT, input, energyGrid, history);
            }
            else if (direction == RIGHT) {
                travel(rowIndex + 1, colIndex, DOWN, input, energyGrid, history);
            }
            else if (direction == DOWN) {
                travel(rowIndex, colIndex + 1, RIGHT, input, energyGrid, history);
            }
            else if (direction == LEFT) {
                travel(rowIndex - 1, colIndex, UP, input, energyGrid, history);
            }
            break;
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
