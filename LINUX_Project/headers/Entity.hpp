#pragma once

#include "Circle.hpp"

class Map;

class Entity : public Circle {
protected:
    double velocity;
    double direction;

public:
    Entity(const Point2D& position, double radius, unsigned int color, double velocity = 150.0, double direction = 0.0);
    virtual ~Entity();

    virtual void moveWithKeyboard(double deltaTime, const Map& map) = 0;
    Point2D calculateMovementVector(double speed, double angleOffset = 0.0) const;

    double getVelocity() const { return velocity; }
    double getDirection() const { return direction; }

    void setVelocity(double vel) { velocity = vel; }
    void setDirection(double dir) { direction = dir; }
};
