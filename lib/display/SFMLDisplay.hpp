#include "IDisplay.hpp"

class SFMLDisplay : IDisplay {
   public:
    SFMLDisplay(uint pxWidth, uint pxHeight);
    void initialize(vec<Boid> boids) override;
    void update(vec<Boid> boids) override;
};
