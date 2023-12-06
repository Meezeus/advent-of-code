/////////////////////////////////////////
// Advent of Code 2023 Day 6 Part 1    //
// https://adventofcode.com/2023/day/6 //
/////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <numeric>
#include <cmath>

int solution1(std::string fileName);
int solution2(std::string fileName);
std::vector<std::string> readLines(std::string fileName);

std::regex digitPattern("\\d+");

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution 1: " << solution1("example.txt") << " (expected 288)" << std::endl;
    std::cout << "Input Solution 1: " << solution1("input.txt") << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Example Solution 2: " << solution2("example.txt") << " (expected 288)" << std::endl;
    std::cout << "Input Solution 2: " << solution2("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution1(std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::sregex_iterator end;
    std::sregex_iterator timesIterator(input[0].begin(), input[0].end(), digitPattern);
    std::vector<int> times = {};
    while (timesIterator != end) {
        times.push_back(stoi((*timesIterator)[0]));
        ++timesIterator;
    }
    std::sregex_iterator distancesIterator(input[1].begin(), input[1].end(), digitPattern);
    std::vector<int> distances = {};
    while (distancesIterator != end) {
        distances.push_back(stoi((*distancesIterator)[0]));
        ++distancesIterator;
    }

    std::vector<int> numberOfWinningStrategies = {};
    for (int index = 0; index < times.size(); ++index) {
        int time = times[index];
        int distanceToBeat = distances[index];

        int lowerBound;
        for (int t1 = 0; t1 < time; ++t1) {
            int t2 = time - t1;
            int distance = t1 * t2;
            if (distance > distanceToBeat) {
                lowerBound = t1;
                break;
            }
        }
        int upperBound;
        for (int t1 = time; t1 >= 0; --t1) {
            int t2 = time - t1;
            int distance = t1 * t2;
            if (distance > distanceToBeat) {
                upperBound = t1;
                break;
            }
        }

        numberOfWinningStrategies.push_back(upperBound - lowerBound + 1);
    }

    return std::accumulate(numberOfWinningStrategies.begin(), numberOfWinningStrategies.end(), 1, std::multiplies<int>());
}

int solution2(std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::sregex_iterator end;
    std::sregex_iterator timesIterator(input[0].begin(), input[0].end(), digitPattern);
    std::vector<int> times = {};
    while (timesIterator != end) {
        times.push_back(stoi((*timesIterator)[0]));
        ++timesIterator;
    }
    std::sregex_iterator distancesIterator(input[1].begin(), input[1].end(), digitPattern);
    std::vector<int> distances = {};
    while (distancesIterator != end) {
        distances.push_back(stoi((*distancesIterator)[0]));
        ++distancesIterator;
    }

    std::vector<int> numberOfWinningStrategies = {};
    for (int index = 0; index < times.size(); ++index) {
        int time = times[index];
        int distanceToBeat = distances[index];

        float x1 = (time + std::sqrt((pow(time, 2)) - 4 * distanceToBeat)) / 2;
        float x2 = (time - std::sqrt((pow(time, 2)) - 4 * distanceToBeat)) / 2;

        int upperBound = std::ceil(x1) - 1;
        int lowerBound = std::floor(x2) + 1;

        numberOfWinningStrategies.push_back(upperBound - lowerBound + 1);
    }

    return std::accumulate(numberOfWinningStrategies.begin(), numberOfWinningStrategies.end(), 1, std::multiplies<int>());
}

std::vector<std::string> readLines(std::string fileName) {
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
