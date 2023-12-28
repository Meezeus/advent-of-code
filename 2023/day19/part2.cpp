//////////////////////////////////////////
// Advent of Code 2023 Day 19 Part 2    //
// https://adventofcode.com/2023/day/19 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <regex>

enum Category { X, M, A, S };

std::string categoryToCategoryString(int category) {
    switch (category) {
    case X:
        return "x";
    case M:
        return "m";
    case A:
        return "a";
    case S:
        return "s";
    }
}

int categoryStringToCategory(std::string categoryString) {
    if (categoryString == "x") {
        return X;
    }
    else if (categoryString == "m") {
        return M;
    }
    else if (categoryString == "a") {
        return A;
    }
    else if (categoryString == "s") {
        return S;
    }
}

// The order of these is important and should not change!
enum Comparison { LESS_THAN, LESS_THAN_EQUAL_TO, GREATER_THAN_EQUAL_TO, GREATER_THAN };

std::string comparisonToComparisonString(int comparison) {
    switch (comparison) {
    case LESS_THAN:
        return "<";
    case LESS_THAN_EQUAL_TO:
        return "<=";
    case GREATER_THAN_EQUAL_TO:
        return ">=";
    case GREATER_THAN:
        return ">";
    }
}

int comparisonStringToComparison(std::string comparisonString) {
    if (comparisonString == "<") {
        return LESS_THAN;
    }
    else if (comparisonString == "<=") {
        return LESS_THAN_EQUAL_TO;
    }
    else if (comparisonString == ">=") {
        return GREATER_THAN_EQUAL_TO;
    }
    else if (comparisonString == ">") {
        return GREATER_THAN;
    }
}

class Condition {
private:
    int category;
    int comparison;
    int value;

public:
    Condition (int category, int comparison, int value)
    : category(category), comparison(comparison), value(value)
    {}

    Condition getInvertedCondition() const {
        return Condition(category, (comparison + 2) % 4, value);
    }

    friend std::ostream& operator<<(std::ostream& os, const Condition& condition) {
        os << categoryToCategoryString(condition.category) << comparisonToComparisonString(condition.comparison) << std::to_string(condition.value);
        return os;
    }

    int getCategory() const {
        return category;
    }

    int getComparison() const {
        return comparison;
    }

    int getValue() const {
        return value;
    }
};

class Rule {
private:
    Condition condition;
    std::string nextWorkflowName;

public:
    Rule(Condition condition, std::string nextWorkflowName)
    : condition(condition), nextWorkflowName(nextWorkflowName)
    {}

    Rule(int category, int comparison, int value, std::string nextWorkflowName)
    : condition(category, comparison, value), nextWorkflowName(nextWorkflowName)
    {}

    friend std::ostream& operator<<(std::ostream& os, const Rule& rule) {
        os << rule.condition << ":" << rule.nextWorkflowName;
        return os;
    }

    Condition getCondition() const {
        return condition;
    }

    std::string getNextWorkflowName() const {
        return nextWorkflowName;
    }
};

class Workflow {
private:
    std::vector<Rule> rules;
    std::string fallbackWorkflowName;

public:
    Workflow(std::vector<Rule> rules, std::string fallbackWorkflowName)
    : rules(rules), fallbackWorkflowName(fallbackWorkflowName)
    {}

    Workflow()
    : fallbackWorkflowName("")
    {}

    friend std::ostream& operator<<(std::ostream& os, const Workflow& workflow) {
        os << "{";
        for (Rule rule : workflow.rules) {
            os << rule << ",";
        }
        os << workflow.fallbackWorkflowName << "}";
        return os;
    }

    std::vector<Rule> getRules() const {
        return rules;
    }

    std::string getFallbackWorkflowName() const {
        return fallbackWorkflowName;
    }
};

long long solution(const std::string fileName);
void constructAcceptablePaths(const std::map<std::string, Workflow>& workflows, std::vector<std::vector<Condition>>& paths, std::vector<Condition> currentPath, const std::string& currentWorkflowName);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 167409079868000)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

long long solution(const std::string fileName) {
    // Read the input.
    std::vector<std::string> input = readLines(fileName);

    // Create the workflows from the input.
    std::map<std::string, Workflow> workflows;
    std::regex rulePattern("([xmas])(<|>|<=|>=)(\\d+):([a-zAR]+)");
    for (std::string line : input) {
        if (line.empty()) {
            break;
        }

        int openBracePos = line.find('{');
        int closeBracePos = line.find('}');
        std::string workflowName = line.substr(0, openBracePos);
        std::string workflowString = line.substr(openBracePos + 1, closeBracePos - openBracePos - 1);
        std::string fallbackWorkflowName = workflowString;

        std::vector<Rule> rules;
        std::sregex_iterator ruleIterator(workflowString.begin(), workflowString.end(), rulePattern);
        std::sregex_iterator end;
        while (ruleIterator != end) {
            int category = categoryStringToCategory((*ruleIterator)[1]);
            int comparison = comparisonStringToComparison((*ruleIterator)[2]);
            int value = std::stoi((*ruleIterator)[3]);
            std::string nextWorkflowName = (*ruleIterator)[4];
            rules.emplace_back(category, comparison, value, nextWorkflowName);

            fallbackWorkflowName = ruleIterator->suffix();
            ++ruleIterator;
        }
        if (fallbackWorkflowName[0] == ',') {
            fallbackWorkflowName = fallbackWorkflowName.substr(1);
        }

        workflows[workflowName] = Workflow(rules, fallbackWorkflowName);
    }

    // Get all the paths (a path being a list of conditions) that result in a
    // part being accepted.
    std::vector<std::vector<Condition>> paths;
    std::vector<Condition> currentPath;
    constructAcceptablePaths(workflows, paths, currentPath, "in");

    // Using the paths, find the total number of acceptable combinations.
    long long totalNumberOfAcceptableCombinations = 0;
    for (const std::vector<Condition>& path : paths) {
        // Create the initial lower and upper bounds for each category.
        std::vector<std::pair<int, int>> bounds(4, {1, 4001});

        // Find the lower and upper bound for each category that satisfies all
        // conditions in this path.
        for (const Condition& condition : path) {
            switch (condition.getComparison()) {
            case LESS_THAN: {
                int upperBoundExclusive = bounds[condition.getCategory()].second;
                if (condition.getValue() < upperBoundExclusive) {
                    bounds[condition.getCategory()].second = condition.getValue();
                }
            } break;
            case LESS_THAN_EQUAL_TO: {
                int upperBoundExclusive = bounds[condition.getCategory()].second;
                if (condition.getValue() + 1 < upperBoundExclusive) {
                    bounds[condition.getCategory()].second = condition.getValue() + 1;
                }
            } break;
            case GREATER_THAN: {
                int lowerBoundInclusive = bounds[condition.getCategory()].first;
                if (condition.getValue() + 1 > lowerBoundInclusive) {
                    bounds[condition.getCategory()].first = condition.getValue() + 1;
                }
            } break;
            case GREATER_THAN_EQUAL_TO: {
                int lowerBoundInclusive = bounds[condition.getCategory()].first;
                if (condition.getValue() > lowerBoundInclusive) {
                    bounds[condition.getCategory()].first = condition.getValue();
                }
            } break;
            }
        }

        // Using the bounds, find the number of acceptable combinations for this
        // path.
        long long numberOfAcceptableCombinations = 1;
        for (std::pair<int, int> bound : bounds) {
            if (bound.second - bound.first <= 0) {
                numberOfAcceptableCombinations = 0;
                break;
            }
            else {
                numberOfAcceptableCombinations *= (bound.second - bound.first);
            }
        }

        // Increase the total.
        totalNumberOfAcceptableCombinations += numberOfAcceptableCombinations;
    }

    // Return the total.
    return totalNumberOfAcceptableCombinations;
}

void constructAcceptablePaths(const std::map<std::string, Workflow>& workflows, std::vector<std::vector<Condition>>& paths, std::vector<Condition> currentPath, const std::string& currentWorkflowName) {
    // Base case: add path to paths.
    if (currentWorkflowName == "A") {
        paths.push_back(currentPath);
    }
    // Base case: do nothing.
    else if (currentWorkflowName == "R") {}
    // Inductive case: use the current workflow to extend the path.
    else {
        Workflow currentWorkflow = workflows.at(currentWorkflowName);
        // For each rule in the current workflow: construct a new path that
        // extends the current path by satisfying the rule condition and then
        // consider the next workflow; extend the current path by adding the
        // inverted rule condition and consider the next rule.
        for (const Rule& rule : currentWorkflow.getRules()) {
            std::vector<Condition> newPath = currentPath;
            newPath.push_back(rule.getCondition());
            constructAcceptablePaths(workflows, paths, newPath, rule.getNextWorkflowName());
            currentPath.push_back(rule.getCondition().getInvertedCondition());
        }
        // Extend the current path by considering the fallback workflow.
        constructAcceptablePaths(workflows, paths, currentPath, currentWorkflow.getFallbackWorkflowName());
    }
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
