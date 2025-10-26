#pragma once
#include <SFML/Graphics.hpp>
#include "Point2D.hpp"

class Object2D
{
protected:
    Point2D position;
public:
    Object2D();
    Object2D(const Point2D& _position);
    ~Object2D();
    virtual Point2D getPos() const;
    virtual void setPos(const Point2D& _position);
};
