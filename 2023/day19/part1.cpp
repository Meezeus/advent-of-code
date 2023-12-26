//////////////////////////////////////////
// Advent of Code 2023 Day 19 Part 1    //
// https://adventofcode.com/2023/day/19 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <regex>

class Part {
private:
    int x;
    int m;
    int a;
    int s;

public:
    Part(int x, int m, int a, int s) : x(x), m(m), a(a), s(s) {}

    friend std::ostream& operator<<(std::ostream& os, const Part& part) {
        os << "x=" << part.x
        << ",m=" << part.m
        << ",a=" << part.a
        << ",s=" << part.s;
        return os;
    }

    int getX() {
        return x;
    }

    int getM() {
        return m;
    }

    int getA() {
        return a;
    }

    int getS() {
        return s;
    }
};

enum Category { X, M, A, S };

char categoryToCategoryChar(int category) {
    switch (category) {
    case X:
        return 'x';
    case M:
        return 'm';
    case A:
        return 'a';
    case S:
        return 's';
    }
}

int categoryCharToCategory(char categoryChar) {
    switch (categoryChar) {
    case 'x':
        return X;
    case 'm':
        return M;
    case 'a':
        return A;
    case 's':
        return S;
    }
}

enum Comparison { LESS_THAN, GREATER_THAN };

char comparisonToComparisonChar(int comparison) {
    switch (comparison) {
    case LESS_THAN:
        return '<';
    case GREATER_THAN:
        return '>';
    }
}

int comparisonCharToComparison(char comparisonChar) {
    switch (comparisonChar) {
    case '<':
        return LESS_THAN;
    case '>':
        return GREATER_THAN;
    }
}

class Rule {
private:
    int category;
    int comparison;
    int value;
    std::string nextWorkflowName;

public:
    Rule(int category, int comparison, int value, std::string nextWorkflowName)
    : category(category), comparison(comparison), value(value), nextWorkflowName(nextWorkflowName) {}

    Rule(std::string nextWorkflowName)
    : category(-1), comparison(-1), value(-1), nextWorkflowName(nextWorkflowName) {}

    std::string check(Part part) {
        int rating;
        switch (category) {
        case X:
            rating = part.getX();
            break;
        case M:
            rating = part.getM();
            break;
        case A:
            rating = part.getA();
            break;
        case S:
            rating = part.getS();
            break;
        }

        if (comparison == LESS_THAN) {
            if (rating < value) {
                return nextWorkflowName;
            }
            else {
                return "";
            }
        }
        else if (comparison == GREATER_THAN) {
            if (rating > value) {
                return nextWorkflowName;
            }
            else {
                return "";
            }
        }
        else {
            return nextWorkflowName;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Rule& rule) {
        if (rule.category != -1 && rule.comparison != -1 && rule.value != -1) {
            os << categoryToCategoryChar(rule.category) << comparisonToComparisonChar(rule.comparison) << rule.value << ":" << rule.nextWorkflowName;
        }
        else {
            os << rule.nextWorkflowName;
        }
        return os;
    }
};

int solution(const std::string fileName);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 19114)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::map<std::string, std::vector<Rule>> workflows;
    std::vector<Part> parts;

    std::regex rulePattern("[xmas][<>](\\d+):([a-zAR]+)");
    std::regex digitPattern("\\d+");
    bool newlineSeen = false;
    for (std::string line : input) {
        if (line.empty()) {
            newlineSeen = true;
            continue;
        }

        if (!newlineSeen) {
            int openBracePos = line.find('{');
            int closeBracePos = line.find('}');
            std::string workflowName = line.substr(0, openBracePos);
            std::string workflowString = line.substr(openBracePos + 1, closeBracePos - openBracePos - 1);

            std::vector<Rule> workflow;
            std::sregex_iterator ruleIterator(line.begin(), line.end(), rulePattern);
            std::sregex_iterator end;
            while (ruleIterator != end) {
                std::string ruleString = (*ruleIterator)[0];

                int category = categoryCharToCategory(ruleString[0]);
                int comparison = comparisonCharToComparison(ruleString[1]);
                int value = std::stoi((*ruleIterator)[1]);
                std::string nextWorkflowName = (*ruleIterator)[2];
                workflow.emplace_back(category, comparison, value, nextWorkflowName);

                workflowString = ruleIterator->suffix();
                ++ruleIterator;
            }
            if (workflowString[0] == ',') {
                workflowString = workflowString.substr(1);
            }
            workflowString = workflowString.substr(0, workflowString.length() - 1);
            workflow.emplace_back(workflowString);

            workflows[workflowName] = workflow;
        }
        else {
            std::sregex_iterator it(line.begin(), line.end(), digitPattern);
            int x = std::stoi((*it)[0]);
            ++it;
            int m = std::stoi((*it)[0]);
            ++it;
            int a = std::stoi((*it)[0]);
            ++it;
            int s = std::stoi((*it)[0]);
            parts.emplace_back(x, m, a, s);
        }
    }

    int sum = 0;
    for (Part part : parts) {
        std::string currentWorkflowName = "in";
        while (currentWorkflowName != "A" && currentWorkflowName != "R") {
            std::vector<Rule> currentWorkflow = workflows[currentWorkflowName];
            for (Rule rule : currentWorkflow) {
                std::string nextWorkflowName = rule.check(part);
                if (nextWorkflowName != "") {
                    currentWorkflowName = nextWorkflowName;
                    break;
                }
            }
        }

        if (currentWorkflowName == "A") {
            sum += part.getX() + part.getM() + part.getA() + part.getS();
        }
    }

    return sum;
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
