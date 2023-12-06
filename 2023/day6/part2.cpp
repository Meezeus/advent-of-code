/////////////////////////////////////////
// Advent of Code 2023 Day 6 Part 2    //
// https://adventofcode.com/2023/day/6 //
/////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <algorithm>
#include <cmath>

long long solution1(std::string fileName);
long long solution2(std::string fileName);
std::vector<std::string> readLines(std::string fileName);

std::regex digitPattern("\\d+");

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution 1: " << solution1("example.txt") << " (expected 71503)" << std::endl;
    std::cout << "Input Solution 1: " << solution1("input.txt") << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Example Solution 2: " << solution2("example.txt") << " (expected 71503)" << std::endl;
    std::cout << "Input Solution 2: " << solution2("input.txt") << std::endl;
    std::cout << std::endl;
}

long long solution1(std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::string timeString = input[0];
    timeString.erase(std::remove_if(timeString.begin(), timeString.end(), ::isspace), timeString.end());
    std::string distanceString = input[1];
    distanceString.erase(std::remove_if(distanceString.begin(), distanceString.end(), ::isspace), distanceString.end());

    std::smatch match;
    std::regex_search(timeString, match, digitPattern);
    long long time = std::stoll(match[0]);
    std::regex_search(distanceString, match, digitPattern);
    long long distanceToBeat = std::stoll(match[0]);

    long long lowerBound;
    for (long long t1 = 0; t1 < time; ++t1) {
        long long t2 = time - t1;
        long long distance = t1 * t2;
        if (distance > distanceToBeat) {
            lowerBound = t1;
            break;
        }
    }
    long long upperBound;
    for (long long t1 = time; t1 >= 0; --t1) {
        long long t2 = time - t1;
        long long distance = t1 * t2;
        if (distance > distanceToBeat) {
            upperBound = t1;
            break;
        }
    }

    return upperBound - lowerBound + 1;
}

long long solution2(std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::string timeString = input[0];
    timeString.erase(std::remove_if(timeString.begin(), timeString.end(), ::isspace), timeString.end());
    std::string distanceString = input[1];
    distanceString.erase(std::remove_if(distanceString.begin(), distanceString.end(), ::isspace), distanceString.end());

    std::smatch match;
    std::regex_search(timeString, match, digitPattern);
    long long time = std::stoll(match[0]);
    std::regex_search(distanceString, match, digitPattern);
    long long distanceToBeat = std::stoll(match[0]);

    double x1 = (time + std::sqrt((pow(time, 2)) - 4 * distanceToBeat)) / 2;
    double x2 = (time - std::sqrt((pow(time, 2)) - 4 * distanceToBeat)) / 2;

    long long upperBound = std::ceil(x1) - 1;
    long long lowerBound = std::floor(x2) + 1;

    return upperBound - lowerBound + 1;
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
