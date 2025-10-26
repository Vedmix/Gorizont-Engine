#include "../headers/Object2D.hpp"

Object2D::Object2D():position(0,0)
{

}
Object2D::Object2D(const Point2D& _position):position(_position)
{

}

Point2D Object2D::getPos() const
{
    return position;
}

void Object2D::setPos(const Point2D& _position)
{
    this->position = _position;
}

ObjectType Object2D::getObjectType(){return objType;}
