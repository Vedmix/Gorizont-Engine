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
    void setMapOption2();
    void setMapOption3();
    void handleEvents();
    void update(double deltaTime);
    void render();
    void display2DMap(sf::RenderWindow& window);

    ////
    // Методы для доступа к компонентам
    Camera& getCamera() { return camera; }
    const Camera& getCamera() const { return camera; }

    Map& getMap() { return map; }

    // Методы для обновления
    void updateLogic(double deltaTime) {
        camera.moveWithKeyboard(deltaTime);
        setCircleMovable(deltaTime);
    }
private:
    void setCircleMovable(double deltaTime);

};
