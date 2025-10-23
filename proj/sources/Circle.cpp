#include "../headers/Circle.hpp"

Circle::Circle(const Point& _position, float _radius) :Object2D(_position), radius(_radius)
{
    circle.setRadius(radius);
    circle.setPosition(static_cast<float>(position.getX()),static_cast<float>(position.getY()));
    circle.setFillColor(sf::Color::White);
}

void Circle::setRadius(float _radius){
    this->radius = _radius;
    circle.setRadius(radius);
}

void Circle::setFillColor(const sf::Color &_color){
    circle.setFillColor(_color);
}

void Circle::setOutLine(const sf::Color &_color, float _thickness){
    circle.setOutlineColor(_color);
    circle.setOutlineThickness(_thickness);
}

void Circle::draw(sf::RenderWindow& window) const {
    window.draw(circle);
}

float Circle::getRadius() const{
    return radius;
}
