#pragma once
#include "settings.hpp"
#include "Point2D.hpp"
#include "ObjectType.hpp"

class Object2D
{
protected:
    Point2D position;
    sf::Color color;
    ObjectType objType;
    
public:
    Object2D(const Point2D& _position, const unsigned int _color);
    Object2D(const Object2D& other);
    virtual ~Object2D() = default;
    
    ObjectType getObjectType() const;
    Point2D getPos() const;
    void setColor(unsigned int _color);
    void setPos(const Point2D& _position);
    virtual void draw(sf::RenderWindow& window) = 0;
};