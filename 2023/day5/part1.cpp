/////////////////////////////////////////
// Advent of Code 2023 Day 5 Part 1    //
// https://adventofcode.com/2023/day/5 //
/////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <tuple>

class RangeMap {
private:
    std::vector<std::tuple<long long, long long, long long>> rangeMappings = {};

public:
    void addRangeMapping(long long sourceStart, long long destinationStart, long long rangeLength) {
        rangeMappings.push_back(std::tuple<long long, long long, long long>(sourceStart, destinationStart, rangeLength));
    }

    long long getValue(long long key) {
        long long value = key;
        for (std::tuple<long long, long long, long long> rangeMapping : rangeMappings) {
            long long sourceStart = std::get<0>(rangeMapping);
            long long destinationStart = std::get<1>(rangeMapping);
            long long rangeLength = std::get<2>(rangeMapping);
            if (key >= sourceStart && key < sourceStart + rangeLength) {
                long long offset = key - sourceStart;
                value = destinationStart + offset;
                break;
            }
        }
        return value;
    }
};

long long solution(std::string fileName);
std::vector<long long> getSeeds(std::string line);
RangeMap getMap(std::vector<std::string>* inputPtr, std::string mapName);
std::vector<std::string> readLines(std::string fileName);

std::regex digitPattern("\\d+");

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 35)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

long long solution(std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::vector<long long> seeds = getSeeds(input[0]);
    std::vector<RangeMap> rangeMaps = {};
    rangeMaps.push_back(getMap(&input, "seed-to-soil map:"));
    rangeMaps.push_back(getMap(&input, "soil-to-fertilizer map:"));
    rangeMaps.push_back(getMap(&input, "fertilizer-to-water map:"));
    rangeMaps.push_back(getMap(&input, "water-to-light map:"));
    rangeMaps.push_back(getMap(&input, "light-to-temperature map:"));
    rangeMaps.push_back(getMap(&input, "temperature-to-humidity map:"));
    rangeMaps.push_back(getMap(&input, "humidity-to-location map:"));

    std::vector<long long> locations = {};
    for (long long seed : seeds) {
        long long destination = seed;
        for (RangeMap rangeMap : rangeMaps) {
            destination = rangeMap.getValue(destination);
        }
        locations.push_back(destination);
    }

    return *(std::min_element(locations.begin(), locations.end()));
}

std::vector<long long> getSeeds(std::string line) {
    std::vector<long long> seeds = {};
    std::sregex_iterator iterator(line.begin(), line.end(), digitPattern);
    std::sregex_iterator end;
    for (; iterator != end; ++iterator) {
        seeds.push_back(std::stoll((*iterator)[0]));
    }
    return seeds;
}

RangeMap getMap(std::vector<std::string>* inputPtr, std::string mapName) {
    RangeMap rangeMap;
    bool searching = true;
    for (std::string line : *inputPtr) {
        if (searching && line != mapName) {
            continue;
        }
        else if (searching && line == mapName) {
            searching = false;
        }
        else if (!searching && !line.empty()) {
            std::sregex_iterator iterator(line.begin(), line.end(), digitPattern);
            long long destinationStart = std::stoll((*iterator)[0]);
            ++iterator;
            long long sourceStart = std::stoll((*iterator)[0]);
            ++iterator;
            long long rangeLength = std::stoll((*iterator)[0]);
            rangeMap.addRangeMapping(sourceStart, destinationStart, rangeLength);
        }
        else if (!searching && line.empty()) {
            break;
        }
    }
    return rangeMap;
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
