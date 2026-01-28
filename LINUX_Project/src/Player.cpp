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

// Поиск нормали к ближайшей стене
Point2D Player::findWallNormal(const Point2D& fromPoint, const Map& map) const {
    // Пока простой вариант: определяем с какой стороны стена

    // Проверяем 4 направления для простоты
    const double checkDistance = getRadius() + 5.0; // Немного больше радиуса

    // Справа
    Point2D rightCheck(fromPoint.getX() + checkDistance, fromPoint.getY());
    // Слева
    Point2D leftCheck(fromPoint.getX() - checkDistance, fromPoint.getY());
    // Сверху
    Point2D upCheck(fromPoint.getX(), fromPoint.getY() - checkDistance);
    // Снизу
    Point2D downCheck(fromPoint.getX(), fromPoint.getY() + checkDistance);

    // Проверяем коллизии
    bool wallRight = !isPositionFree(rightCheck, map);
    bool wallLeft = !isPositionFree(leftCheck, map);
    bool wallUp = !isPositionFree(upCheck, map);
    bool wallDown = !isPositionFree(downCheck, map);

    // Возвращаем нормаль (перпендикуляр к поверхности)
    if (wallRight) return Point2D(-1, 0);   // Стена справа → нормаль влево
    if (wallLeft)  return Point2D(1, 0);    // Стена слева → нормаль вправо
    if (wallUp)    return Point2D(0, 1);    // Стена сверху → нормаль вниз
    if (wallDown)  return Point2D(0, -1);   // Стена снизу → нормаль вверх

    // Если не нашли стену, возвращаем нулевую нормаль
    return Point2D(0, 0);
}

void Player::applyWallSliding(const Point2D& startPos, const Point2D& moveVec, const Map& map) {
    // Всегда пробуем простые оси, даже без поиска нормали

    // 1. Пробуем только X (горизонтальное скольжение)
    if (moveVec.getX() != 0) {
        Point2D moveX(moveVec.getX(), 0);
        Point2D posX = startPos + moveX;

        if (canMoveTo(posX, map)) {
            setPos(posX);
            camera.setPos(posX);
            return;
        }
    }

    // 2. Пробуем только Y (вертикальное скольжение)
    if (moveVec.getY() != 0) {
        Point2D moveY(0, moveVec.getY());
        Point2D posY = startPos + moveY;

        if (canMoveTo(posY, map)) {
            setPos(posY);
            camera.setPos(posY);
            return;
        }
    }

    // 3. Пробуем 45° диагонали (для углов)
    if (moveVec.getX() != 0 && moveVec.getY() != 0) {
        // Если движение было диагональное, пробуем под 45°
        Point2D diagonal45(moveVec.getX(), 0); // или другая комбинация
        Point2D pos45 = startPos + diagonal45;

        if (canMoveTo(pos45, map)) {
            setPos(pos45);
            camera.setPos(pos45);
            return;
        }
    }

    // 4. Пробуем уменьшенный вектор
    Point2D reduced = moveVec * 0.3;
    Point2D posReduced = startPos + reduced;

    if (canMoveTo(posReduced, map)) {
        setPos(posReduced);
        camera.setPos(posReduced);
    }
}

// Вспомогательный метод (уже есть, но обновим)
void Player::trySeparateAxisMovement(const Point2D& startPos, const Point2D& move, const Map& map) {
    // Пробуем только X
    if (move.getX() != 0) {
        Point2D moveX(move.getX(), 0);
        Point2D posX = startPos + moveX;
        if (canMoveTo(posX, map)) {
            setPos(posX);
            camera.setPos(posX);
            return;
        }
    }

    // Пробуем только Y
    if (move.getY() != 0) {
        Point2D moveY(0, move.getY());
        Point2D posY = startPos + moveY;
        if (canMoveTo(posY, map)) {
            setPos(posY);
            camera.setPos(posY);
            return;
        }
    }
}

void Player::moveWithKeyboard(double deltaTime, const Map& map) {
    Point2D currPos = this->getPos();
    double speed = velocity * deltaTime;
    Point2D newPos;
    Point2D moveVec;

    // Движение вперед (W)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        moveVec = calculateMovementVector(speed);
        newPos = position + moveVec;  // Используем position (текущую)

        if(canMoveTo(newPos, map)) {
            setPos(newPos);
            camera.setPos(newPos);
        } else {
            // Коллизия! Пробуем скольжение
            applyWallSliding(position, moveVec, map);
        }
    }

    // Движение назад (S)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        moveVec = calculateMovementVector(-speed);  // Отрицательная скорость = назад
        newPos = position + moveVec;

        if(canMoveTo(newPos, map)) {
            setPos(newPos);
            camera.setPos(newPos);
        } else {
            applyWallSliding(position, moveVec, map);
        }
    }

    // Страф влево (A)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        moveVec = calculateMovementVector(speed, -M_PI/2);
        newPos = position + moveVec;

        if(canMoveTo(newPos, map)) {
            setPos(newPos);
            camera.setPos(newPos);
        } else {
            applyWallSliding(position, moveVec, map);
        }
    }

    // Страф вправо (D)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        moveVec = calculateMovementVector(speed, M_PI/2);
        newPos = position + moveVec;

        if(canMoveTo(newPos, map)) {
            setPos(newPos);
            camera.setPos(newPos);
        } else {
            applyWallSliding(position, moveVec, map);
        }
    }

    // Поворот (стрелки)
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
