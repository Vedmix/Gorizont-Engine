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

void Map::draw(sf::RenderWindow& window){
    for(auto& obj:objectSet){
        obj->draw(window);
    }
}

Map::~Map(){}