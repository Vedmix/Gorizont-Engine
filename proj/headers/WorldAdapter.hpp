#ifndef WORLDADAPTER_HPP
#define WORLDADAPTER_HPP

#include "Map.hpp"
#include "Camera.hpp"
#include "World.hpp"
#include "Circle.hpp"
#include "Polygon2D.hpp"
#include "settings.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <thread>
#include <QDebug>
#include <cmath>

class WorldAdapter {
public:
    WorldAdapter(Map& _map, Camera& _camera, World& _world);

    // Основной метод рендеринга
    void renderWorld(sf::RenderTarget& target);

    // Методы для обновления и обработки событий
    void update(double deltaTime);
    void handleEvents();

private:
    Map& map;
    Camera& camera;
    World& world;

    // Приватные вспомогательные методы
    void drawCameraView(sf::RenderTarget& target);
    void calculateHeights(double leftExtRay, double rightExtRay, int segmentNum,
                          std::vector<double>& heights, int numRays, int numThreads);
    void drawOneCameraSegment(sf::RenderTarget& target, double viewH, int segmentNum, double sectorWidth);
    void drawMinimap(sf::RenderTarget& target);
};

#endif // WORLDADAPTER_HPP
