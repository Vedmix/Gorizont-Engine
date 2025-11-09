#pragma once
#include <memory>
#include <set>
#include <vector>
#include "../headers/Circle.hpp"
#include "../headers/Polygon2D.hpp"
#include "../headers/Wall.hpp"

class Map
{
private:
    std::set<std::shared_ptr<Object2D>> objectSet;
    static constexpr double MAP_SCALE = 1;
public:
    Map();
    Map(const Map& other);
    ~Map();
    Map(std::vector<std::shared_ptr<Object2D>> objects);
    Map& operator=(const Map& other);
    void addObject(std::shared_ptr<Object2D> object);
    void setMap(std::vector<std::shared_ptr<Object2D>> objects);
    void render(sf::RenderWindow& window);
    const std::set<std::shared_ptr<Object2D>>& getObjects() const;
    
    friend class World;
    friend class Camera;
};