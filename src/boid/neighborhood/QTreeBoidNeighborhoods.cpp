#include "boid/neighborhood/QTreeBoidNeighborhoods.hpp"

#include "qtree/QTree.hpp"

QTreeBoidNeighborhoods::QTreeBoidNeighborhoods(double size,
                                               uint maxBoidsPerQuad,
                                               double boidNeighborhoodRadius)
    : size(size),
      maxBoidsPerQuad(maxBoidsPerQuad),
      boidNeighborhoodRadius(boidNeighborhoodRadius) {}

umap<size_t, uset<size_t>> QTreeBoidNeighborhoods::calculate(
    const vec<Boid>& boids, bool periodicBoundary) {
    if (!periodicBoundary) {
        auto qtree = QTree(size, maxBoidsPerQuad);
        for (size_t i = 0; i < boids.size(); i++) {
            qtree.insert(boids[i].getX(), boids[i].getY(), i);
        }
        return qtree.getNeighborhoods(boidNeighborhoodRadius);
    } else {
        auto qtree = QTree(size * 2, maxBoidsPerQuad);
        size_t numBoids = boids.size();

        for (size_t i = 0; i < numBoids; i++) {
            double x = boids[i].getX(), y = boids[i].getY();
            qtree.insert(x, y, i);
            qtree.insert(x + size, y, i + numBoids);
            qtree.insert(x, y + size, i + 2 * numBoids);
            qtree.insert(x + size, y + size, i + 3 * numBoids);
        }
        auto neighborhoodsExtended =
            qtree.getNeighborhoods(boidNeighborhoodRadius);

        umap<size_t, uset<size_t>> neighborhoods;
        for (size_t i = 0; i < numBoids; i++) {
            neighborhoods[i] = uset<size_t>();
        }

        for (auto [i, neighbors] : neighborhoodsExtended) {
            size_t actualI = i % numBoids;
            for (size_t j : neighbors) {
                neighborhoods[actualI].insert(j % numBoids);
            }
        }
        return neighborhoods;
    }
}
