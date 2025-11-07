#include "../headers/Camera.hpp"

Camera::Camera(const Point2D& _position, double _radius, unsigned int _color, const Map& _map):Circle(_position, _radius, _color){
    map=_map;
    velocity = 300;
    fov = PI/2;
    renderDistance = 900;
    objType = ObjectType::CAMERA;
}

Camera::Camera(const Camera& other):Circle(other){
    velocity = other.velocity;
    map = other.map;
}

void Camera::setMap(const Map& _map){
    map=_map;
}

void Camera::render(sf::RenderWindow& window){
    this->draw(window);
}

void Camera::moveWithKeyboard(double deltaTime){
    Point2D currPos = this->getPos();
    double currX = currPos.getX();
    double currY = currPos.getY();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        currY -= velocity * deltaTime;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        currX -= velocity * deltaTime;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        currY += velocity * deltaTime;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        currX += velocity * deltaTime;
    }
    this->setPos(Point2D(currX, currY));
}

Camera::~Camera(){}