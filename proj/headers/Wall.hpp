#pragma once
#include "Polygon2D.hpp"

class Wall : public Polygon2D
{
public:
    Wall(const Point2D& _position, int _width, int _lenght, unsigned int _color);
    Wall(const Wall& other);
    ~Wall();
};