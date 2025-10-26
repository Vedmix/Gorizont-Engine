#include "../headers/Circle.hpp"

Circle::Circle(const Point2D& _position, double _radius) :Object2D(_position), radius(_radius)
{
    circle.setRadius(radius);
    circle.setPosition(position.getX(),position.getY());
    circle.setFillColor(sf::Color::White);
}

void Circle::setRadius(double _radius){
    this->radius = _radius;
    circle.setRadius(radius);
}

void Circle::setFillColor(const sf::Color &_color){
    circle.setFillColor(_color);
}

void Circle::setOutLine(const sf::Color &_color, double _thickness){
    circle.setOutlineColor(_color);
    circle.setOutlineThickness(_thickness);
}

void Circle::draw(sf::RenderWindow& window) const {
    window.draw(circle);
}

double Circle::getRadius() const{
    return radius;
}
