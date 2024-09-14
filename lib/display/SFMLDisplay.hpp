#include <SFML/Graphics.hpp>

#include "IDisplay.hpp"

class SFMLDisplay : public IDisplay {
    uint pxWidth, pxHeight;
    sf::RenderWindow window;
    double baseHalf, altThird, alt2Thirds;

    static constexpr double RAD_TO_DEG = 180.0 / M_PI;

    sf::ConvexShape createTriangle(double x, double y, double rotation);

   public:
    SFMLDisplay(uint pxWidth, uint pxHeight);
    void initialize(vec<Boid> boids) override;
    void update(vec<Boid> boids) override;
};
