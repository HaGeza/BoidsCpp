#include "boid/Boid.hpp"

Boid::Boid() : x(0), y(0) {}

Boid::Boid(double x, double y, double rotation)
    : x(x), y(y), rotation(rotation) {}

double Boid::getX() const { return x; }

double Boid::getY() const { return y; }

double Boid::getRotation() const { return rotation; }

dd Boid::getUpdate(const uset<size_t>& neighbors, const vec<Boid>& boids,
                   double speed, double separationRadius) const {
    uint neighborCount = neighbors.size();
    uint closeNeibghborCount = 0;

    dd cohesion = {0, 0}, separation = {0, 0}, alignment = {0, 0}, diff;
    double seperationRadius2 = separationRadius * separationRadius;

    for (size_t i : neighbors) {
        cohesion.first += boids[i].x;
        cohesion.second += boids[i].y;

        // TODO: use boid specific speed
        alignment.first += cos(boids[i].rotation);
        alignment.second += sin(boids[i].rotation);

        diff = {x - boids[i].x, y - boids[i].y};
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
        update.first += (cohesion.first - x) * 10 * speed;
        update.first += (cohesion.second - x) * 10 * speed;

        alignment.first /= neighborCount;
        alignment.second /= neighborCount;
        // TODO: use weight parameter
        update.first += alignment.first * speed;
        update.first += alignment.second * speed;

        if (closeNeibghborCount > 0) {
            separation.first /= closeNeibghborCount;
            separation.second /= closeNeibghborCount;
            // TODO: use weight parameter
            update.first += separation.first * 10 * speed;
            update.second += separation.second * 10 * speed;
        }
    }

    return update;
}

void Boid::update(dd change, double size) {
    // TODO: account for periodic boundary conditions
    x = fmod(size + x + change.first, size);
    y = fmod(size + y + change.second, size);
    rotation = atan2(change.second, change.first);
}
