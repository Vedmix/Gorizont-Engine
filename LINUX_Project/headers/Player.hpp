#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "Camera.hpp"
#include <SFML/Window/Keyboard.hpp>

class Player : public Entity {
private:
    Camera camera;  // Камера для рендеринга

public:
    Player(const Point2D& position, double radius, unsigned int color,
           Map& map,
           double velocity = 150.0, double direction = 0.0);

    // Реализация абстрактного метода Entity
    void moveWithKeyboard(double deltaTime, const Map& map) override;

    // Методы доступа к Camera
    Camera& getCamera() { return camera; }
    const Camera& getCamera() const { return camera; }

    // Методы коллизий
    bool isPositionFree(const Point2D& checkPos, const Map& map) const;
    bool canMoveTo(const Point2D& targetPos, const Map& map) const;

    ~Player() override = default;
};

#endif
