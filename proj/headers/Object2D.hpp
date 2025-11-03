#pragma once
#include <SFML/Graphics.hpp>
#include "Point2D.hpp"
#include "ObjectType.hpp"

class Object2D
{
protected:
    Point2D position;
    std::vector<Point2D> points;
    sf::Color color;
    ObjectType objType;
public:
    Object2D(const Point2D& _position, std::vector<Point2D> _points, const unsigned int _color); //int color in hexadecimal (example: 0xFF00AAFF (RGBA format. A - ALPHA for opacity))
    Object2D(const Object2D& other);
    virtual ~Object2D()=default;
    ObjectType getObjectType();
    Point2D getPos() const;
    void setColor(unsigned int _color);
    void setPos(const Point2D& _position);
    void draw(sf::RenderWindow& window);
};
