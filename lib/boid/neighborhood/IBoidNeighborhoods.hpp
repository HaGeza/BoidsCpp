#ifndef IBOID_NEIGHBORHOODS_HPP
#define IBOID_NEIGHBORHOODS_HPP

#include "boid/Boid.hpp"
#include "typedefs.hpp"

class IBoidNeighborhoods {
   public:
    virtual umap<size_t, uset<size_t>> calculate(const vec<Boid>& boids) = 0;
};

#endif  // IBOID_NEIGHBORHOODS_HPP
