#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <regex>
#include <fstream>
#include <memory>
#include <set>
#include "../headers/Map.hpp"
#include "../headers/Camera.hpp"
#include "../headers/settings.hpp"

class World {
private:
    Map map;
    Camera camera;
    sf::RenderWindow window;
    sf::Clock clock;
    sf::Font font;
    bool isRunning;
    std::string XMLFilePath;
    sf::Color color;

    int frameCount;
    float fps;
    float currentFPS = 0;

public:
    World();
    ~World();

    // SFML
    void run();
    void update(double deltaTime);
    void handleEvents();
    void render();
    void initFPSCounter();
    void updateFPS();
    void drawFPS();

    // Qt
    void renderToTexture(sf::RenderTexture& texture);

    void setMap(const Map& newMap);
    const Map& getMap() const;
    Camera& getCamera() { return camera; }
    void addObject(std::shared_ptr<Object2D> object);
    void setColor(unsigned int _color);

    void loadMapFromXML();
    void display2DMap(sf::RenderTarget& target);

private:
    void readWallsXML();
    void readCirclesXML();
    void setCircleMovable(double deltaTime);
};
