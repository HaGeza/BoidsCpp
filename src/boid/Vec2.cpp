#include "boid/Vec2.hpp"

Vec2 Vec2::operator+(const Vec2& rhs) const { return {x + rhs.x, y + rhs.y}; }
Vec2 Vec2::operator+=(const Vec2& rhs) { return {x += rhs.x, y += rhs.y}; }

Vec2 Vec2::operator-(const Vec2& rhs) const { return {x - rhs.x, y - rhs.y}; }
Vec2 Vec2::operator-=(const Vec2& rhs) { return {x -= rhs.x, y -= rhs.y}; }

Vec2 Vec2::operator*(const double multiplier) const {
    return {x * multiplier, y * multiplier};
}
Vec2 Vec2::operator*=(const double multiplier) {
    return {x *= multiplier, y *= multiplier};
}

Vec2 Vec2::operator/(const double divisor) const {
    return {x / divisor, y / divisor};
}
Vec2 Vec2::operator/=(const double divisor) {
    return {x /= divisor, y /= divisor};
}

double Vec2::squaredL2Norm() { return x * x + y * y; }
