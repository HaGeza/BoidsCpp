#include "display/SFMLDisplay.hpp"

#include <iostream>

SFMLDisplay::SFMLDisplay(uint pxWidth, uint pxHeight)
    : pxWidth(pxWidth),
      pxHeight(pxHeight),
      window(sf::VideoMode(pxWidth, pxHeight), "Boids"),
      running(false) {
    baseHalf = 0.025 * pxHeight;
    altThird = 0.025 * pxHeight;
    alt2Thirds = 2 * altThird;
}

SFMLDisplay::~SFMLDisplay() {
    running = false;
    if (renderThread.joinable()) {
        renderThread.join();
    }
}

sf::ConvexShape SFMLDisplay::createTriangle(double x, double y,
                                            double rotation) {
    sf::ConvexShape triangle(3);

    triangle.setPoint(0, sf::Vector2f(-altThird, baseHalf));
    triangle.setPoint(1, sf::Vector2f(-altThird, -baseHalf));
    triangle.setPoint(2, sf::Vector2f(alt2Thirds, 0));

    triangle.setPosition(x * pxWidth, y * pxHeight);
    triangle.setRotation(rotation * RAD_TO_DEG);

    return triangle;
}

void SFMLDisplay::startRenderLoop() {
    window.setActive(true);
    sf::Clock clock;
    auto frameDuration = sf::milliseconds(FRAME_MS);

    while (running) {
        clock.restart();
        {
            std::lock_guard<std::mutex> lock(windowMutex);
            window.clear();
            {
                std::lock_guard<std::mutex> lock(boidsMutex);
                for (Boid& boid : boids) {
                    double x = boid.getX(), y = boid.getY(),
                           rotation = boid.getRotation();
                    window.draw(createTriangle(x, y, rotation));
                }
            }
            window.display();
        }

        auto elapsed = clock.getElapsedTime();
        if (elapsed < frameDuration) {
            sf::sleep(frameDuration - elapsed);
        }
    }
}

void SFMLDisplay::initialize(vec<Boid> boids) {
    {
        std::lock_guard<std::mutex> lock(boidsMutex);
        this->boids = boids;
    }
    running = true;
    window.setActive(false);
    renderThread = std::thread([this]() { startRenderLoop(); });
}

void SFMLDisplay::update(vec<Boid> boids) {
    std::lock_guard<std::mutex> lock(boidsMutex);
    this->boids = boids;
}

void SFMLDisplay::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            std::lock_guard<std::mutex> lock(windowMutex);
            running = false;
            window.close();
        }
    }
}

bool SFMLDisplay::isRunning() { return running; }
