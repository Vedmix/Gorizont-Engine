#pragma once
#include "Map.hpp"

class Camera:public Circle
{
private:
    Map map;
    double fov;
    double renderDistance;
public:
    Camera();
    Camera(const Point2D& _position, double _radius, unsigned int _color, const Map& _map);
    Camera(const Camera& other);
    void setMap(const Map& _map);
    void render(sf::RenderWindow& window);
    ~Camera();
};