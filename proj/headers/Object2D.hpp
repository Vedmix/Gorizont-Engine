#pragma once
#include <SFML/Graphics.hpp>
#include "Point.hpp"

class Object2D
{
protected:
    Point position;
public:
    Object2D();
    Object2D(const Point& _position);
    ~Object2D();
    virtual Point getPos() const;
    virtual void setPos(const Point& _position);
};
