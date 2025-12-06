#include "../headers/Polygon2D.hpp"

Polygon2D::Polygon2D(const Point2D& _position, const std::vector<Point2D> _points, unsigned int _color) :Object2D(_position, _color)
{
    objType = ObjectType::POLYGON;
    points = _points;
    pointsOnPlane.resize(points.size());
    for(size_t i=0; i<points.size();i++){
        pointsOnPlane[i] = points[i]+position;
    }
}

Polygon2D::Polygon2D(const Polygon2D& other):Object2D(other), points(other.points), pointsOnPlane(other.pointsOnPlane){}

Polygon2D::~Polygon2D(){}

void Polygon2D::draw(sf::RenderTarget& window, const double mapScale){
    sf::ConvexShape polygon;
    polygon.setPointCount(points.size());
    int i=0;
    for(auto& point:points){
        polygon.setPoint(i, sf::Vector2f(point.getX()*mapScale, point.getY()*mapScale));
        i++;
    }
    polygon.setFillColor(color);
    polygon.setPosition(position.getX()*mapScale, position.getY()*mapScale);
    window.draw(polygon);
}

bool Polygon2D::pointOnSegment(const Point2D& a, const Point2D& b, const Point2D& p) {
    const double eps = 1e-9;

    // Проверка коллинеарности (векторное произведение ~ 0)
    double cross = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
    if (std::fabs(cross) > eps)
        return false;

    // Проверка, что проекция точки p лежит между a и b
    double dot = (p.x - a.x) * (b.x - a.x) + (p.y - a.y) * (b.y - a.y);
    if (dot < -eps)
        return false;

    double len2 = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
    if (dot - len2 > eps)
        return false;

    return true;
}

void Polygon2D::setPos(const Point2D& pos){
    position = pos;
    for(size_t i=0; i<points.size();i++){
        pointsOnPlane[i] = points[i]+position;
    }
}

bool Polygon2D::isCrossing(const Point2D& p) {
    size_t n = points.size();
    if (n < 3)
        return false;

    const double eps = 1e-9;

    // Быстрая проверка bounding box — корректная версия сдвига точки, а не границ
    double minx = pointsOnPlane[0].x, maxx = pointsOnPlane[0].x;
    double miny = pointsOnPlane[0].y, maxy = pointsOnPlane[0].y;
    for (size_t i = 1; i < n; ++i) {
        minx = std::min(minx, pointsOnPlane[i].x);
        maxx = std::max(maxx, pointsOnPlane[i].x);
        miny = std::min(miny, pointsOnPlane[i].y);
        maxy = std::max(maxy, pointsOnPlane[i].y);
    }

    // Если точка явно вне bbox (с учётом eps), то вне полигона
    if (p.x + eps < minx || p.x - eps > maxx || p.y + eps < miny || p.y - eps > maxy){
        return false;
    }

    // Сначала проверяем, лежит ли точка на каком-то ребре
    for (size_t i = 0, j = n - 1; i < n; j = i++) {
        if (pointOnSegment(pointsOnPlane[j], pointsOnPlane[i], p))
            return true; // на границе считаем попавшей
    }

    // Алгоритм even-odd (ray casting)
    bool inside = false;
    for (size_t i = 0, j = n - 1; i < n; j = i++) {
        const Point2D& a = pointsOnPlane[j];
        const Point2D& b = pointsOnPlane[i];

        // Проверяем, пересекает ли горизонтальная линия y=p.y отрезок (a,b)
        bool intersect = ((a.y > p.y) != (b.y > p.y));
        if (intersect) {
            double x_intersect = a.x + (p.y - a.y) * (b.x - a.x) / (b.y - a.y);
            if (p.x < x_intersect)
                inside = !inside;
        }
    }

    return inside;
}


