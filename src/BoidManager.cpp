#include "BoidManager.hpp"

#include <math.h>

#define RAND_DBL ((double)rand() / RAND_MAX)

BoidManager::BoidManager(IDisplay* display, uint numBoids) : display(display) {
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

        int i = 0;
        for (Boid& boid : boids) {
            boid.x = std::fmod(1.0 + boid.x + cos(boid.rotation) * 0.01, 1.0);
            boid.y = std::fmod(1.0 + boid.y + sin(boid.rotation) * 0.01, 1.0);
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
