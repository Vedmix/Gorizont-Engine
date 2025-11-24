#pragma once
#include <vector>
#include "Object2D.hpp"

class Polygon2D:public Object2D
{
protected:
    std::vector<Point2D> points;
    std::vector<Point2D> pointsOnPlane;
    bool pointOnSegment(const Point2D& a, const Point2D& b, const Point2D& p);
public:
    Polygon2D(const Point2D& _position, const std::vector<Point2D> _points, unsigned int _color);
    Polygon2D(const Polygon2D& other);
    void draw(sf::RenderTarget& window, const double mapScale) override;
    bool isCrossing(const Point2D& p)override;
    void setPos(const Point2D& _position) override;
    ~Polygon2D();

    ////
     const std::vector<Point2D>& getPoints() const { return points; }
    ///
};
