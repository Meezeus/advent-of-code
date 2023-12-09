/////////////////////////////////////////
// Advent of Code 2023 Day 9 Part 1    //
// https://adventofcode.com/2023/day/9 //
/////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <numeric>

int solution(const std::string fileName);
std::vector<int> calculateLastSequenceNumbers(const std::vector<int>& sequence);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 114)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);
    std::regex numberPattern("-?\\d+");

    int sum = 0;
    for (std::string line : input) {
        std::vector<int> baseSequence;
        std::sregex_iterator iterator(line.begin(), line.end(), numberPattern);
        std::sregex_iterator end;
        while (iterator != end) {
            baseSequence.push_back(std::stoi((*iterator)[0]));
            ++iterator;
        }

        std::vector<int> lastSequenceNumbers = calculateLastSequenceNumbers(baseSequence);
        sum += std::accumulate(lastSequenceNumbers.begin(), lastSequenceNumbers.end(), 0);
    }

    return sum;
}

std::vector<int> calculateLastSequenceNumbers(const std::vector<int>& sequence) {
    bool allZeros = true;
    for (int n : sequence) {
        if (n != 0) {
            allZeros = false;
        }
    }

    if (allZeros) {
        return {0};
    }
    else {
        std::vector<int> newSequence;
        for (int i = 0; i < sequence.size() - 1; ++i) {
            newSequence.push_back(sequence[i+1] - sequence[i]);
        }
        std::vector<int> lastSequenceNumbers = calculateLastSequenceNumbers(newSequence);
        lastSequenceNumbers.push_back(sequence.back());
        return lastSequenceNumbers;
    }
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
