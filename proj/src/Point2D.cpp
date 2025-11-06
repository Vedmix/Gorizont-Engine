#include "../headers/Point2D.hpp"

Point2D::Point2D(){
    x=0;
    y=0;
}

Point2D::Point2D(double _x, double _y){
    x=_x;
    y=_y;
}

Point2D::Point2D(const Point2D& other){
    x=other.x;
    y=other.y;
}

Point2D::~Point2D(){}

Point2D& Point2D::operator=(const Point2D& other){
    if(this==&other){
        return *this;
    }
    x=other.x;
    y=other.y;
    return *this;
}

Point2D Point2D::operator+(const Point2D& other)const{
    Point2D res(x+other.x, y+other.y);
    return res;
}
Point2D Point2D::operator-(const Point2D& other)const{
    if(x<other.x || y<other.y){
        throw std::invalid_argument("Result Point2D coordinates must be more than 0!\n");
    }
    Point2D res(x+other.x, y+other.y);
    return res;
}

bool Point2D::operator==(const Point2D& other)const{
    return this->x == other.x && this->y == other.y;
}

bool Point2D::operator!=(const Point2D& other)const{
    return !(this->x == other.x && this->y == other.y);
}

double Point2D::getX()const{return x;}
double Point2D::getY()const{return y;}
void Point2D::setX(const double _x){x=_x;}
void Point2D::setY(const double _y){y=_y;}
void Point2D::setPoint(const double _x, const double _y){x=_x;y=_y;}