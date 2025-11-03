#include "../headers/Polygon2D.hpp"

Polygon2D::Polygon2D(const Point2D& _position, std::vector<Point2D> _points, unsigned int _color) :Object2D(_position, _points, _color)
{
    objType = POLYGON;
}

Polygon2D::Polygon2D(const Polygon2D& other):Object2D(other){}

Polygon2D::~Polygon2D(){}