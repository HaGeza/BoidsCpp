#include "Boid.hpp"

Boid::Boid() : x(0), y(0) {}

Boid::Boid(double x, double y, double rotation)
    : x(x), y(y), rotation(rotation) {}

double Boid::getX() const { return x; }

double Boid::getY() const { return y; }

double Boid::getRotation() const { return rotation; }
