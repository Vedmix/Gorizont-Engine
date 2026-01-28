#include "Player.hpp"

Player::Player(const Point2D& position, double radius, unsigned int color, Map& map, double velocity, double direction):Entity(position, radius, color, velocity, direction), camera(position, radius, color, map)
{
    objType = ObjectType::PLAYER;
    camera.setPos(position);
}

bool Player::isPositionFree(const Point2D& checkPos, const Map& map) const {
    for(auto obj : map.getObjects()) {
        if(obj->getObjectType() == ObjectType::PLAYER) {
            continue;
        }

        if(obj->isCrossing(checkPos)) {
            return false;
        }
    }

    return true;
}

bool Player::canMoveTo(const Point2D& targetPos, const Map& map) const {
    return isPositionFree(targetPos, map);
}

void Player::moveWithKeyboard(double deltaTime, const Map& map) {
    Point2D currPos = this->getPos();
    double speed = velocity * deltaTime;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        Point2D moveVec = calculateMovementVector(speed);
        Point2D newPos = currPos + moveVec;

        if(canMoveTo(newPos, map)) {
            setPos(newPos);
            camera.setPos(newPos);
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        Point2D moveVec = calculateMovementVector(speed);
        Point2D newPos = position - moveVec;

        if(canMoveTo(newPos, map)) {
            setPos(newPos);
            camera.setPos(newPos);
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        Point2D moveVec = calculateMovementVector(speed, -M_PI/2);
        Point2D newPos = position + moveVec;

        if(canMoveTo(newPos, map)) {
            setPos(newPos);
            camera.setPos(newPos);
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        Point2D moveVec = calculateMovementVector(speed, M_PI/2);
        Point2D newPos = position + moveVec;

        if(canMoveTo(newPos, map)) {
            setPos(newPos);
            camera.setPos(newPos);
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        direction += velocity * deltaTime * 0.007;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        direction -= velocity * deltaTime * 0.007;
    }

    // Нормализация угла
    while (direction >= 2 * M_PI) direction -= 2 * M_PI;
    while (direction < 0) direction += 2 * M_PI;
}
