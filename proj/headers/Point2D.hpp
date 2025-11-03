#pragma once

#include <iostream>
#include <stdexcept>

class Point2D
{
private:
    double x, y;
public:
    Point2D();
    Point2D(double _x, double _y);
    Point2D(const Point2D& other);
    ~Point2D();

    Point2D& operator=(const Point2D& other);
    Point2D operator+(const Point2D& other)const;
    Point2D operator-(const Point2D& other)const;
    bool operator==(const Point2D& other)const;
    bool operator!=(const Point2D& other)const;

    int getX()const;
    int getY()const;
    void setX(const double _x);
    void setY(const double _y);
    void setPoint(const double _x, const double _y);
};
