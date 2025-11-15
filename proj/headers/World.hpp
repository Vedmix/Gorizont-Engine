#pragma once
#include "Map.hpp"
#include "Camera.hpp"

class World
{
private:
    Map map;
    Camera camera;
    sf::Color color;
    bool isRunning;
    sf::Clock clock;

public:
    World();
    ~World();
    void setMap(const Map& newMap);
    const Map& getMap() const;
    void addObject(std::shared_ptr<Object2D> object);
    void run();
    void setColor(unsigned int _color);
    void setMapOption1();
    void setMapOption2();
    void setMapOption3();
    void update(double deltaTime);
    void render();

    // Новые методы для работы с RenderTexture
    void renderToTexture(sf::RenderTexture& texture);

private:
    void setCircleMovable(double deltaTime);
    void handleEvents();
    void display2DMap(sf::RenderWindow& window);
};
