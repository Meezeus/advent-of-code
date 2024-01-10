//////////////////////////////////////////
// Advent of Code 2023 Day 23 Part 2    //
// https://adventofcode.com/2023/day/23 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <stack>
#include <iomanip>
#include <cstddef>
//#include <cassert>

class Node {
private:
    static int idCounter;

    int id;
    std::pair<int, int> position;

public:
    static void resetIDCounter() {
        idCounter = 0;
    }

    Node(std::pair<int, int> position) : id(idCounter++), position(position) {};

    int getID() const {
        return id;
    }

    std::pair<int, int> getPosition() const {
        return position;
    }

    bool operator==(const Node& other) const {
        return (id == other.id);
    }

    friend std::ostream& operator<<(std::ostream& os, const Node& node) {
        os << "NodeID: " << node.id
        << ", Position: (" << node.position.first << ", " << node.position.second << ")";
        return os;
    }
};
int Node::idCounter = 0;

class Edge {
private:
    const Node& sourceNode;
    const Node& targetNode;
    int weight;

public:
    Edge(const Node& sourceNode, const Node& targetNode, int weight)
    : sourceNode(sourceNode), targetNode(targetNode), weight(weight) {};

    const Node& getSourceNode() const {
        return sourceNode;
    }

    const Node& getTargetNode() const {
        return targetNode;
    }

    int getWeight() const {
        return weight;
    }

    friend std::ostream& operator<<(std::ostream& os, const Edge& edge) {
        os << edge.sourceNode << " --" << edge.weight << "-> " << edge.targetNode;
        return os;
    }
};

class Path {
private:
    uint64_t visitedBitMap;
    const Node& node;
    int length;

public:
    Path(const Node& startNode) : visitedBitMap(0), node(startNode), length(0) {
        visitedBitMap |= (1ULL << node.getID());
    }

    Path(uint64_t visitedBitMap, const Node& node, int length)
    : visitedBitMap(visitedBitMap), node(node), length(length) {}

    bool visited(const Node& node) const {
        return visitedBitMap & (1ULL << node.getID());
    }

    uint64_t getVisitedBitMap() const {
        return visitedBitMap;
    }

    const Node& getNode() const {
        return node;
    }

    int getLength() const {
        return length;
    }
};

int solution(const std::string fileName);
std::pair<std::vector<Node>, std::vector<Edge>> constructGraph(const std::vector<std::string>& input);
std::vector<std::pair<int, int>> getNeighbours(const std::vector<std::string>& input, const std::pair<int, int>& position);
void saveGrid(const std::vector<std::string>& grid, const std::vector<Node>& nodes);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 154)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    Node::resetIDCounter();
    std::vector<std::string> input = readLines(fileName);

    auto [nodes, edges] = constructGraph(input);

    // Find the node that connects to the end node.
    const Node& penultimateNode = [&nodes, &edges]() -> const Node& {
        for (const Edge& edge : edges) {
            if (edge.getTargetNode() == nodes.back()) {
                return edge.getSourceNode();
            }
        }
        //assert(false && "Could not find penultimate node!"); return nodes.back();
    }();

    // The length of the longest path that ends in the penultimate node.
    int longestPathLength = 0;

    // Find the longest path length.
    std::stack<Path> stack;
    stack.emplace(nodes.front());
    while(!stack.empty()) {
        Path path = stack.top();
        stack.pop();

        const Node& currentNode = path.getNode();
        int currentPathLength = path.getLength();

        if (currentNode == penultimateNode) {
            if (currentPathLength > longestPathLength) {
                longestPathLength = currentPathLength;
            }
        }
        else {
            for (const Edge& edge : edges) {
                if (edge.getSourceNode() == currentNode && !path.visited(edge.getTargetNode())) {
                    stack.emplace(path.getVisitedBitMap() | (1ULL << edge.getTargetNode().getID()), edge.getTargetNode(), currentPathLength + edge.getWeight());
                }
            }
        }
    }

    // Get the distance from the penultimate node to the end node.
    int endDistance = 0;
    for (const Edge& edge : edges) {
        if (edge.getTargetNode() == nodes.back()) {
            endDistance = edge.getWeight();
            break;
        }
    }
    //assert(endDistance != 0);

    return longestPathLength + endDistance;
}

std::pair<std::vector<Node>, std::vector<Edge>> constructGraph(const std::vector<std::string>& input) {
    // Construct nodes.
    std::vector<Node> nodes;
    std::map<std::pair<int, int>, bool> isANode;

    // Construct the start node.
    std::pair<int, int> startPosition = {0, 1};
    isANode[startPosition] = true;
    nodes.emplace_back(startPosition);

    // Find and construct nodes in the graph.
    for (size_t rowNumber = 0; rowNumber < input.size(); ++rowNumber) {
        for (size_t colNumber = 0; colNumber < input[0].length(); ++colNumber) {
            std::pair<int, int> nodePosition = {rowNumber, colNumber};
            if (input[rowNumber][colNumber] != '#' && getNeighbours(input, nodePosition).size() >= 3) {
                isANode[nodePosition] = true;
                nodes.emplace_back(nodePosition);
            }
        }
    }

    // Construct the end node.
    std::pair<int, int> endPosition = {input.size() - 1, input[0].length() - 2};
    isANode[endPosition] = true;
    nodes.emplace_back(endPosition);

    // Construct edges.
    std::vector<Edge> edges;

    // DFS from each node to find other adjacent nodes.
    for (const Node& sourceNode : nodes) {
        std::map<std::pair<int, int>, bool> visited;
        std::stack<std::pair<std::pair<int, int>, int>> stack;

        std::pair<int, int> currentPosition = sourceNode.getPosition();
        int currentStepCount = 0;
        visited[currentPosition] = true;

        for (std::pair<int, int> nextPosition : getNeighbours(input, currentPosition)) {
            stack.emplace(nextPosition, currentStepCount + 1);
        }

        while (!stack.empty()) {
            currentPosition = stack.top().first;
            currentStepCount = stack.top().second;
            stack.pop();
            visited[currentPosition] = true;

            if (isANode[currentPosition]) {
                const Node& targetNode = [&nodes, currentPosition]() -> const Node& {
                    for (const Node& node : nodes) {
                        if (node.getPosition() == currentPosition) {
                            return node;
                        }
                    }
                    //assert(false && "Could not find target node!"); return nodes.back();
                }();
                edges.emplace_back(sourceNode, targetNode, currentStepCount);
            }
            else {
                for (std::pair<int, int> nextPosition : getNeighbours(input, currentPosition)) {
                    if (!visited[nextPosition]) {
                        stack.emplace(nextPosition, currentStepCount + 1);
                    }
                }
            }
        }
    }

    std::pair<std::vector<Node>, std::vector<Edge>> graph = {std::move(nodes), std::move(edges)};

    return graph;
}

std::vector<std::pair<int, int>> getNeighbours(const std::vector<std::string>& input, const std::pair<int, int>& position) {
    std::vector<std::pair<int, int>> newPositions;
    std::pair<int, int> newPosition;
    // Check UP
    newPosition = {position.first - 1, position.second};
    if (newPosition.first >= 0 && input[newPosition.first][newPosition.second] != '#') {
        newPositions.push_back(newPosition);
    }
    // Check RIGHT
    newPosition = {position.first, position.second + 1};
    if (static_cast<size_t>(newPosition.second) <= input[0].length() - 1 && input[newPosition.first][newPosition.second] != '#') {
        newPositions.push_back(newPosition);
    }
    // Check DOWN
    newPosition = {position.first + 1, position.second};
    if (static_cast<size_t>(newPosition.first) <= input.size() - 1 && input[newPosition.first][newPosition.second] != '#') {
        newPositions.push_back(newPosition);
    }
    // Check LEFT
    newPosition = {position.first, position.second - 1};
    if (newPosition.second >= 0 && input[newPosition.first][newPosition.second] != '#') {
        newPositions.push_back(newPosition);
    }
    return newPositions;
}

void saveGrid(const std::vector<std::string>& grid, const std::vector<Node>& nodes) {
    // Open the file.
    std::ofstream outputFile("grid.txt");

    int maxWidth = std::to_string(nodes.back().getID()).length();
    for (size_t rowNumber = 0; rowNumber < grid.size(); ++rowNumber) {
        for (size_t colNumber = 0; colNumber < grid[0].length(); ++colNumber) {
            bool found = false;
            for (const Node& node : nodes) {
                if (node.getPosition() == std::pair<int, int>(rowNumber, colNumber)) {
                    outputFile << std::setw(maxWidth + 1) << node.getID();
                    found = true;
                    break;
                }
            }
            if (!found) {
                outputFile << std::setw(maxWidth + 1) << std::string(maxWidth, grid[rowNumber][colNumber]);
            }
        }
        outputFile << "\n";
    }

    // Close the file.
    outputFile.close();
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
