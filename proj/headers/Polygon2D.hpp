#pragma once
#include <vector>
#include "Object2D.hpp"

class Polygon2D:public Object2D
{
protected:
    std::vector<Point2D> points;
public:
    Polygon2D(const Point2D& _position, const std::vector<Point2D> _points, unsigned int _color);
    Polygon2D(const Polygon2D& other);
    void draw(sf::RenderWindow& window) override;
    bool isCrossing(const Point2D& point) override;
    ~Polygon2D();
};