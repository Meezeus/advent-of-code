/////////////////////////////////////////
// Advent of Code 2023 Day 5 Part 2    //
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
        std::sort(rangeMappings.begin(), rangeMappings.end());
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

    long long findSourceOverlapPoint(long long rangeStart, long long rangeLength) {
        long long rangeEnd = rangeStart + rangeLength - 1;
        long long overlapPoint = rangeEnd;
        for (std::tuple<long long, long long, long long> rangeMapping : rangeMappings) {
            long long mappingSourceStart = std::get<0>(rangeMapping);
            long long mappingRangeLength = std::get<2>(rangeMapping);
            long long mappingSourceEnd = mappingSourceStart + mappingRangeLength - 1;
            // Check if range overlaps the current mapping range.
            if (rangeStart >= mappingSourceStart && rangeStart <= mappingSourceEnd) {
                // Head overlap found, set overlap point to mapping source end
                // or range end depending on whats smaller.
                overlapPoint = std::min(mappingSourceEnd, rangeEnd);
                break;
            }
            else if (rangeStart < mappingSourceStart && rangeEnd >= mappingSourceStart) {
                // Tail overlap found, set overlap point to mapping range start - 1.
                overlapPoint = mappingSourceStart - 1;
                break;
            }
        }
        return overlapPoint;
    }
};

long long solution(std::string fileName);
std::vector<std::pair<long long, long long>> getSeedRanges(std::string line);
RangeMap getMap(std::vector<std::string>* inputPtr, std::string mapName);
std::vector<std::pair<long long, long long>> transformSourceRanges(std::vector<std::pair<long long, long long>> sourceRanges, RangeMap rangeMap);
std::vector<std::string> readLines(std::string fileName);

std::regex digitPattern("\\d+");

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 46)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

long long solution(std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::vector<std::pair<long long, long long>> seedRanges = getSeedRanges(input[0]);
    std::vector<RangeMap> rangeMaps = {};
    rangeMaps.push_back(getMap(&input, "seed-to-soil map:"));
    rangeMaps.push_back(getMap(&input, "soil-to-fertilizer map:"));
    rangeMaps.push_back(getMap(&input, "fertilizer-to-water map:"));
    rangeMaps.push_back(getMap(&input, "water-to-light map:"));
    rangeMaps.push_back(getMap(&input, "light-to-temperature map:"));
    rangeMaps.push_back(getMap(&input, "temperature-to-humidity map:"));
    rangeMaps.push_back(getMap(&input, "humidity-to-location map:"));

    std::vector<std::pair<long long, long long>> destinationRanges = seedRanges;
    for (RangeMap rangeMap : rangeMaps) {
        destinationRanges = transformSourceRanges(destinationRanges, rangeMap);
    }

    std::vector<long long> locations = {};
    for (std::pair<long long, long long> locationRange : destinationRanges) {
        locations.push_back(locationRange.first);
    }

    return *(std::min_element(locations.begin(), locations.end()));
}

std::vector<std::pair<long long, long long>> getSeedRanges(std::string line) {
    std::vector<std::pair<long long, long long>> seedRanges = {};
    std::sregex_iterator iterator(line.begin(), line.end(), digitPattern);
    std::sregex_iterator end;
    while (iterator != end) {
        long long rangeStart = std::stoll((*iterator)[0]);
        ++iterator;
        long long rangeLength = std::stoll((*iterator)[0]);
        ++iterator;
        seedRanges.push_back(std::pair<long long, long long>(rangeStart, rangeLength));
    }
    return seedRanges;
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

std::vector<std::pair<long long, long long>> transformSourceRanges(std::vector<std::pair<long long, long long>> sourceRanges, RangeMap rangeMap) {
    std::vector<std::pair<long long, long long>> destinationRanges = {};
    int sourceRangesIndex = 0;
    std::pair<long long, long long> sourceRange = sourceRanges[sourceRangesIndex];
    bool finished = false;
    while (!finished) {
        long long sourceStart = sourceRange.first;
        long long rangeLength = sourceRange.second;
        long long sourceEnd = sourceStart + rangeLength - 1;

        long long newSourceEnd = rangeMap.findSourceOverlapPoint(sourceStart, rangeLength);
        long long newRangeLength = newSourceEnd - sourceStart + 1;

        long long destinationStart = rangeMap.getValue(sourceStart);
        destinationRanges.push_back(std::pair<long long, long long>(destinationStart, newRangeLength));

        // If the new source end is equal to the original source end, move to the next source range.
        if (newSourceEnd == sourceEnd) {
            ++sourceRangesIndex;
            if (sourceRangesIndex == sourceRanges.size()) {
                finished = true;
            }
            else {
                sourceRange = sourceRanges[sourceRangesIndex];
            }
        }
        // Otherwise, there is a bit of the range left over. Make that the next source range.
        else {
            sourceRange = std::pair<long long, long long>(newSourceEnd + 1, rangeLength - newRangeLength);
        }
    }
    return destinationRanges;
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
