#include "boid/BoidManager.hpp"

#include <math.h>

#include <iostream>

#define RAND_DBL ((double)rand() / RAND_MAX)

BoidManager::BoidManager(IDisplay* display, uint numBoids,
                         IBoidNeighborhoods* boidNeighborhood, dd boidRadii,
                         dd boidSpeed, ddd boidForceWeights, uint randomSeed,
                         bool periodicBoundary)
    : display(display),
      boidNeighborhoods(boidNeighborhood),
      running(false),
      boidRadii(boidRadii),
      boidSpeed(boidSpeed),
      boidForceWeights(boidForceWeights),
      periodicBoundary(periodicBoundary) {
    boids = vec<Boid>(numBoids);

    double speedRange = boidSpeed.second - boidSpeed.first;

    srand(randomSeed);
    for (uint i = 0; i < numBoids; i++) {
        boids[i] = Boid(RAND_DBL, RAND_DBL, RAND_DBL * M_PI * 2,
                        RAND_DBL * speedRange + boidSpeed.first);
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
            boidNeighborhoods->calculate(boids, periodicBoundary);

        vec<Vec2> updates(boids.size());
        for (size_t i = 0; i < boids.size(); i++) {
            updates[i] =
                boids[i].getUpdate(neighborhoods[i], boids, boidRadii,
                                   boidForceWeights, SIZE, periodicBoundary);
        }

        for (size_t i = 0; i < boids.size(); i++) {
            boids[i].update(updates[i], SIZE, periodicBoundary);
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
