#include "../headers/Polygon2D.hpp"

Polygon2D::Polygon2D(const Point2D& _position, const std::vector<Point2D> _points, unsigned int _color) :Object2D(_position, _color)
{
    objType = POLYGON;
    points = _points;
}

Polygon2D::Polygon2D(const Polygon2D& other):Object2D(other){
    this->points = other.points;
}

Polygon2D::~Polygon2D(){}

void Polygon2D::draw(sf::RenderWindow& window){
    sf::ConvexShape polygon;
    polygon.setPointCount(points.size());
    int i=0;
    for(auto& point:points){
        polygon.setPoint(i, sf::Vector2f(point.getX(), point.getY()));
        i++;
    }
    polygon.setFillColor(color);
    polygon.setPosition(position.getX(), position.getY());
    window.draw(polygon);
}