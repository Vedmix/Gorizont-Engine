#include "../headers/Camera.hpp"

Camera::Camera(const Point2D& _position, double _radius, unsigned int _color, Map& _map):Circle(_position, _radius, _color), map(_map), numThreads(10),RENDER_DISTANCE(1000), NUMBER_OF_RAYS_IN_FOV(SCREEN_WIDTH/2){
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

bool Camera::isPointOnRay(double x, double y, double x0, double y0, double currAngle){
    double vecX = x - x0;
    double vecY = y - y0;
    double dotProduct = vecX*cos(currAngle) + vecY*sin(currAngle);
    
    if(dotProduct<=0){
        return false;
    }

    double crossProduct = vecX*sin(currAngle) - vecY*cos(currAngle);
    if(fabs(crossProduct)>1e-6){
        return false;
    }

    double dist = sqrt(vecX*vecX + vecY*vecY);
    if(dist>RENDER_DISTANCE){
        return false;
    }
    
    return true;
}


void Camera::CalculateHeights(double leftExtRay, double rightExtRay, int sigmentNum){
    double rayInterval = fov/NUMBER_OF_RAYS_IN_FOV;
    int raysPerThread = NUMBER_OF_RAYS_IN_FOV / numThreads;
    
    for(double currAngle=rightExtRay, i=sigmentNum*raysPerThread;currAngle<leftExtRay && i<(sigmentNum+1)*raysPerThread;currAngle+=rayInterval,i++){
        double rayDistance = RENDER_DISTANCE+1;
        double x0 = position.getX(), y0 = position.getY();
        double k = tan(currAngle);
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
                double d = pow(k*b - xc - yc*k, 2) - (1 + k*k)*(b*b - r*r + xc*xc + yc*yc - 2*yc*b);
                
                if(d>0){
                    double xCross1 = (xc + yc*k - k*b + sqrt(d)) / (1 + k*k);
                    double yCross1 = k*xCross1 + b;
                    double xCross2 = (xc + yc*k - k*b - sqrt(d)) / (1 + k*k);
                    double yCross2 = k*xCross2 + b;
                    
                    if(isPointOnRay(xCross1, yCross1, x0, y0,currAngle)){
                        crossPoints.push_back(Point2D(xCross1, yCross1));
                    }
                    if(isPointOnRay(xCross1, yCross1, x0, y0,currAngle)){
                        crossPoints.push_back(Point2D(xCross2, yCross2));
                    }
                }
                else if(d==0){
                    double xCross1 = (xc + yc*k - k*b) / (1 + k*k);
                    double yCross1 = k*xCross1 + b;
                    
                    if(isPointOnRay(xCross1, yCross1, x0, y0,currAngle)){
                        crossPoints.push_back(Point2D(xCross1, yCross1));
                    }
                }
            }
            else if(obj->getObjectType()==ObjectType::POLYGON){
                Polygon2D* pol = dynamic_cast<Polygon2D*>(obj.get());
                std::vector<Point2D> polygonPoints = pol->getPointsPlane();
                Point2D segmentCameraRay[2] = {Point2D(x0, y0), Point2D(x0+RENDER_DISTANCE*cos(currAngle), y0+RENDER_DISTANCE*sin(currAngle))};
                Point2D segmentPolygonSide[2];
                double k1, b1;
                for(int i=0;i<polygonPoints.size();i++){
                    if(i==polygonPoints.size()-1){
                        segmentPolygonSide[0] = polygonPoints[i-1];
                        segmentPolygonSide[1] = polygonPoints[0];
                    }
                    else{
                        segmentPolygonSide[0] = polygonPoints[i];
                        segmentPolygonSide[1] = polygonPoints[i+1];
                    }
                    k1=(segmentPolygonSide[0].getY()-segmentPolygonSide[1].getY())/(segmentPolygonSide[0].getX()-segmentPolygonSide[1].getX());
                    if(k1==k){
                        continue;
                    }
                    b1=segmentPolygonSide[0].getY() - k1*segmentPolygonSide[0].getX();
                    double xCross1=(b1-b)/(k-k1), yCross1 = k*xCross1 - b;
                    if(isPointOnRay(xCross1, yCross1, x0, y0,currAngle)){
                        crossPoints.push_back(Point2D(xCross1, yCross1));
                    }
                }
            }
        }
        
        if(crossPoints.size() == 0){
            heights[i] = -1;
        }
        else{
            for(const auto& point:crossPoints){
                double dist = this->position.distance(point);
                if(rayDistance > dist){
                    rayDistance = dist;
                }
            }
            heights[i] = (Object2D::height - rayDistance*tan(atan(Object2D::height/rayDistance) - (PI/120)));
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
