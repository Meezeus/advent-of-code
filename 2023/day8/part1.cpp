/////////////////////////////////////////
// Advent of Code 2023 Day 8 Part 1    //
// https://adventofcode.com/2023/day/8 //
/////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <regex>

int solution(const std::string fileName);
std::map<std::string, std::pair<std::string, std::string>> buildNetworkMap(const std::vector<std::string>& input);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example 1 Solution: " << solution("part1_example_1.txt") << " (expected 2)" << std::endl;
    std::cout << "Example 2 Solution: " << solution("part1_example_2.txt") << " (expected 6)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::string instructions = input[0];
    std::map<std::string, std::pair<std::string, std::string>> networkMap = buildNetworkMap(input);

    int stepCounter = 0;
    int instructionIndex = 0;
    std::string currentLocation = "AAA";
    while (currentLocation != "ZZZ") {
        std::pair<std::string, std::string> nodes = networkMap[currentLocation];
        char instruction = instructions[instructionIndex];

        if (instruction == 'L') {
            currentLocation = nodes.first;
        }
        else if (instruction == 'R') {
            currentLocation = nodes.second;
        }

        ++stepCounter;
        ++instructionIndex;
        if (instructionIndex == instructions.length()) {
            instructionIndex = 0;
        }
    }

    return stepCounter;
}

std::map<std::string, std::pair<std::string, std::string>> buildNetworkMap(const std::vector<std::string>& input) {
    std::map<std::string, std::pair<std::string, std::string>> networkMap = {};
    std::regex locationPattern("[A-Z]{3}");
    for (int index = 2; index < input.size(); ++index) {
        std::sregex_iterator iterator(input[index].begin(), input[index].end(), locationPattern);
        std::string key = (*iterator)[0];
        ++iterator;
        std::string value1 = (*iterator)[0];
        ++iterator;
        std::string value2 = (*iterator)[0];
        networkMap[key] = std::pair<std::string, std::string>(value1, value2);
    }
    return networkMap;
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
