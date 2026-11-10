#pragma once

#include <iostream>
#include <stdexcept>
#include <cmath>

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

    double distance(const Point2D& other)const;
    double getX()const;
    double getY()const;
    void setX(const double _x);
    void setY(const double _y);
    void setPoint(const double _x, const double _y);

    friend class Polygon2D;

    /////
    Point2D operator*(double scalar) const {
        return Point2D(x * scalar, y * scalar);
    }

    Point2D operator/(double scalar) const {
        return Point2D(x / scalar, y / scalar);
    }

    Point2D& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Point2D& operator/=(double scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    ///
};
