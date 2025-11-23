#include "../headers/WorldAdapter.hpp"


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

void WorldAdapter::drawMinimap(sf::RenderTarget& target) {
    // Сохраняем текущий вид
    sf::View originalView = target.getView();

    // Устанавливаем вид для мини-карты в координатах ЭКРАНА
    sf::View minimapView(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    target.setView(minimapView);

    // Фон мини-карты
    sf::RectangleShape minimapBg(sf::Vector2f(SCREEN_WIDTH / 8.0, SCREEN_HEIGHT / 8.0));
    minimapBg.setFillColor(sf::Color::Black);
    minimapBg.setPosition(0, 0);
    target.draw(minimapBg);

    // Рендерим объекты на мини-карте с ПРОСТЫМ масштабом
    for (const auto& obj : map.getObjects()) {
        if (obj->getObjectType() == ObjectType::CAMERA) continue;

        if (auto circle = std::dynamic_pointer_cast<Circle>(obj)) {
            Point2D pos = circle->getPos();
            double radius = circle->getRadius();

            sf::CircleShape shape(radius / 4.0);
            shape.setFillColor(sf::Color(200, 200, 200));
            shape.setPosition(pos.getX() / 4.0, pos.getY() / 4.0);
            shape.setOrigin(radius / 4.0, radius / 4.0);
            target.draw(shape);
        }
        else if (auto polygon = std::dynamic_pointer_cast<Polygon2D>(obj)) {
            const std::vector<Point2D>& points = polygon->getPoints();
            if (points.empty()) continue;

            Point2D pos = polygon->getPos();

            sf::ConvexShape shape;
            shape.setPointCount(points.size());

            for (size_t i = 0; i < points.size(); ++i) {
                shape.setPoint(i, sf::Vector2f((pos.getX() + points[i].getX()) / 4.0, (pos.getY() + points[i].getY()) / 4.0));
            }

            shape.setFillColor(sf::Color(150, 150, 150));
            target.draw(shape);
        }
    }

    // Камера на мини-карте
    Point2D cameraPos = camera.getPos();
    sf::CircleShape cameraShape(8);
    cameraShape.setFillColor(sf::Color::Green);
    cameraShape.setPosition(cameraPos.getX() / 4.0, cameraPos.getY() / 4.0);
    cameraShape.setOrigin(8, 8);
    target.draw(cameraShape);

    // Направление камеры на мини-карте
    double camDirection = camera.getDirection();

    sf::Vertex directionLine[] = {
        sf::Vertex(sf::Vector2f(cameraPos.getX() / 4.0, cameraPos.getY() / 4.0), sf::Color::Red),
        sf::Vertex(sf::Vector2f(cameraPos.getX() / 4.0 + cos(camDirection) * 30, cameraPos.getY() / 4.0 + sin(camDirection) * 30), sf::Color::Red)
    };
    target.draw(directionLine, 2, sf::Lines);

    // Восстанавливаем оригинальный вид
    target.setView(originalView);
}

void WorldAdapter::renderWorld(sf::RenderTarget& target) {

    target.clear(sf::Color::Black);

    drawCameraView(target);

    drawMinimap(target);
}

void WorldAdapter::update(double deltaTime) {
    Q_UNUSED(deltaTime);
}

void WorldAdapter::handleEvents() {
}
