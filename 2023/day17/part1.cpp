//////////////////////////////////////////
// Advent of Code 2023 Day 17 Part 1    //
// https://adventofcode.com/2023/day/17 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <queue>

const int VERY_BIG_INT = 1000000;

const int MAX_STRAIGHT_MOVE_COUNT = 3;

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
std::pair<std::unordered_map<Node, Node, Node::Hash>, std::unordered_map<Node, int, Node::Hash>> dijkstra(const std::vector<std::string>& input, const std::pair<int, int>& startPosition);
void printPath(std::vector<std::string> input, const std::unordered_map<Node, Node, Node::Hash>& parents, Node endNode);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 102)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::pair<int, int> startPosition(0, 0);
    std::pair<int, int> endPosition(input.size() - 1, input[0].length() - 1);

    auto [parents, distances] = dijkstra(input, startPosition);

    int minDistance = VERY_BIG_INT;
    Node minNode;
    for (int direction = 0; direction <= 3; ++direction) {
        for (int straightMoveCount = 1; straightMoveCount <= MAX_STRAIGHT_MOVE_COUNT; ++straightMoveCount) {
            Node endNode(endPosition, direction, straightMoveCount);
            int endNodeDistance = distances[endNode];
            if (endNodeDistance < minDistance) {
                minDistance = endNodeDistance;
                minNode = endNode;
            }
        }
    }
    //printPath(input, parents, minNode);

    return minDistance;
}

std::pair<std::unordered_map<Node, Node, Node::Hash>, std::unordered_map<Node, int, Node::Hash>> dijkstra(const std::vector<std::string>& input, const std::pair<int, int>& startPosition) {
    struct Comparator {
        bool operator()(const std::pair<int, Node>& pair1, const std::pair<int, Node>& pair2) const {
            return pair1.first > pair2.first;
        }
    };

    std::unordered_map<Node, Node, Node::Hash> parents;
    std::unordered_map<Node, int, Node::Hash> distances;
    std::priority_queue<std::pair<int, Node>, std::vector<std::pair<int, Node>>, Comparator> priorityQueue;

    for (int i1 = 0; i1 < input.size(); ++i1) {
        for (int i2 = 0; i2 < input[0].length(); ++i2) {
            for (int direction = 0; direction <= 3; ++direction) {
                for (int straightMoveCount = 1; straightMoveCount <= MAX_STRAIGHT_MOVE_COUNT; ++straightMoveCount) {
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
        priorityQueue.pop();

        std::vector<Node> neighbours;
        // Check UP
        if (nodePosition.first > 0 && node.getDirection() != DOWN && (node.getDirection() != UP || node.getStraightMoveCount() < MAX_STRAIGHT_MOVE_COUNT)) {
            neighbours.emplace_back(nodePosition.first - 1, nodePosition.second, UP, node.getDirection() != UP ? 1 : node.getStraightMoveCount() + 1);
        }
        // Check RIGHT
        if (nodePosition.second < input[0].length() - 1 && node.getDirection() != LEFT && (node.getDirection() != RIGHT || node.getStraightMoveCount() < MAX_STRAIGHT_MOVE_COUNT)) {
            neighbours.emplace_back(nodePosition.first, nodePosition.second + 1, RIGHT, node.getDirection() != RIGHT ? 1 : node.getStraightMoveCount() + 1);
        }
        // Check DOWN
        if (nodePosition.first < input.size() - 1 && node.getDirection() != UP && (node.getDirection() != DOWN || node.getStraightMoveCount() < MAX_STRAIGHT_MOVE_COUNT)) {
            neighbours.emplace_back(nodePosition.first + 1, nodePosition.second, DOWN, node.getDirection() != DOWN ? 1 : node.getStraightMoveCount() + 1);
        }
        // Check LEFT
        if (nodePosition.second > 0 && node.getDirection() != RIGHT && (node.getDirection() != LEFT || node.getStraightMoveCount() < MAX_STRAIGHT_MOVE_COUNT)) {
            neighbours.emplace_back(nodePosition.first, nodePosition.second - 1, LEFT, node.getDirection() != LEFT ? 1 : node.getStraightMoveCount() + 1);
        }

        for (Node neighbour : neighbours) {
            std::pair<int, int> neighbourPosition = neighbour.getPosition();
            if (distances[neighbour] > distances[node] + (input[neighbourPosition.first][neighbourPosition.second] - '0')) {
                distances[neighbour] = distances[node] + (input[neighbourPosition.first][neighbourPosition.second] - '0');
                parents[neighbour] = node;
                priorityQueue.emplace(distances[neighbour], neighbour);
            }
        }
    }

    return std::pair<std::unordered_map<Node, Node, Node::Hash>, std::unordered_map<Node, int, Node::Hash>>(parents, distances);
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
