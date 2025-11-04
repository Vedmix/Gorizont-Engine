#pragma once
#include "Map.hpp"
#include "Camera.hpp"

class World
{
private:
    Map map;
    Camera player;
public:
    World();
    ~World();
};