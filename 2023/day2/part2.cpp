/////////////////////////////////////////
// Advent of Code 2023 Day 2 Part 2    //
// https://adventofcode.com/2023/day/2 //
/////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <numeric>

int solution(std::string fileName);
std::vector<std::string> readLines(std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 2286)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::vector<std::regex> regexPatterns = {};
    std::regex redPattern("(\\d+) red", std::regex::icase);
    regexPatterns.push_back(redPattern);
    std::regex greenPattern("(\\d+) green", std::regex::icase);
    regexPatterns.push_back(greenPattern);
    std::regex bluePattern("(\\d+) blue", std::regex::icase);
    regexPatterns.push_back(bluePattern);

    int counter = 0;
    for (std::string line : input) {
        std::vector<int> minCubeNumbers = {};
        for (std::regex pattern : regexPatterns) {
            int largestNumber = 0;
            std::smatch matches;
            std::string searchString = line;
            while(regex_search(searchString, matches, pattern)) {
                int count = std::stoi(matches[1]);
                if (count > largestNumber) {largestNumber = count;}
                searchString = matches.suffix();
            }
            minCubeNumbers.push_back(largestNumber);
        }
        counter += std::accumulate(minCubeNumbers.begin(), minCubeNumbers.end(), 1, std::multiplies<int>());
    }

    return counter;
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
