#include "../headers/Circle.hpp"

Circle::Circle(const Point2D& _position, double _radius, unsigned int _color) :Object2D(_position, {}, _color), radius(_radius)
{
    objType = CIRCLE;
}

Circle::~Circle(){}

void Circle::setRadius(double _radius){
    this->radius = _radius;
}

double Circle::getRadius() const{
    return radius;
}
