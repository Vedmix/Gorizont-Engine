#include "Entity.hpp"

Entity::Entity(const Point2D& position, double radius, unsigned int color,double velocity, double direction): Circle(position, radius, color),velocity(velocity),direction(direction) {
    objType = ObjectType::ENTITY;
}

Point2D Entity::calculateMovementVector(double speed, double angleOffset) const {
    double moveAngle = direction + angleOffset;
    return Point2D(speed * cos(moveAngle), speed * sin(moveAngle));
}

Entity::~Entity() {}

bool Entity::isPositionFree(const Point2D& checkPos, const Map& map) const {
    for(auto obj : map.getObjects()){
        if(obj.get() == this) {
            continue;
        }
        if(obj->isCrossing(checkPos)){
            return false;
        }
    }
    return true;
}

bool Entity::canMoveTo(const Point2D& targetPos, const Map& map) const {
    return isPositionFree(targetPos, map);
}

void Entity::applyWallSliding(const Point2D& startPos, const Point2D& moveVec, const Map& map) {
    if(moveVec.getX() != 0){
        Point2D moveX(moveVec.getX(), 0);
        Point2D posX = startPos + moveX;
        if(canMoveTo(posX, map)){
            setPos(posX);
            return;
        }
    }

    if(moveVec.getY() != 0){
        Point2D moveY(0, moveVec.getY());
        Point2D posY = startPos + moveY;
        if(canMoveTo(posY, map)){
            setPos(posY);
            return;
        }
    }

    Point2D reduced = moveVec * 0.3;
    Point2D posReduced = startPos + reduced;
    if(canMoveTo(posReduced, map)){
        setPos(posReduced);
    }
}
