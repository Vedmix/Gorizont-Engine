#include "../headers/Map.hpp"

Map::Map(){}

Map::Map(const Map& other){
    this->objectSet = other.objectSet;
}

Map::Map(std::vector<std::shared_ptr<Object2D>> objects){
    for(auto& obj:objects){
        objectSet.insert(obj);
    }
}

void Map::addObject(std::shared_ptr<Object2D> object){
    objectSet.insert(object);
}

std::vector<std::shared_ptr<Object2D>> Map::getObjectsAtPosition(const Point2D& position) const {
    std::vector<std::shared_ptr<Object2D>> result;
    
    for (const auto& obj : objectSet) {
        if (obj->getObjectType() == ObjectType::CIRCLE) {
            Circle* circle = dynamic_cast<Circle*>(obj.get());
            if (circle && circle->isCrossing(position)) {
                result.push_back(obj);
            }
        }
        else if (obj->getObjectType() == ObjectType::POLYGON) {
            Polygon2D* polygon = dynamic_cast<Polygon2D*>(obj.get());
            if (polygon && polygon->isCrossing(position)) {
                result.push_back(obj);
            }
        }
    }
    
    return result;
}

void Map::render(sf::RenderWindow& window){
    for(auto& obj:objectSet){
        obj->draw(window);
    }
}

void Map::setMap(std::vector<std::shared_ptr<Object2D>> objects){
    objectSet.clear();
    for(auto& obj:objects){
        objectSet.insert(obj);
    }
}

const std::set<std::shared_ptr<Object2D>>& Map::getObjects() const {
    return objectSet;
}

void Map::removeObject(std::shared_ptr<Object2D> object) {
    objectSet.erase(object);
}

Map::~Map(){}