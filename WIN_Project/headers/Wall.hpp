#pragma once
#include "Polygon2D.hpp"

class Wall : public Polygon2D
{
public:
    Wall(const Point2D& _position, const double _width, const double _lenght, unsigned int _color);
    Wall(const Wall& other);
    ~Wall();

    void setSize(const double _width, const double _lenght);
};