#include "../headers/Wall.hpp"

Wall::Wall(const Point2D& _position, const double _width, const double _lenght, unsigned int _color):Polygon2D(_position, {}, _color){
    objType = ObjectType::POLYGON;
    points.resize(4);
    points[1]=(Point2D(_width, 0));
    points[2]=(Point2D(_width, _lenght));
    points[3]=(Point2D(0, _lenght));
    points[0]=(Point2D(0, 0));
    pointsOnPlane.resize(points.size());
    for(size_t i=0; i<points.size();i++){
        pointsOnPlane[i] = points[i]+position;
    }
}

Wall::Wall(const Wall& other):Polygon2D(other){

}

Wall::~Wall(){}

void Wall::setSize(const double _width, const double _length){
    points[1]=(Point2D(_width, 0));
    points[2]=(Point2D(_width, _length));
    points[3]=(Point2D(0, _length));
    points[0]=(Point2D(0, 0));
    for(size_t i=0; i<points.size();i++){
        pointsOnPlane[i] = points[i]+position;
    }
}

QRectF Wall::boundingRect() const{
    return QRectF(position.getX(), position.getY(), getWidth(), getHeight());
}

void Wall::drawQt(QPainter& painter){
    painter.setBrush(QColor(color.r, color.g, color.b, color.a));
    painter.setPen(Qt::NoPen);
    painter.drawRect(boundingRect());
}

void Wall::drawSelection(QPainter& painter){
    QRectF rect = boundingRect();

    QPen pen(QColor(80, 160, 255), 2);
    pen.setCosmetic(true);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(rect);

    painter.setBrush(Qt::white);
    painter.drawEllipse(QPointF(rect.center().x(), rect.top()),    4, 4);   // верх
    painter.drawEllipse(QPointF(rect.right(),      rect.center().y()), 4, 4); // право
    painter.drawEllipse(QPointF(rect.center().x(), rect.bottom()), 4, 4);   // низ
    painter.drawEllipse(QPointF(rect.left(),       rect.center().y()), 4, 4); // лево
}

int Wall::hitHandle(const Point2D& point, double tolerance) const{
    QRectF rect = boundingRect();

    Point2D handles[4] = {
        Point2D(rect.center().x(), rect.top()),
        Point2D(rect.right(),      rect.center().y()),
        Point2D(rect.center().x(), rect.bottom()),
        Point2D(rect.left(),       rect.center().y())
    };

    int best = -1;
    double bestDistance = tolerance;
    for (int i = 0; i < 4; ++i) {
        double distance = handles[i].distance(point);
        if (distance <= bestDistance) {
            bestDistance = distance;
            best = i;
        }
    }
    return best;
}

void Wall::dragHandle(int handle, const Point2D& point){
    QRectF rect = boundingRect();

    double left   = rect.left();
    double right  = rect.right();
    double top    = rect.top();
    double bottom = rect.bottom();

    double x = qMax(0.0, point.getX());
    double y = qMax(0.0, point.getY());

    switch (handle) {
    case 0: top    = qMin(y, bottom - 10); break;
    case 1: right  = qMax(x, left + 10);   break;
    case 2: bottom = qMax(y, top + 10);    break;
    case 3: left   = qMin(x, right - 10);  break;
    }

    setPos(Point2D(left, top));
    setSize(right - left, bottom - top);
}

QString Wall::toXml() const{
    QString colorHex = QString::number(color.toInteger(), 16).rightJustified(8, '0').toUpper();

    return QString("        <wall x=\"%1\" y=\"%2\" width=\"%3\" height=\"%4\" color=\"0x%5\"/>\n")
        .arg(position.getX(), 0, 'f', 1)
        .arg(position.getY(), 0, 'f', 1)
        .arg(getWidth(), 0, 'f', 1)
        .arg(getHeight(), 0, 'f', 1)
        .arg(colorHex);
}
