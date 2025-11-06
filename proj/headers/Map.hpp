#pragma once
#include <memory>
#include <set>
#include "../headers/Circle.hpp"
#include "../headers/Polygon2D.hpp"

class Map
{
private:
    std::set<std::shared_ptr<Object2D>> objectSet;
public:
    Map();
    Map(const Map& other);
    Map(std::vector<std::shared_ptr<Object2D>> objects);
    ~Map();

    void addObject(std::shared_ptr<Object2D> object);
    Map& operator=(const Map& other);
    void render(sf::RenderWindow& window);
    
    friend class World;
    friend class Camera;
};