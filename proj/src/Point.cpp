#include "../headers/Point.hpp"

Point::Point(size_t _x, size_t _y){
    x=_x;
    y=_y;
}

Point::Point(const Point& other){
    x=other.x;
    y=other.y;
}

Point::~Point(){}

Point& Point::operator=(const Point& other){
    if(this==&other){
        return *this;
    }
    x=other.x;
    y=other.y;
    return *this;
}

Point Point::operator+(const Point& other)const{
    Point res(x+other.x, y+other.y);
    return res;
}
Point Point::operator-(const Point& other)const{
    if(x<other.x || y<other.y){
        throw std::invalid_argument("Result point coordinates must be more than 0!\n");
    }
    Point res(x+other.x, y+other.y);
    return res;
}

bool Point::operator==(const Point& other)const{
    return this->x == other.x && this->y == other.y;
}

bool Point::operator!=(const Point& other)const{
    return !(this->x == other.x && this->y == other.y);
}

int Point::getX()const{return x;}
int Point::getY()const{return y;}
void Point::setX(const size_t _x){x=_x;}
void Point::setY(const size_t _y){y=_y;}
void Point::setPoint(const size_t _x, const size_t _y){x=_x;y=_y;}