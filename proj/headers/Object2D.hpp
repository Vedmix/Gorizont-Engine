#pragma once
#include <SFML/Graphics.hpp>
#include "Point2D.hpp"
#include "ObjectType.hpp"

class Object2D
{
protected:
    Point2D position;
    ObjectType objType;
public:
    Object2D();
    Object2D(const Point2D& _position);
    ~Object2D();
    ObjectType getObjectType();
    Point2D getPos() const;
    void setPos(const Point2D& _position);
};
