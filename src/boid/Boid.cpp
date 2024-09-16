#include "boid/Boid.hpp"

#include <iostream>

Boid::Boid() : x(0), y(0) {}

Boid::Boid(double x, double y, double rotation)
    : x(x), y(y), rotation(rotation) {}

double Boid::getX() const { return x; }

double Boid::getY() const { return y; }

double Boid::getRotation() const { return rotation; }

dd Boid::getUpdate(const uset<size_t>& neighbors, const vec<Boid>& boids,
                   double speed, double separationRadius, double gridSize,
                   bool periodicBoundary) const {
    uint neighborCount = neighbors.size();
    uint closeNeibghborCount = 0;

    dd cohesion = {0, 0}, separation = {0, 0}, alignment = {0, 0}, diff,
       closest, pushedNeighbor, push, pushedDiff;
    double seperationRadius2 = separationRadius * separationRadius;

    for (size_t i : neighbors) {
        closest = {boids[i].x, boids[i].y};
        diff = {x - boids[i].x, y - boids[i].y};
        if (periodicBoundary) {
            pushedNeighbor = {
                boids[i].x + (x > boids[i].x ? gridSize : -gridSize),
                boids[i].y + (y > boids[i].y ? gridSize : -gridSize),
            };
            pushedDiff = {x - pushedNeighbor.first, y - pushedNeighbor.second};

            if (abs(pushedDiff.first) < abs(diff.first)) {
                diff.first = pushedDiff.first;
                closest.first = pushedNeighbor.first;
            }
            if (abs(pushedDiff.second) < abs(diff.second)) {
                diff.second = pushedDiff.second;
                closest.second = pushedNeighbor.second;
            }
        }

        cohesion.first += closest.first;
        cohesion.second += closest.second;

        // TODO: use boid specific speed
        alignment.first += cos(boids[i].rotation);
        alignment.second += sin(boids[i].rotation);

        if (pow(diff.first, 2) + pow(diff.second, 2) < seperationRadius2) {
            separation.first += diff.first;
            separation.second += diff.second;
            closeNeibghborCount++;
        }
    }

    dd update = {speed * cos(rotation), speed * sin(rotation)};

    if (neighborCount > 0) {
        cohesion.first /= neighborCount;
        cohesion.second /= neighborCount;
        // TODO: use weight parameter
        update.first += (cohesion.first - x) * 0.3 * speed;
        update.second += (cohesion.second - y) * 0.3 * speed;

        alignment.first /= neighborCount;
        alignment.second /= neighborCount;
        // TODO: use weight parameter
        update.first += alignment.first * 0.2 * speed;
        update.second += alignment.second * 0.2 * speed;

        if (closeNeibghborCount > 0) {
            separation.first /= closeNeibghborCount;
            separation.second /= closeNeibghborCount;
            // TODO: use weight parameter
            update.first += separation.first * 0.5 * speed;
            update.second += separation.second * 0.5 * speed;

            // std::cout << "separation: " << separation.first << ", "
            //           << separation.second << std::endl;
        }
    }

    return update;
}

void Boid::update(dd change, double size) {
    // TODO: account for periodic boundary conditions
    x = fmod(size + x + change.first, size);
    y = fmod(size + y + change.second, size);
    rotation = (rotation + atan2(change.second, change.first)) / 2;
}
