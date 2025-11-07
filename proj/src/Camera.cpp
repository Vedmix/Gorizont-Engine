#include "../headers/Camera.hpp"

Camera::Camera(const Point2D& _position, double _radius, unsigned int _color, const Map& _map):Circle(_position, _radius, _color){
    map=_map;
    fov = PI/2;
    renderDistance = 900;
    objType = ObjectType::CAMERA;
}

Camera::Camera(const Camera& other):Circle(other){
    map = other.map;
}

void Camera::setMap(const Map& _map){
    map=_map;
}

void Camera::render(sf::RenderWindow& window){
    this->draw(window);
}

Camera::~Camera(){}