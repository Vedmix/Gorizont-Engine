#pragma once
#include "Polygon2D.hpp"

class Wall : public Polygon2D
{
private:
    double rotation; // угол вращения в радианах
    double width;    // ширина стены
    double length;   // длина стены
    
public:
    // Конструктор с int (для совместимости)
    Wall(const Point2D& _position, int _width, int _length, unsigned int _color);
    // Конструктор с double (для DebugPanel)
    Wall(const Point2D& _position, double _width, double _length, unsigned int _color);
    Wall(const Wall& other);
    ~Wall();
    void setRotation(double angle);
    double getRotation() const;
    
private:
    void updatePoints();
};