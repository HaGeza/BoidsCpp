#include "display/SFMLDisplay.hpp"

#include <iostream>

SFMLDisplay::SFMLDisplay(uint pxWidth, uint pxHeight)
    : pxWidth(pxWidth),
      pxHeight(pxHeight),
      window(sf::VideoMode(pxWidth, pxHeight), "Boids") {
    baseHalf = 0.025 * pxHeight;
    altThird = 0.025 * pxHeight;
    alt2Thirds = 2 * altThird;
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

void SFMLDisplay::initialize(vec<Boid> boids) {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        std::cout << "initialize" << std::endl;
        for (Boid& boid : boids) {
            double x = boid.getX(), y = boid.getY(),
                   rotation = boid.getRotation();
            window.draw(createTriangle(x, y, rotation));
        }
        window.display();
    }
}

void SFMLDisplay::update(vec<Boid> boids) {
    // window.clear();
    // for (Boid& boid : boids) {
    //     sf::CircleShape circle(5);
    //     circle.setPosition(boid.position.x, boid.position.y);
    //     circle.setFillColor(sf::Color::White);
    //     window.draw(circle);
    // }
    // window.display();
}
