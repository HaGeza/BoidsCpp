#ifndef VEC2_HPP
#define VEC2_HPP

#include <ostream>

struct Vec2 {
    double x, y;

    Vec2 operator+(const Vec2& rhs) const;
    Vec2 operator+=(const Vec2& rhs);

    Vec2 operator-(const Vec2& rhs) const;
    Vec2 operator-=(const Vec2& rhs);

    Vec2 operator*(const double multiplier) const;
    Vec2 operator*=(const double multiplier);

    Vec2 operator/(const double divisor) const;
    Vec2 operator/=(const double divisor);

    double squaredL2Norm();
};

std::ostream& operator<<(std::ostream& os, const Vec2& v2);

#endif  // VEC2_HPP
