#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <regex>
#include <fstream>
#include <memory>
#include <set>
#include "Map.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "AppSettings.hpp"


class World {
private:
    // Игровые параметры
    double m_fov;
    double m_renderDistance;
    int m_numberOfRays;
    double m_playerSpeed;

    Map map;
    Player player;
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
    void updateFPS();
    void drawFPS();

    // Qt
    void renderToTexture(sf::RenderTexture& texture);

    void setMap(const Map& newMap);
    const Map& getMap() const;
    void addObject(std::shared_ptr<Object2D> object);
    void setColor(unsigned int _color);

    void loadMapFromXML();
    void display2DMap(sf::RenderTarget& target);

    void applySettings();

    void setFOV(double fov);
    void setRenderDistance(double distance);
    void setNumberOfRays(int rays);
    void setPlayerSpeed(double speed);

private:
    void readWallsXML();
    void readCirclesXML();
    void setCircleMovable(double deltaTime);
};
