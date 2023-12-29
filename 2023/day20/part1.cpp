//////////////////////////////////////////
// Advent of Code 2023 Day 20 Part 1    //
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
};

int solution(const std::string fileName);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example 1 Solution: " << solution("example_1.txt") << " (expected 32000000)" << std::endl;
    std::cout << "Example 2 Solution: " << solution("example_2.txt") << " (expected 11687500)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
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

    int lowPulseCount = 0;
    int highPulseCount = 0;
    std::queue<Pulse> queue;
    for (int i = 0; i < 1000; ++i) {
        queue.emplace("button", LOW, "broadcaster");
        while(!queue.empty()) {
            Pulse currentPulse = queue.front();
            queue.pop();

            if (currentPulse.getPulseStrength() == LOW) {
                lowPulseCount++;
            }
            else if (currentPulse.getPulseStrength() == HIGH) {
                highPulseCount++;
            }

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

    return lowPulseCount * highPulseCount;
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
