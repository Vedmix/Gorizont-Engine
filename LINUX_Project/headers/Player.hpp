#pragma once

#include "Entity.hpp"
#include "Camera.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

class Player : public Entity {
private:
    Camera camera;

public:
    Player(const Point2D& position, double radius, unsigned int color, Map& map, double velocity = 150.0, double direction = 0.0);
    ~Player() override = default;

    bool isPositionFree(const Point2D& checkPos, const Map& map) const;
    bool canMoveTo(const Point2D& targetPos, const Map& map) const;

    void applyWallSliding(const Point2D& startPos, const Point2D& desiredMove, const Map& map);
    Point2D findWallNormal(const Point2D& fromPoint, const Map& map) const;
    void trySeparateAxisMovement(const Point2D& startPos, const Point2D& move, const Map& map);

    void moveWithKeyboard(double deltaTime, const Map& map) override;

    Camera& getCamera() { return camera; }
    const Camera& getCamera() const { return camera; }
};
