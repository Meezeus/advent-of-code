//////////////////////////////////////////
// Advent of Code 2023 Day 10 Part 2    //
// https://adventofcode.com/2023/day/10 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <algorithm>
#include <iomanip>

enum Direction { TOP, RIGHT, BOTTOM, LEFT };

int solution1(const std::string fileName);
std::vector<std::vector<char>> expandGrid(const std::vector<std::vector<char>>& inputGrid);
std::vector<std::vector<char>> contractGrid(const std::vector<std::vector<char>>& expandedGrid);

int solution2(const std::string fileName);

char decodeS(const std::vector<std::vector<char>>& inputGrid, const std::pair<int, int>& startPos);
std::vector<std::pair<int, int>> getNeighbours(const std::vector<std::vector<char>>& grid, const std::pair<int, int>& currentPos);
bool compatible(const char symbol, const Direction direction, const char adjacent);
void printCharGrid(const std::vector<std::vector<char>>& grid);
void printNumberGrid(const std::vector<std::vector<int>>& grid);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example 1 Solution 1: " << solution1("part2_example_1.txt") << " (expected 4)" << std::endl;
    std::cout << "Example 2 Solution 1: " << solution1("part2_example_2.txt") << " (expected 4)" << std::endl;
    std::cout << "Example 3 Solution 1: " << solution1("part2_example_3.txt") << " (expected 8)" << std::endl;
    std::cout << "Example 4 Solution 1: " << solution1("part2_example_4.txt") << " (expected 10)" << std::endl;
    std::cout << "Input Solution 1: " << solution1("input.txt") << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Example 1 Solution 2: " << solution2("part2_example_1.txt") << " (expected 4)" << std::endl;
    std::cout << "Example 2 Solution 2: " << solution2("part2_example_2.txt") << " (expected 4)" << std::endl;
    std::cout << "Example 3 Solution 2: " << solution2("part2_example_3.txt") << " (expected 8)" << std::endl;
    std::cout << "Example 4 Solution 2: " << solution2("part2_example_4.txt") << " (expected 10)" << std::endl;
    std::cout << "Input Solution 2: " << solution2("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution1(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::vector<std::vector<char>> inputGrid(input.size(), std::vector<char>(input[0].length()));
    std::vector<std::vector<int>> cycleGrid(input.size(), std::vector<int>(input[0].length(), -1));
    std::pair<int, int> startPos;
    for (int i1 = 0; i1 < input.size(); ++i1) {
        for (int i2 = 0; i2 < input[i1].length(); ++i2) {
            inputGrid[i1][i2] = input[i1][i2];
            if (input[i1][i2] == 'S') {
                startPos = std::pair<int, int>(i1, i2);
            }
        }
    }
    inputGrid[startPos.first][startPos.second] = decodeS(inputGrid, startPos);
    cycleGrid[startPos.first][startPos.second] = 0;

    std::queue<std::pair<int, int>> cycleQueue({startPos});
    while (!cycleQueue.empty()) {
        std::pair<int, int> currentPos = cycleQueue.front();
        cycleQueue.pop();
        std::vector<std::pair<int, int>> neighbours = getNeighbours(inputGrid, currentPos);
        for (std::pair<int, int> neighbour : neighbours) {
            if (cycleGrid[neighbour.first][neighbour.second] == -1) {
                cycleQueue.push(neighbour);
                cycleGrid[neighbour.first][neighbour.second] = cycleGrid[currentPos.first][currentPos.second] + 1;
            }
        }
    }

    for (int i1 = 0; i1 < inputGrid.size(); ++i1) {
        for (int i2 = 0; i2 < inputGrid[i1].size(); ++i2) {
            if (cycleGrid[i1][i2] == -1) {
                inputGrid[i1][i2] = '.';
            }
        }
    }

    std::vector<std::vector<char>> expandedGrid = expandGrid(inputGrid);

    std::queue<std::pair<int, int>> areaQueue({std::pair<int, int>(0, 0)});
    while (!areaQueue.empty()) {
        std::pair<int, int> currentPos = areaQueue.front();
        areaQueue.pop();
        std::vector<std::pair<int, int>> neighbours = getNeighbours(expandedGrid, currentPos);
        for (std::pair<int, int> neighbour : neighbours) {
            if (expandedGrid[neighbour.first][neighbour.second] == '.') {
                areaQueue.push(neighbour);
                expandedGrid[neighbour.first][neighbour.second] = 'O';
            }
        }
    }

    std::vector<std::vector<char>> contractedGrid = contractGrid(expandedGrid);

    int counter = 0;
    for (int i1 = 0; i1 < contractedGrid.size(); ++i1) {
        for (int i2 = 0; i2 < contractedGrid[i1].size(); ++i2) {
            if (contractedGrid[i1][i2] == '.') {
                ++counter;
            }
        }
    }

    return counter;
}

std::vector<std::vector<char>> expandGrid(const std::vector<std::vector<char>>& inputGrid) {
    std::vector<std::vector<char>> expandedGrid((inputGrid.size() * 2) + 1, std::vector<char>((inputGrid[0].size() * 2) + 1));

    for (int i1 = 0; i1 < expandedGrid.size(); ++i1) {
        for (int i2 = 0; i2 < expandedGrid[i1].size(); ++i2) {
            if (i1 % 2 == 0) {
                if (i1 > 0 && (expandedGrid[i1 - 1][i2] == '|' ||  expandedGrid[i1 - 1][i2] == '7' || expandedGrid[i1 - 1][i2] == 'F')){
                    expandedGrid[i1][i2] = '|';
                }
                else {
                    expandedGrid[i1][i2] = '.';
                }
            }
            else {
                if (i2 % 2 == 0) {
                    if (i2 > 0 && (expandedGrid[i1][i2 - 1] == '-' ||  expandedGrid[i1][i2 - 1] == 'L' || expandedGrid[i1][i2 - 1] == 'F')){
                        expandedGrid[i1][i2] = '-';
                    }
                    else {
                        expandedGrid[i1][i2] = '.';
                    }
                }
                else {
                    expandedGrid[i1][i2] = inputGrid[(i1 - 1) / 2][(i2 - 1) / 2];
                }
            }
        }
    }

    return expandedGrid;
}

std::vector<std::vector<char>> contractGrid(const std::vector<std::vector<char>>& expandedGrid) {
    std::vector<std::vector<char>> contractedGrid((expandedGrid.size() - 1) / 2, std::vector<char>((expandedGrid[0].size() - 1) / 2));

    for (int i1 = 0; i1 < expandedGrid.size(); ++i1) {
        for (int i2 = 0; i2 < expandedGrid[i1].size(); ++i2) {
            if (i1 % 2 != 0 && i2 % 2 != 0) {
                contractedGrid[(i1 - 1) / 2][(i2 - 1) / 2] = expandedGrid[i1][i2];
            }
        }
    }

    return contractedGrid;
}

int solution2(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::vector<std::vector<char>> inputGrid(input.size(), std::vector<char>(input[0].length()));
    std::vector<std::vector<int>> cycleGrid(input.size(), std::vector<int>(input[0].length(), -1));
    std::pair<int, int> startPos;
    for (int i1 = 0; i1 < input.size(); ++i1) {
        for (int i2 = 0; i2 < input[i1].length(); ++i2) {
            inputGrid[i1][i2] = input[i1][i2];
            if (input[i1][i2] == 'S') {
                startPos = std::pair<int, int>(i1, i2);
            }
        }
    }
    inputGrid[startPos.first][startPos.second] = decodeS(inputGrid, startPos);
    cycleGrid[startPos.first][startPos.second] = 0;

    std::queue<std::pair<int, int>> cycleQueue({startPos});
    while (!cycleQueue.empty()) {
        std::pair<int, int> currentPos = cycleQueue.front();
        cycleQueue.pop();
        std::vector<std::pair<int, int>> neighbours = getNeighbours(inputGrid, currentPos);
        for (std::pair<int, int> neighbour : neighbours) {
            if (cycleGrid[neighbour.first][neighbour.second] == -1) {
                cycleQueue.push(neighbour);
                cycleGrid[neighbour.first][neighbour.second] = cycleGrid[currentPos.first][currentPos.second] + 1;
            }
        }
    }

    int counter = 0;
    for (int i1 = 0; i1 < inputGrid.size(); ++i1) {
        bool outside = true;
        char horizontalStart;
        for (int i2 = 0; i2 < inputGrid[i1].size(); ++i2) {
            if (cycleGrid[i1][i2] == -1) {
                inputGrid[i1][i2] = outside ? 'O' : 'I';
                counter += outside ? 0 : 1;
            }
            else {
                if (inputGrid[i1][i2] == '|') {
                    outside = !outside;
                }
                else if (inputGrid[i1][i2] == 'F' || inputGrid[i1][i2] == 'L') {
                    horizontalStart = inputGrid[i1][i2];
                }
                else if (inputGrid[i1][i2] == 'J' && horizontalStart == 'F') {
                    outside = !outside;
                }
                else if (inputGrid[i1][i2] == '7' && horizontalStart == 'L') {
                    outside = !outside;
                }
            }
        }
    }

    return counter;
}

char decodeS(const std::vector<std::vector<char>>& inputGrid, const std::pair<int, int>& startPos) {
    std::vector<char> allChars = {'|', '-', 'L', 'J', '7', 'F'};
    std::vector<char> possibleChars;
    for (char c : allChars) {
        if (startPos.first > 0) {
            if (compatible(c, TOP, inputGrid[startPos.first - 1][startPos.second])) {
                possibleChars.push_back(c);
            }
        }
        if (startPos.second < inputGrid[startPos.first].size() - 1) {
            if (compatible(c, RIGHT, inputGrid[startPos.first][startPos.second + 1])) {
                possibleChars.push_back(c);
            }
        }
        if (startPos.first < inputGrid.size() - 1) {
            if (compatible(c, BOTTOM, inputGrid[startPos.first + 1][startPos.second])) {
                possibleChars.push_back(c);
            }
        }
        if (startPos.second > 0) {
            if (compatible(c, LEFT, inputGrid[startPos.first][startPos.second - 1])) {
                possibleChars.push_back(c);
            }
        }
    }

    std::sort(possibleChars.begin(), possibleChars.end());
    for (int i = 0; i < possibleChars.size() - 1; ++i) {
        if (possibleChars[i] == possibleChars[i+1]) {
            return possibleChars[i];
        }
    }
}

std::vector<std::pair<int, int>> getNeighbours(const std::vector<std::vector<char>>& grid, const std::pair<int, int>& currentPos) {
    std::vector<std::pair<int, int>> neighbours;

    char symbol = grid[currentPos.first][currentPos.second];

    if (currentPos.first > 0) {
        if (compatible(symbol, TOP, grid[currentPos.first - 1][currentPos.second])) {
            neighbours.push_back(std::pair<int, int>(currentPos.first - 1, currentPos.second));
        }
    }
    if (currentPos.second < grid[currentPos.first].size() - 1) {
        if (compatible(symbol, RIGHT, grid[currentPos.first][currentPos.second + 1])) {
            neighbours.push_back(std::pair<int, int>(currentPos.first, currentPos.second + 1));
        }
    }
    if (currentPos.first < grid.size() - 1) {
        if (compatible(symbol, BOTTOM, grid[currentPos.first + 1][currentPos.second])) {
            neighbours.push_back(std::pair<int, int>(currentPos.first + 1, currentPos.second));
        }
    }
    if (currentPos.second > 0) {
        if (compatible(symbol, LEFT, grid[currentPos.first][currentPos.second - 1])) {
            neighbours.push_back(std::pair<int, int>(currentPos.first, currentPos.second - 1));
        }
    }

    return neighbours;
}

bool compatible(const char symbol, const Direction direction, const char adjacent) {
    switch (symbol) {
        case '|':
            switch (direction) {
                case TOP:
                    return adjacent == '|' || adjacent == '7' || adjacent == 'F';
                case RIGHT:
                    return false;
                case BOTTOM:
                    return adjacent == '|' || adjacent == 'J' || adjacent == 'L';
                case LEFT:
                    return false;
            }
        case '-':
            switch (direction) {
                case TOP:
                    return false;
                case RIGHT:
                    return adjacent == '-' || adjacent == 'J' || adjacent == '7';
                case BOTTOM:
                    return false;
                case LEFT:
                    return adjacent == '-' || adjacent == 'L' || adjacent == 'F';
            }
        case 'L':
            switch (direction) {
                case TOP:
                    return adjacent == '|' || adjacent == '7' || adjacent == 'F';
                case RIGHT:
                    return adjacent == '-' || adjacent == 'J' || adjacent == '7';
                case BOTTOM:
                    return false;
                case LEFT:
                    return false;
            }
        case 'J':
            switch (direction) {
                case TOP:
                    return adjacent == '|' || adjacent == '7' || adjacent == 'F';
                case RIGHT:
                    return false;
                case BOTTOM:
                    return false;
                case LEFT:
                    return adjacent == '-' || adjacent == 'L' || adjacent == 'F';
            }
        case '7':
            switch (direction) {
                case TOP:
                    return false;
                case RIGHT:
                    return false;
                case BOTTOM:
                    return adjacent == '|' || adjacent == 'J' || adjacent == 'L';
                case LEFT:
                    return adjacent == '-' || adjacent == 'L' || adjacent == 'F';
            }
        case 'F':
            switch (direction) {
                case TOP:
                    return false;
                case RIGHT:
                    return adjacent == '-' || adjacent == 'J' || adjacent == '7';
                case BOTTOM:
                    return adjacent == '|' || adjacent == 'J' || adjacent == 'L';
                case LEFT:
                    return false;
            }
        case '.':
            return (adjacent == '.');
    }
}

void printCharGrid(const std::vector<std::vector<char>>& grid) {
    std::cout << "" << std::endl;
    for (int i1 = 0; i1 < grid.size(); ++i1) {
        for (int i2 = 0; i2 < grid[i1].size(); ++i2) {
            std::cout << grid[i1][i2];
        }
        std::cout << "" << std::endl;
    }
    std::cout << "" << std::endl;
}

void printNumberGrid(const std::vector<std::vector<int>>& grid) {
    int maxDigits = 0;
    for (std::vector<int> row : grid) {
        for (int num : row) {
            int digits = std::to_string(num).length();
            maxDigits = std::max(maxDigits, digits);
        }
    }

    std::cout << "" << std::endl;
    for (std::vector<int> row : grid) {
        for (int num : row) {
            std::cout << std::setw(maxDigits + 1) << num;
        }
        std::cout << "" << std::endl;;
    }
    std::cout << "" << std::endl;;
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
