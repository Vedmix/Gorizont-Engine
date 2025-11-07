#include "../headers/Circle.hpp"

Circle::Circle(const Point2D& _position, double _radius, unsigned int _color) :Object2D(_position, _color), radius(_radius)
{
    objType = ObjectType::CIRCLE;
}

Circle::Circle(const Circle& other):Object2D(other){
    this->radius = other.radius;
}

Circle::~Circle(){}

void Circle::setRadius(double _radius){
    this->radius = _radius;
}

double Circle::getRadius() const{
    return radius;
}

void Circle::draw(sf::RenderWindow& window){
    sf::CircleShape cirShape;
    cirShape.setRadius(radius);
    cirShape.setPosition(position.getX(), position.getY());
    cirShape.setFillColor(color);
    cirShape.setOrigin(radius, radius);
    window.draw(cirShape);
}