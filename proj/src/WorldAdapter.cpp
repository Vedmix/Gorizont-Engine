#include "../headers/WorldAdapter.hpp"
#include <QDebug>
#include <cmath>

WorldAdapter::WorldAdapter(Map& _map, Camera& _camera, World& _world)
    : map(_map), camera(_camera), world(_world) {}

// Оптимизированный raycasting с черно-белой схемой
void WorldAdapter::drawCameraView(sf::RenderTarget& target) {
    // СИЛЬНОЕ УМЕНЬШЕНИЕ ЛУЧЕЙ ДЛЯ FPS
    int numRays = SCREEN_WIDTH / 6; // Немного увеличил для качества
    double fov = PI / 2;
    double renderDistance = 700.0;

    double rayAngleStep = fov / numRays;
    double sectorWidth = (double)SCREEN_WIDTH / numRays;

    Point2D cameraPos = camera.getPos();
    double cameraDir = camera.getDirection();

    // Начальный угол (левый край FOV)
    double startAngle = cameraDir - fov / 2;

    // Черный фон (пол и потолок) - ОДИН РАЗ
    target.clear(sf::Color::Black);

    // БЫСТРЫЙ raycasting
    for (int i = 0; i < numRays; i++) {
        double rayAngle = startAngle + i * rayAngleStep;
        double distance = ultraFastCastRay(cameraPos, rayAngle, renderDistance);

        if (distance < renderDistance) {
            drawWallSegmentBW(target, i, sectorWidth, distance);
        }
    }

    // Рисуем БОЛЬШУЮ мини-карту в левом верхнем углу
    drawMinimap(target);
}

// СУПЕР-БЫСТРЫЙ raycasting (ИСПРАВЛЕННАЯ СИГНАТУРА)
double WorldAdapter::ultraFastCastRay(const Point2D& start, double angle, double maxDistance) {
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);

    // БОЛЬШОЙ ШАГ для максимальной производительности
    for (double t = 8.0; t < maxDistance; t += 8.0) {
        Point2D testPoint;
        testPoint.setX(start.getX() + t * cosAngle);
        testPoint.setY(start.getY() + t * sinAngle);

        // Быстрая проверка пересечений
        for (const auto& obj : map.getObjects()) {
            if (obj->getObjectType() == ObjectType::CAMERA) continue;

            if (obj->isCrossing(testPoint)) {
                return t;
            }
        }
    }

    return maxDistance;
}

// Черно-белый сегмент стены
void WorldAdapter::drawWallSegmentBW(sf::RenderTarget& target, int segmentIndex,
                                     double sectorWidth, double distance) {
    // Упрощенная формула высоты стены
    double wallHeight = (SCREEN_HEIGHT * 1.5) / (distance * 0.02 + 0.001);
    wallHeight = std::min(wallHeight, (double)SCREEN_HEIGHT * 1.5);

    // Позиция сегмента
    double xPos = segmentIndex * sectorWidth;
    double yPos = (SCREEN_HEIGHT - wallHeight) / 2;

    // Создаем сегмент стены
    sf::RectangleShape wallSegment(sf::Vector2f(sectorWidth + 1, wallHeight));
    wallSegment.setPosition(xPos, yPos);

    // Черно-белая схема: яркость зависит от расстояния
    int brightness = 255 - (distance / 700.0) * 200;
    brightness = std::max(brightness, 55);

    wallSegment.setFillColor(sf::Color(brightness, brightness, brightness));

    target.draw(wallSegment);
}

// БОЛЬШАЯ мини-карта в левом верхнем углу
void WorldAdapter::drawMinimap(sf::RenderTarget& target) {
    int minimapSize = 300; // УВЕЛИЧИЛ ДО 300px (ОЧЕНЬ БОЛЬШАЯ)
    int margin = 10;

    // ФИКСИРОВАННЫЕ ГРАНИЦЫ КАРТЫ (1920x1050)
    double mapWidth = 1920.0;
    double mapHeight = 1050.0;
    double mapScale = minimapSize / std::max(mapWidth, mapHeight);

    // Сохраняем текущий вид
    sf::View originalView = target.getView();

    // Устанавливаем вид для мини-карты (фиксированные координаты)
    sf::View minimapView(sf::FloatRect(0, 0, mapWidth, mapHeight));
    target.setView(minimapView);

    // ОПТИМИЗИРОВАННЫЙ РЕНДЕРИНГ - только видимые объекты
    for (const auto& obj : map.getObjects()) {
        if (obj->getObjectType() == ObjectType::CAMERA) continue;

        if (auto circle = std::dynamic_pointer_cast<Circle>(obj)) {
            Point2D pos = circle->getPos();
            double radius = circle->getRadius();

            // Позиция на фиксированной карте
            double mapX = pos.getX() * mapScale + margin;
            double mapY = pos.getY() * mapScale + margin;

            sf::CircleShape shape(radius * mapScale);
            shape.setFillColor(sf::Color(200, 200, 200));
            shape.setPosition(mapX, mapY);
            shape.setOrigin(radius * mapScale, radius * mapScale);
            target.draw(shape);
        }
        else if (auto polygon = std::dynamic_pointer_cast<Polygon2D>(obj)) {
            const std::vector<Point2D>& points = polygon->getPoints();
            if (points.empty()) continue;

            Point2D pos = polygon->getPos();

            sf::ConvexShape shape;
            shape.setPointCount(points.size());

            for (size_t i = 0; i < points.size(); ++i) {
                double pointX = (pos.getX() + points[i].getX()) * mapScale + margin;
                double pointY = (pos.getY() + points[i].getY()) * mapScale + margin;
                shape.setPoint(i, sf::Vector2f(pointX, pointY));
            }

            shape.setFillColor(sf::Color(150, 150, 150));
            target.draw(shape);
        }
    }

    // Камера на мини-карте
    Point2D cameraPos = camera.getPos();
    double camMapX = cameraPos.getX() * mapScale + margin;
    double camMapY = cameraPos.getY() * mapScale + margin;

    // Камера - зеленый круг (увеличил размер)
    sf::CircleShape cameraShape(8);
    cameraShape.setFillColor(sf::Color::Green);
    cameraShape.setPosition(camMapX, camMapY);
    cameraShape.setOrigin(8, 8);
    target.draw(cameraShape);

    // Направление камеры на мини-карте (увеличил длину)
    double camDirection = camera.getDirection();
    double dirX = cos(camDirection) * 30;
    double dirY = sin(camDirection) * 30;

    sf::Vertex directionLine[] = {
        sf::Vertex(sf::Vector2f(camMapX, camMapY), sf::Color::Red),
        sf::Vertex(sf::Vector2f(camMapX + dirX, camMapY + dirY), sf::Color::Red)
    };
    target.draw(directionLine, 2, sf::Lines);

    // Восстанавливаем оригинальный вид
    target.setView(originalView);
}

// Основной метод рендеринга
void WorldAdapter::renderWorld(sf::RenderTarget& target) {
    // Очищаем целевой буфер
    target.clear(sf::Color::Black);

    // Отрисовка 3D вида с мини-картой
    drawCameraView(target);
}

// Пустые методы (для компиляции)
void WorldAdapter::renderObject(std::shared_ptr<Object2D> obj, sf::RenderTarget& target, double scale) {
    Q_UNUSED(obj); Q_UNUSED(target); Q_UNUSED(scale);
}

void WorldAdapter::drawObjectOnMap(std::shared_ptr<Object2D> obj, sf::RenderTarget& target, double mapScale) {
    Q_UNUSED(obj); Q_UNUSED(target); Q_UNUSED(mapScale);
}

void WorldAdapter::renderMap(sf::RenderTarget& target) {
    Q_UNUSED(target);
}

void WorldAdapter::drawMapOnMap(sf::RenderTarget& target) {
    Q_UNUSED(target);
}

void WorldAdapter::renderCircle(std::shared_ptr<Circle> circle, sf::RenderTarget& target, double scale) {
    Q_UNUSED(circle); Q_UNUSED(target); Q_UNUSED(scale);
}

void WorldAdapter::renderPolygon(std::shared_ptr<Polygon2D> polygon, sf::RenderTarget& target, double scale) {
    Q_UNUSED(polygon); Q_UNUSED(target); Q_UNUSED(scale);
}

void WorldAdapter::update(double deltaTime) {
    Q_UNUSED(deltaTime);
}

void WorldAdapter::handleEvents() {
}

void WorldAdapter::drawCameraOnMap(sf::RenderTarget& target) {
    Q_UNUSED(target);
}
