#include "../headers/Circle.hpp"

Circle::Circle(const Point2D& _position, double _radius, unsigned int _color) :Object2D(_position, {}, _color), radius(_radius)
{
    objType = CIRCLE;
    double angle;
    Point2D point;
    for(int i=0;i<CIRCLE_FACES_NUMBER;i++){
        angle = (double)i / CIRCLE_FACES_NUMBER * 2*PI;
        point.setPoint(radius*cos(angle), radius*sin(angle));
        points.push_back(point);
    }
}

Circle::Circle(const Circle& other):Object2D(other){
    this->radius = other.radius;
}

Circle::~Circle(){}

void Circle::setRadius(double _radius){
    this->radius = _radius;
}

double Circle::getRadius() const{
    return radius;
}
