#include "Player.hpp"
#include <cmath>

Player::Player(const Point2D& position, double radius, unsigned int color,
               Map& map, double velocity, double direction)
    : Entity(position, radius, color, velocity, direction),
    camera(position, radius, color, map)  // Инициализируем камеру
{
    objType = ObjectType::PLAYER;

    // Синхронизируем начальные параметры с камерой
    camera.setVelocity(velocity);
    camera.setDirection(direction);
    camera.setPos(position);
}

bool Player::isPositionFree(const Point2D& checkPos, const Map& map) const {
    for(auto obj : map.getObjects()) {
        // Пропускаем самого себя
        if(obj->getObjectType() == ObjectType::PLAYER) {
            continue;
        }

        // Проверяем пересечение
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
    Point2D startPos = this->getPos();  // Фиксируем начальную позицию
    double speed = velocity * deltaTime;

    // Проверяем каждое направление независимо от стартовой позиции

    // Движение вперед (W)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        Point2D testPos = startPos;
        testPos.setX(testPos.getX() + speed * cos(direction));
        testPos.setY(testPos.getY() + speed * sin(direction));

        if(canMoveTo(testPos, map)) {
            setPos(testPos);
            camera.setPos(testPos);  // Обновляем позицию камеры
        }
    }

    // Движение назад (S)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        Point2D testPos = startPos;
        testPos.setX(testPos.getX() - speed * cos(direction));
        testPos.setY(testPos.getY() - speed * sin(direction));

        if(canMoveTo(testPos, map)) {
            setPos(testPos);
            camera.setPos(testPos);
        }
    }

    // Страф влево (A)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        Point2D testPos = startPos;
        double strafeAngle = direction - M_PI / 2.0;
        testPos.setX(testPos.getX() + speed * cos(strafeAngle));
        testPos.setY(testPos.getY() + speed * sin(strafeAngle));

        if(canMoveTo(testPos, map)) {
            setPos(testPos);
            camera.setPos(testPos);
        }
    }

    // Страф вправо (D)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        Point2D testPos = startPos;
        double strafeAngle = direction + M_PI / 2.0;
        testPos.setX(testPos.getX() + speed * cos(strafeAngle));
        testPos.setY(testPos.getY() + speed * sin(strafeAngle));

        if(canMoveTo(testPos, map)) {
            setPos(testPos);
            camera.setPos(testPos);
        }
    }

    // Поворот (стрелки)
    double rotationSpeed = velocity * 0.007 * deltaTime;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        direction += rotationSpeed;
        camera.setDirection(direction);  // Обновляем направление камеры
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        direction -= rotationSpeed;
        camera.setDirection(direction);  // Обновляем направление камеры
    }

    // Нормализуем угол к диапазону [0, 2π)
    while (direction >= 2 * M_PI) direction -= 2 * M_PI;
    while (direction < 0) direction += 2 * M_PI;
}
