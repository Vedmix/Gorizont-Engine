#include <iostream>
#include <stdexcept>

class Point
{
private:
    std::size_t x, y;
public:
    //constructors, destructors
    Point(size_t _x, size_t _y);
    Point(const Point& other); //copy constructor
    ~Point();

    //operators
    Point& operator=(const Point& other);
    Point operator+(const Point& other)const;
    Point operator-(const Point& other)const;
    
    //get/set-methods
    int getX()const;
    int getY()const;
    void setX(const size_t _x);
    void setY(const size_t _y);
    void setPoint(const size_t _x, const size_t _y);
};