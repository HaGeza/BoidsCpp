#ifndef BOID_HPP
#define BOID_HPP

#include "boid/Vec2.hpp"
#include "typedefs.hpp"

class Boid {
    Vec2 pos;
    double rotation, speed;
    Vec2 getUpdate(const uset<size_t>& neighbors, const vec<Boid>& boids,
                   dd radii, ddd forceWeights, double gridSize,
                   bool periodicBoundary = true) const;

    void update(Vec2 change, double size, bool periodicBoundary);

   public:
    Boid();
    Boid(double x, double y, double rotation, double speed);
    Boid(Vec2 pos, double rotation, double speed);

    double getX() const;
    double getY() const;
    double getRotation() const;

    friend class BoidManager;
};

#endif  // BOID_HPP
