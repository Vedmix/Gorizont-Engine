#pragma once

#include "Object2D.hpp"

class Circle : public Object2D
{
protected:
    double radius;
public:
    Circle(const Point2D& _position, double _radius, unsigned int _color);
    Circle(const Circle& other);
    ~Circle();
    void setRadius(double _radius);
    double getRadius() const;
};
