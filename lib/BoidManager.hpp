#include "IDisplay.hpp"

class BoidManager {
   public:
    BoidManager(const IDisplay& display);
    BoidManager(const IDisplay& display, unsigned int numBoids);
    ~BoidManager();

    void start();
    void stop();
};
