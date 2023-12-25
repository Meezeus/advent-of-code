//////////////////////////////////////////
// Advent of Code 2023 Day 18 Part 2    //
// https://adventofcode.com/2023/day/18 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

enum Direction { RIGHT, DOWN, LEFT, UP };

long long solution(const std::string fileName);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 952408144115)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

// We want to count the number of '#' squares. This number can be split into two
// parts:
//  1. The squares that make up the border (b)
//  2. The squares inside the border (i).
//
// We can easily calculate b by summing the distances in the directions. To find
// i, we can use Pick's theorem, which states: A = i + (b/2) - 1, where A is the
// area of a polygon that has i integer vertices inside of it and b integer
// vertices on its border.
//
// We are trying to find i; we know b already but not A, so we must first
// calculate A. To do this, we will use the [shoelace
// formula](https://www.youtube.com/watch?v=0KjG8Pg6LGk). Note that the area A
// is NOT the same as the total number of squares (our original goal). This is
// because we treating the squares as points and considering the polygon created
// by joining these points. So, for example, the following rectangle
//
//          #―#―#―#
// ####     |1 2 3|
// #··# --> # · · #
// ####     |4 5 6|
//          #―#―#―#
//
// has side lengths of 3 and 2 (not 4 and 3) and an area of 6 (not 12). This can
// be seen clearly when you look at its coordinates: (0, 0), (3, 0), (3, -2) and
// (0, -2).

// Using the example above:
//  A = i + (b/2) - 1
//  6 = i + (10/2) - 1
//  i = 2
// which checks out as there are 2 points (squares) inside the rectangle.

// We can now solve our original problem by doing b + i = 10 + 2 = 12.

long long solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::vector<std::pair<long long, long long>> vertices;
    std::pair<long long, long long> currentVertex(0, 0);
    vertices.push_back(currentVertex);

    // Calculate the number of border points and all the vertices.
    long long border = 0;
    for (std::string line : input) {
        std::vector<std::string> tokens;
        std::istringstream iss(line);
        std::string token;
        while (std::getline(iss, token, ' ')) {
            tokens.push_back(token);
        }

        tokens[2] = tokens[2].substr(2, tokens[2].length() - 3); // remove '(#' and ')'
        long long distance = std::stoi(tokens[2].substr(0, tokens[2].length() - 1), nullptr, 16);
        int direction = tokens[2].back() - '0';

        border += distance;

        switch (direction) {
        case UP:
            currentVertex.first -= distance;
            break;
        case RIGHT:
            currentVertex.second += distance;
            break;
        case DOWN:
            currentVertex.first += distance;
            break;
        case LEFT:
            currentVertex.second -= distance;
            break;
        }
        vertices.push_back(currentVertex);
    }

    // Calculate the area using the shoelace formula.
    long long area = 0;
    for (int vertexIndex = 0; vertexIndex < vertices.size() - 1; ++ vertexIndex) {
        std::pair<long long, long long>& vertex1 = vertices[vertexIndex];
        std::pair<long long, long long>& vertex2 = vertices[vertexIndex + 1];
        area += (vertex1.first * vertex2.second) - (vertex1.second * vertex2.first);
    }
    area = 0.5 * std::abs(area);

    // Calculate the number of interior points using Pick's theorem.
    long long interior = area - (border/2) + 1;

    // Return the total number of points.
    return border + interior;
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
