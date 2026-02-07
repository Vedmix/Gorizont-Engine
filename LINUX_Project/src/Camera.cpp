#include "../headers/Camera.hpp"

Camera::Camera(const Point2D& _position, double _radius, unsigned int _color, Map& _map):Circle(_position, _radius, _color), map(_map), numThreads(10),RENDER_DISTANCE(1000), NUMBER_OF_RAYS_IN_FOV(SCREEN_WIDTH){
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

bool Camera::isPointOnRay(double x, double y, double currAngle){
    double vecX = x - position.getX();
    double vecY = y - position.getY();
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

std::vector<Point2D> Camera::findCrossPointsCircle(double currAngle, Circle* circle, double _k, double _b){
    std::vector<Point2D> crossPoints;
    double k=_k, b=_b;
    double xc = circle->getPos().getX();
    double yc = circle->getPos().getY();
    double r = circle->getRadius();
    double d = pow(k*b - xc - yc*k, 2) - (1 + k*k)*(b*b - r*r + xc*xc + yc*yc - 2*yc*b);
    
    if(d>0){
        double xCross1 = (xc + yc*k - k*b + sqrt(d)) / (1 + k*k);
        double yCross1 = k*xCross1 + b;
        double xCross2 = (xc + yc*k - k*b - sqrt(d)) / (1 + k*k);
        double yCross2 = k*xCross2 + b;
        
        if(isPointOnRay(xCross1, yCross1,currAngle)){
            crossPoints.push_back(Point2D(xCross1, yCross1));
        }
        if(isPointOnRay(xCross2, yCross2,currAngle)){
            crossPoints.push_back(Point2D(xCross2, yCross2));
        }
    }
    else if(d==0){
        double xCross1 = (xc + yc*k - k*b) / (1 + k*k);
        double yCross1 = k*xCross1 + b;
        
        if(isPointOnRay(xCross1, yCross1,currAngle)){
            crossPoints.push_back(Point2D(xCross1, yCross1));
        }
    }
    return crossPoints;
}

std::vector<Point2D> Camera::findCrossPointsPolygon(double currAngle, Polygon2D* polygon){
    std::vector<Point2D> crossPoints;
    std::vector<Point2D> polygonPoints = polygon->getPointsPlane();
    double x0=position.getX(), y0=position.getY();
    for(size_t i = 0; i < polygonPoints.size(); i++){
        Point2D p1, p2;
        
        if(i == polygonPoints.size() - 1){
            p1 = polygonPoints[i];
            p2 = polygonPoints[0];
        }
        else{
            p1 = polygonPoints[i];
            p2 = polygonPoints[i + 1];
        }
        double xCross, yCross;
        double rayDirX = cos(currAngle);
        double rayDirY = sin(currAngle);
        double edgeDirX = p2.getX() - p1.getX();
        double edgeDirY = p2.getY() - p1.getY();
        double denominator = rayDirX * edgeDirY - rayDirY * edgeDirX;
        
        if(fabs(denominator) < 1e-10) {
            continue;
        }
        
        double dx = p1.getX() - x0;
        double dy = p1.getY() - y0;
        
        double t = (edgeDirY * dx - edgeDirX * dy) / denominator;
        double u = (rayDirY * dx - rayDirX * dy) / denominator;
        
        if(t >= 0 && u >= 0 && u <= 1) {
            xCross = x0 + t * rayDirX;
            yCross = y0 + t * rayDirY;
            double distance = sqrt((xCross - x0)*(xCross - x0) + (yCross - y0)*(yCross - y0));
            if(distance <= RENDER_DISTANCE + 1e-9) {
                crossPoints.push_back(Point2D(xCross, yCross));
            }
        }
    }
    return crossPoints;
}

std::vector<Point2D> Camera::findCrossPoints(double currAngle){
    std::vector<Point2D> crossPoints;
    double x0 = position.getX(), y0 = position.getY();
    double k = tan(currAngle);
    double b = y0 - k*x0;
    for(auto& obj:map.objectSet){
        if(obj->getObjectType()==ObjectType::CAMERA){
            continue;
        }
        if(obj->getObjectType()==ObjectType::CIRCLE){
            std::vector<Point2D> crossPointsCirc = findCrossPointsCircle(currAngle, dynamic_cast<Circle*>(obj.get()), k, b);
            crossPoints.insert(crossPoints.end(), crossPointsCirc.begin(), crossPointsCirc.end());
        }
        else if(obj->getObjectType() == ObjectType::POLYGON){
            std::vector<Point2D> crossPointsPolygon = findCrossPointsPolygon(currAngle, dynamic_cast<Polygon2D*>(obj.get()));
            crossPoints.insert(crossPoints.end(), crossPointsPolygon.begin(), crossPointsPolygon.end());
        }
    }
    return crossPoints;
}

void Camera::CalculateHeights(double leftExtRay, double rightExtRay, int sigmentNum){
    double rayInterval = fov/NUMBER_OF_RAYS_IN_FOV;
    int raysPerThread = NUMBER_OF_RAYS_IN_FOV / numThreads;
    
    for(double currAngle=rightExtRay, i=sigmentNum*raysPerThread;currAngle<leftExtRay && i<(sigmentNum+1)*raysPerThread;currAngle+=rayInterval,i++){
        double rayDistance = RENDER_DISTANCE+1;
        std::vector<Point2D> crossPoints = findCrossPoints(currAngle);
        
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
