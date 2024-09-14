#include "display/IDisplay.hpp"

class BoidManager {
    IDisplay* display;
    vec<Boid> boids;
    // QTree* qTree;
    // ThreadPool* threadPool;

   public:
    BoidManager(IDisplay* display, uint numBoids);
    ~BoidManager();

    void start();
    void stop();
};
