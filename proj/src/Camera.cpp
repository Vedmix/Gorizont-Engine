#include "../headers/Camera.hpp"

Camera::Camera(const Point2D& _position, double _radius, unsigned int _color, const Map& _map):Circle(_position, _radius, _color), RENDER_DISTANCE(100), NUMBER_OF_RAYS_IN_FOV(SCREEN_WIDTH/8){
    map=_map;
    velocity = 300;
    direction = 0;
    fov = PI/2;
    objType = ObjectType::CAMERA;
}

Camera::Camera(const Camera& other):Circle(other), RENDER_DISTANCE(other.RENDER_DISTANCE), NUMBER_OF_RAYS_IN_FOV(other.NUMBER_OF_RAYS_IN_FOV){
    velocity = other.velocity;
    map = other.map;
    direction = other.direction;
}

void Camera::setMap(const Map& _map){
    map=_map;
}

void Camera::render(sf::RenderWindow& window){
    this->draw(window, map.MAP_SCALE);
    this->drawCameraView(window);
}

void Camera::drawOneCameraSigment(sf::RenderWindow& window, double distance){
    double height = Object2D::height;
    
}

void Camera::drawCameraView(sf::RenderWindow& window){
    Point2D currRayEnd;
    bool isCrossed=false;
    double currRayDir;
    double rayDistance;
    currRayEnd.setX(this->position.getX()+RENDER_DISTANCE*cos(direction));
    currRayEnd.setY(this->position.getY()+RENDER_DISTANCE*sin(direction));

    sf::VertexArray ray(sf::Lines, 2);
    ray[0].position = sf::Vector2f(this->position.getX()*map.MAP_SCALE, this->position.getY()*map.MAP_SCALE);
    ray[0].color = sf::Color::Green;

    double rightExtRay = direction + fov/2;
    double leftExtRay = direction - fov/2;
    double raySectorWidth = SCREEN_WIDTH/NUMBER_OF_RAYS_IN_FOV;
    double rayInterval = fov/NUMBER_OF_RAYS_IN_FOV;

    for(double i=leftExtRay;i<rightExtRay;i+=rayInterval){
        currRayDir = i;
        isCrossed=false;
        for(int j=0;j<RENDER_DISTANCE && !isCrossed;j+=1){
            for(auto& obj:map.objectSet){
                if(obj->getObjectType()==ObjectType::CAMERA){
                    continue;
                }
                currRayEnd.setX(this->position.getX()+j*cos(currRayDir));
                currRayEnd.setY(this->position.getY()+j*sin(currRayDir));
                if(obj->isCrossing(currRayEnd)){
                    isCrossed=true;
                    rayDistance = j;
                    break;
                }
            }
        }
        if(isCrossed){
            drawOneCameraSigment(window, rayDistance);
        }
        ray[1].position = sf::Vector2f(currRayEnd.getX()*map.MAP_SCALE, currRayEnd.getY()*map.MAP_SCALE);
        ray[1].color = sf::Color::Green;
        window.draw(ray);
    }
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
        deltaTime=0.00025;
        direction += velocity*deltaTime;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        deltaTime=0.00025;
        direction -= velocity*deltaTime;
    }

    position.setX(currX);
    position.setY(currY);
}

Camera::~Camera(){}

void Camera::setFOV(const double _fov){fov=_fov;}
void Camera::setVelocity(const double vel){velocity=vel;}
void Camera::setRenderDistance(const double dist){RENDER_DISTANCE=dist;}
void Camera::setNumberRaysInFov(const int numRays){NUMBER_OF_RAYS_IN_FOV = numRays;}
