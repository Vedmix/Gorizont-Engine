#include "../headers/Object2D.hpp"

Object2D::Object2D(const Point2D& _position, const unsigned int _color):position(_position)
{
    color = sf::Color(_color);
}

Object2D::Object2D(const Object2D& other){
    this->position = other.position;
    this->color = other.color;
    this->objType = other.objType;
}

Point2D Object2D::getPos() const
{
    return position;
}

void Object2D::setColor(unsigned int _color){
    color = sf::Color(_color);
}

void Object2D::setPos(const Point2D& _position)
{
    this->position = _position;
}

ObjectType Object2D::getObjectType() const{
    return objType;
}

bool Object2D::isPositionFree(const Point2D& checkPos, const Map& map) const {
    return true;
}

bool Object2D::canMoveTo(const Point2D& targetPos, const Map& map) const {
    return isPositionFree(targetPos, map);
}
