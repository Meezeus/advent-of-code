//////////////////////////////////////////
// Advent of Code 2023 Day 24 Part 2    //
// https://adventofcode.com/2023/day/24 //
//////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <tuple>
#include <random>
#include <ctime>
//#include <cassert>

struct Vector {
    long long x;
    long long y;
    long long z;

    Vector(long long x, long long y, long long z) : x(x), y(y), z(z) {}

    friend std::ostream& operator<<(std::ostream& os, const Vector& vector) {
        os << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
        return os;
    }
};

struct Hailstone {
    Vector p0;
    Vector v;

    Hailstone(Vector p0, Vector v) : p0(p0), v(v) {}

    friend std::ostream& operator<<(std::ostream& os, const Hailstone& hailstone) {
        os << "(x, y, z) = " << hailstone.p0 << " + " << hailstone.v << "t";
        return os;
    }
};

// Describes an equation of the form:
//   (a_px * P_x(r,0))
// + (a_py * P_y(r,0))
// + (a_pz * P_z(r,0))
// + (a_vx * V_x(r))
// + (a_vy * V_y(r))
// + (a_vz * V_z(r))
// = a_
struct Equation {
    long long a_px;
    long long a_py;
    long long a_pz;
    long long a_vx;
    long long a_vy;
    long long a_vz;
    long long a_;

    Equation(long long a_px, long long a_py, long long a_pz, long long a_vx, long long a_vy, long long a_vz, long long a_)
    : a_px(a_px), a_py(a_py), a_pz(a_pz), a_vx(a_vx), a_vy(a_vy), a_vz(a_vz), a_(a_) {}

    friend std::ostream& operator<<(std::ostream& os, const Equation& equation) {
          os << "(" << equation.a_px << " * P_x(r,0))"
        << "  +  (" << equation.a_py << " * P_y(r,0))"
        << "  +  (" << equation.a_pz << " * P_z(r,0))"
        << "  +  (" << equation.a_vx << " * V_x(r))"
        << "  +  (" << equation.a_vy << " * V_y(r))"
        << "  +  (" << equation.a_vz << " * V_z(r))"
        << "   =   " << equation.a_;
        return os;
    }
};

long long solution(const std::string fileName);
std::tuple<Equation, Equation, Equation> getEquations(const Hailstone& hailstone, const Hailstone& hailstonePrime);
std::vector<long double> gaussianElimination(const std::vector<std::vector<long double>>& augmentedMatrix);
std::vector<std::string> readLines(const std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 47)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

// When the rock (r) collides with a hailstone (i) at time (t), their positions
// (P) are the same. Therefore we have the equation:

// P(r,t) = P(i,t)
// P(r,0) + V(r)*t  =  P(i,0) + V(i)*t    (0)

// Split (0) into 3 equations for each of the dimensions.

// P_x(r,0) + V_x(r)*t  =  P_x(i,0) + V_x(i)*t
// P_y(r,0) + V_y(r)*t  =  P_y(i,0) + V_y(i)*t
// P_z(r,0) + V_z(r)*t  =  P_z(i,0) + V_z(i)*t

// We wish to get rid of t so that we have one less unknown to worry about.
// First, we rearrange the equations.

// t  =  (P_x(i,0) - P_x(r,0))  /  (V_x(r) - V_x(i))    (1)
// t  =  (P_y(i,0) - P_y(r,0))  /  (V_y(r) - V_y(i))    (2)
// t  =  (P_z(i,0) - P_z(r,0))  /  (V_z(r) - V_z(i))    (3)

// Since t is the same for all three equations, we can set them equal to each
// other. Using (1) and (2) as an example:

// (P_x(i,0) - P_x(r,0))  /  (V_x(r) - V_x(i))   =   (P_y(i,0) - P_y(r,0))  /  (V_y(r) - V_y(i))
// (P_x(i,0) - P_x(r,0))  *  (V_y(r) - V_y(i))   =   (P_y(i,0) - P_y(r,0))  *  (V_x(r) - V_x(i))
// (P_x(i,0) * V_y(r))  -  (P_x(i,0) * V_y(i))  -  (P_x(r,0) * V_y(r))  +  (P_x(r,0) * V_y(i))
//    =   (P_y(i,0) * V_x(r))  -  (P_y(i,0) * V_x(i))  -  (P_y(r,0) * V_x(r))  +  (P_y(r,0) * V_x(i))

// This equation is unfortunately non-linear due to the (P_x(r,0) * V_y(r)) and
// (P_y(r,0) * V_x(r)) terms. We wish to eliminate them from the equations, just
// like we did with t. So once again, we rearrange the equation.

// (P_y(r,0) * V_x(r))  -  (P_x(r,0) * V_y(r)
//       =   (P_y(i,0) * V_x(r))  -  (P_y(i,0) * V_x(i))  +  (P_y(r,0) * V_x(i))  -  (P_x(i,0) * V_y(r))  + (P_x(i,0) * V_y(i))  -  (P_x(r,0) * V_y(i))    (4)

// The LHS of this equation is independent of the hailstone (i). Therefore it
// will be the same for all hailstones. We can equate the RHS of the equation
// for different hailstones.

// (P_y(i,0) * V_x(r))  -  (P_y(i,0) * V_x(i))  +  (P_y(r,0) * V_x(i))  -  (P_x(i,0) * V_y(r))  + (P_x(i,0) * V_y(i))  -  (P_x(r,0) * V_y(i))
//       =   (P_y(i',0) * V_x(r))  -  (P_y(i',0) * V_x(i'))  +  (P_y(r,0) * V_x(i'))  -  (P_x(i',0) * V_y(r))  +  (P_x(i',0) * V_y(i'))  -  (P_x(r,0) * V_y(i'))

// Rearranging the equation above..

// (P_y(i,0) * V_x(r))  +  (P_y(r,0) * V_x(i))  -  (P_x(i,0) * V_y(r)  -  (P_x(r,0) * V_y(i))  -  (P_y(i',0) * V_x(r))  -  (P_y(r,0) * V_x(i'))  + (P_x(i',0) * V_y(r))  +  (P_x(r,0) * V_y(i'))
//       =   (P_y(i,0) * V_x(i))  -  (P_x(i,0) * V_y(i))  -  (P_y(i',0) * V_x(i'))  +  (P_x(i',0) * V_y(i'))

// ... and factorising out the variables...

// ([V_y(i') - V_y(i)]  *  P_x(r,0))   +   ([V_x(i) - V_x(i')]  *  P_y(r,0))   +   ([P_y(i,0) - P_y(i',0)]  *  V_x(r))   +   ([P_x(i',0) - P_x(i,0)]  *  V_y(r))
//       =   (P_y(i,0) * V_x(i))  -  (P_x(i,0) * V_y(i))  -  (P_y(i',0) * V_x(i'))  +  (P_x(i',0) * V_y(i'))    (5)

// ... gives us a linear equation.

// Repeating using (1) and (3), and (2) and (3):

// ([V_z(i') - V_z(i)]  *  P_x(r,0))   +   ([V_x(i) - V_x(i')]  *  P_z(r,0))   +   ([P_z(i,0) - P_z(i',0)]  *  V_x(r))   +   ([P_x(i',0) - P_x(i,0)]  *  V_z(r))
//       =   (P_z(i,0) * V_x(i))  -  (P_x(i,0) * V_z(i))  -  (P_z(i',0) * V_x(i'))  +  (P_x(i',0) * V_z(i'))    (6)

// ([V_z(i') - V_z(i)]  *  P_y(r,0))   +   ([V_y(i) - V_y(i')]  *  P_z(r,0))   +   ([P_z(i,0) - P_z(i',0)]  *  V_y(r))   +   ([P_y(i',0) - P_y(i,0)]  *  V_z(r))
//       =   (P_z(i,0) * V_y(i))  -  (P_y(i,0) * V_z(i))  -  (P_z(i',0) * V_y(i'))  +  (P_y(i',0) * V_z(i'))    (7)

// We now have a system of linear equations. There are 6 unknown variables in
// our system (each equation features 4 variables, but you can imagine the other
// 2 variables having a coefficient of 0). Since there are 6 unknown variables
// in total, we need 6 equations. Each pair of hailstones gives us 3 equations,
// so we need 2 pairs of hailstones, which means we need 3 different hailstones.
// We can then solve the system of linear equations using Gaussian Elimination.

long long solution(const std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    // Create each hailstone from the input.
    std::vector<Hailstone> hailstones;
    for (std::string line : input) {
        // Remove whitespace and find the position of the @ sign.
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        int atSignPosition = line.find('@');

        // Tokenise and extract the position coordinates.
        std::istringstream positionStream(line.substr(0, atSignPosition));
        std::vector<long long> positionCoordinates;
        std::string positionCoordinate;
        for (int i = 0; i < 3; ++i) {
            std::getline(positionStream, positionCoordinate, ',');
            positionCoordinates.push_back(std::stold(positionCoordinate));
        }

        // Tokenise and extract the velocity components.
        std::istringstream velocityStream(line.substr(atSignPosition + 1));
        std::vector<long long> velocityComponents;
        std::string velocityComponent;
        for (int i = 0; i < 3; ++i) {
            std::getline(velocityStream, velocityComponent, ',');
            velocityComponents.push_back(std::stold(velocityComponent));
        }

        hailstones.emplace_back(Vector(positionCoordinates[0], positionCoordinates[1], positionCoordinates[2]),
                                Vector(velocityComponents[0], velocityComponents[1], velocityComponents[2]));
    }

    // Pick 3 random hailstones.
    std::vector<Hailstone> randomHailstones;
    std::sample(hailstones.begin(), hailstones.end(), std::back_inserter(randomHailstones), 3, std::mt19937(std::time(nullptr)));

    // Create the equations.
    std::vector<Equation> equations;
    auto [equation1, equation2, equation3] = getEquations(randomHailstones[0], randomHailstones[1]);
    equations.push_back(equation1);
    equations.push_back(equation2);
    equations.push_back(equation3);
    auto [equation4, equation5, equation6] = getEquations(randomHailstones[1], randomHailstones[2]);
    equations.push_back(equation4);
    equations.push_back(equation5);
    equations.push_back(equation6);

    // Create the augmented matrix.
    std::vector<std::vector<long double>> augmentedMatrix;
    for (const Equation& equation : equations) {
        augmentedMatrix.push_back({static_cast<long double>(equation.a_px),
                                   static_cast<long double>(equation.a_py),
                                   static_cast<long double>(equation.a_pz),
                                   static_cast<long double>(equation.a_vx),
                                   static_cast<long double>(equation.a_vy),
                                   static_cast<long double>(equation.a_vz),
                                   static_cast<long double>(equation.a_)});
    }

    // Use Gaussian Elimination to solve the system of linear equations.
    std::vector<long double> answers = gaussianElimination(augmentedMatrix);

    // Return the sum of the rock's p0 coordinates.
    return answers[0] + answers[1] + answers[2];
}

std::tuple<Equation, Equation, Equation> getEquations(const Hailstone& hailstone, const Hailstone& hailstonePrime) {
    Vector p0 = hailstone.p0;
    Vector v = hailstone.v;

    Vector p0Prime = hailstonePrime.p0;
    Vector vPrime = hailstonePrime.v;

    long long a_px, a_py, a_pz, a_vx, a_vy, a_vz, a_;

    // Create an equation using X and Y.
    a_px = vPrime.y - v.y;
    a_py = v.x - vPrime.x;
    a_pz = 0;
    a_vx = p0.y - p0Prime.y;
    a_vy = p0Prime.x - p0.x;
    a_vz = 0;
    a_ = (p0.y * v.x) - (p0.x * v.y) - (p0Prime.y * vPrime.x) + (p0Prime.x * vPrime.y);
    Equation equationXY = Equation(a_px, a_py, a_pz, a_vx, a_vy, a_vz, a_);

    // Create an equation using X and Z.
    a_px = vPrime.z - v.z;
    a_py = 0;
    a_pz = v.x - vPrime.x;
    a_vx = p0.z - p0Prime.z;
    a_vy = 0;
    a_vz = p0Prime.x - p0.x;
    a_ = (p0.z * v.x) - (p0.x * v.z) - (p0Prime.z * vPrime.x) + (p0Prime.x * vPrime.z);
    Equation equationXZ = Equation(a_px, a_py, a_pz, a_vx, a_vy, a_vz, a_);

    // Create an equation using Y and Z.
    a_px = 0;
    a_py = vPrime.z - v.z;
    a_pz = v.y - vPrime.y;
    a_vx = 0;
    a_vy = p0.z - p0Prime.z;
    a_vz = p0Prime.y - p0.y;
    a_ = (p0.z * v.y) - (p0.y * v.z) - (p0Prime.z * vPrime.y) + (p0Prime.y * vPrime.z);
    Equation equationYZ = Equation(a_px, a_py, a_pz, a_vx, a_vy, a_vz, a_);

    return {equationXY, equationXZ, equationYZ};
}

std::vector<long double> gaussianElimination(const std::vector<std::vector<long double>>& augmentedMatrix) {
    // Copy the augmented matrix.
    std::vector<std::vector<long double>> rowEchelonMatrix = augmentedMatrix;

    // Create the row echelon matrix.
    for (size_t diagonalIndex = 0; diagonalIndex < rowEchelonMatrix.size(); diagonalIndex++) {
        // If the diagonal element is 0, swap it with a row below that has a
        // non-zero element in that column.
        if (std::abs(rowEchelonMatrix[diagonalIndex][diagonalIndex] - 0.0L) < 0.000001L) {
            bool swapped = false;
            for (size_t rowIndex = diagonalIndex + 1; rowIndex < rowEchelonMatrix.size(); rowIndex++) {
                if (std::abs(rowEchelonMatrix[rowIndex][diagonalIndex] - 0.0L) >= 0.000001L)  {
                    std::swap(rowEchelonMatrix[diagonalIndex], rowEchelonMatrix[rowIndex]);
                    swapped = true;
                    break;
                }
            }
            if (!swapped) {
                //assert(false && "Could not find a non-zero row to swap with!");
            }
        }

        // Ensure all rows below have a 0 in this column.
        for (size_t rowIndex = diagonalIndex + 1; rowIndex < rowEchelonMatrix.size(); rowIndex++) {
            long double multiplier = rowEchelonMatrix[rowIndex][diagonalIndex] / rowEchelonMatrix[diagonalIndex][diagonalIndex];
            for (size_t colIndex = diagonalIndex; colIndex < rowEchelonMatrix[0].size(); colIndex++) {
                rowEchelonMatrix[rowIndex][colIndex] -= multiplier * rowEchelonMatrix[diagonalIndex][colIndex];
            }
        }
    }

    // Create a vector to store the variables. Initially, the variables are all
    // 0.
    std::vector<long double> answers(augmentedMatrix.size(), 0.0L);

    // Perform back substituion to find the values of the variables.
    for (size_t answerIndex = answers.size(); answerIndex > 0; answerIndex--) {
        long double answer = rowEchelonMatrix[answerIndex - 1].back();
        for (size_t colIndex = 0; colIndex < rowEchelonMatrix[0].size() - 1; colIndex++) {
            answer -= rowEchelonMatrix[answerIndex - 1][colIndex] * answers[colIndex];
        }
        // We know all our variables will be integers, so we round the answer.
        answers[answerIndex - 1] = std::round(answer / rowEchelonMatrix[answerIndex - 1][answerIndex - 1]);
    }

    return answers;
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
