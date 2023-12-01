/////////////////////////////////////////
// Advent of Code 2023 Day 1 Part 2    //
// https://adventofcode.com/2023/day/1 //
/////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <map>

int solution(std::string fileName);
std::vector<std::string> readLines(std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("part2_example.txt") << " (expected 281)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::map<std::string, std::string> wordStrToNumberStr = {
        {"zero", "0"},
        {"one", "1"},
        {"two", "2"},
        {"three", "3"},
        {"four", "4"},
        {"five", "5"},
        {"six", "6"},
        {"seven", "7"},
        {"eight", "8"},
        {"nine", "9"}
    };
    std::regex digitPattern("zero|one|two|three|four|five|six|seven|eight|nine|[0-9]", std::regex::icase);

    int counter = 0;
    for (std::string line : input) {
        std::string firstDigit;
        std::string secondDigit;
        std::string searchString = line;
        std::smatch matches;
        while(regex_search(searchString, matches, digitPattern)) {
            if (firstDigit.empty()) {
                firstDigit = (matches[0].length() > 1) ? wordStrToNumberStr[matches[0]] : matches[0];
            }
            secondDigit = (matches[0].length() > 1) ? wordStrToNumberStr[matches[0]] : matches[0];
            // Since the numbers might overlap (e.g. twone), change the search
            // string to everything after the start of the found match.
            searchString = searchString.substr(matches.position(0) + 1);
        }
        counter += stoi(firstDigit + secondDigit);
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
