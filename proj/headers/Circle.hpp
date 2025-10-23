#pragma once

#include "Object2D.hpp"

class Circle : public Object2D
{
private:
    sf::CircleShape circle;
    float radius;
public:
    Circle(const Point& _position, float _radius);
    void setRadius(float _radius);
    void setFillColor(const sf::Color& _color);
    void setOutLine(const sf::Color& _color, float _thickness);
    void draw(sf::RenderWindow& window) const;
    float getRadius() const;
};
