#include "IDisplay.hpp"

class ASCIIDisplay : public IDisplay {
    uint numCols;
    uint numRows;
    vec<vec<char>> grid;

   public:
    ASCIIDisplay(uint numCols, uint numRows);
    void initialize(vec<Boid> boids) override;
    void update(vec<Boid> boids) override;
};
