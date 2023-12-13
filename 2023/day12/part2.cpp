//////////////////////////////////////////
// Advent of Code 2023 Day 12 Part 2    //
// https://adventofcode.com/2023/day/12 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <unordered_map>

long long solution(const std::string fileName);
long long getNumberOfPossibleSpringStrings(const std::string springString, const std::vector<int>& damagedSpringNumbers, std::unordered_map<std::string, long long>& memo);
long long processWorkingSpring(const std::string springString, const std::vector<int>& damagedSpringNumbers, std::unordered_map<std::string, long long>& memo);
long long processDamagedSpring(const std::string springString, const std::vector<int>& damagedSpringNumbers, std::unordered_map<std::string, long long>& memo);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 525152)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

long long solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);
    std::regex digitPattern("\\d+");

    long long total = 0;
    for (std::string line : input) {
        std::string foldedSpringString = line.substr(0, line.find(" "));

        std::string unfoldedSpringString = foldedSpringString;
        for (int i = 0; i < 4; ++i) {
            unfoldedSpringString += "?";
            unfoldedSpringString += foldedSpringString;
        }

        std::vector<int> foldedDamagedSpringNumbers;
        std::sregex_iterator iterator(line.begin(), line.end(), digitPattern);
        std::sregex_iterator end;
        while (iterator != end) {
            foldedDamagedSpringNumbers.push_back(std::stoi((*iterator)[0]));
            ++iterator;
        }

        std::vector<int> unfoldedDamagedSpringNumbers(foldedDamagedSpringNumbers);
        for (int i = 0; i < 4; ++i) {
            unfoldedDamagedSpringNumbers.insert(unfoldedDamagedSpringNumbers.end(), foldedDamagedSpringNumbers.begin(), foldedDamagedSpringNumbers.end());
        }

        std::unordered_map<std::string, long long> memo;
        long long count = getNumberOfPossibleSpringStrings(unfoldedSpringString, unfoldedDamagedSpringNumbers, memo);
        total += count;
    }
    return total;
}

long long getNumberOfPossibleSpringStrings(const std::string springString, const std::vector<int>& damagedSpringNumbers, std::unordered_map<std::string, long long>& memo) {
    // Combine the spring string and the damaged spring numbers into a single string.
    std::string conditionRecordString = springString;
    conditionRecordString += " ";
    for (int damagedSpringNumber : damagedSpringNumbers) {
        conditionRecordString += std::to_string(damagedSpringNumber);
        conditionRecordString += ",";
    }
    conditionRecordString.erase(conditionRecordString.end() - 1);

    // If the result has already been computed and stored in memo, return it.
    std::unordered_map<std::string, long long>::iterator memoIterator = memo.find(conditionRecordString);
    if (memoIterator != memo.end()) {
        return memoIterator->second;
    }

    // If the spring string is empty, return 1 if there are no more damaged
    // spring numbers left, or 0 otherwise.
    if (springString.length() == 0) {
        int number = damagedSpringNumbers.empty() ? 1 : 0;
        memo[conditionRecordString] = number;
        return number;
    }

    // If there are no more damaged spring numbers left, return 1 only if the
    // spring string does not contain any damaged springs, or 0 otherwise.
    if (damagedSpringNumbers.empty()) {
        for (char spring : springString) {
            if (spring == '#') {
                memo[conditionRecordString] = 0;
                return 0;
            }
        }
        memo[conditionRecordString] = 1;
        return 1;
    }

    // The spring string is not empty and there are damaged spring numbers left.
    // Process the next spring.
    long long numberOfPossibleSpringStrings;
    if (springString[0] == '.') {
        numberOfPossibleSpringStrings = processWorkingSpring(springString, damagedSpringNumbers, memo);
    }
    else if (springString[0] == '#') {
        numberOfPossibleSpringStrings = processDamagedSpring(springString, damagedSpringNumbers, memo);
    }
    else if (springString[0] == '?') {
        numberOfPossibleSpringStrings = processWorkingSpring(springString, damagedSpringNumbers, memo) + processDamagedSpring(springString, damagedSpringNumbers, memo);
    }

    memo[conditionRecordString] = numberOfPossibleSpringStrings;
    return numberOfPossibleSpringStrings;
}

long long processWorkingSpring(const std::string springString, const std::vector<int>& damagedSpringNumbers, std::unordered_map<std::string, long long>& memo) {
    // Check the remainder of the string.
    std::string newSpringString = springString.substr(1);
    return getNumberOfPossibleSpringStrings(newSpringString, damagedSpringNumbers, memo);
}

long long processDamagedSpring(const std::string springString, const std::vector<int>& damagedSpringNumbers, std::unordered_map<std::string, long long>& memo) {
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
    return getNumberOfPossibleSpringStrings(newSpringString, newDamagedSpringNumbers, memo);}

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
