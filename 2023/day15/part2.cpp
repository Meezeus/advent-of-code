//////////////////////////////////////////
// Advent of Code 2023 Day 15 Part 2    //
// https://adventofcode.com/2023/day/15 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include <algorithm>

template <class K, class V>
class HashMap {
private:
    int multiplier = 17;
    int capacity = 256;
    std::vector<std::list<std::pair<K, V>>> buckets;

public:
    HashMap() : buckets(capacity) {};

    V& operator[](const K key) {
        std::list<std::pair<K, V>>& bucket = buckets[hash(key)];
        typename std::list<std::pair<K, V>>::iterator iterator = std::find_if(bucket.begin(), bucket.end(), [key](const std::pair<K, V>& pair) {return pair.first == key;});
        if (iterator != bucket.end()) {
            return iterator->second;
        }
        else {
            bucket.emplace_back(key, V{});
            return bucket.back().second;
        }
    }

    const V& get(const K key) const {
        const std::list<std::pair<K, V>>& bucket = buckets[hash(key)];
        typename std::list<std::pair<K, V>>::const_iterator iterator = std::find_if(bucket.begin(), bucket.end(), [key](const std::pair<K, V>& pair) {return pair.first == key;});
        if (iterator != bucket.end()) {
            return iterator->second;
        }
    }

    void insert(const K key, const V value) {
        std::list<std::pair<K, V>>& bucket = buckets[hash(key)];
        typename std::list<std::pair<K, V>>::iterator iterator = std::find_if(bucket.begin(), bucket.end(), [key](const std::pair<K, V>& pair) {return pair.first == key;});
        if (iterator != bucket.end()) {
            iterator->second = value;
        }
        else {
            bucket.emplace_back(key, value);
        }
    }

    void remove(const K key) {
        std::list<std::pair<K, V>>& bucket = buckets[hash(key)];
        typename std::list<std::pair<K, V>>::iterator iterator = std::find_if(bucket.begin(), bucket.end(), [key](const std::pair<K, V>& pair) {return pair.first == key;});
        if (iterator != bucket.end()) {
            bucket.erase(iterator);
        }
    }

    const std::vector<std::list<std::pair<K, V>>>& getBuckets() const {
        return buckets;
    }

private:
    int hash(const std::string str) const {
        int value = 0;
        for (char c : str) {
            value += c;
            value *= multiplier;
            value %= capacity;
        }
        return value;
    }
};

int solution(const std::string fileName);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 145)" << std::endl;
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

    HashMap<std::string, int> hashmap;
    for (const std::string instruction : instructions) {
        std::string label;
        int focalLength;
        if (instruction.back() == '-') {
            label = instruction.substr(0, instruction.length() - 1);
            hashmap.remove(label);
        }
        else {
            label = instruction.substr(0, instruction.length() - 2);
            focalLength = instruction.back() - '0';
            // Either one of the two lines below should work.
            //hashmap[label] = focalLength;
            hashmap.insert(label, focalLength);
        }
    }

    int total = 0;
    const std::vector<std::list<std::pair<std::string, int>>>& buckets = hashmap.getBuckets();
    for (int bucketIndex = 0; bucketIndex < buckets.size(); ++bucketIndex) {
        const std::list<std::pair<std::string, int>>& bucket = buckets[bucketIndex];
        int lensNumber = 1;
        for (std::list<std::pair<std::string, int>>::const_iterator iterator = bucket.begin(); iterator != bucket.end(); ++iterator) {
            total += (bucketIndex + 1) * lensNumber * iterator->second;
            ++lensNumber;
        }
    }
    return total;
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
