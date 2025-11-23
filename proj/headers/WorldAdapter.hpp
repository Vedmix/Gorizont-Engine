#ifndef WORLDADAPTER_HPP
#define WORLDADAPTER_HPP

#include "../headers/Map.hpp"
#include "../headers/Camera.hpp"
#include "../headers/World.hpp"
#include "../headers/Circle.hpp"
#include "../headers/Polygon2D.hpp"
#include "../headers/settings.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <thread>

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
