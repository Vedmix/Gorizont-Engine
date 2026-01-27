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
    Point2D deltaPos;
    double speed = velocity * deltaTime;

    Point2D newPos = currPos;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        newPos = currPos;
        deltaPos.setX(speed * cos(direction));
        deltaPos.setY(speed * sin(direction));
        newPos = newPos + deltaPos;

        if(canMoveTo(newPos, map)){
            setPos(newPos);
            camera.setPos(newPos);
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        newPos = position;
        deltaPos.setX(speed * cos(direction - (M_PI/2)));
        deltaPos.setY(speed * sin(direction - (M_PI/2)));
        newPos = newPos + deltaPos;

        if(canMoveTo(newPos, map)){
            setPos(newPos);
            camera.setPos(newPos);
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        newPos = position;
        deltaPos.setX(speed * cos(direction));
        deltaPos.setY(speed * sin(direction));
        newPos = newPos - deltaPos;

        if(canMoveTo(newPos, map)){
            setPos(newPos);
            camera.setPos(newPos);
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        newPos = position;
        deltaPos.setX(speed * cos(direction - (M_PI/2)));
        deltaPos.setY(speed * sin(direction - (M_PI/2)));
        newPos = newPos - deltaPos;

        if(canMoveTo(newPos, map)){
            setPos(newPos);
            camera.setPos(newPos);
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        direction += velocity * deltaTime * 0.007;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        direction -= velocity * deltaTime * 0.007;
    }
}
