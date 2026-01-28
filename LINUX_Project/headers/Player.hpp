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

    void moveWithKeyboard(double deltaTime, const Map& map);

    Camera& getCamera() { return camera; }
    const Camera& getCamera() const { return camera; }
};
