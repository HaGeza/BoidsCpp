#include "Boid.hpp"
#include "typedefs.hpp"

class IDisplay {
    virtual void initialize(vec<Boid> boids);
    virtual void update(vec<Boid> boids);
};
