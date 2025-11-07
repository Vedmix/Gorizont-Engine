#pragma once
#include "Map.hpp"

class Camera:public Circle
{
private:
    const double RENDER_DISTANCE;
    const int NUMBER_OF_RAYS_IN_FOV;
    Map map;
    double velocity;
    double direction;
    double fov;
    void drawRays(sf::RenderWindow& window);
public:
    Camera();
    Camera(const Point2D& _position, double _radius, unsigned int _color, const Map& _map);
    Camera(const Camera& other);
    void setMap(const Map& _map);
    void render(sf::RenderWindow& window);
    void moveWithKeyboard(double deltaTime);
    ~Camera();
};