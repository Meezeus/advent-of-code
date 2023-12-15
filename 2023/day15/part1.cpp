//////////////////////////////////////////
// Advent of Code 2023 Day 15 Part 1    //
// https://adventofcode.com/2023/day/15 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

int solution(const std::string fileName);
int hash(std::string str);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 1320)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::vector<std::string> instructions;
    std::istringstream ss(input[0]);
    std::string instruction;
    while (std::getline(ss, instruction, ',')) {
        instructions.push_back(instruction);
    }

    int total = 0;
    for (std::string instruction : instructions) {
        int hashedInstruction = hash(instruction);
        total += hashedInstruction;
    }
    return total;
}

int hash(std::string str) {
    int value = 0;
    for (char c : str) {
        value += c;
        value *= 17;
        value %= 256;
    }
    return value;
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
