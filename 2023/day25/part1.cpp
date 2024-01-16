//////////////////////////////////////////
// Advent of Code 2023 Day 25 Part 1    //
// https://adventofcode.com/2023/day/25 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <queue>
#include <algorithm>
#include <sstream>
//#include <cassert>

int solution(const std::string fileName);
std::pair<std::map<std::pair<std::string, std::string>, int>, int>
fordFulkerson
(
    const std::vector<std::pair<std::string, std::string>>& edges,
    const std::map<std::pair<std::string, std::string>, int>& capacities,
    const std::string& source,
    const std::string& sink
);
std::map<std::pair<std::string, std::string>, int>
constructResidualGraph
(
    const std::vector<std::pair<std::string, std::string>>& edges,
    const std::map<std::pair<std::string, std::string>, int>& capacities,
    const std::map<std::pair<std::string, std::string>, int>& flow
);
std::vector<std::string>
findAugmentingPath
(
    const std::vector<std::pair<std::string, std::string>>& edges,
    const std::map<std::pair<std::string, std::string>, int>& residualCapacities,
    const std::string& source,
    const std::string& sink
);
std::vector<std::pair<std::string, std::string>> getPathEdges(const std::vector<std::string>& pathNodes);
int calculatePathCapacity(const std::map<std::pair<std::string, std::string>, int>& capacities, const std::vector<std::pair<std::string, std::string>>& pathEdges);
int calculateFlowValue(const std::map<std::pair<std::string, std::string>, int>& flow, const std::string& node);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 54)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

// Implementation of Ford-Fulkerson was taken from
// https://github.com/Meezeus/graph-algorithms/ and adapted to work for
// undirected graphs with the help of
// https://www.inf.ufpr.br/pos/techreport/RT_DINF003_2004.pdf (note that the
// example in this paper seems to be incorrect as it does not follow the
// principle of conservation of flow).
int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::vector<std::string> nodes;
    std::vector<std::pair<std::string, std::string>> edges;
    std::map<std::pair<std::string, std::string>, int> capacities;

    // Construct the graph.
    for (std::string line : input) {
        int colonPosition = line.find(':');

        std::string node1 = line.substr(0, colonPosition);
        if (std::find(nodes.begin(), nodes.end(), node1) == nodes.end()) {
            nodes.push_back(node1);
        }

        std::istringstream iss(line.substr(colonPosition + 2));
        std::string node2;
        while (std::getline(iss, node2, ' ')) {
            if (std::find(nodes.begin(), nodes.end(), node2) == nodes.end()) {
                nodes.push_back(node2);
            }

            edges.emplace_back(node1, node2);
            capacities[{node1, node2}] = 1;
            edges.emplace_back(node2, node1);
            capacities[{node2, node1}] = 1;
        }
    }

    // Fix the source node, then find a sink node such that the maximum flow is
    // 3.
    std::string source = nodes[0];
    for (const std::string& sink : nodes) {
        if (sink != source) {
            // Run Ford-Fulkerson to find the maximum flow.
            const auto [flow, flowValue] = fordFulkerson(edges, capacities, source, sink);

            // Using the maximum flow = minimum cut theorem.
            if (flowValue == 3) {
                // Construct the residual graph using the maximum flow.
                std::map<std::pair<std::string, std::string>, int> residualCapacities = constructResidualGraph(edges, capacities, flow);

                // BFS in the residual graph, starting at the source node, to find
                // all the nodes that belong in the source group.
                std::vector<std::string> sourceGroup;
                sourceGroup.push_back(source);

                std::queue<std::string> queue;
                queue.push(source);
                while (!queue.empty()) {
                    std::string currentNode = std::move(queue.front());
                    queue.pop();

                    for (const auto& [u, v] : edges) {
                        if (u == currentNode && residualCapacities.at({u, v}) > 0 && std::find(sourceGroup.begin(), sourceGroup.end(), v) == sourceGroup.end()) {
                            sourceGroup.push_back(v);
                            queue.push(v);
                        }
                    }
                }

                // Return the answer.
                return sourceGroup.size() * (nodes.size() - sourceGroup.size());
            }
        }
    }
    //assert(false && "Could not find a small enough cut!"); return -1;
}

// Finds the maximum flow in a flow graph using the Ford-Fulkerson method.
std::pair<std::map<std::pair<std::string, std::string>, int>, int>
fordFulkerson
(
    const std::vector<std::pair<std::string, std::string>>& edges,
    const std::map<std::pair<std::string, std::string>, int>& capacities,
    const std::string& source,
    const std::string& sink
)
{
    // Initialise the flow.
    std::map<std::pair<std::string, std::string>, int> flow;
    for (const std::pair<std::string, std::string>& edge : edges) {
        flow[edge] = 0;
    }

    // Build residual graph and find augmenting path.
    std::map<std::pair<std::string, std::string>, int> residualCapacities = constructResidualGraph(edges, capacities, flow);
    std::vector<std::string> augmentingPathNodes = findAugmentingPath(edges, residualCapacities, source, sink);

    // Keep augmenting the flow until an augmenting path cannot be found.
    while (!augmentingPathNodes.empty()) {
        // Get the augmenting path edges and the augmenting path capacity.
        std::vector<std::pair<std::string, std::string>> augmentingPathEdges = getPathEdges(augmentingPathNodes);
        int augmentingPathCapacity = calculatePathCapacity(residualCapacities, augmentingPathEdges);

        // Augment the current flow with the augmenting path.
        for (const auto& [u, v] : edges) {
            if (std::find(augmentingPathEdges.begin(), augmentingPathEdges.end(), std::make_pair(u, v)) != augmentingPathEdges.end()) {
                flow[{u, v}] += augmentingPathCapacity;
                flow[{v, u}] = -flow[{u, v}];
            }
        }

        // Build residual graph and find augmenting path.
        residualCapacities = constructResidualGraph(edges, capacities, flow);
        augmentingPathNodes = findAugmentingPath(edges, residualCapacities, source, sink);
    }

    // Once an augmenting path cannot be found, return the flow.
    return {flow, calculateFlowValue(flow, source)};
}

// Constructs a residual graph.
std::map<std::pair<std::string, std::string>, int>
constructResidualGraph
(
    const std::vector<std::pair<std::string, std::string>>& edges,
    const std::map<std::pair<std::string, std::string>, int>& capacities,
    const std::map<std::pair<std::string, std::string>, int>& flow
)
{
    std::map<std::pair<std::string, std::string>, int> residualCapacities;
    for (const auto& [u, v] : edges) {
        residualCapacities[{u, v}] = capacities.at({u, v}) - flow.at({u, v});
    }
    return residualCapacities;
}

// Attempts to find an augmenting path in a residual graph, by performing BFS.
std::vector<std::string>
findAugmentingPath
(
    const std::vector<std::pair<std::string, std::string>>& edges,
    const std::map<std::pair<std::string, std::string>, int>& residualCapacities,
    const std::string& source,
    const std::string& sink
)
{
    std::vector<std::string> visited;
    visited.push_back(source);

    std::queue<std::vector<std::string>> paths;
    paths.push({source});

    while (!paths.empty()) {
        std::vector<std::string> currentPath = paths.front();
        paths.pop();

        std::string currentNode = currentPath.back();

        if (currentNode == sink) {
            return currentPath;
        }
        else {
            for (const auto& [u, v] : edges) {
                if (u == currentNode && residualCapacities.at({u, v}) > 0 && std::find(visited.begin(), visited.end(), v) == visited.end()) {
                    visited.push_back(v);
                    std::vector<std::string> newPath = currentPath;
                    newPath.push_back(v);
                    paths.push(std::move(newPath));
                }
            }
        }
    }

    return {};
}

//  Turns a path represented by a list of nodes into a path represented by a
//  list of edges, where an edge is a pair of nodes (u, v).
std::vector<std::pair<std::string, std::string>> getPathEdges(const std::vector<std::string>& pathNodes) {
    std::vector<std::pair<std::string, std::string>> pathEdges;
    for (size_t index = 0; index < pathNodes.size() - 1; index++) {
        pathEdges.emplace_back(pathNodes.at(index), pathNodes.at(index + 1));
    }
    return pathEdges;
}

// Calculates the capacity of a path in a flow graph.
int calculatePathCapacity(const std::map<std::pair<std::string, std::string>, int>& capacities, const std::vector<std::pair<std::string, std::string>>& pathEdges) {
    int pathCapacity = capacities.at(pathEdges.front());
    for (const std::pair<std::string, std::string>& edge : pathEdges) {
        if (capacities.at(edge) < pathCapacity) {
            pathCapacity = capacities.at(edge);
        }
    }
    return pathCapacity;
}

// Calculates the total net outgoing flow from a node in a flow graph.
int calculateFlowValue(const std::map<std::pair<std::string, std::string>, int>& flow, const std::string& node) {
    int flowValue = 0;
    for (const auto& [edge, edgeFlow] : flow) {
        if (edgeFlow > 0) {
            if (edge.first == node) {
                flowValue += edgeFlow;
            }
            if (edge.second == node) {
                flowValue -= edgeFlow;
            }
        }
    }
    return flowValue;
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
