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

void Circle::setPos(const Point2D& pos){
    position = pos;
}

double Circle::getRadius() const{
    return radius;
}

void Circle::draw(sf::RenderTarget& window, const double mapScale){
    sf::CircleShape cirShape;
    cirShape.setRadius(radius*mapScale);
    cirShape.setPosition(position.getX()*mapScale, position.getY()*mapScale);
    cirShape.setFillColor(color);
    cirShape.setOrigin(radius*mapScale, radius*mapScale);
    window.draw(cirShape);
}

bool Circle::isCrossing(const Point2D& point){
    return position.distance(point) <= radius;
}
