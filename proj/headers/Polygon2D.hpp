#include <vector>
#include "Object2D.hpp"

class Polygon2D:public Object2D
{
private:
    std::vector<Point2D> points;
public:
    Polygon2D();
    Polygon2D(const Point2D& _position, const std::vector<Point2D>& _points);
    void draw();
    ~Polygon2D();
};