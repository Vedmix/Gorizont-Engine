#include "../headers/Object2D.hpp"

Object2D::Object2D():position(0,0)
{

}
Object2D::Object2D(const Point& _position):position(_position)
{

}

Object2D::~Object2D()
{

}

Point Object2D::getPos() const
{
    return position;
}

void Object2D::setPos(const Point& _position)
{
    this->position = _position;
}
