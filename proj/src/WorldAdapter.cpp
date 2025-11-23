#include "../headers/WorldAdapter.hpp"
#include <QDebug>
#include <cmath>
#include <vector>
#include <thread>

WorldAdapter::WorldAdapter(Map& _map, Camera& _camera, World& _world)
    : map(_map), camera(_camera), world(_world) {}

void WorldAdapter::drawCameraView(sf::RenderTarget& target) {
    sf::Vector2u targetSize = target.getSize();

    // Параметры как в оригинальной камере
    int numThreads = 10;
    int NUMBER_OF_RAYS_IN_FOV = targetSize.x / 10;
    double fov = PI / 2;

    double raySectorWidth = (double)targetSize.x / NUMBER_OF_RAYS_IN_FOV;

    // Вектор для хранения высот
    std::vector<double> heights(NUMBER_OF_RAYS_IN_FOV, -1);

    // МНОГОПОТОЧНЫЙ РАСЧЕТ ВЫСОТ
    std::vector<std::thread> threads;
    double rightAngle = camera.getDirection() - fov/2;
    double angleStep = fov/numThreads;
    double currRightAngle = rightAngle;
    double currLeftAngle = rightAngle + angleStep;

    // Запускаем потоки для расчета высот
    for(int i = 0; i < numThreads; i++) {
        threads.emplace_back(&WorldAdapter::calculateHeights, this, currLeftAngle, currRightAngle, i, std::ref(heights), NUMBER_OF_RAYS_IN_FOV, numThreads);
        currRightAngle += angleStep;
        currLeftAngle += angleStep;
    }

    // Ждем завершения всех потоков
    for(auto& thread : threads) {
        thread.join();
    }

    // Рисуем сегменты
    for(int i = 0; i < (int)heights.size(); i++) {
        drawOneCameraSegment(target, heights[i], i, raySectorWidth);
    }
}

void WorldAdapter::calculateHeights(double leftExtRay, double rightExtRay, int segmentNum, std::vector<double>& heights, int numRays, int numThreads) {
    double fov = PI / 2;
    double rayInterval = fov / numRays;
    int raysPerThread = numRays / numThreads;
    double renderDistance = 700.0;

    Point2D cameraPos = camera.getPos();

    for(double currAngle = rightExtRay, i = segmentNum * raysPerThread;
         currAngle < leftExtRay && i < (segmentNum + 1) * raysPerThread;
         currAngle += rayInterval, i++) {

        bool isCrossed = false;
        Point2D currRayEnd;
        double rayDistance = renderDistance;

        for(int j = 0; j < renderDistance && !isCrossed; j += 2) {
            for(auto& obj : map.getObjects()) {
                if(obj->getObjectType() == ObjectType::CAMERA) {
                    continue;
                }

                currRayEnd.setX(cameraPos.getX() + j * cos(currAngle));
                currRayEnd.setY(cameraPos.getY() + j * sin(currAngle));

                if(obj->isCrossing(currRayEnd)) {
                    isCrossed = true;
                    rayDistance = j;
                    break;
                }
            }
        }

        if(!isCrossed) {
            heights[i] = -1;
        } else {
            heights[i] = (Object2D::height - rayDistance * tan(atan(Object2D::height / rayDistance) - (PI / 120)));
        }
    }
}

// Отрисовка одного сегмента
void WorldAdapter::drawOneCameraSegment(sf::RenderTarget& target, double viewH, int segmentNum, double sectorWidth) {
    if(viewH == -1) {
        return;
    }

    sf::RectangleShape segment(sf::Vector2f(sectorWidth, viewH * 2));
    segment.setPosition(sectorWidth * segmentNum, SCREEN_HEIGHT / 2 - viewH);

    double brightness = 255 * (viewH / Object2D::height);
    segment.setFillColor(sf::Color(255, 255, 255, brightness));

    target.draw(segment);
}

// Большая мини-карта в левом верхнем углу
void WorldAdapter::drawMinimap(sf::RenderTarget& target) {
    int minimapSize = 250;
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

    // Рендерим объекты на мини-карте
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

    // Камера - зеленый круг
    sf::CircleShape cameraShape(8);
    cameraShape.setFillColor(sf::Color::Green);
    cameraShape.setPosition(camMapX, camMapY);
    cameraShape.setOrigin(8, 8);
    target.draw(cameraShape);

    // Направление камеры на мини-карте
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
    drawMinimap(target);
}

void WorldAdapter::update(double deltaTime) {
    Q_UNUSED(deltaTime);
}

void WorldAdapter::handleEvents() {
}
