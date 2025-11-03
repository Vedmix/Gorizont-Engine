#include "../headers/Object2D.hpp"

Object2D::Object2D(const Point2D& _position, const std::vector<Point2D> _points, const unsigned int _color):position(_position),points(_points)
{
    color = sf::Color(_color);
}

Object2D::Object2D(const Object2D& other){
    this->position = other.position;
    this->points = other.points;
    this->color = other.color;
    this->objType = other.objType;
}

Point2D Object2D::getPos() const
{
    return position;
}

void Object2D::setColor(unsigned int _color){
    color = sf::Color(_color);
}

void Object2D::setPos(const Point2D& _position)
{
    this->position = _position;
}

void Object2D::draw(sf::RenderWindow& window){
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

ObjectType Object2D::getObjectType(){return objType;}
