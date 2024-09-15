#include "IDisplay.hpp"

class ASCIIDisplay : public IDisplay {
    uint numCols;
    uint numRows;
    vec<vec<char>> grid;

   public:
    ASCIIDisplay(uint numCols, uint numRows);
    void initialize(const vec<Boid>& boids) override;
    void update(const vec<Boid>& boids) override;

    void updateGrid(const vec<Boid>& boids);
    void writeGrid();
};
