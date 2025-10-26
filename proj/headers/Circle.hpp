#pragma once

#include "Object2D.hpp"

class Circle : public Object2D
{
protected:
    sf::CircleShape circle;
    double radius;
public:
    Circle(const Point2D& _position, double _radius);
    ~Circle();
    void setRadius(double _radius);
    void setFillColor(const sf::Color& _color);
    void setOutLine(const sf::Color& _color, double _thickness);
    void draw(sf::RenderWindow& window);
    double getRadius() const;
};
