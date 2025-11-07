#include "../headers/Camera.hpp"

Camera::Camera(const Point2D& _position, double _radius, unsigned int _color, const Map& _map):Circle(_position, _radius, _color){
    map=_map;
    velocity = 300;
    direction = 0;
    fov = PI/2;
    renderDistance = 400;
    objType = ObjectType::CAMERA;
}

Camera::Camera(const Camera& other):Circle(other){
    velocity = other.velocity;
    map = other.map;
    direction = other.direction;
}

void Camera::setMap(const Map& _map){
    map=_map;
}

void Camera::render(sf::RenderWindow& window){
    this->draw(window);
    this->drawRays(window);
}

void Camera::drawRays(sf::RenderWindow& window){
    Point2D currRayEnd;
    bool isCrossed=false;

    currRayEnd.setX(this->position.getX()+renderDistance*cos(direction));
    currRayEnd.setY(this->position.getY()+renderDistance*sin(direction));

    sf::VertexArray ray(sf::Lines, 2);
    ray[0].position = sf::Vector2f(this->position.getX(), this->position.getY());
    ray[0].color = sf::Color::Green;

    for(int i=0;i<renderDistance && !isCrossed;i++){
        for(auto& obj:map.objectSet){
            if(obj->getObjectType()==ObjectType::CAMERA){
                continue;
            }
            currRayEnd.setX(this->position.getX()+i*cos(direction));
            currRayEnd.setY(this->position.getY()+i*sin(direction));
            if(obj->isCrossing(currRayEnd)){
                isCrossed=true;
                break;
            }
        }
    }
    //std::cout << currRayEnd.getX() << '\n';
    ray[1].position = sf::Vector2f(currRayEnd.getX(), currRayEnd.getY());
    ray[1].color = sf::Color::Green;
    window.draw(ray);
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

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        deltaTime=0.00001;
        direction += velocity*deltaTime;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        deltaTime=0.00001;
        direction -= velocity*deltaTime;
    }

    position.setX(currX);
    position.setY(currY);
}

Camera::~Camera(){}