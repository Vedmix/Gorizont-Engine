#pragma once

#include <iostream>
#include <stdexcept>

class Point
{
private:
    std::size_t x, y;
public:
    Point(size_t _x, size_t _y);
    Point(const Point& other);
    ~Point();

    Point& operator=(const Point& other);
    Point operator+(const Point& other)const;
    Point operator-(const Point& other)const;

    int getX()const;
    int getY()const;
    void setX(const size_t _x);
    void setY(const size_t _y);
    void setPoint(const size_t _x, const size_t _y);
};
