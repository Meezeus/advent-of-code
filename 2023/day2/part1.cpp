/////////////////////////////////////////
// Advent of Code 2023 Day 2 Part 1    //
// https://adventofcode.com/2023/day/2 //
/////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>

int solution(std::string fileName);
std::vector<std::string> readLines(std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 8)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::vector<int> cubeNumbers = {12, 13, 14};
    std::vector<std::regex> regexPatterns = {};
    std::regex redPattern("(\\d+) red", std::regex::icase);
    regexPatterns.push_back(redPattern);
    std::regex greenPattern("(\\d+) green", std::regex::icase);
    regexPatterns.push_back(greenPattern);
    std::regex bluePattern("(\\d+) blue", std::regex::icase);
    regexPatterns.push_back(bluePattern);
    std::regex gameIDPattern("Game (\\d+)", std::regex::icase);

    int counter = 0;
    for (std::string line : input) {
        bool possible = true;

        for (int i = 0; i < 3; ++i) {
            std::smatch matches;
            std::string searchString = line;
            while(regex_search(searchString, matches, regexPatterns[i])) {
                int count = std::stoi(matches[1]);
                if (count > cubeNumbers[i]) {possible = false;}
                searchString = matches.suffix();
            }
        }

        if (possible) {
            std::smatch matches;
            if (regex_search(line, matches, gameIDPattern)) {
                int gameID = std::stoi(matches[1]);
                counter += gameID;
            }
        }
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
