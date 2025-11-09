#include "../headers/Wall.hpp"

Wall::Wall(const Point2D& _position, const int _width, const int _lenght, unsigned int _color):Polygon2D(_position, {}, _color){
    objType = ObjectType::POLYGON;
    
    double width = _width / 2;
    double lenght = _lenght / 2;
    points.resize(4);
    points[1]=(Point2D(_width, 0));
    points[2]=(Point2D(_width, _lenght));
    points[3]=(Point2D(0, _lenght));
    points[0]=(Point2D(0, 0));
    pointsOnPlane.resize(points.size());
    for(size_t i=0; i<points.size();i++){
        pointsOnPlane[i] = points[i]+position;
    }
}

Wall::Wall(const Wall& other):Polygon2D(other){

}

Wall::~Wall(){}
