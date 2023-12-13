//////////////////////////////////////////
// Advent of Code 2023 Day 12 Part 1    //
// https://adventofcode.com/2023/day/12 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>

int solution1(const std::string fileName);
std::vector<int> getDamagedSpringNumbers(const std::string springString);

int solution2(const std::string fileName);
int getNumberOfPossibleSpringStrings(const std::string springString, const std::vector<int>& damagedSpringNumbers);
int processWorkingSpring(const std::string springString, const std::vector<int>& damagedSpringNumbers);
int processDamagedSpring(const std::string springString, const std::vector<int>& damagedSpringNumbers);

std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution 1: " << solution1("example.txt") << " (expected 21)" << std::endl;
    std::cout << "Input Solution 1: " << solution1("input.txt") << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Example Solution 2: " << solution2("example.txt") << " (expected 21)" << std::endl;
    std::cout << "Input Solution 2: " << solution2("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution1(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);
    std::regex digitPattern("\\d+");

    int total = 0;
    for (std::string line : input) {
        std::string originalSpringString = line.substr(0, line.find(" "));

        std::vector<int> originalDamagedSpringNumbers;
        std::sregex_iterator iterator(line.begin(), line.end(), digitPattern);
        std::sregex_iterator end;
        while (iterator != end) {
            originalDamagedSpringNumbers.push_back(std::stoi((*iterator)[0]));
            ++iterator;
        }

        std::vector<std::string> possibleSpringStrings = {originalSpringString};
        for (int charIndex = 0; charIndex < originalSpringString.length(); ++charIndex) {
            if (originalSpringString[charIndex] == '?') {
                std::vector<std::string> newPossibleSpringStrings;
                for (std::string possibleSpringString : possibleSpringStrings) {
                    for (char c : std::vector<char>({'.', '#'})) {
                        std::string newPossibleSpringString = possibleSpringString;
                        newPossibleSpringString[charIndex] = c;

                        std::vector<int> newPossibleDamagedSpringNumbers = getDamagedSpringNumbers(newPossibleSpringString);
                        bool viable = true;
                        for (int numberIndex = 0; numberIndex < newPossibleDamagedSpringNumbers.size(); ++numberIndex) {
                            if (newPossibleDamagedSpringNumbers[numberIndex] != originalDamagedSpringNumbers[numberIndex]) {
                                viable = false;
                            }
                        }
                        if (viable) {
                            newPossibleSpringStrings.push_back(newPossibleSpringString);
                        }

                    }
                }
                possibleSpringStrings = newPossibleSpringStrings;
            }
        }

        int count = 0;
        for (std::string possibleSpringString : possibleSpringStrings) {
            std::vector<int> possibleDamagedSpringNumbers = getDamagedSpringNumbers(possibleSpringString);
            if (possibleDamagedSpringNumbers == originalDamagedSpringNumbers) {
                count += 1;
            }
        }
        total += count;
    }
    return total;
}

std::vector<int> getDamagedSpringNumbers(const std::string springString) {
    std::vector<int> damagedSpringNumbers;
    int count = 0;
    for (int i = 0; i < springString.length(); ++i) {
        if (springString[i] == '?') {
            break;
        }
        else if (springString[i] == '#') {
            count += 1;
            if (i == springString.length() - 1) {
                damagedSpringNumbers.push_back(count);
            }
        }
        else if (springString[i] == '.' && count > 0) {
            damagedSpringNumbers.push_back(count);
            count = 0;
        }
    }
    return damagedSpringNumbers;
}

int solution2(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);
    std::regex digitPattern("\\d+");

    int total = 0;
    for (std::string line : input) {
        std::string springString = line.substr(0, line.find(" "));

        std::vector<int> damagedSpringNumbers;
        std::sregex_iterator iterator(line.begin(), line.end(), digitPattern);
        std::sregex_iterator end;
        while (iterator != end) {
            damagedSpringNumbers.push_back(std::stoi((*iterator)[0]));
            ++iterator;
        }

        int count = getNumberOfPossibleSpringStrings(springString, damagedSpringNumbers);
        total += count;
    }
    return total;
}

int getNumberOfPossibleSpringStrings(const std::string springString, const std::vector<int>& damagedSpringNumbers) {
    // If the spring string is empty, return 1 if there are no more damaged
    // spring numbers left, or 0 otherwise.
    if (springString.length() == 0) {
        return damagedSpringNumbers.empty() ? 1 : 0;
    }

    // If there are no more damaged spring numbers left, return 1 only if the
    // spring string does not contain any damaged springs, or 0 otherwise.
    if (damagedSpringNumbers.empty()) {
        for (char spring : springString) {
            if (spring == '#') {
                return 0;
            }
        }
        return 1;
    }

    // The spring string is not empty and there are damaged spring numbers left.
    // Process the next spring.
    int numberOfPossibleSpringStrings;
    if (springString[0] == '.') {
        numberOfPossibleSpringStrings = processWorkingSpring(springString, damagedSpringNumbers);
    }
    else if (springString[0] == '#') {
        numberOfPossibleSpringStrings = processDamagedSpring(springString, damagedSpringNumbers);
    }
    else if (springString[0] == '?') {
        numberOfPossibleSpringStrings = processWorkingSpring(springString, damagedSpringNumbers) + processDamagedSpring(springString, damagedSpringNumbers);
    }

    return numberOfPossibleSpringStrings;
}

int processWorkingSpring(const std::string springString, const std::vector<int>& damagedSpringNumbers) {
    // Simply check the remainder of the string.
    std::string newSpringString = springString.substr(1);
    return getNumberOfPossibleSpringStrings(newSpringString, damagedSpringNumbers);
}

int processDamagedSpring(const std::string springString, const std::vector<int>& damagedSpringNumbers) {
    // Get the group of springs that should satisfy the next number of damaged
    // springs.
    std::string group = springString.substr(0, damagedSpringNumbers[0]);

    // If the group is too short to satisfy the number of damaged springs,
    // return 0.
    if (group.length() < damagedSpringNumbers[0]) {
        return 0;
    }

    // If the group contains any operational springs, return 0 as it does not
    // satisfy the number of damaged springs.
    for (char spring : group) {
        if (spring == '.') {
            return 0;
        }
    }

    // If the group is the entirety of the spring string, return 1 if there are
    // no other damaged spring numbers, or 0 otherwise.
    if (group.length() == springString.length()) {
        return damagedSpringNumbers.size() - 1 == 0 ? 1 : 0;
    }
    // Otherwise, check the spring following the group. If it is damaged, return
    // 0 as the group is too big.
    else if (springString[damagedSpringNumbers[0]] == '#') {
        return 0;
    }

    // There is a sufficient number of damaged and unknown springs to satisfy
    // the next number of damaged springs, and there is an operational spring to
    // separate the group from the remaining springs. Check the remaining
    // springs against the remaining damaged spring numbers.
    std::string newSpringString = springString.substr(damagedSpringNumbers[0] + 1); // + 1 to move past the operational spring following the group
    std::vector<int> newDamagedSpringNumbers(damagedSpringNumbers.begin() + 1, damagedSpringNumbers.end());
    return getNumberOfPossibleSpringStrings(newSpringString, newDamagedSpringNumbers);
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
