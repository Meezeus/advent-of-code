/////////////////////////////////////////
// Advent of Code 2023 Day 8 Part 2    //
// https://adventofcode.com/2023/day/8 //
/////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <regex>
#include <cmath>

long long solution(const std::string fileName);
std::vector<std::string> getStartingLocations(const std::vector<std::string>& input);
std::map<std::string, std::pair<std::string, std::string>> buildNetworkMap(const std::vector<std::string>& input);
long long getSteps(const std::string instructions, const std::map<std::string, std::pair<std::string, std::string>>& networkMap, const std::string location);
long long getLCM(std::vector<long long> numbers);
std::vector<int> getPrimeFactors(long long number);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("part2_example.txt") << " (expected 6)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

long long solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::string instructions = input[0];
    std::vector<std::string> startingLocations = getStartingLocations(input);
    std::map<std::string, std::pair<std::string, std::string>> networkMap = buildNetworkMap(input);

    std::vector<long long> stepCounts = {};
    for (std::string location : startingLocations) {
        stepCounts.push_back(getSteps(instructions, networkMap, location));
    }

    return getLCM(stepCounts);
}

std::vector<std::string> getStartingLocations(const std::vector<std::string>& input) {
    std::vector<std::string> locations = {};
    std::regex startingLocationPattern("[A-Z0-9]{2}A");
    for (int index = 2; index < input.size(); ++index) {
        std::string location = input[index].substr(0, 3);
        if (std::regex_match(location, startingLocationPattern)) {
            locations.push_back(location);
        }
    }
    return locations;
}

std::map<std::string, std::pair<std::string, std::string>> buildNetworkMap(const std::vector<std::string>& input) {
    std::map<std::string, std::pair<std::string, std::string>> networkMap = {};
    std::regex locationPattern("[A-Z0-9]{3}");
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

long long getSteps(const std::string instructions, const std::map<std::string, std::pair<std::string, std::string>>& networkMap, const std::string location) {
    long long stepCounter = 0;
    int instructionIndex = 0;
    std::string currentLocation = location;
    std::regex finalLocationPattern("[A-Z0-9]{2}Z");
    while (!std::regex_match(currentLocation, finalLocationPattern)) {
        std::pair<std::string, std::string> nodes = networkMap.at(currentLocation);
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

long long getLCM(std::vector<long long> numbers) {
    std::map<int, int> maxPrimeFactorCounts = {};

    for (long long number : numbers) {
        std::vector<int> primeFactors = getPrimeFactors(number);

        std::map<int, int> primeFactorCounts = {};
        for (int primeFactor : primeFactors) {
            primeFactorCounts[primeFactor]++;
        }

        for (std::pair<int, int> factorAndCount : primeFactorCounts) {
            maxPrimeFactorCounts[factorAndCount.first] = std::max(maxPrimeFactorCounts[factorAndCount.first], factorAndCount.second);
        }
    }

    long long lcm = 1;
    for (std::pair<int, int> factorAndMaxCount : maxPrimeFactorCounts) {
        lcm *= std::pow(factorAndMaxCount.first, factorAndMaxCount.second);
    }
    return lcm;
}

std::vector<int> getPrimeFactors(long long number) {
    std::vector<int> primeFactors = {};

    while (number % 2 == 0) {
        primeFactors.push_back(2);
        number /= 2;
    }

    for (long long i = 3; i <= std::sqrt(number); i += 2) {
        while (number % i == 0) {
            primeFactors.push_back(i);
            number /= i;
        }
    }

    if (number > 2) {
        primeFactors.push_back(number);
    }

    return primeFactors;
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
