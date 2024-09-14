#include "display/ASCIIDisplay.hpp"

#include <iostream>

ASCIIDisplay::ASCIIDisplay(uint numCols, uint numRows)
    : numCols(numCols),
      numRows(numRows),
      grid(vec<vec<char>>(numRows, vec<char>(numCols, ' '))) {}

void ASCIIDisplay::initialize(vec<Boid> boids) {
    for (const Boid& boid : boids) {
        uint x = static_cast<uint>(boid.getX() * numCols);
        uint y = static_cast<uint>(boid.getY() * numRows);

        double rotation = boid.getRotation();
        if (rotation < M_PI_2) {
            grid[y][x] = '>';
        } else if (rotation < M_PI) {
            grid[y][x] = 'v';
        } else if (rotation < 3 * M_PI_2) {
            grid[y][x] = '<';
        } else {
            grid[y][x] = '^';
        }
    }

    for (const auto& row : grid) {
        for (char cell : row) {
            std::cout << cell;
        }
        std::cout << std::endl;
    }
}

void ASCIIDisplay::update(vec<Boid> boids) { return; }
