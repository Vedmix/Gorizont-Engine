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
    ~Map();
};