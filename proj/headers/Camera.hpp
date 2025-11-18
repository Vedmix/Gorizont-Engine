#pragma once
#include "Map.hpp"
#include <thread>

class Camera:public Circle
{
private:
    const int numThreads;
    double RENDER_DISTANCE;
    int NUMBER_OF_RAYS_IN_FOV;
    Map map;
    double velocity;
    double direction;
    double fov;
    void drawOneCameraSigment(sf::RenderWindow& window, double distance, int sigmentNum, double sectorWidth);
    void renderView(sf::RenderWindow& window, double rightExtRay, double leftExtRay, int& sigmentNum);
public:
    Camera();
    Camera(const Point2D& _position, double _radius, unsigned int _color, const Map& _map);
    Camera(const Camera& other);
    void drawCameraOnMap(sf::RenderWindow& window);
    void drawCameraView(sf::RenderWindow& window);
    void moveWithKeyboard(double deltaTime);

    void setMap(const Map& _map);
    void setRenderDistance(const double dist);
    void setNumberRaysInFov(const int numRays); //Качество графики (чем больше лучей, тем чётче)
    void setVelocity(const double vel);
    void setFOV(const double _fov);
    ~Camera();
};