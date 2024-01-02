//////////////////////////////////////////
// Advent of Code 2023 Day 21 Part 2    //
// https://adventofcode.com/2023/day/21 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>
#include <iomanip>

long long solution(const std::string fileName, const int desiredNumberOfSteps);
int getNumberOfPlots(const std::vector<std::string>& input, const int desiredNumberOfSteps);
std::set<std::pair<int, int>> takeStep(const std::vector<std::string>& input, std::set<std::pair<int, int>>& lastPositions, std::set<std::pair<int, int>>& currentPositions);
int getModularPositionNumber(const int positionNumber, const int gridSize);
std::vector<std::vector<float>> getInverseOfMatrix(const std::vector<std::vector<int>>& matrix);
template <typename T> void printMatrix(const std::vector<std::vector<T>>& matrix);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    //std::cout << "Example Solution (steps=6): " << getNumberOfPlots(readLines("example.txt"), 6) << " (expected 16)" << std::endl;
    //std::cout << "Example Solution (steps=10): " << getNumberOfPlots(readLines("example.txt"), 10) << " (expected 50)" << std::endl;
    //std::cout << "Example Solution (steps=50): " << getNumberOfPlots(readLines("example.txt"), 50) << " (expected 1594)" << std::endl;
    //std::cout << "Example Solution (steps=100): " << getNumberOfPlots(readLines("example.txt"), 100) << " (expected 6536)" << std::endl;
    //std::cout << "Example Solution (steps=500): " << getNumberOfPlots(readLines("example.txt"), 500) << " (expected 167004)" << std::endl;
    //std::cout << "Example Solution (steps=1000): " << getNumberOfPlots(readLines("example.txt"), 1000) << " (expected 668697)" << std::endl;
    //std::cout << "Example Solution (steps=5000): " << getNumberOfPlots(readLines("example.txt"), 5000) << " (expected 16733044)" << std::endl;
    std::cout << "Input Solution (steps=26501365): " << solution("input.txt", 26501365) << std::endl;
    std::cout << std::endl;
}

// In order to find the number of garden plots that can be reached in 26501365
// steps, we must utilise the fact that the grid repeats. Because the grid
// repeats in the same manner each time, we should also expect the search to
// expand in the same way each time it comes across a new grid. So the number of
// garden plots that can be reached at each expansion should also follow some
// sort of pattern. To find this pattern, we will look at the number of garden
// plots reached in various super-grids (expansions of the original grid).
//
// The input has a special property that is not present in the example input:
// there is a clear straight-line path from the starting position to each of the
// edges of the grid. This is important, because it means that our search will
// reach all 4 edges of the grid at the same step. This step is equal to (grid
// size - 1) / 2 (the straight-line distance from S to each edge).
//
// Let's consider the number of garden plots that can be reached when the search
// has just arrived at the edges of the original grid. As mentioned above, this
// happens after (grid size - 1) / 2 steps. This is a small enough number for
// BFS to find the answer. Let's call the number of garden plots reached at this
// point s1.
//
// The search space is then expanded. You can think of this as adding 8 more
// grids around the original grid, just like in the example, resulting in a 3x3
// super-grid.
//
// The next number we consider is when the search has moved on from edge of the
// original grid and across the entirety of the next grid (using the
// straight-line path mentioned previously). This happens when the step count is
// ((grid size - 1) / 2) + grid size. The number of garden plots reached at this
// point is s2. We use BFS to find this number.
//
// We repeat this process one more time. The super-grid is expanded and now
// becomes a 5x5 grid. The search continues for another grid size steps, so that
// it reaches the new edges of the super-grid. The step count is now equal to
// ((grid size - 1) / 2) + (grid size * 2). The number of garden plots reached
// is s3. Once again, BFS should be able to handle this.
//
// Now we have a sequence of numbers s1, s2, s3, representing the number of
// garden plots reached at each point when the search is about to expand into a
// new grid. As mentioned before, we believe that there is some sort of
// relationship between those numbers, due to the repeating nature of the grid.
// Furthermore, we can assume that the relationship is a second-degree
// polynomial, because the grid expansion is in two dimensions (think about the
// super-grid sizes: 1^2 = 1, 3^2 = 9, 5^2 = 25). Because it is a second-degree
// polynomial, we need a minimum of 3 numbers in the sequence in order to solve
// it (hence s1, s2 and s3).
//
// The formula for a second-degree polynomial sequence is

// sn = a0 + (a1 * n) + (a2 * n^2),

// where n is the position of the number in the sequence.

// Since we have three sequence numbers, we can write three different formulas:

// s1 = a0 + (1 * a1) + (1 * a2)
// s2 = a0 + (2 * a1) + (4 * a2)
// s3 = a0 + (3 * a1) + (9 * a2)

// We can represent this using matrices:

//             (1 1 1)
// (a0 a1 a2)  (1 2 3) = (s1 s2 s3)
//             (1 4 9)
//
//    (1x3)     (3x3)       (1x3)
//
//      A         N    =      S

// Our goal is to find A: the coefficients of the polynomial. We can do this by
// rearranging the formula.

// AN = S
// ANN^-1 = SN^-1
// AI = SN^-1
// A = SN^-1

// To find the inverse of the N matrix, we used the method of minors, cofactors
// and adjugate:
// https://www.mathsisfun.com/algebra/matrix-inverse-minors-cofactors-adjugate.html

// Once we have A, we can predict any term in the sequence. But how does that
// help us find the number of garden plots reached after 26501365 steps?
//
// The nth number in the sequence is the number of garden plots reached after
// ((grid size - 1) / 2) + (grid size * (n-1)) steps. It just happens that
// 26501365 steps corresponds to the 202301st number in the sequence! So if we
// find the 202301st number in the sequence, we find the number of garden plots
// that can be reached in 26501365 steps!

// s202301 = a0 + (202301 * a1) + (202301^2 * a2)

// Using the formula above and the coefficients we found before, we can find the
// answer!

long long solution(const std::string fileName, const int desiredNumberOfSteps) {
    std::vector<std::string> input = readLines(fileName);
    const int gridSize = input.size();

    std::vector<std::vector<int>> nMatrix;
    nMatrix.push_back({1, 1, 1});
    nMatrix.push_back({1, 2, 3});
    nMatrix.push_back({1, 4, 9});
    std::vector<std::vector<float>> nInverseMatrix = getInverseOfMatrix(nMatrix);

    int s1 = getNumberOfPlots(input, ((gridSize - 1) / 2) + (gridSize * 0));
    int s2 = getNumberOfPlots(input, ((gridSize - 1) / 2) + (gridSize * 1));
    int s3 = getNumberOfPlots(input, ((gridSize - 1) / 2) + (gridSize * 2));

    long long a0 = (s1 * nInverseMatrix[0][0]) + (s2 * nInverseMatrix[1][0]) + (s3 * nInverseMatrix[2][0]);
    long long a1 = (s1 * nInverseMatrix[0][1]) + (s2 * nInverseMatrix[1][1]) + (s3 * nInverseMatrix[2][1]);
    long long a2 = (s1 * nInverseMatrix[0][2]) + (s2 * nInverseMatrix[1][2]) + (s3 * nInverseMatrix[2][2]);

    long long n = ((desiredNumberOfSteps - ((gridSize - 1) / 2)) / gridSize) + 1;
    long long numberOfPlots = a0 + (a1 * n) + (a2 * n * n);

    return numberOfPlots;
}

int getNumberOfPlots(const std::vector<std::string>& input, const int desiredNumberOfSteps) {
    // The grid is a square and the grid size is odd. The starting position is
    // in the middle. There is no need to +1 at the end because we are taking
    // the index.
    const int gridSize = input.size();
    std::pair<int, int> startingPosition(((gridSize - 1) / 2), ((gridSize - 1) / 2));

    std::set<std::pair<int, int>> lastPositions;
    std::set<std::pair<int, int>> currentPositions;
    currentPositions.insert(startingPosition);
    int endPositionCount = (desiredNumberOfSteps % 2 == 0) ? 1 : 0;

    for (int step = 1; step <= desiredNumberOfSteps; ++step) {
        currentPositions = takeStep(input, lastPositions, currentPositions);
        if (step % 2 == desiredNumberOfSteps % 2) {
            endPositionCount += currentPositions.size();
        }
    }

    return endPositionCount;
}

std::set<std::pair<int, int>> takeStep(const std::vector<std::string>& input, std::set<std::pair<int, int>>& lastPositions, std::set<std::pair<int, int>>& currentPositions) {
    std::set<std::pair<int, int>> newPositions;
    std::pair<int, int> newPosition;

    for (const std::pair<int, int>& currentPosition : currentPositions) {
        // Check UP
        newPosition = {currentPosition.first - 1, currentPosition.second};
        if (lastPositions.find(newPosition) == lastPositions.end()
        && input[getModularPositionNumber(newPosition.first, input.size())][getModularPositionNumber(newPosition.second, input.size())] != '#') {
            newPositions.insert(newPosition);
        }
        // Check RIGHT
        newPosition = {currentPosition.first, currentPosition.second + 1};
        if (lastPositions.find(newPosition) == lastPositions.end()
        && input[getModularPositionNumber(newPosition.first, input.size())][getModularPositionNumber(newPosition.second, input.size())] != '#') {
            newPositions.insert(newPosition);
        }
        // Check DOWN
        newPosition = {currentPosition.first + 1, currentPosition.second};
        if (lastPositions.find(newPosition) == lastPositions.end()
        && input[getModularPositionNumber(newPosition.first, input.size())][getModularPositionNumber(newPosition.second, input.size())] != '#') {
            newPositions.insert(newPosition);
        }
        // Check LEFT
        newPosition = {currentPosition.first, currentPosition.second - 1};
        if (lastPositions.find(newPosition) == lastPositions.end()
        && input[getModularPositionNumber(newPosition.first, input.size())][getModularPositionNumber(newPosition.second, input.size())] != '#') {
            newPositions.insert(newPosition);
        }
    }

    lastPositions = std::move(currentPositions);
    return newPositions;
}

int getModularPositionNumber(const int positionNumber, const int gridSize) {
    int mod = positionNumber % gridSize;
    if (positionNumber >= 0) {
        return mod;
    }
    else {
        if (mod < 0) {
            return gridSize + mod;
        }
        else if (mod == 0) {
            return 0;
        }
        else {
            std::cout << "ERROR: When the dividend is negative, the remainder should also be negative!" << std::endl;
        }
    }
}

std::vector<std::vector<float>> getInverseOfMatrix(const std::vector<std::vector<int>>& matrix) {
    std::vector<std::vector<int>> matrixOfMinors(3, std::vector<int>(3, 0));

    matrixOfMinors[0][0] = (matrix[1][1] * matrix[2][2]) - (matrix[1][2] * matrix[2][1]);
    matrixOfMinors[0][1] = (matrix[1][0] * matrix[2][2]) - (matrix[1][2] * matrix[2][0]);
    matrixOfMinors[0][2] = (matrix[1][0] * matrix[2][1]) - (matrix[1][1] * matrix[2][0]);

    matrixOfMinors[1][0] = (matrix[0][1] * matrix[2][2]) - (matrix[0][2] * matrix[2][1]);
    matrixOfMinors[1][1] = (matrix[0][0] * matrix[2][2]) - (matrix[0][2] * matrix[2][0]);
    matrixOfMinors[1][2] = (matrix[0][0] * matrix[2][1]) - (matrix[0][1] * matrix[2][0]);

    matrixOfMinors[2][0] = (matrix[0][1] * matrix[1][2]) - (matrix[0][2] * matrix[1][1]);
    matrixOfMinors[2][1] = (matrix[0][0] * matrix[1][2]) - (matrix[0][2] * matrix[1][0]);
    matrixOfMinors[2][2] = (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);

    std::vector<std::vector<int>> matrixOfCofactors = matrixOfMinors;

    matrixOfCofactors[0][1] = matrixOfCofactors[0][1] * -1;
    matrixOfCofactors[1][0] = matrixOfCofactors[1][0] * -1;
    matrixOfCofactors[1][2] = matrixOfCofactors[1][2] * -1;
    matrixOfCofactors[2][1] = matrixOfCofactors[2][1] * -1;

    std::vector<std::vector<int>> adjugate(3, std::vector<int>(3, 0));

    adjugate[0][0] = matrixOfCofactors[0][0];
    adjugate[1][0] = matrixOfCofactors[0][1];
    adjugate[2][0] = matrixOfCofactors[0][2];

    adjugate[0][1] = matrixOfCofactors[1][0];
    adjugate[1][1] = matrixOfCofactors[1][1];
    adjugate[2][1] = matrixOfCofactors[1][2];

    adjugate[0][2] = matrixOfCofactors[2][0];
    adjugate[1][2] = matrixOfCofactors[2][1];
    adjugate[2][2] = matrixOfCofactors[2][2];

    std::vector<std::vector<float>> inverse(3, std::vector<float>(3, 0));

    float determinant = (matrix[0][0] * matrixOfMinors[0][0]) - (matrix[0][1] * matrixOfMinors[0][1]) + (matrix[0][2] * matrixOfMinors[0][2]);

    inverse[0][0] = (1.0 / determinant) * adjugate[0][0];
    inverse[0][1] = (1.0 / determinant) * adjugate[0][1];
    inverse[0][2] = (1.0 / determinant) * adjugate[0][2];

    inverse[1][0] = (1.0 / determinant) * adjugate[1][0];
    inverse[1][1] = (1.0 / determinant) * adjugate[1][1];
    inverse[1][2] = (1.0 / determinant) * adjugate[1][2];

    inverse[2][0] = (1.0 / determinant) * adjugate[2][0];
    inverse[2][1] = (1.0 / determinant) * adjugate[2][1];
    inverse[2][2] = (1.0 / determinant) * adjugate[2][2];

    return inverse;
}

template <typename T>
void printMatrix(const std::vector<std::vector<T>>& matrix) {
    int maxWidth = 0;
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            int currentWidth = std::to_string(matrix[i][j]).length();
            if (currentWidth > maxWidth) {
                maxWidth = currentWidth;
            }
        }
    }

    std::cout << "" << std::endl;
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            std::cout << std::setw(maxWidth + 1) << matrix[i][j];
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
