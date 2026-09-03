#include "Player.hpp"

Player::Player(const Point2D& position, double radius, unsigned int color, Map& map, double velocity, double direction):Entity(position, radius, color, velocity, direction), camera(position, radius, color, map)
{
    objType = ObjectType::PLAYER;
    camera.setPos(position);
}

void Player::moveWithKeyboard(double deltaTime, const Map& map) {
    double speed = velocity * deltaTime;
    Point2D newPos;
    Point2D moveVec;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
        moveVec = calculateMovementVector(speed);
        newPos = position + moveVec;

        if(canMoveTo(newPos, map)){
            setPos(newPos);
            camera.setPos(newPos);
        }else{
            applyWallSliding(position, moveVec, map);
            camera.setPos(position);
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
        moveVec = calculateMovementVector(-speed);
        newPos = position + moveVec;

        if(canMoveTo(newPos, map)){
            setPos(newPos);
            camera.setPos(newPos);
        }else{
            applyWallSliding(position, moveVec, map);
            camera.setPos(position);
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
        moveVec = calculateMovementVector(speed, -M_PI/2);
        newPos = position + moveVec;

        if(canMoveTo(newPos, map)){
            setPos(newPos);
            camera.setPos(newPos);
        }else{
            applyWallSliding(position, moveVec, map);
            camera.setPos(position);
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
        moveVec = calculateMovementVector(speed, M_PI/2);
        newPos = position + moveVec;

        if(canMoveTo(newPos, map)){
            setPos(newPos);
            camera.setPos(newPos);
        }else{
            applyWallSliding(position, moveVec, map);
            camera.setPos(position);
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
        direction += velocity * deltaTime * 0.01;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
        direction -= velocity * deltaTime * 0.01;
    }

    while(direction >= 2 * M_PI){
        direction -= 2 * M_PI;
    }
    while(direction < 0){
        direction += 2 * M_PI;
    }
}
