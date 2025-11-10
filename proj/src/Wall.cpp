#include "../headers/Wall.hpp"
#include <cmath>

// Конструктор с int
Wall::Wall(const Point2D& _position, const int _width, const int _length, unsigned int _color)
    : Polygon2D(_position, {}, _color), rotation(0), width(static_cast<double>(_width)), length(static_cast<double>(_length)) {
    objType = ObjectType::POLYGON;
    updatePoints();
}

// Конструктор с double
Wall::Wall(const Point2D& _position, double _width, double _length, unsigned int _color)
    : Polygon2D(_position, {}, _color), rotation(0), width(_width), length(_length) {
    objType = ObjectType::POLYGON;
    updatePoints();
}

Wall::Wall(const Wall& other)
    : Polygon2D(other), rotation(other.rotation), width(other.width), length(other.length) {
    updatePoints();
}

Wall::~Wall(){}

void Wall::setRotation(double angle) {
    rotation = angle;
    updatePoints();
}

double Wall::getRotation() const {
    return rotation;
}

void Wall::updatePoints() {
    double halfWidth = width / 2.0;
    double halfLength = length / 2.0;
    
    // ИСПРАВЛЕННЫЕ БАЗОВЫЕ ТОЧКИ
    std::vector<Point2D> basePoints = {
        Point2D(-halfWidth, -halfLength),
        Point2D( halfWidth, -halfLength),  
        Point2D( halfWidth,  halfLength),
        Point2D(-halfWidth,  halfLength)
    };
    
    if (rotation != 0) {
        double cosAngle = std::cos(rotation);
        double sinAngle = std::sin(rotation);
        
        points.resize(4);
        for (size_t i = 0; i < basePoints.size(); ++i) {
            double x = basePoints[i].getX() * cosAngle - basePoints[i].getY() * sinAngle;
            double y = basePoints[i].getX() * sinAngle + basePoints[i].getY() * cosAngle;
            points[i] = Point2D(x, y);
        }
    } else {
        points = basePoints;
    }
    
    updatePointsOnPlane();
}