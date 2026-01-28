#pragma once

#include "Circle.hpp"
#include "Map.hpp"

class Entity : public Circle {
protected:
    double velocity;
    double direction;

public:
    Entity(const Point2D& position, double radius, unsigned int color, double velocity = 150.0, double direction = 0.0);
    ~Entity();

    Point2D calculateMovementVector(double speed, double angleOffset = 0.0) const;

    bool isPositionFree(const Point2D& checkPos, const Map& map) const;
    bool canMoveTo(const Point2D& targetPos, const Map& map) const;
    void applyWallSliding(const Point2D& startPos, const Point2D& desiredMove, const Map& map);

    double getVelocity() const { return velocity; }
    double getDirection() const { return direction; }

    void setVelocity(double vel) { velocity = vel; }
    void setDirection(double dir) { direction = dir; }
};
