#pragma once
#include <vector>
#include "Object2D.hpp"

class Polygon2D:public Object2D
{
public:
    Polygon2D(const Point2D& _position, const std::vector<Point2D> _points, unsigned int _color);
    Polygon2D(const Polygon2D& other);
    ~Polygon2D();
};