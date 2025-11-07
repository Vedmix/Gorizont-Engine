#pragma once
#include "Map.hpp"
#include "Camera.hpp"

class World
{
private:
    Map map;
    Camera camera;
    sf::RenderWindow window;
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

private:
    void handleEvents();
    void update(float deltaTime);
    void render();
};