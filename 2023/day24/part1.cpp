//////////////////////////////////////////
// Advent of Code 2023 Day 24 Part 1    //
// https://adventofcode.com/2023/day/24 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
//#include <cassert>

class Line {
private:
    std::pair<long double, long double> startPosition;
    std::pair<long double, long double> velocity;
    long double gradient;
    long double yIntercept;

public:
    Line(std::pair<long double, long double> startPosition, std::pair<long double, long double> velocity)
    : startPosition(startPosition), velocity(velocity) {
        gradient = velocity.second / velocity.first;
        yIntercept = (gradient * (-startPosition.first)) + startPosition.second;
    }

    bool inTheFuture(std::pair<long double, long double> position) {
        // If the position does not lie on the line, it is not in the future (or
        // the past for that matter).
        if (std::abs(position.second - ((gradient * position.first) + yIntercept)) >= 0.001L) {
            std::cout << "WARNING: given point does not lie on the line!" << std::endl;
            return false;
        }
        // The x-velocity and the difference between the x-coordinates of the
        // position and the start position must have the same sign.
        else if (velocity.first != 0) {
            return (position.first - startPosition.first) / velocity.first > 0;
        }
        // The y-velocity and the difference between the y-coordinates of the
        // position and the start position must have the same sign.
        else if (velocity.second != 0) {
            return (position.second - startPosition.second) / velocity.second > 0;
        }
        else {
            //assert(false && "The hailstone is not moving and therefore is not a line!"); return false;
        }
    }

    long double getGradient() const {
        return gradient;
    }

    long double getYIntercept() const {
        return yIntercept;
    }

    friend std::ostream& operator<<(std::ostream& os, const Line& line) {
        os << "y = ";

        if (line.gradient != 1) {
            os << line.gradient;
        }
        os << "x";

        if (line.yIntercept > 0) {
            os << " + " << line.yIntercept;
        }
        else if (line.yIntercept < 0) {
            os << " - " << -line.yIntercept;
        }

        return os;
    }
};

int solution(const std::string fileName, unsigned long long minBound, unsigned long long maxBound);
std::pair<long double, long double> findIntersection(const Line& line1, const Line& line2);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution (minBound=7, maxBound=27): " << solution("example.txt", 7, 27) << " (expected 2)" << std::endl;
    std::cout << "Input Solution (minBound=200000000000000, maxBound=400000000000000): " << solution("input.txt", 200000000000000, 400000000000000) << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName, unsigned long long minBound, unsigned long long maxBound) {
    std::vector<std::string> input = readLines(fileName);

    // Model each hailstone in the input as a line.
    std::vector<Line> lines;
    for (std::string line : input) {
        // Remove whitespace and find the position of the @ sign.
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        int atSignPosition = line.find('@');

        // Tokenise and extract the position coordinates.
        std::istringstream positionStream(line.substr(0, atSignPosition));
        std::vector<long double> positionCoordinates;
        std::string positionCoordinate;
        for (int i = 0; i < 3; ++i) {
            std::getline(positionStream, positionCoordinate, ',');
            positionCoordinates.push_back(std::stold(positionCoordinate));
        }

        // Tokenise and extract the velocity components.
        std::istringstream velocityStream(line.substr(atSignPosition + 1));
        std::vector<long double> velocityComponents;
        std::string velocityComponent;
        for (int i = 0; i < 3; ++i) {
            std::getline(velocityStream, velocityComponent, ',');
            velocityComponents.push_back(std::stold(velocityComponent));
        }

        lines.emplace_back(std::make_pair(positionCoordinates[0], positionCoordinates[1]), std::make_pair(velocityComponents[0], velocityComponents[1]));
    }

    // Find the intersection between each pair of lines - if it exists - and
    // check if it lies within the test area and if it happens in the future for
    // both lines. Find the sum of lines that satisfy these conditions.
    int sum = 0;
    for (size_t index1 = 0; index1 < lines.size() - 1; ++index1) {
        for (size_t index2 = index1 + 1; index2 < lines.size(); ++index2) {
            std::pair<long double, long double> intersection = findIntersection(lines[index1], lines[index2]);
            if (!std::isinf(intersection.first) && !std::isinf(intersection.second)
                && intersection.first >= minBound && intersection.first <= maxBound && intersection.second >= minBound && intersection.second <= maxBound
                && lines[index1].inTheFuture(intersection) && lines[index2].inTheFuture(intersection))
            {
                sum += 1;
            }
        }
    }

    return sum;
}

std::pair<long double, long double> findIntersection(const Line& line1, const Line& line2) {
    if (line1.getGradient() == line2.getGradient()) {
        return {std::numeric_limits<long double>::infinity(), std::numeric_limits<long double>::infinity()};
    }
    else {
        long double x = (line2.getYIntercept() - line1.getYIntercept()) / (line1.getGradient() - line2.getGradient());
        long double y1 = (line1.getGradient() * x) + line1.getYIntercept();
        long double y2 = (line2.getGradient() * x) + line2.getYIntercept();
        //assert(std::abs(y1 - y2) < 0.1L && "Discrepancy between y values!");
        return {x, y1};
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
