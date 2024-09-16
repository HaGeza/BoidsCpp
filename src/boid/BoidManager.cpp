#include "boid/BoidManager.hpp"

#include <math.h>

#include <iostream>

#define RAND_DBL ((double)rand() / RAND_MAX)

BoidManager::BoidManager(IDisplay* display, uint numBoids,
                         IBoidNeighborhoods* boidNeighborhood,
                         double boidNeighborhoodRadius, double boidSpeed)
    : display(display),
      boidNeighborhoods(boidNeighborhood),
      running(false),
      boidNeighborhoodRadius(boidNeighborhoodRadius),
      boidSeparationRadius(boidNeighborhoodRadius * 0.35),
      boidSpeed(boidSpeed) {
    boids = vec<Boid>(numBoids);

    for (uint i = 0; i < numBoids; i++) {
        boids[i] = Boid(RAND_DBL, RAND_DBL, RAND_DBL * M_PI * 2);
    }

    display->initialize(boids);
}

BoidManager::~BoidManager() {
    running = false;
    if (simulationThread.joinable()) {
        simulationThread.join();
    }
}

void BoidManager::startSimulation() {
    running = true;
    const auto frameDuration = std::chrono::milliseconds(16);

    while (running) {
        auto start = std::chrono::high_resolution_clock::now();

        umap<size_t, uset<size_t>> neighborhoods =
            // TODO: pass in periodicity as a parameter
            boidNeighborhoods->calculate(boids, true);

        vec<dd> updates(boids.size());
        for (size_t i = 0; i < boids.size(); i++) {
            updates[i] = boids[i].getUpdate(neighborhoods[i], boids, boidSpeed,
                                            boidSeparationRadius, SIZE, true);
        }

        for (size_t i = 0; i < boids.size(); i++) {
            boids[i].update(updates[i], SIZE);
        }

        display->update(boids);

        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        if (elapsed < frameDuration) {
            std::this_thread::sleep_for(frameDuration - elapsed);
        }
    }
}

void BoidManager::run() {
    simulationThread = std::thread([this]() { startSimulation(); });

    while (display->isRunning()) {
        display->handleEvents();
    }
}
