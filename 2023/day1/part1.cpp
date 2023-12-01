/////////////////////////////////////////
// Advent of Code 2023 Day 1 Part 1    //
// https://adventofcode.com/2023/day/1 //
/////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

int solution(std::string fileName);
std::vector<std::string> readLines(std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("part1_example.txt") << " (expected 142)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    int counter = 0;
    for (std::string line : input) {
        char firstDigit = ' ';
        char lastDigit = ' ';
        std::string number;
        for (char c : line) {
            if (firstDigit == ' ' && isdigit(c)) {
                firstDigit = c;
            }
            if (isdigit(c)) {
                lastDigit = c;
            }
        }
        number += firstDigit;
        number += lastDigit;
        counter += stoi(number);
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
