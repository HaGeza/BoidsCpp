#include <SFML/Graphics.hpp>
#include <atomic>
#include <mutex>
#include <thread>

#include "IDisplay.hpp"

class SFMLDisplay : public IDisplay {
    // Display properties
    uint pxWidth, pxHeight;
    sf::RenderWindow window;
    double baseHalf, altThird, alt2Thirds;

    // Threading
    vec<Boid> boids;
    std::mutex boidsMutex;
    std::mutex windowMutex;
    std::thread renderThread;
    std::atomic<bool> running;

    // Constants
    static constexpr uint FRAME_MS = 16;
    static constexpr double RAD_TO_DEG = 180.0 / M_PI;

    // Helper function
    sf::ConvexShape createTriangle(double x, double y, double rotation);

    // Main loop
    void startRenderLoop();

   public:
    SFMLDisplay(uint pxWidth, uint pxHeight);
    ~SFMLDisplay();
    void initialize(const vec<Boid>& boids) override;
    void update(const vec<Boid>& boids) override;
    void handleEvents() override;
    bool isRunning() override;
};
