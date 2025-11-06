#pragma once
#include "Map.hpp"
#include "Camera.hpp"

class World
{
private:
    Map map;
    Camera player;
    sf::RenderWindow window;
    sf::Color color;
    bool isRunning;

public:   
    World();
    ~World();
    void setMap(const Map& newMap);
    const Map& getMap() const;
    void addObject(std::shared_ptr<Object2D> object);
    void run();

private:
    void handleEvents();
    void render();
};
