#pragma once
#include "Map.hpp"
#include "Camera.hpp"

#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include <sstream>

class World
{
private:
    Map map;
    Camera camera;
    sf::RenderWindow window;
    sf::Color color;
    bool isRunning;
    sf::Clock clock;
    std::string XMLFilePath;

public:   
    World();
    ~World();
    void setMap(const Map& newMap);
    const Map& getMap() const;
    void addObject(std::shared_ptr<Object2D> object);
    void run();
    void setColor(unsigned int _color);

private:
    void setCircleMovable(double deltaTime);
    void handleEvents();
    void update(double deltaTime);
    void render();
    void display2DMap(sf::RenderWindow& window);
    void loadMapFromXML();
    void readWallsXML();
    void readCirclesXML();
};