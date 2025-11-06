#include "../headers/Wall.hpp"

Wall::Wall(const Point2D& _position, const int _width, const int _lenght, unsigned int _color):Polygon2D(_position, {}, _color){
    double width = _width / 2;
    double lenght = _lenght / 2;
    double x = _position.getX();
    double y = _position.getY();
    points.resize(4);
    points[0]=(Point2D(x + lenght, y + width));
    points[1]=(Point2D(x + lenght, y - width));
    points[2]=(Point2D(x - lenght, y - width));
    points[3]=(Point2D(x - lenght, y + width));
}

Wall::Wall(const Wall& other):Polygon2D(other){

}

Wall::~Wall(){}
