#include "boid/Boid.hpp"

#include <iostream>

Boid::Boid() : x(0), y(0) {}

Boid::Boid(double x, double y, double rotation, double speed)
    : x(x), y(y), rotation(rotation), speed(speed) {}

double Boid::getX() const { return x; }

double Boid::getY() const { return y; }

double Boid::getRotation() const { return rotation; }

dd Boid::getUpdate(const uset<size_t>& neighbors, const vec<Boid>& boids,
                   dd radii, ddd forceWeights, double worldSize,
                   bool periodicBoundary) const {
    uint neighborCount = neighbors.size();
    uint closeNeighborCount = 0;

    dd cohesion = {0, 0}, separation = {0, 0}, alignment = {0, 0}, diff,
       closest, pushedNeighbor, push, pushedDiff;
    double sepRadius2 = radii.first * radii.first;

    for (size_t i : neighbors) {
        closest = {boids[i].x, boids[i].y};
        diff = {x - boids[i].x, y - boids[i].y};
        if (periodicBoundary) {
            pushedNeighbor = {
                boids[i].x + (x > boids[i].x ? worldSize : -worldSize),
                boids[i].y + (y > boids[i].y ? worldSize : -worldSize),
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

        alignment.first += cos(boids[i].rotation);
        alignment.second += sin(boids[i].rotation);

        if (pow(diff.first, 2) + pow(diff.second, 2) < sepRadius2) {
            separation.first += diff.first;
            separation.second += diff.second;
            closeNeighborCount++;
        }
    }

    dd update = {speed * cos(rotation), speed * sin(rotation)};

    double cohesionSpeed = std::get<0>(forceWeights) * speed,
           alignmentSpeed = std::get<1>(forceWeights) * speed,
           separationSpeed = std::get<2>(forceWeights) * speed;

    if (neighborCount > 0) {
        cohesion.first /= neighborCount;
        cohesion.second /= neighborCount;
        update.first += (cohesion.first - x) * cohesionSpeed;
        update.second += (cohesion.second - y) * cohesionSpeed;

        alignment.first /= neighborCount;
        alignment.second /= neighborCount;
        update.first += alignment.first * alignmentSpeed;
        update.second += alignment.second * alignmentSpeed;

        if (closeNeighborCount > 0) {
            separation.first /= closeNeighborCount;
            separation.second /= closeNeighborCount;
            update.first += separation.first * separationSpeed;
            update.second += separation.second * separationSpeed;
        }
    }

    if (!periodicBoundary) {
        double threshold = worldSize * 0.15;
        dd avoidance = {0, 0};

        if (x < threshold) {
            avoidance.first = threshold - x;
        } else if (worldSize - x < threshold) {
            avoidance.first = -threshold + worldSize - x;
        }

        if (y < threshold) {
            avoidance.second = threshold - y;
        } else if (worldSize - y < threshold) {
            avoidance.second = -threshold + worldSize - y;
        }

        update.first += speed * 4 * avoidance.first;
        update.second += speed * 4 * avoidance.second;
    }

    return update;
}

void Boid::update(dd change, double worldSize, bool periodicBoundary) {
    // double norm =
    //     std::sqrt(std::pow(change.first, 2) + std::pow(change.second, 2));

    // double scaler = std::min(1.0, 3 * speed / norm);
    // change = {change.first * scaler, change.second * scaler};

    if (periodicBoundary) {
        x = std::fmod(worldSize + x + change.first, worldSize);
        y = std::fmod(worldSize + y + change.second, worldSize);
    } else {
        x = std::max(0.0001, std::min(worldSize - 0.0001, x + change.first));
        y = std::max(0.0001, std::min(worldSize - 0.0001, y + change.second));
    }
    rotation = atan2(change.second, change.first);
}
