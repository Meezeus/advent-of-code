//////////////////////////////////////////
// Advent of Code 2023 Day 10 Part 1    //
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

int solution(const std::string fileName);
char decodeS(const std::vector<std::string>& input, const std::pair<int, int>& startPos);
std::vector<std::pair<int, int>> getNeighbours(const std::vector<std::string>& input, const std::pair<int, int>& currentPos);
bool compatible(const char symbol, const Direction direction, const char adjacent);
void printGrid(const std::vector<std::vector<int>>& grid);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example 1 Solution: " << solution("part1_example_1.txt") << " (expected 4)" << std::endl;
    std::cout << "Example 2 Solution: " << solution("part1_example_2.txt") << " (expected 8)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::vector<std::vector<int>> grid(input.size(), std::vector<int>(input[0].length(), -1));
    std::pair<int, int> startPos;
    for (int i1 = 0; i1 < input.size(); ++i1) {
        for (int i2 = 0; i2 < input[i1].length(); ++i2) {
            if (input[i1][i2] == 'S') {
                startPos = std::pair<int, int>(i1, i2);
                input[i1][i2] = decodeS(input, startPos);
                grid[i1][i2] = 0;
                goto endLoops;
            }
        }
    }
    endLoops:

    int maxDistance = 0;
    std::queue<std::pair<int, int>> queue({startPos});
    while (!queue.empty()) {
        std::pair<int, int> currentPos = queue.front();
        queue.pop();
        std::vector<std::pair<int, int>> neighbours = getNeighbours(input, currentPos);
        for (std::pair<int, int> neighbour : neighbours) {
            if (grid[neighbour.first][neighbour.second] == -1) {
                queue.push(neighbour);
                grid[neighbour.first][neighbour.second] = grid[currentPos.first][currentPos.second] + 1;
                if (grid[neighbour.first][neighbour.second] > maxDistance) {
                    maxDistance = grid[neighbour.first][neighbour.second];
                }
            }
        }
    }

    return maxDistance;
}

char decodeS(const std::vector<std::string>& input, const std::pair<int, int>& startPos) {
    std::vector<char> allChars = {'|', '-', 'L', 'J', '7', 'F'};
    std::vector<char> possibleChars;
    for (char c : allChars) {
        if (startPos.first > 0) {
            if (compatible(c, TOP, input[startPos.first - 1][startPos.second])) {
                possibleChars.push_back(c);
            }
        }
        if (startPos.second < input[startPos.first].length() - 1) {
            if (compatible(c, RIGHT, input[startPos.first][startPos.second + 1])) {
                possibleChars.push_back(c);
            }
        }
        if (startPos.first < input.size() - 1) {
            if (compatible(c, BOTTOM, input[startPos.first + 1][startPos.second])) {
                possibleChars.push_back(c);
            }
        }
        if (startPos.second > 0) {
            if (compatible(c, LEFT, input[startPos.first][startPos.second - 1])) {
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

std::vector<std::pair<int, int>> getNeighbours(const std::vector<std::string>& input, const std::pair<int, int>& currentPos) {
    std::vector<std::pair<int, int>> neighbours;

    char symbol = input[currentPos.first][currentPos.second];

    if (currentPos.first > 0) {
        if (compatible(symbol, TOP, input[currentPos.first - 1][currentPos.second])) {
            neighbours.push_back(std::pair<int, int>(currentPos.first - 1, currentPos.second));
        }
    }
    if (currentPos.second < input[currentPos.first].length() - 1) {
        if (compatible(symbol, RIGHT, input[currentPos.first][currentPos.second + 1])) {
            neighbours.push_back(std::pair<int, int>(currentPos.first, currentPos.second + 1));
        }
    }
    if (currentPos.first < input.size() - 1) {
        if (compatible(symbol, BOTTOM, input[currentPos.first + 1][currentPos.second])) {
            neighbours.push_back(std::pair<int, int>(currentPos.first + 1, currentPos.second));
        }
    }
    if (currentPos.second > 0) {
        if (compatible(symbol, LEFT, input[currentPos.first][currentPos.second - 1])) {
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
    }
}

void printGrid(const std::vector<std::vector<int>>& grid) {
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
