#include "Entity.hpp"

Entity::Entity(const Point2D& position, double radius, unsigned int color,double velocity, double direction): Circle(position, radius, color),velocity(velocity),direction(direction) {
    objType = ObjectType::ENTITY;
}

Point2D Entity::calculateMovementVector(double speed, double angleOffset) const {
    double moveAngle = direction + angleOffset;
    return Point2D(speed * cos(moveAngle), speed * sin(moveAngle));
}

Entity::~Entity() {}
