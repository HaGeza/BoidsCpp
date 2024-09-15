#include "BoidManager.hpp"

#include <math.h>

#define RAND_DBL ((double)rand() / RAND_MAX)

BoidManager::BoidManager(IDisplay* display, uint numBoids) : display(display) {
    boids = vec<Boid>(numBoids);

    for (uint i = 0; i < numBoids; i++) {
        boids[i] = Boid(RAND_DBL, RAND_DBL, RAND_DBL * M_PI * 2);
    }

    display->initialize(boids);

    while (display->isRunning()) {
        display->handleEvents();
    }
}

BoidManager::~BoidManager() {}

void BoidManager::start() {}

void BoidManager::stop() {}
