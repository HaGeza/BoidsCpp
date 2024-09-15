#pragma once

#include "Boid.hpp"
#include "typedefs.hpp"

class IDisplay {
   public:
    virtual void initialize(const vec<Boid>& boids) = 0;
    virtual void update(const vec<Boid>& boids) = 0;
    virtual void handleEvents();
    virtual bool isRunning();
};
