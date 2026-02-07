#pragma once
#include "Map.hpp"
#include <thread>

class Camera:public Circle
{
private:
    Map& map;
    const int numThreads;
    std::vector<double> heights;
    double RENDER_DISTANCE;
    int NUMBER_OF_RAYS_IN_FOV;
    double fov;
    bool isPointOnRay(double x, double y, double currAngle);
    void drawOneCameraSigment(sf::RenderTarget& window, double viewH, int sigmentNum, double sectorWidth);
    void CalculateHeights(double leftExtRay, double rightExtRay, int sigmentNum);
    std::vector<Point2D> findCrossPoints(double currAngle);
    std::vector<Point2D> findCrossPointsCircle(double currAngle, Circle* circle, double _k, double _b);
    std::vector<Point2D> findCrossPointsPolygon(double currAngle, Polygon2D* polygon);
public:
    Camera(const Point2D& _position, double _radius, unsigned int _color, Map& _map);
    ~Camera();

    void drawCameraView(sf::RenderTarget& window, double playerDirection);
    void drawCameraOnMap(sf::RenderTarget& window, double playerDirection);

    void setRenderDistance(const double dist);
    void setNumberRaysInFov(const int numRays);
    void setFOV(const double _fov);

    double getRenderDistance() const { return RENDER_DISTANCE; }
    int getNumberOfRays() const { return NUMBER_OF_RAYS_IN_FOV; }
    double getFov() const { return fov; }
};
