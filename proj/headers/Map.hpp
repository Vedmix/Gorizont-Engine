#pragma once
#include <memory>
#include <set>
#include "../headers/Object2D.hpp"

class Map
{
private:
    std::set<std::shared_ptr<Object2D>> objectSet;
public:
    Map();
    Map(const Map& other);
    Map(std::vector<std::shared_ptr<Object2D>> objects);
    void addObject(std::shared_ptr<Object2D> object);
    void draw(sf::RenderWindow& window);
    ~Map();
    friend class World;
    friend class Camera;
};