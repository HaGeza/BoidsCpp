#ifndef BOID_HPP
#define BOID_HPP

class Boid {
    double x, y, rotation;

   public:
    Boid();
    Boid(double x, double y, double rotation);

    double getX() const;
    double getY() const;
    double getRotation() const;

    friend class BoidManager;
};

#endif  // BOID_HPP
