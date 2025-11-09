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
    sf::ConvexShape background;
    background.setPointCount(4);
    background.setPoint(0, sf::Vector2f(0, 0));
    background.setPoint(1, sf::Vector2f(SCREEN_WIDTH*MAP_SCALE, 0));
    background.setPoint(2, sf::Vector2f(SCREEN_WIDTH*MAP_SCALE, SCREEN_HEIGHT*MAP_SCALE));
    background.setPoint(3, sf::Vector2f(0, SCREEN_HEIGHT*MAP_SCALE));
    background.setFillColor(sf::Color::White);
    background.setPosition(0, 0);
    window.draw(background);

    for(auto& obj:objectSet){
        obj->draw(window, MAP_SCALE);
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

Map::~Map(){}