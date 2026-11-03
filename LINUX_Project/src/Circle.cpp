#include "../headers/Circle.hpp"

Circle::Circle(const Point2D& _position, double _radius, unsigned int _color) :Object2D(_position, _color), radius(_radius)
{
    objType = ObjectType::CIRCLE;
}

Circle::Circle(const Circle& other):Object2D(other){
    this->radius = other.radius;
}

Circle::~Circle(){}

void Circle::setRadius(double _radius){
    this->radius = _radius;
}

void Circle::setPos(const Point2D& pos){
    position = pos;
}

double Circle::getRadius() const{
    return radius;
}

void Circle::draw(sf::RenderTarget& window, const double mapScale){
    sf::CircleShape cirShape;
    cirShape.setRadius(radius*mapScale);
    cirShape.setPosition(sf::Vector2f(position.getX()*mapScale, position.getY()*mapScale));
    cirShape.setFillColor(color);
    cirShape.setOrigin(sf::Vector2f(radius*mapScale, radius*mapScale));
    window.draw(cirShape);
}

bool Circle::isCrossing(const Point2D& point){
    return position.distance(point) <= radius;
}


void Circle::setRadiusQt(double _radius){
    this->radius = qBound(10.0, _radius, 1000.0);
}

void Circle::drawQt(QPainter& painter){
    painter.setBrush(QColor(color.r, color.g, color.b, color.a));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(position.getX(), position.getY()), radius, radius);
}

void Circle::drawSelection(QPainter& painter){
    QPointF center(position.getX(), position.getY());

    QPen pen(QColor(80, 160, 255), 2);
    pen.setCosmetic(true);          // толщина в пикселях экрана, а не в единицах карты
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(center, radius, radius);

    QVector<QPointF> handles;
    handles.push_back(center + QPointF(radius, 0));
    handles.push_back(center + QPointF(-radius, 0));
    handles.push_back(center + QPointF(0, radius));
    handles.push_back(center + QPointF(0, -radius));

    painter.setBrush(Qt::white);
    for (const QPointF& handle : handles) {
        painter.drawEllipse(handle, 4, 4);
    }
}

// Возвращает 0, если точка у границы круга, иначе -1
int Circle::hitHandle(const Point2D& point, double tolerance) const{
    double distance = position.distance(point);
    if (qAbs(distance - radius) <= tolerance) {
        return 0;
    }
    return -1;
}

void Circle::dragHandle(int handle, const Point2D& point){
    Q_UNUSED(handle);   // у круга одна ручка
    setRadiusQt(position.distance(point));
}

QRectF Circle::boundingRect() const{
    return QRectF(position.getX() - radius, position.getY() - radius, radius * 2, radius * 2);
}
