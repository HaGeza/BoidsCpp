#ifndef BOID_HPP
#define BOID_HPP

#include "typedefs.hpp"

class Boid {
    double x, y, rotation, speed;
    dd getUpdate(const uset<size_t>& neighbors, const vec<Boid>& boids,
                 dd radii, ddd forceWeights, double gridSize,
                 bool periodicBoundary = true) const;

    void update(dd change, double size, bool periodicBoundary);

   public:
    Boid();
    Boid(double x, double y, double rotation, double speed);

    double getX() const;
    double getY() const;
    double getRotation() const;

    friend class BoidManager;
};

#endif  // BOID_HPP
