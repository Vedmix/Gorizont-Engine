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
    static constexpr double height = 1080;
    Object2D(const Point2D& _position, const unsigned int _color);
    Object2D(const Object2D& other);
    virtual ~Object2D() = default;
    
    ObjectType getObjectType() const;
    Point2D getPos() const;
    void setColor(unsigned int _color);
    virtual void setPos(const Point2D& _position)=0;
    virtual bool isCrossing(const Point2D& point)=0;
    virtual void draw(sf::RenderTarget& window, const double mapScale) = 0;

    /////
    sf::Color getColor() const { return color; }
    ///
};
