//////////////////////////////////////////
// Advent of Code 2023 Day 17 Part 2    //
// https://adventofcode.com/2023/day/17 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <tuple>


const int VERY_BIG_INT = 1000000;

const int MIN_STRAIGHT_MOVE_COUNT = 4;
const int MAX_STRAIGHT_MOVE_COUNT = 10;

enum Direction { UP, RIGHT, DOWN, LEFT };

std::string directionToString(const int direction) {
    switch (direction) {
    case UP:
        return "UP";
    case RIGHT:
        return "RIGHT";
    case DOWN:
        return "DOWN";
    case LEFT:
        return "LEFT";
    }
}

class Node {
private:
    std::pair<int, int> position;
    int direction;
    int straightMoveCount;

public:
    Node() : position(-1, -1), direction(-1), straightMoveCount(-1) {}

    Node(int rowNumber, int colNumber, int direction, int straightMoveCount)
        : position(rowNumber, colNumber)
        , direction(direction)
        , straightMoveCount(straightMoveCount)
    {}

    Node(std::pair<int, int> position, int direction, int straightMoveCount)
        : position(position)
        , direction(direction)
        , straightMoveCount(straightMoveCount)
    {}

    bool operator==(const Node& other) const {
        return (position == other.position && direction == other.direction && straightMoveCount == other.straightMoveCount);
    }

    friend std::ostream& operator<<(std::ostream& os, const Node& node) {
        os
        << "Position: (" << node.position.first << ", " << node.position.second << ")"
        << ", Direction: " << directionToString(node.direction)
        << ", Straight Move Count: " << node.straightMoveCount;
        return os;
    }

    std::pair<int, int> getPosition() const {
        return position;
    }

    int getDirection() const {
        return direction;
    }

    int getStraightMoveCount() const {
        return straightMoveCount;
    }

    struct Hash {
        size_t operator()(const Node& node) const {
            std::string hashString =
            std::to_string(node.position.first)
            + " " + std::to_string(node.position.second)
            + " " + std::to_string(node.direction)
            + " " + std::to_string(node.straightMoveCount);
            return std::hash<std::string>()(hashString);
        }
    };
};

int solution(const std::string fileName);
std::tuple<std::unordered_map<Node, Node, Node::Hash>, std::unordered_map<Node, int, Node::Hash>, int> dijkstra(const std::vector<std::string>& input, const std::pair<int, int>& startPosition, const std::pair<int, int>& endPosition);
int calculateCost(const std::vector<std::string>& input, const Node& neighbourNode);
void printPath(std::vector<std::string> input, const std::unordered_map<Node, Node, Node::Hash>& parents, Node endNode);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example 1 Solution: " << solution("example.txt") << " (expected 94)" << std::endl;
    std::cout << "Example 2 Solution: " << solution("part2_example.txt") << " (expected 71)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::pair<int, int> startPosition(0, 0);
    std::pair<int, int> endPosition(input.size() - 1, input[0].length() - 1);

    auto [parents, distances, cost] = dijkstra(input, startPosition, endPosition);

    //printPath(input, parents, minNode);

    return cost;
}

std::tuple<std::unordered_map<Node, Node, Node::Hash>, std::unordered_map<Node, int, Node::Hash>, int> dijkstra(const std::vector<std::string>& input, const std::pair<int, int>& startPosition, const std::pair<int, int>& endPosition) {
    struct Comparator {
        bool operator()(const std::pair<int, Node>& pair1, const std::pair<int, Node>& pair2) const {
            return pair1.first > pair2.first;
        }
    };

    std::unordered_map<Node, Node, Node::Hash> parents;
    std::unordered_map<Node, int, Node::Hash> distances;
    int cost = -1;
    std::priority_queue<std::pair<int, Node>, std::vector<std::pair<int, Node>>, Comparator> priorityQueue;

    for (int i1 = 0; i1 < input.size(); ++i1) {
        for (int i2 = 0; i2 < input[0].length(); ++i2) {
            for (int direction = 0; direction <= 3; ++direction) {
                for (int straightMoveCount = MIN_STRAIGHT_MOVE_COUNT; straightMoveCount <= MAX_STRAIGHT_MOVE_COUNT; ++straightMoveCount) {
                    Node node(i1, i2, direction, straightMoveCount);
                    parents[node] = Node();
                    if (i1 == startPosition.first && i2 == startPosition.second) {
                        distances[node] = 0;
                        priorityQueue.emplace(0, node);
                    }
                    else {
                        distances[node] = VERY_BIG_INT;
                        priorityQueue.emplace(VERY_BIG_INT, node);
                    }
                }
            }
        }
    }

    while (!priorityQueue.empty()) {
        Node node = priorityQueue.top().second;
        std::pair<int, int> nodePosition = node.getPosition();
        if (nodePosition == endPosition) {
            cost = priorityQueue.top().first;
            break;
        }
        priorityQueue.pop();

        std::vector<Node> neighbours;

        // Keep going UP
        if (nodePosition.first > 0 && node.getDirection() == UP && node.getStraightMoveCount() < MAX_STRAIGHT_MOVE_COUNT) {
            neighbours.emplace_back(nodePosition.first - 1, nodePosition.second, UP, node.getStraightMoveCount() + 1);
        }
        // Turn UP
        else if (nodePosition.first - MIN_STRAIGHT_MOVE_COUNT >= 0 && node.getDirection() != DOWN && node.getDirection() != UP) {
            neighbours.emplace_back(nodePosition.first - MIN_STRAIGHT_MOVE_COUNT, nodePosition.second, UP, MIN_STRAIGHT_MOVE_COUNT);
        }

        // Keep going RIGHT
        if (nodePosition.second < input[0].length() - 1 && node.getDirection() == RIGHT && node.getStraightMoveCount() < MAX_STRAIGHT_MOVE_COUNT) {
            neighbours.emplace_back(nodePosition.first, nodePosition.second + 1, RIGHT, node.getStraightMoveCount() + 1);
        }
        // Turn RIGHT
        else if (nodePosition.second + MIN_STRAIGHT_MOVE_COUNT <= input[0].length() - 1 && node.getDirection() != LEFT && node.getDirection() != RIGHT) {
            neighbours.emplace_back(nodePosition.first, nodePosition.second + MIN_STRAIGHT_MOVE_COUNT, RIGHT, MIN_STRAIGHT_MOVE_COUNT);
        }

        // Keep going DOWN
        if (nodePosition.first < input.size() - 1 && node.getDirection() == DOWN && node.getStraightMoveCount() < MAX_STRAIGHT_MOVE_COUNT) {
            neighbours.emplace_back(nodePosition.first + 1, nodePosition.second, DOWN, node.getStraightMoveCount() + 1);
        }
        // Turn DOWN
        else if (nodePosition.first + MIN_STRAIGHT_MOVE_COUNT <= input.size() - 1 && node.getDirection() != UP && node.getDirection() != DOWN) {
            neighbours.emplace_back(nodePosition.first + MIN_STRAIGHT_MOVE_COUNT, nodePosition.second, DOWN, MIN_STRAIGHT_MOVE_COUNT);
        }

        // Keep going LEFT
        if (nodePosition.second > 0 && node.getDirection() == LEFT && node.getStraightMoveCount() < MAX_STRAIGHT_MOVE_COUNT) {
            neighbours.emplace_back(nodePosition.first, nodePosition.second - 1, LEFT, node.getStraightMoveCount() + 1);
        }
        // Turn LEFT
        else if (nodePosition.second - MIN_STRAIGHT_MOVE_COUNT >= 0 && node.getDirection() != RIGHT && node.getDirection() != LEFT) {
            neighbours.emplace_back(nodePosition.first, nodePosition.second - MIN_STRAIGHT_MOVE_COUNT, LEFT, MIN_STRAIGHT_MOVE_COUNT);
        }

        for (Node neighbour : neighbours) {
            std::pair<int, int> neighbourPosition = neighbour.getPosition();
            int cost = calculateCost(input, neighbour);
            if (distances[neighbour] > distances[node] + cost) {
                distances[neighbour] = distances[node] + cost;
                parents[neighbour] = node;
                priorityQueue.emplace(distances[neighbour], neighbour);
            }
        }
    }

    return std::tuple<std::unordered_map<Node, Node, Node::Hash>, std::unordered_map<Node, int, Node::Hash>, int>(parents, distances, cost);
}

int calculateCost(const std::vector<std::string>& input, const Node& neighbourNode) {
    std::pair<int, int> neighbourPosition = neighbourNode.getPosition();
    if (neighbourNode.getStraightMoveCount() == MIN_STRAIGHT_MOVE_COUNT) {
        int cost = 0;
        for (int i = 0; i < MIN_STRAIGHT_MOVE_COUNT; ++i) {
            switch (neighbourNode.getDirection()) {
            case UP:
                cost += input[neighbourPosition.first + i][neighbourPosition.second] - '0';
                break;
            case RIGHT:
                cost += input[neighbourPosition.first][neighbourPosition.second - i] - '0';
                break;
            case DOWN:
                cost += input[neighbourPosition.first - i][neighbourPosition.second] - '0';
                break;
            case LEFT:
                cost += input[neighbourPosition.first][neighbourPosition.second + i] - '0';
                break;
            }
        }
        return cost;
    }
    else {
        return input[neighbourPosition.first][neighbourPosition.second] - '0';
    }
}

void printPath(std::vector<std::string> input, const std::unordered_map<Node, Node, Node::Hash>& parents, Node endNode) {
    std::pair<int, int> endNodePosition = endNode.getPosition();
    input[endNodePosition.first][endNodePosition.second] = '.';

    Node parent = parents.at(endNode);
    std::pair<int, int> parentPosition = parent.getPosition();
    while (parentPosition != std::pair<int, int>(-1, -1)) {
        input[parentPosition.first][parentPosition.second] = '.';
        parent = parents.at(parent);
        parentPosition = parent.getPosition();
    }

    std::cout << "" << std::endl;
    for (auto row : input) {
        for (auto c : row) {
            std::cout << c;
        }
        std::cout << "" << std::endl;
    }
    std::cout << "" << std::endl;
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
