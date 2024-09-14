#pragma once

#include "Boid.hpp"
#include "typedefs.hpp"

class IDisplay {
   public:
    virtual void initialize(vec<Boid> boids) = 0;
    virtual void update(vec<Boid> boids) = 0;
};
