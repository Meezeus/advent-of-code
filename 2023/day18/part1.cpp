//////////////////////////////////////////
// Advent of Code 2023 Day 18 Part 1    //
// https://adventofcode.com/2023/day/18 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <sstream>
#include <queue>

enum Direction { UP, RIGHT, DOWN, LEFT };

int solution(const std::string fileName);
void digTrench(std::map<std::pair<int, int>, char>& grid, std::pair<int, int>& currentPosition, int direction, int distance);
void saveGrid(const std::map<std::pair<int, int>, char>& grid, int minRowNumber, int maxRowNumber, int minColNumber, int maxColNumber, std::string fileName);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 62)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::map<std::pair<int, int>, char> grid;
    std::pair<int, int> currentPosition(0, 0);

    // Dig the lagoon border.
    grid[currentPosition] = '#';
    for (std::string line : input) {
        std::vector<std::string> tokens;
        std::istringstream iss(line);
        std::string token;
        while (std::getline(iss, token, ' ')) {
            tokens.push_back(token);
        }

        switch (tokens[0][0]) {
        case 'U':
            digTrench(grid, currentPosition, UP, std::stoi(tokens[1]));
            break;
        case 'R':
            digTrench(grid, currentPosition, RIGHT, std::stoi(tokens[1]));
            break;
        case 'D':
            digTrench(grid, currentPosition, DOWN, std::stoi(tokens[1]));
            break;
        case 'L':
            digTrench(grid, currentPosition, LEFT, std::stoi(tokens[1]));
            break;
        }
    }

    // Find bounds.
    int minRowNumber = 0;
    int maxRowNumber = 0;
    int minColNumber = 0;
    int maxColNumber = 0;
    for (const std::pair<std::pair<int, int>, char>& pair : grid) {
        if (pair.first.first < minRowNumber) {
            minRowNumber = pair.first.first;
        }
        if (pair.first.first > maxRowNumber) {
            maxRowNumber = pair.first.first;
        }
        if (pair.first.second < minColNumber) {
            minColNumber = pair.first.second;
        }
        if (pair.first.second > maxColNumber) {
            maxColNumber = pair.first.second;
        }
    }
    minRowNumber--;
    maxRowNumber++;
    minColNumber--;
    maxColNumber++;

    // Fill the grid.
    for (int rowNumber = minRowNumber; rowNumber <= maxRowNumber; ++rowNumber) {
        for (int colNumber = minColNumber; colNumber <= maxColNumber; ++colNumber) {
            if (grid[{rowNumber, colNumber}] != '#') {
                grid[{rowNumber, colNumber}] = '.';
            }
        }
    }
    //saveGrid(grid, minRowNumber, maxRowNumber, minColNumber, maxColNumber, fileName.substr(0, fileName.length() - 4) + "-grid-v1.txt");

    // Mark the outside.
    std::queue<std::pair<int, int>> queue;
    queue.push({minRowNumber, minColNumber});
    while(!queue.empty()) {
        currentPosition = queue.front();
        queue.pop();
        // Check UP
        if (currentPosition.first > minRowNumber && grid[{currentPosition.first - 1, currentPosition.second}] == '.') {
            grid[{currentPosition.first - 1, currentPosition.second}] = 'O';
            queue.emplace(currentPosition.first - 1, currentPosition.second);
        }
        // Check RIGHT
        if (currentPosition.second < maxColNumber && grid[{currentPosition.first, currentPosition.second + 1}] == '.') {
            grid[{currentPosition.first, currentPosition.second + 1}] = 'O';
            queue.emplace(currentPosition.first, currentPosition.second + 1);
        }
        // Check DOWN
        if (currentPosition.first < maxRowNumber && grid[{currentPosition.first + 1, currentPosition.second}] == '.') {
            grid[{currentPosition.first + 1, currentPosition.second}] = 'O';
            queue.emplace(currentPosition.first + 1, currentPosition.second);
        }
        // Check LEFT
        if (currentPosition.second > minColNumber && grid[{currentPosition.first, currentPosition.second - 1}] == '.') {
            grid[{currentPosition.first, currentPosition.second - 1}] = 'O';
            queue.emplace(currentPosition.first, currentPosition.second - 1);
        }
    }
    //saveGrid(grid, minRowNumber, maxRowNumber, minColNumber, maxColNumber, fileName.substr(0, fileName.length() - 4) + "-grid-v2.txt");

    // Count the inside.
    int count = 0;
    for (int rowNumber = minRowNumber; rowNumber <= maxRowNumber; ++rowNumber) {
        for (int colNumber = minColNumber; colNumber <= maxColNumber; ++colNumber) {
            if (grid[{rowNumber, colNumber}] != 'O') {
                count += 1;
            }
        }
    }
    return count;
}

void digTrench(std::map<std::pair<int, int>, char>& grid, std::pair<int, int>& currentPosition, int direction, int distance) {
    for (int i = 1; i <= distance; ++i) {
        switch (direction) {
        case UP:
            currentPosition.first -= 1;
            break;
        case RIGHT:
            currentPosition.second += 1;
            break;
        case DOWN:
            currentPosition.first += 1;
            break;
        case LEFT:
            currentPosition.second -= 1;
            break;
        }
        grid[currentPosition] = '#';
    }
}

void saveGrid(const std::map<std::pair<int, int>, char>& grid, int minRowNumber, int maxRowNumber, int minColNumber, int maxColNumber, std::string fileName) {
    std::ofstream outputFile(fileName);
    for (int rowNumber = minRowNumber; rowNumber <= maxRowNumber; ++rowNumber) {
        for (int colNumber = minColNumber; colNumber <= maxColNumber; ++colNumber) {
            outputFile << grid.at({rowNumber, colNumber});
        }
        outputFile << '\n';
    }
    outputFile.close();
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
