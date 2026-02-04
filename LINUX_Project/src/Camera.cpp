#include "../headers/Camera.hpp"

Camera::Camera(const Point2D& _position, double _radius, unsigned int _color, Map& _map):Circle(_position, _radius, _color), map(_map), numThreads(10),RENDER_DISTANCE(700), NUMBER_OF_RAYS_IN_FOV(SCREEN_WIDTH/10){
    heights.resize(NUMBER_OF_RAYS_IN_FOV, -1);
    fov = PI/2;
    objType = ObjectType::CAMERA;
}


Camera::~Camera(){}

void Camera::drawCameraOnMap(sf::RenderTarget& window, double playerDirection){
    sf::CircleShape cameraShape(8);
    cameraShape.setFillColor(sf::Color::Green);
    cameraShape.setPosition(position.getX() * Map::MAP_SCALE, position.getY() * Map::MAP_SCALE);
    cameraShape.setOrigin(8, 8);
    window.draw(cameraShape);

    sf::Vertex directionLine[] = {
        sf::Vertex(sf::Vector2f(position.getX() * Map::MAP_SCALE, position.getY() * Map::MAP_SCALE), sf::Color::Red),
        sf::Vertex(sf::Vector2f(position.getX() * Map::MAP_SCALE + cos(playerDirection) * 30, position.getY() * Map::MAP_SCALE + sin(playerDirection) * 30), sf::Color::Red)
    };

    window.draw(directionLine, 2, sf::Lines);
}

void Camera::drawOneCameraSigment(sf::RenderTarget& window, double viewH, int sigmentNum, double sectorWidth){
    if(viewH==-1){
        return;
    }
    sf::RectangleShape sigment(sf::Vector2f(sectorWidth, viewH*2));
    sigment.setPosition(sectorWidth*sigmentNum, SCREEN_HEIGHT/2 - viewH);

    double brightess = 255 * (viewH/Object2D::height);
    sigment.setFillColor(sf::Color(255,255,255,brightess));

    window.draw(sigment);
}

void Camera::CalculateHeights(double leftExtRay, double rightExtRay, int sigmentNum){
    double rayInterval = fov/NUMBER_OF_RAYS_IN_FOV;
    int raysPerThread = NUMBER_OF_RAYS_IN_FOV / numThreads;
    for(double currAngle=rightExtRay, i=sigmentNum*raysPerThread;currAngle<leftExtRay && i<(sigmentNum+1)*raysPerThread;currAngle+=rayInterval,i++){
        Point2D currRayEnd;
        double rayDistance = RENDER_DISTANCE+1;
        /*for(int j=0;j<RENDER_DISTANCE && !isCrossed;j+=2){
            for(auto& obj:map.objectSet){
                if(obj->getObjectType()==ObjectType::CAMERA){
                    continue;
                }
                currRayEnd.setX(this->position.getX()+j*cos(currAngle));
                currRayEnd.setY(this->position.getY()+j*sin(currAngle));
                if(obj->isCrossing(currRayEnd)){
                    isCrossed=true;
                    rayDistance = j;
                }
            }
        }*/
        double l = RENDER_DISTANCE, x0 = position.getX(), y0 = position.getY();
        double k = (y0 - l*sin(currAngle))/(x0 - l*cos(currAngle));
        double b = y0 - k*x0;
        std::vector<Point2D> crossPoints;
        for(auto& obj:map.objectSet){
            if(obj->getObjectType()==ObjectType::CAMERA){
                continue;
            }
            if(obj->getObjectType()==ObjectType::CIRCLE){
                double xc = obj->getPos().getX();
                double yc = obj->getPos().getY();
                Circle* circle = dynamic_cast<Circle*>(obj.get());
                double r = circle->getRadius();
                double d=pow(k*b-xc-yc*k, 2) - (1+k)*(b*b-r*r+xc*xc+yc*yc-2*yc*b);
                double xCross1, yCross1;
                if(d>0){
                    double xCross2 = (xc+yc*k-k*b - sqrt(d))/(1+k), yCross2 = k*xCross2 + b;
                    xCross1 = (xc+yc*k-k*b + sqrt(d))/(1+k);
                    yCross1 = k*xCross1 + b;
                    crossPoints.push_back(Point2D(xCross1, yCross1));
                    crossPoints.push_back(Point2D(xCross2, yCross2));
                }
                else if(d==0){
                    xCross1 = (xc+yc*k-k*b)/(1+k);
                    yCross1 = k*xCross1 + b;
                    crossPoints.push_back(Point2D(xCross1, yCross1));
                }
                
            }
        }
        if(crossPoints.size()==0){
            heights[i]=-1;
        }
        else{
            for(const auto& point:crossPoints){
                double dist = this->position.distance(point);
                if(rayDistance>dist){
                    rayDistance=dist;
                }
            }
            heights[i] = (Object2D::height - rayDistance*tan(atan(Object2D::height/rayDistance)- (PI/120)));
        }
    }
}

void Camera::drawCameraView(sf::RenderTarget& window, double playerDirection){
    std::thread threads[10];
    double rightAngle = playerDirection - fov/2;
    double angleStep = fov/numThreads;
    double raySectorWidth = SCREEN_WIDTH/NUMBER_OF_RAYS_IN_FOV;
    double currRightAngle=rightAngle;
    double currLeftAngle=rightAngle+angleStep;

    for(int i=0;i<numThreads;i++){
        threads[i] = std::thread(&Camera::CalculateHeights, this, currLeftAngle, currRightAngle, i);
        currRightAngle += angleStep;
        currLeftAngle += angleStep;
    }

    for(int i=0; i<numThreads;i++){
        threads[i].join();
    }

    for(int i=0;i<(int)heights.size();i++){
        drawOneCameraSigment(window, heights[i], i, raySectorWidth);
    }
}
