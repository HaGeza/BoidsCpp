#include "IDisplay.hpp"

class ASCIIDisplay : IDisplay {
   public:
    ASCIIDisplay(uint numCols, uint numRows);
    void initialize(vec<Boid> boids) override;
    void update(vec<Boid> boids) override;
};
