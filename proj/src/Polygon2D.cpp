#include "../headers/Polygon2D.hpp"

Polygon2D::Polygon2D(){
    position = Point2D(0, 0);
    objType=POLYGON;
}

Polygon2D::Polygon2D(const Point2D& _position, const std::vector<Point2D>& _points):Object2D(_position){
    points = _points;
    objType=POLYGON;
}

Polygon2D::Polygon2D(const Polygon2D& other){
    points = other.points;
    objType = POLYGON;
    position = other.position;
}

Polygon2D::~Polygon2D(){}

void Polygon2D::draw(sf::RenderWindow& window)const{
    sf::ConvexShape polygon;
    polygon.setPointCount(points.size());
    int i=0;
    for(auto& point:points){
        polygon.setPoint(i, sf::Vector2f(point.getX(), point.getY()));
        i++;
    }
    polygon.setFillColor(sf::Color::Red);
    polygon.setPosition(position.getX(), position.getY());
    window.draw(polygon);
}