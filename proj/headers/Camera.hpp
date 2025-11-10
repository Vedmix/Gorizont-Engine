#pragma once
#include "Map.hpp"

class Camera : public Circle {
private:
    Map* map;  // Указатель на карту, а не копия
    double velocity;
    double direction;
    double fov;
    double RENDER_DISTANCE;
    int NUMBER_OF_RAYS_IN_FOV;
    
public:
    Camera(const Point2D& _position, double _radius, unsigned int _color, Map* _map); // Изменить на указатель
    Camera(const Camera& other);
    ~Camera();
    
    void setMap(Map* _map); // Изменить на указатель
    void drawRays(sf::RenderWindow& window);
    void moveWithKeyboard(double deltaTime);
    
    void setFOV(const double _fov);
    void setVelocity(const double vel);
    void setRenderDistance(const double dist);
    void setNumberRaysInFov(const int numRays);
};