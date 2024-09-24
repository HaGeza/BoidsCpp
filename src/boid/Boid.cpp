#include "boid/Boid.hpp"

#include <iostream>

Boid::Boid() : pos({0, 0}) {}

Boid::Boid(double x, double y, double rotation, double speed)
    : pos({x, y}), rotation(rotation), speed(speed) {}

Boid::Boid(Vec2 pos, double rotation, double speed)
    : pos(pos), rotation(rotation), speed(speed) {}

double Boid::getX() const { return pos.x; }

double Boid::getY() const { return pos.y; }

double Boid::getRotation() const { return rotation; }

Vec2 Boid::getUpdate(const uset<size_t>& neighbors, const vec<Boid>& boids,
                     dd radii, ddd forceWeights, double worldSize,
                     bool periodicBoundary) const {
    uint neighborCount = neighbors.size();
    uint closeNeighborCount = 0;

    Vec2 cohesion = {0, 0}, separation = {0, 0}, alignment = {0, 0}, diff,
         closest, pushedNeighbor, push, pushedDiff;
    double sepRadius2 = radii.first * radii.first;

    for (size_t i : neighbors) {
        closest = boids[i].pos;
        diff = pos - boids[i].pos;
        if (periodicBoundary) {
            pushedNeighbor = {
                boids[i].pos.x +
                    (pos.x > boids[i].pos.x ? worldSize : -worldSize),
                boids[i].pos.y +
                    (pos.y > boids[i].pos.y ? worldSize : -worldSize),
            };
            pushedDiff = pos - pushedNeighbor;

            if (abs(pushedDiff.x) < abs(diff.x)) {
                diff.x = pushedDiff.x;
                closest.x = pushedNeighbor.x;
            }
            if (abs(pushedDiff.y) < abs(diff.y)) {
                diff.y = pushedDiff.y;
                closest.y = pushedNeighbor.y;
            }
        }

        cohesion += closest;
        alignment += {cos(boids[i].rotation), sin(boids[i].rotation)};

        if (diff.squaredL2Norm() < sepRadius2) {
            separation += diff;
            closeNeighborCount++;
        }
    }

    Vec2 update = {speed * cos(rotation), speed * sin(rotation)};

    double cohesionSpeed = std::get<0>(forceWeights),
           alignmentSpeed = std::get<1>(forceWeights),
           separationSpeed = std::get<2>(forceWeights);
    double weightSum = cohesionSpeed + alignmentSpeed + separationSpeed;

    cohesionSpeed = cohesionSpeed / weightSum * speed;
    alignmentSpeed = alignmentSpeed / weightSum * speed;
    separationSpeed = separationSpeed / weightSum * speed;

    if (neighborCount > 0) {
        update += (cohesion / neighborCount - pos) * cohesionSpeed +
                  alignment / neighborCount * alignmentSpeed;

        if (closeNeighborCount > 0) {
            update += separation * separationSpeed / closeNeighborCount;
        }
    }

    if (!periodicBoundary) {
        double threshold = worldSize * 0.15;
        Vec2 avoidance = {0, 0};

        if (pos.x < threshold) {
            avoidance.x = threshold - pos.x;
        } else if (worldSize - pos.x < threshold) {
            avoidance.x = -threshold + worldSize - pos.x;
        }

        if (pos.y < threshold) {
            avoidance.y = threshold - pos.y;
        } else if (worldSize - pos.y < threshold) {
            avoidance.y = -threshold + worldSize - pos.y;
        }

        update += avoidance * speed * 4;
    }

    return update;
}

void Boid::update(Vec2 change, double worldSize, bool periodicBoundary) {
    if (periodicBoundary) {
        pos.x = std::fmod(worldSize + pos.x + change.x, worldSize);
        pos.y = std::fmod(worldSize + pos.y + change.y, worldSize);
    } else {
        pos.x =
            std::max(0.0001, std::min(worldSize - 0.0001, pos.x + change.x));
        pos.y =
            std::max(0.0001, std::min(worldSize - 0.0001, pos.y + change.y));
    }
    rotation = atan2(change.y, change.x);
}

std::ostream& operator<<(std::ostream& os, const Vec2& v2) {
    return os << v2.x << " " << v2.y;
}
