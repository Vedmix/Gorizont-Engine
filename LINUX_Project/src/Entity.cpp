#include "Entity.hpp"

Entity::Entity(const Point2D& position, double radius, unsigned int color,double velocity, double direction): Circle(position, radius, color),velocity(velocity),direction(direction) {
    objType = ObjectType::ENTITY;
}

Entity::~Entity() {}
