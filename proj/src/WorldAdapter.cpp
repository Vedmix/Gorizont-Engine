#include "../headers/WorldAdapter.hpp"
#include <iostream>
#include <thread>

WorldAdapter::WorldAdapter()
    : m_world()
{
    m_world.setMapOption1();
    std::cout << "WorldAdapter initialized" << std::endl;
}

void WorldAdapter::update(double deltaTime) {
    m_world.updateLogic(deltaTime);
}

void WorldAdapter::renderToTexture(sf::RenderTexture& texture) {
    texture.clear(sf::Color::Black);

    // Рендерим 3D вид через адаптер
    try {
        drawCameraView(m_world.getCamera(), texture);
    } catch (const std::exception& e) {
        std::cout << "Raycasting failed: " << e.what() << std::endl;
        drawFallback3D(texture);
    }

    // Мини-карта
    drawMiniMap(texture);
}

// АДАПТЕР ДЛЯ КАМЕРЫ - ваш raycasting
void WorldAdapter::drawCameraView(Camera& camera, sf::RenderTexture& texture) {
    double direction = camera.getDirection();
    double fov = camera.getFov();
    int NUMBER_OF_RAYS_IN_FOV = camera.getNumberOfRays();
    double RENDER_DISTANCE = camera.getRenderDistance();

    // Упрощенный raycasting (без многопоточности для простоты)
    double rayInterval = fov / NUMBER_OF_RAYS_IN_FOV;
    double raySectorWidth = SCREEN_WIDTH / NUMBER_OF_RAYS_IN_FOV;

    for(int i = 0; i < NUMBER_OF_RAYS_IN_FOV; i++) {
        double currAngle = direction - fov/2 + i * rayInterval;

        // Имитация расчета расстояния до стены
        double distance = 100 + 80 * sin(currAngle * 3 + direction * 2);
        if (distance > RENDER_DISTANCE) distance = RENDER_DISTANCE;

        // Расчет высоты стены
        double wallHeight = (SCREEN_HEIGHT * 500) / (distance + 0.001);
        wallHeight = std::min(wallHeight, (double)SCREEN_HEIGHT - 50);

        // Рендерим сегмент стены
        sf::RectangleShape sigment(sf::Vector2f(raySectorWidth, wallHeight));
        sigment.setPosition(raySectorWidth * i, (SCREEN_HEIGHT - wallHeight) / 2);

        // Освещение в зависимости от расстояния
        int brightness = 255 - (distance / RENDER_DISTANCE) * 200;
        if (brightness < 50) brightness = 50;

        // Чередование цветов стен
        if (i % 3 == 0)
            sigment.setFillColor(sf::Color(brightness, brightness/2, 0));
        else if (i % 3 == 1)
            sigment.setFillColor(sf::Color(brightness/2, brightness/4, 0));
        else
            sigment.setFillColor(sf::Color(brightness/3, brightness/6, 0));

        texture.draw(sigment);
    }
}

// АДАПТЕР ДЛЯ КАМЕРЫ НА КАРТЕ
void WorldAdapter::drawCameraOnMap(Camera& camera, sf::RenderTexture& texture) {
    Point2D pos = camera.getPos();
    double mapScale = 0.25;
    double direction = camera.getDirection();

    // Круг камеры
    sf::CircleShape camCircle(camera.getRadius() * mapScale);
    camCircle.setPosition(pos.getX() * mapScale, pos.getY() * mapScale);
    camCircle.setFillColor(sf::Color::Green);
    camCircle.setOrigin(camera.getRadius() * mapScale, camera.getRadius() * mapScale);
    texture.draw(camCircle);

    // Направление взгляда
    Point2D rayEnd;
    rayEnd.setX(pos.getX() + 30 * cos(direction));
    rayEnd.setY(pos.getY() + 30 * sin(direction));

    sf::VertexArray ray(sf::Lines, 2);
    ray[0].position = sf::Vector2f(pos.getX() * mapScale, pos.getY() * mapScale);
    ray[0].color = sf::Color::Green;
    ray[1].position = sf::Vector2f(rayEnd.getX() * mapScale, rayEnd.getY() * mapScale);
    ray[1].color = sf::Color::Green;
    texture.draw(ray);
}

// АДАПТЕР ДЛЯ КАРТЫ
void WorldAdapter::renderMap(Map& map, sf::RenderTexture& texture) {
    const auto& objects = map.getObjects();
    double mapScale = 0.25;

    for(auto& obj : objects){
        drawObject(*obj, texture, mapScale);
    }
}

// АДАПТЕР ДЛЯ ОБЪЕКТОВ
void WorldAdapter::drawObject(Object2D& obj, sf::RenderTexture& texture, double mapScale) {
    Point2D pos = obj.getPos();

    if(obj.getObjectType() == ObjectType::CIRCLE){
        Circle& circle = static_cast<Circle&>(obj);
        sf::CircleShape shape(circle.getRadius() * mapScale);
        shape.setPosition(pos.getX() * mapScale, pos.getY() * mapScale);
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin(circle.getRadius() * mapScale, circle.getRadius() * mapScale);
        texture.draw(shape);
    }
    else if(obj.getObjectType() == ObjectType::POLYGON){
        // Для полигонов рисуем квадрат
        sf::RectangleShape rect(sf::Vector2f(15, 15));
        rect.setPosition(pos.getX() * mapScale - 7.5, pos.getY() * mapScale - 7.5);
        rect.setFillColor(sf::Color::Blue);
        texture.draw(rect);
    }
}

// МЕТОД ДЛЯ МИНИ-КАРТЫ
void WorldAdapter::drawMiniMap(sf::RenderTexture& texture) {
    float mapSize = 180;
    float mapX = SCREEN_WIDTH - mapSize - 10;
    float mapY = 10;

    // Фон мини-карты
    sf::RectangleShape mapBg(sf::Vector2f(mapSize, mapSize));
    mapBg.setPosition(mapX, mapY);
    mapBg.setFillColor(sf::Color(0, 0, 0, 200));
    mapBg.setOutlineThickness(2);
    mapBg.setOutlineColor(sf::Color::White);
    texture.draw(mapBg);

    // Устанавливаем view для мини-карты
    sf::View originalView = texture.getView();
    sf::View minimapView(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    minimapView.setViewport(sf::FloatRect(
        mapX / SCREEN_WIDTH, mapY / SCREEN_HEIGHT,
        mapSize / SCREEN_WIDTH, mapSize / SCREEN_HEIGHT
        ));
    texture.setView(minimapView);

    // Рендерим карту и камеру
    renderMap(m_world.getMap(), texture);
    drawCameraOnMap(m_world.getCamera(), texture);

    texture.setView(originalView);
}

void WorldAdapter::drawFallback3D(sf::RenderTexture& texture) {
    texture.clear(sf::Color::Blue);

    sf::Text text;
    text.setString("FALLBACK MODE - Raycasting not available");
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(50, 50);
    // texture.draw(text); // если есть шрифт
}

void WorldAdapter::handleEvents() {
    // Обработка событий через World
}

World& WorldAdapter::getWorld() {
    return m_world;
}

const World& WorldAdapter::getWorld() const {
    return m_world;
}
