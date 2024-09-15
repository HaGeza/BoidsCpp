#include "boid/neighborhood/QTreeBoidNeighborhoods.hpp"

#include "qtree/QTree.hpp"

QTreeBoidNeighborhoods::QTreeBoidNeighborhoods(double size,
                                               uint maxBoidsPerQuad,
                                               double boidNeighborhoodRadius)
    : size(size),
      maxBoidsPerQuad(maxBoidsPerQuad),
      boidNeighborhoodRadius(boidNeighborhoodRadius) {}

umap<size_t, uset<size_t>> QTreeBoidNeighborhoods::calculate(
    const vec<Boid>& boids) {
    auto qtree = QTree(size, maxBoidsPerQuad);
    for (size_t i = 0; i < boids.size(); i++) {
        qtree.insert(boids[i].getX(), boids[i].getY(), i);
    }
    return qtree.getNeighborhoods(boidNeighborhoodRadius);
}
