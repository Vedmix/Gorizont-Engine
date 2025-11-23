#ifndef WORLDADAPTER_HPP
#define WORLDADAPTER_HPP

#include "../headers/Map.hpp"
#include "../headers/Camera.hpp"
#include "../headers/World.hpp"
#include "../headers/Circle.hpp"
#include "../headers/Polygon2D.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class WorldAdapter {
public:
    WorldAdapter(Map& _map, Camera& _camera, World& _world);

    // Основной метод рендеринга
    void renderWorld(sf::RenderTarget& target);

    // Методы для обновления и обработки событий
    void update(double deltaTime);
    void handleEvents();

    // Пустые методы для компиляции
    void renderObject(std::shared_ptr<Object2D> obj, sf::RenderTarget& target, double scale = 1.0);
    void drawObjectOnMap(std::shared_ptr<Object2D> obj, sf::RenderTarget& target, double mapScale);
    void renderMap(sf::RenderTarget& target);
    void drawMapOnMap(sf::RenderTarget& target);
    void drawCameraOnMap(sf::RenderTarget& target);

private:
    Map& map;
    Camera& camera;
    World& world;

    // Приватные вспомогательные методы
    void drawCameraView(sf::RenderTarget& target);
    void drawWallSegmentBW(sf::RenderTarget& target, int segmentIndex, double sectorWidth, double distance);
    double ultraFastCastRay(const Point2D& start, double angle, double maxDistance); // ИСПРАВЛЕНА СИГНАТУРА
    void drawMinimap(sf::RenderTarget& target);

    void renderCircle(std::shared_ptr<Circle> circle, sf::RenderTarget& target, double scale);
    void renderPolygon(std::shared_ptr<Polygon2D> polygon, sf::RenderTarget& target, double scale);
};

#endif // WORLDADAPTER_HPP
