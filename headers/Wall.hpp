#pragma once
#include "Polygon2D.hpp"

class Wall : public Polygon2D
{
public:
    Wall(const Point2D& _position, const double _width, const double _lenght, unsigned int _color);
    Wall(const Wall& other);
    ~Wall();

    void setSize(const double _width, const double _lenght);
    double getWidth() const  { return points[2].getX(); }
    double getHeight() const { return points[2].getY(); }

    void drawQt(QPainter& painter) override;
    void drawSelection(QPainter& painter) override;
    int  hitHandle(const Point2D& point, double tolerance) const override;
    void dragHandle(int handle, const Point2D& point) override;
    QRectF boundingRect() const override;
    QString toXml() const override;

};
