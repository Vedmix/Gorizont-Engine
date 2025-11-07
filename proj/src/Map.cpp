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

Map& Map::operator=(const Map& other){
    if(this != &other){
        this->objectSet = other.objectSet;
        return *this;
    }
    return *this;
}

void Map::addObject(std::shared_ptr<Object2D> object){
    objectSet.insert(object);
}

void Map::render(sf::RenderWindow& window){
    for(auto& obj:objectSet){
        obj->draw(window);
    }
}

const std::set<std::shared_ptr<Object2D>>& Map::getObjects() const {
    return objectSet;
}

Map::~Map(){}