//////////////////////////////////////////
// Advent of Code 2023 Day 22 Part 2    //
// https://adventofcode.com/2023/day/22 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <set>
#include <map>
#include <memory>
#include <sstream>
#include <algorithm>
#include <iomanip>

enum Dimension { NONE, X, Y, Z };

std::string dimensionToDimensionString(int dimension) {
    switch (dimension) {
    case X:
        return "X";
    case Y:
        return "Y";
    case Z:
        return "Z";
    case NONE:
        return "-";
    default:
        std::cout << "ERROR: Incorrect Dimension!" << std::endl;
        return "!";
    }
}

class Brick {
private:
    int id;

    int startX;
    int startY;
    int startZ;

    int endX;
    int endY;
    int endZ;

    int changingDimension;

    std::set<int> supportBrickIDs;
    std::set<int> supportedBrickIDs;

public:
    Brick(int id, std::vector<int> startCoordinates, std::vector<int> endCoordinates) : id(id) {
        startX = startCoordinates[0];
        startY = startCoordinates[1];
        startZ = startCoordinates[2];

        endX = endCoordinates[0];
        endY = endCoordinates[1];
        endZ = endCoordinates[2];

        if (startX != endX) {
            changingDimension = X;
        }
        else if (startY != endY) {
            changingDimension = Y;
        }
        else if (startZ != endZ) {
            changingDimension = Z;
        }
        else {
            changingDimension = NONE;
        }
    }

    void decrementHeight() {
        if (!onTheGround()) {
            startZ--;
            endZ--;
        }
        else {
            std::cout << "Cannot go any lower!" << std::endl;
        }
    }

    void addSupportBrickID(int brickID) {
        supportBrickIDs.insert(brickID);
    }

    void addSupportedBrickID(int brickID) {
        supportedBrickIDs.insert(brickID);
    }

    bool onTheGround() const {
        return startZ == 1;
    }

    std::vector<std::tuple<int, int, int>> getBrickCoordinates() const {
        std::vector<std::tuple<int, int, int>> coordinates;
        for (int offset = 0; offset < getLength(); ++offset) {
            switch (changingDimension) {
            case X:
                coordinates.emplace_back(startX + offset, startY, startZ);
                break;
            case Y:
                coordinates.emplace_back(startX, startY + offset, startZ);
                break;
            case Z:
                coordinates.emplace_back(startX, startY, startZ + offset);
                break;
            case NONE:
                coordinates.emplace_back(startX, startY, startZ);
                break;
            default:
                std::cout << "ERROR: Incorrect Dimension!" << std::endl;
            }
        }
        return coordinates;
    }

    int getLength() const {
        switch (changingDimension) {
        case X:
            return (endX - startX) + 1;
        case Y:
            return (endY - startY) + 1;
        case Z:
            return (endZ - startZ) + 1;
        case NONE:
            return 1;
        default:
            std::cout << "ERROR: Incorrect Dimension!" << std::endl;
            return -1;
        }
    }

    std::set<int> getSupportBrickIDs() const {
        return supportBrickIDs;
    }

    std::set<int> getSupportedBrickIDs() const {
        return supportedBrickIDs;
    }

    int getID() const {
        return id;
    }

    friend std::ostream& operator<<(std::ostream& os, const Brick& brick) {
        os << "Brick ID: " << brick.id
        << ", changing dimension: " << dimensionToDimensionString(brick.changingDimension)
        << ", " << brick.startX << "," << brick.startY << "," << brick.startZ << "~" << brick.endX << "," << brick.endY << "," << brick.endZ
        << ", supported by ";

        for (int brickID : brick.supportBrickIDs) {
            os << brickID << ",";
        }

        os << " and supports ";

        for (int brickID : brick.supportedBrickIDs) {
            os << brickID << ",";
        }

        return os;
    }

    bool operator<(const Brick& other) const {
        return startZ < other.startZ;
    }
};

int solution(const std::string fileName);
void fall(const std::map<int, std::shared_ptr<Brick>>& brickMap, std::map<std::tuple<int, int, int>, int>& grid, std::shared_ptr<Brick>& brickPtr);
int getChainReactionCount(const std::map<int, std::shared_ptr<Brick>>& brickMap, const std::shared_ptr<Brick>& brickPtr);
void saveGrid(const std::map<std::tuple<int, int, int>, int>& grid);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 7)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::map<std::tuple<int, int, int>, int> grid;
    std::map<int, std::shared_ptr<Brick>> brickMap;
    std::vector<std::shared_ptr<Brick>> brickList;

    // Create the bricks.
    int idCounter = 0;
    for (std::string line : input) {
        // Get the necessary information to the create the brick.
        idCounter++;
        int tildeIndex = line.find('~');

        std::vector<int> startCoordinates;
        std::istringstream startCoordinatesISS(line.substr(0, tildeIndex));
        std::string startCoordinateToken;
        for (int i = 0; i < 3; ++i) {
            std::getline(startCoordinatesISS, startCoordinateToken, ',');
            startCoordinates.push_back(std::stoi(startCoordinateToken));
        }

        std::vector<int> endCoordinates;
        std::istringstream endCoordinatesISS(line.substr(tildeIndex + 1));
        std::string endCoordinateToken;
        for (int i = 0; i < 3; ++i) {
            std::getline(endCoordinatesISS, endCoordinateToken, ',');
            endCoordinates.push_back(std::stoi(endCoordinateToken));
        }

        // Create the brick and store it.
        std::shared_ptr<Brick> brickPtr = std::make_shared<Brick>(idCounter, startCoordinates, endCoordinates);
        brickMap[brickPtr->getID()] = brickPtr;
        brickList.push_back(brickPtr);

        // Add the brick to the grid.
        for (std::tuple<int, int, int> coordinates : brickPtr->getBrickCoordinates()) {
            grid[coordinates] = brickPtr->getID();
        }
    }

    // Sort the bricks and then drop them.
    std::sort(brickList.begin(), brickList.end(), [](const std::shared_ptr<Brick>& a, const std::shared_ptr<Brick>& b) { return *a < *b; });
    for (std::shared_ptr<Brick>& brickPtr : brickList) {
        fall(brickMap, grid, brickPtr);
    }

    // Remove bricks that can be safely disintegrated from the brick list.
    for (const auto& [brickID, brickPtr] : brickMap) {
        bool canBeSafelyDisintegrated = true;
        for (int supportedBrickID : brickPtr->getSupportedBrickIDs()) {
            if (brickMap[supportedBrickID]->getSupportBrickIDs().size() == 1) {
                canBeSafelyDisintegrated = false;
            }
        }
        if (canBeSafelyDisintegrated) {
            brickList.erase(std::find(brickList.begin(), brickList.end(), brickPtr));
        }
    }

    // Get the sum of the number of bricks that will fall if each brick were to
    // be disintegrated.
    int sum = 0;
    for (std::shared_ptr<Brick>& brickPtr : brickList) {
        sum += getChainReactionCount(brickMap, brickPtr);
    }

    return sum;
}

void fall(const std::map<int, std::shared_ptr<Brick>>& brickMap, std::map<std::tuple<int, int, int>, int>& grid, std::shared_ptr<Brick>& brickPtr) {
    if (brickPtr->onTheGround()) {
        return;
    }

    bool canFall = true;
    for (std::tuple<int, int, int> coordinates : brickPtr->getBrickCoordinates()) {
        std::get<2>(coordinates)--;
        if (grid[coordinates] != 0 && grid[coordinates] != brickPtr->getID()) {
            canFall = false;
            brickPtr->addSupportBrickID(grid[coordinates]);
            brickMap.at(grid[coordinates])->addSupportedBrickID(brickPtr->getID());
        }
    }

    if (canFall) {
        for (std::tuple<int, int, int> coordinates : brickPtr->getBrickCoordinates()) {
            grid[coordinates] = 0;
        }
        brickPtr->decrementHeight();
        for (std::tuple<int, int, int> coordinates : brickPtr->getBrickCoordinates()) {
            grid[coordinates] = brickPtr->getID();
        }
        fall(brickMap, grid, brickPtr);
    }
}

int getChainReactionCount(const std::map<int, std::shared_ptr<Brick>>& brickMap, const std::shared_ptr<Brick>& brickPtr) {
    std::set<int> fallenBrickIDs;
    fallenBrickIDs.insert(brickPtr->getID());

    std::set<int> currentBrickIDs;
    for (int supportedBrickID : brickPtr->getSupportedBrickIDs()) {
        currentBrickIDs.insert(supportedBrickID);
    }

    while (!currentBrickIDs.empty()) {
        std::set<int> nextBrickIDs;
        for (int currentBrickID : currentBrickIDs) {
            std::set<int> supportBrickIDs = brickMap.at(currentBrickID)->getSupportBrickIDs();
            if (std::includes(fallenBrickIDs.begin(), fallenBrickIDs.end(), supportBrickIDs.begin(), supportBrickIDs.end())) {
                fallenBrickIDs.insert(currentBrickID);
                std::set<int> supportedBrickIDs = brickMap.at(currentBrickID)->getSupportedBrickIDs();
                nextBrickIDs.insert(supportedBrickIDs.begin(), supportedBrickIDs.end());
            }
        }
        currentBrickIDs = std::move(nextBrickIDs);
    }

    return fallenBrickIDs.size() - 1;
}

void saveGrid(const std::map<std::tuple<int, int, int>, int>& grid) {
    // Find max width to use and max coordinates.
    int maxWidth = 0;
    int maxX = 0;
    int maxY = 0;
    int maxZ = 0;
    for (const auto& [coordinate, brickID] : grid) {
        if (grid.at(coordinate) != 0 && std::get<0>(coordinate) > maxX) {
            maxX = std::get<0>(coordinate);
        }
        if (grid.at(coordinate) != 0 && std::get<1>(coordinate) > maxY) {
            maxY = std::get<1>(coordinate);
        }
        if (grid.at(coordinate) != 0 && std::get<2>(coordinate) > maxZ) {
            maxZ = std::get<2>(coordinate);
        }
        if (grid.at(coordinate) != 0 && std::to_string(brickID).length() > maxWidth) {
            maxWidth = std::to_string(brickID).length();
        }
    }

    // Open the file.
    std::ofstream outputFile("grid.txt");

    // Print the x-axis grid.
    outputFile << std::string(((maxX + 1) * (maxWidth + 1)) / 2, ' ') << "x" << "\n";
    for (int x = 0; x <= maxX; ++x) {
        outputFile << std::setw(maxWidth + 1) << x;
    }
    outputFile << std::string(maxWidth + 1, ' ') << "z" << "\n";
    for (int z = maxZ; z >= 1; --z) {
        for (int x = 0; x <= maxX; ++x) {
            int foundID = 0;
            for (int y = 0; y <= maxY; ++y) {
                if (grid.find({x, y, z}) != grid.end() && grid.at({x, y, z}) != 0) {
                    if (foundID == 0) {
                        foundID = grid.at({x, y, z});
                    }
                    else if (foundID != grid.at({x, y, z})) {
                        foundID = -1;
                        break;
                    }
                }
            }
            if (foundID == -1) {
                outputFile << std::setw(maxWidth + 1) << std::string(maxWidth, '?');
            }
            else if (foundID == 0) {
                outputFile << std::setw(maxWidth + 1) << std::string(maxWidth, '.');
            }
            else {
                outputFile << std::setw(maxWidth + 1) << foundID;
            }
        }
        outputFile << std::string(maxWidth + 1, ' ') << z << "\n";
    }
    outputFile << " " << std::string((maxX + 1) * (maxWidth + 1) - 1, '-') << std::string(maxWidth + 1, ' ') << 0 << "\n";

    // Print separator.
    outputFile << "\n";

    // Print the y-axis grid.
    outputFile << std::string(((maxY + 1) * (maxWidth + 1)) / 2, ' ') << "y" << "\n";
    for (int y = 0; y <= maxY; ++y) {
        outputFile << std::setw(maxWidth + 1) << y;
    }
    outputFile << std::string(maxWidth + 1, ' ') << "z" << "\n";
    for (int z = maxZ; z >= 1; --z) {
        for (int y = 0; y <= maxY; ++y) {
            int foundID = 0;
            for (int x = 0; x <= maxX; ++x) {
                if (grid.find({x, y, z}) != grid.end() && grid.at({x, y, z}) != 0) {
                    if (foundID == 0) {
                        foundID = grid.at({x, y, z});
                    }
                    else if (foundID != grid.at({x, y, z})) {
                        foundID = -1;
                        break;
                    }
                }
            }
            if (foundID == -1) {
                outputFile << std::setw(maxWidth + 1) << std::string(maxWidth, '?');
            }
            else if (foundID == 0) {
                outputFile << std::setw(maxWidth + 1) << std::string(maxWidth, '.');
            }
            else {
                outputFile << std::setw(maxWidth + 1) << foundID;
            }
        }
        outputFile << std::string(maxWidth + 1, ' ') << z << "\n";
    }
    outputFile << " " << std::string((maxX + 1) * (maxWidth + 1) - 1, '-') << std::string(maxWidth + 1, ' ') << 0 << "\n";

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
