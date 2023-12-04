/////////////////////////////////////////
// Advent of Code 2023 Day 4 Part 2    //
// https://adventofcode.com/2023/day/4 //
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
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 30)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::vector<int> cardCopies(input.size(), 1);
    std::regex pattern("\\d+");
    for (int lineNumber = 0; lineNumber < input.size(); ++lineNumber) {
        std::string line = input[lineNumber];
        int numOfMatches = 0;
        int colonIndex = line.find(':');
        int pipeIndex = line.find('|');
        std::string myNumbers = line.substr(colonIndex + 1, pipeIndex - (colonIndex + 1));
        std::string winningNumbers = line.substr(pipeIndex + 1);

        std::sregex_iterator iterator1(myNumbers.begin(), myNumbers.end(), pattern);
        std::sregex_iterator end;
        for (; iterator1 != end; ++iterator1) {
            std::smatch match1 = *iterator1;

            std::sregex_iterator iterator2(winningNumbers.begin(), winningNumbers.end(), pattern);
            for (; iterator2 != end; ++iterator2) {
                std::smatch match2 = *iterator2;

                if (match1[0] == match2[0]) {
                    numOfMatches += 1;
                }
            }
        }

        for (int i = 1 ; i <= numOfMatches; ++i) {
            cardCopies[lineNumber + i] += cardCopies[lineNumber];
        }
    }

    return std::accumulate(cardCopies.begin(), cardCopies.end(), 0);
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
