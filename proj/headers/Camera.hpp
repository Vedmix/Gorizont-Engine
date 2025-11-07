#pragma once
#include "Map.hpp"

class Camera:public Circle
{
private:
    double RENDER_DISTANCE;
    int NUMBER_OF_RAYS_IN_FOV;
    Map map;
    double velocity;
    double direction;
    double fov;
    void drawRays(sf::RenderWindow& window);
public:
    Camera();
    Camera(const Point2D& _position, double _radius, unsigned int _color, const Map& _map);
    Camera(const Camera& other);
    void render(sf::RenderWindow& window);
    void moveWithKeyboard(double deltaTime);

    void setMap(const Map& _map);
    void setRenderDistance(const double dist);
    void setNumberRaysInFov(const int numRays); //Качество графики (чем больше лучей, тем чётче)
    void setVelocity(const double vel);
    void setFOV(const double _fov);
    ~Camera();
};