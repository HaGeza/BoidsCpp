#ifndef BOID_HPP
#define BOID_HPP

#include "typedefs.hpp"

class Boid {
    double x, y, rotation;
    dd getUpdate(const uset<size_t>& neighbors, const vec<Boid>& boids,
                 double speed, double separationRadius, double gridSize,
                 bool periodicBoundary) const;

    void update(dd change, double size);

   public:
    Boid();
    Boid(double x, double y, double rotation);

    double getX() const;
    double getY() const;
    double getRotation() const;

    friend class BoidManager;
};

#endif  // BOID_HPP
