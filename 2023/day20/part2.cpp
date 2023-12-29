//////////////////////////////////////////
// Advent of Code 2023 Day 20 Part 2    //
// https://adventofcode.com/2023/day/20 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <regex>
#include <sstream>
#include <queue>
#include <cmath>

enum PulseStrength { NONE, LOW, HIGH };

class Pulse {
private:
    std::string senderModuleName;
    PulseStrength pulseStrength;
    std::string destinationModuleName;

public:
    Pulse (std::string senderModuleName, PulseStrength pulseStrength, std::string destinationModuleName)
    : senderModuleName(senderModuleName), pulseStrength(pulseStrength), destinationModuleName(destinationModuleName)
    {}

    friend std::ostream& operator<<(std::ostream& os, const Pulse& pulse) {
        os << pulse.getSenderModuleName() << " -" << pulse.getPulseStrength() << "-> " << pulse.getDestinationModuleName();
        return os;
    }

    std::string getSenderModuleName() const {
        return senderModuleName;
    }

    PulseStrength getPulseStrength() const {
        return pulseStrength;
    }

    std::string getDestinationModuleName() const {
        return destinationModuleName;
    }
};

class Module {
private:
    std::vector<std::string> destinationModuleNames;

public:
    Module(std::vector<std::string> destinationModuleNames) : destinationModuleNames(destinationModuleNames) {}

    virtual PulseStrength processPulse(Pulse pulse) {
        return LOW;
    }

    virtual std::string getType() const {
        return "";
    }

    std::vector<std::string> getDestinationModuleNames() const {
        return destinationModuleNames;
    }
};

class FlipFlopModule : public Module {
private:
    bool on;

public:
    FlipFlopModule(std::vector<std::string> destinationModuleNames) : Module(destinationModuleNames), on(false) {}

    PulseStrength processPulse(Pulse pulse) {
        if (pulse.getPulseStrength() == HIGH) {
            return NONE;
        }
        else if (pulse.getPulseStrength() == LOW && !on) {
            on = true;
            return HIGH;
        }
        else if (pulse.getPulseStrength() == LOW && on) {
            on = false;
            return LOW;
        }
    }

    std::string getType() const {
        return "%";
    }
};

class ConjunctionModule : public Module {
private:
    std::map<std::string, PulseStrength> memory;

public:
    ConjunctionModule(std::vector<std::string> destinationModuleNames, std::vector<std::string> inputModuleNames) : Module(destinationModuleNames) {
        for (std::string moduleName : inputModuleNames) {
            memory[moduleName] = LOW;
        }
    }

    PulseStrength processPulse(Pulse pulse) {
        memory[pulse.getSenderModuleName()] = pulse.getPulseStrength();
        bool allHighPulses = true;
        for (const std::pair<std::string, PulseStrength>& pair : memory) {
            if (pair.second == LOW) {
                allHighPulses = false;
                break;
            }
        }
        return allHighPulses ? LOW : HIGH;
    }

    std::string getType() const {
        return "&";
    }
};

long long solution(const std::string fileName);
long long getLCM(std::vector<long long> numbers);
std::vector<int> getPrimeFactors(long long number);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

long long solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);
    std::regex pattern("([%&])?([a-z]+)\\s->\\s([a-z]+(?:,\\s[a-z]+)*)");
    std::smatch match;

    // Find which modules output to which modules.
    std::map<std::string, std::vector<std::string>> inputModuleNames;
    for (std::string line : input) {
        std::regex_match(line, match, pattern);

        std::string moduleName = match[2];
        std::istringstream iss(match[3]);
        std::string destinationModuleName;
        while (std::getline(iss, destinationModuleName, ',')) {
            if (destinationModuleName[0] == ' ') {
                destinationModuleName = destinationModuleName.substr(1);
            }
            inputModuleNames[destinationModuleName].push_back(moduleName);
        }
    }

    // Create the modules.
    std::map<std::string, std::unique_ptr<Module>> modules;
    for (std::string line : input) {
        std::regex_match(line, match, pattern);

        std::string moduleType = match[1];
        std::string moduleName = match[2];
        std::vector<std::string> destinationModuleNames;
        std::istringstream iss(match[3]);
        std::string destinationModuleName;
        while (std::getline(iss, destinationModuleName, ',')) {
            if (destinationModuleName[0] == ' ') {
                destinationModuleName = destinationModuleName.substr(1);
            }
            destinationModuleNames.push_back(destinationModuleName);
        }

        if (moduleType == "%") {
            modules[moduleName] = std::make_unique<FlipFlopModule>(FlipFlopModule(destinationModuleNames));
        }
        else if (moduleType == "&") {
            modules[moduleName] = std::make_unique<ConjunctionModule>(ConjunctionModule(destinationModuleNames, inputModuleNames[moduleName]));
        }
        else {
            modules[moduleName] = std::make_unique<Module>(Module(destinationModuleNames));
        }
    }

    // Going back from "rx", find all layers that are purely ConjunctionModules.
    std::vector<std::vector<std::string>> layers;
    int layerCounter = 0;
    std::vector<std::string> layer;
    layer.push_back("rx");
    layers.push_back(layer);
    bool validLayer = true;
    while (validLayer) {
        layer.clear();
        for (std::string currentModuleName : layers[layerCounter]) {
            for (std::string previousModuleName : inputModuleNames[currentModuleName]) {
                if (modules[previousModuleName]->getType() != "&") {
                    validLayer = false;
                    goto endOfForLoops;
                }
                else {
                    layer.push_back(previousModuleName);
                }
            }
        } endOfForLoops:

        if (validLayer) {
            layers.push_back(layer);
            layerCounter++;
        }
    }
    layers.erase(layers.begin()); // remove the layer with "rx"

    // Determine what pulse strength is needed for the earliest layer found.
    // Note that this layer is the last layer in layers.
    PulseStrength desiredPulseStrength = ((layers.size() - 1) % 2 == 0) ? LOW : HIGH;

    // Keep track of when the desired pulse strength is first emitted by each
    // module.
    std::map<std::string, int> firstOccurrenceOfDesiredPulseStrength;

    // Keep pressing the button until the desired pulse strength has been seen
    // emitted by all modules in the earliest layer.
    bool keepPressing = true;
    int numberOfButtonPresses = 0;
    std::queue<Pulse> queue;
    while (keepPressing) {
        numberOfButtonPresses++;
        queue.emplace("button", LOW, "broadcaster");
        while(!queue.empty()) {
            Pulse currentPulse = queue.front();
            queue.pop();

            // Update first occurrence of desired pulse strength.
            if (currentPulse.getPulseStrength() == desiredPulseStrength && firstOccurrenceOfDesiredPulseStrength[currentPulse.getSenderModuleName()] == 0) {
                firstOccurrenceOfDesiredPulseStrength[currentPulse.getSenderModuleName()] = numberOfButtonPresses;
            }

            // Check if the desired pulse strength has been seen emitted for all
            // modules in the earliest layer.
            bool allPulsesSeen = true;
            for (std::string moduleName : layers.back()) {
                if (firstOccurrenceOfDesiredPulseStrength[moduleName] == 0) {
                    allPulsesSeen = false;
                    break;
                }
            }
            if (allPulsesSeen) {
                keepPressing = false;
                break;
            }

            // Process the pulse.
            if (modules.find(currentPulse.getDestinationModuleName()) != modules.end()) {
                PulseStrength newPulseStrength = modules[currentPulse.getDestinationModuleName()]->processPulse(currentPulse);
                if (newPulseStrength != NONE) {
                    for (std::string newDestinationModuleName : modules[currentPulse.getDestinationModuleName()]->getDestinationModuleNames()) {
                        queue.emplace(currentPulse.getDestinationModuleName(), newPulseStrength, newDestinationModuleName);
                    }
                }
            }
        }
    }

    // Find the LCM of the first occurrence of desired pulse strength numbers
    // for the earliest layer.
    std::vector<long long> numbers;
    for (std::string moduleName : layers.back()) {
        numbers.push_back(firstOccurrenceOfDesiredPulseStrength[moduleName]);
    }
    long long lcm = getLCM(numbers);
    return lcm;
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
