#ifndef QTREE_BOID_NEIGHBORHOODS_HPP
#define QTREE_BOID_NEIGHBORHOODS_HPP

#include "IBoidNeighborhoods.hpp"

class QTreeBoidNeighborhoods : public IBoidNeighborhoods {
    double size, boidNeighborhoodRadius;
    uint maxBoidsPerQuad;

   public:
    QTreeBoidNeighborhoods(double size, uint maxBoidsPerQuad,
                           double boidNeighborhoodRadius);

    umap<size_t, uset<size_t>> calculate(const vec<Boid>& boids,
                                         bool periodicBoundary) override;
};

#endif  // QTREE_BOID_NEIGHBORHOODS_HPP
