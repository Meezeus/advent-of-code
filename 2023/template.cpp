/////////////////////////////////////////
// Advent of Code 2023 Day X Part X    //
// https://adventofcode.com/2023/day/X //
/////////////////////////////////////////

//////////////////////////////////////////
// Advent of Code 2023 Day XX Part X    //
// https://adventofcode.com/2023/day/XX //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

int solution(const std::string fileName);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected ?)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    

    return -1;
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
