#ifndef BOIDMANAGER_HPP
#define BOIDMANAGER_HPP

#include <atomic>
#include <chrono>
#include <thread>

#include "boid/neighborhood/IBoidNeighborhoods.hpp"
#include "display/IDisplay.hpp"

class BoidManager {
    IDisplay* display;
    IBoidNeighborhoods* boidNeighborhoods;
    vec<Boid> boids;
    double boidNeighborhoodRadius, boidSeparationRadius, boidSpeed;

    std::atomic<bool> running;
    std::thread simulationThread;

    static constexpr double FRAME_MS = 16.0, SIZE = 1.0;

    void startSimulation();

   public:
    BoidManager(IDisplay* display, uint numBoids,
                IBoidNeighborhoods* boidNeighborhoods,
                double boidNeighborhoodRadius, double boidSpeed);
    ~BoidManager();

    void run();
};

#endif  // BOIDMANAGER_HPP
