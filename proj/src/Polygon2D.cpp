#include "../headers/Polygon2D.hpp"

Polygon2D::Polygon2D(){
    position = Point2D(0, 0);
    objType=POLYGON;
}

Polygon2D::Polygon2D(const Point2D& _position, const std::vector<Point2D>& _points):Object2D(_position){
    points = _points;
    objType=POLYGON;
}

Polygon2D::~Polygon2D(){}

void Polygon2D::draw(){
    sf::ConvexShape polygon;
    
}