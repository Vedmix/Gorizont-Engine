#pragma once
#include "Map.hpp"
#include <thread>

class Camera:public Circle
{
private:
    const int numThreads;
    std::vector<double> heights;
    double RENDER_DISTANCE;
    int NUMBER_OF_RAYS_IN_FOV;
    Map map;
    double velocity;
    double direction;
    double fov;
    void drawOneCameraSigment(sf::RenderWindow& window, double viewH, int sigmentNum, double sectorWidth);
    void CalculateHeights(double rightExtRay, double leftExtRay, int sigmentNum);
public:
    Camera();
    Camera(const Point2D& _position, double _radius, unsigned int _color, const Map& _map);
    Camera(const Camera& other);
    void drawCameraOnMap(sf::RenderWindow& window);
    void drawCameraView(sf::RenderWindow& window);
    void moveWithKeyboard(double deltaTime);

    void setMap(const Map& _map);
    void setRenderDistance(const double dist);
    void setNumberRaysInFov(const int numRays);
    void setVelocity(const double vel);
    void setFOV(const double _fov);
    ~Camera();

    // ДОБАВЛЯЕМ ГЕТТЕРЫ ДЛЯ WorldAdapter
    double getDirection() const { return direction; }
    double getFov() const { return fov; }
    int getNumThreads() const { return numThreads; }
    int getNumberOfRays() const { return NUMBER_OF_RAYS_IN_FOV; }
    const std::vector<double>& getHeights() const { return heights; }
    double getRenderDistance() const { return RENDER_DISTANCE; }
    const Map& getMap() const { return map; }
};
