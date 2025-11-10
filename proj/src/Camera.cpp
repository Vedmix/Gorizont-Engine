#include "../headers/Camera.hpp"

Camera::Camera(const Point2D& _position, double _radius, unsigned int _color, const Map& _map):Circle(_position, _radius, _color), RENDER_DISTANCE(300), NUMBER_OF_RAYS_IN_FOV(SCREEN_WIDTH/10){
    map=_map;
    velocity = 150;
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

void Camera::drawCameraOnMap(sf::RenderWindow& window){
    this->draw(window, map.MAP_SCALE);
    Point2D currRayEnd;
    currRayEnd.setX(this->position.getX()+RENDER_DISTANCE*cos(direction));
    currRayEnd.setY(this->position.getY()+RENDER_DISTANCE*sin(direction));

    sf::VertexArray ray(sf::Lines, 2);
    ray[0].position = sf::Vector2f(this->position.getX()*map.MAP_SCALE, this->position.getY()*map.MAP_SCALE);
    ray[0].color = sf::Color::Green;
    ray[1].position = sf::Vector2f(currRayEnd.getX()*map.MAP_SCALE, currRayEnd.getY()*map.MAP_SCALE);
    ray[1].color = sf::Color::Green;
    window.draw(ray);
}

void Camera::drawOneCameraSigment(sf::RenderWindow& window, double distance, int sigmentNum, double sectorWidth){
    double realHeight = Object2D::height;
    double viewH;
    viewH = (realHeight - distance*tan(atan(realHeight/distance)- (PI/120)));
    sf::RectangleShape sigment(sf::Vector2f(sectorWidth, viewH*2));
    sigment.setPosition(sectorWidth*sigmentNum, 540 - viewH);

    //sigment.setPosition(sigmentNum*sectorWidth, viewH);
    double brightess = 255 * (viewH/1080);
    sigment.setFillColor(sf::Color(255,255,255,brightess));


    window.draw(sigment);
}

void Camera::drawCameraView(sf::RenderWindow& window){
    Point2D currRayEnd;
    bool isCrossed=false;
    double currRayDir;
    double rayDistance;
    currRayEnd.setX(this->position.getX()+RENDER_DISTANCE*cos(direction));
    currRayEnd.setY(this->position.getY()+RENDER_DISTANCE*sin(direction));

    double rightExtRay = direction + fov/2;
    double leftExtRay = direction - fov/2;
    double raySectorWidth = SCREEN_WIDTH/NUMBER_OF_RAYS_IN_FOV;
    double rayInterval = fov/NUMBER_OF_RAYS_IN_FOV;
    int segmentNum=0;

    for(double i=leftExtRay;i<rightExtRay;i+=rayInterval,segmentNum++){
        currRayDir = i;
        isCrossed=false;
        for(int j=0;j<RENDER_DISTANCE && !isCrossed;j+=2){
            for(auto& obj:map.objectSet){
                if(obj->getObjectType()==ObjectType::CAMERA){
                    continue;
                }
                currRayEnd.setX(this->position.getX()+j*cos(currRayDir));
                currRayEnd.setY(this->position.getY()+j*sin(currRayDir));
                if(obj->isCrossing(currRayEnd)){
                    isCrossed=true;
                    rayDistance = j;
                }
            }
        }
        if(isCrossed){
            drawOneCameraSigment(window, rayDistance, segmentNum, raySectorWidth);
        }
    }
}

void Camera::moveWithKeyboard(double deltaTime){
    Point2D currPos = this->getPos();
    Point2D deltaPos;
    double speed = velocity * deltaTime;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        deltaPos.setX(speed*cos(direction));
        deltaPos.setY(speed*sin(direction));
        position = position + deltaPos;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        deltaPos.setX(speed*cos(direction-(PI/2)));
        deltaPos.setY(speed*sin(direction-(PI/2)));
        position = position + deltaPos;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        deltaPos.setX(speed*cos(direction));
        deltaPos.setY(speed*sin(direction));
        position = position - deltaPos;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        deltaPos.setX(speed*cos(direction-(PI/2)));
        deltaPos.setY(speed*sin(direction-(PI/2)));
        position = position - deltaPos;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        deltaTime=0.00025;
        direction += velocity*deltaTime;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        deltaTime=0.00025;
        direction -= velocity*deltaTime;
    }
}

Camera::~Camera(){}

void Camera::setFOV(const double _fov){fov=_fov;}
void Camera::setVelocity(const double vel){velocity=vel;}
void Camera::setRenderDistance(const double dist){RENDER_DISTANCE=dist;}
void Camera::setNumberRaysInFov(const int numRays){NUMBER_OF_RAYS_IN_FOV = numRays;}
