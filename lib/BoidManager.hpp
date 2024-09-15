#include <atomic>
#include <chrono>
#include <thread>

#include "display/IDisplay.hpp"

class BoidManager {
    IDisplay* display;
    vec<Boid> boids;
    // QTree* qTree;
    // ThreadPool* threadPool;
    std::atomic<bool> running;
    std::thread simulationThread;

    void startSimulation();

   public:
    BoidManager(IDisplay* display, uint numBoids);
    ~BoidManager();

    void run();
};
