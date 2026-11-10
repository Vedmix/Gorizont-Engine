#pragma once

#include <QPixmap>
#include <QVector>
#include <QPointF>
#include <QRectF>
#include <QPainter>
#include <QMouseEvent>
#include <QLineF>

#include "Point2D.hpp"
#include "ObjectType.hpp"
#include <SFML/Graphics.hpp>

class Object2D
{
protected:
    Point2D position;
    sf::Color color;
    ObjectType objType;
public:
    static constexpr double height = 1080;
    Object2D(const Point2D& _position, const unsigned int _color);
    Object2D(const Object2D& other);
    virtual ~Object2D() = default;
    
    ObjectType getObjectType() const;
    Point2D getPos() const;
    void setColor(unsigned int _color);
    virtual void setPos(const Point2D& _position)=0;
    virtual bool isCrossing(const Point2D& point)=0;
    virtual void draw(sf::RenderTarget& window, const double mapScale) = 0;

    virtual void drawQt(QPainter& painter) = 0;
    virtual void drawSelection(QPainter& painter) = 0;
    virtual int  hitHandle(const Point2D& point, double tolerance) const = 0;
    virtual void dragHandle(int handle, const Point2D& point) = 0;
    virtual QRectF boundingRect() const = 0;
    virtual QString toXml() const = 0;

    sf::Color getColor() const { return color; }

};
