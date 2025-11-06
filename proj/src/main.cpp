#include "../headers/settings.hpp"
#include "../headers/Object2D.hpp"
#include "../headers/Circle.hpp"
#include "../headers/Polygon2D.hpp"
#include "../headers/World.hpp"
#include "../headers/Wall.hpp"
int main()
{

    Wall wallNorth(Point2D(960, 15), 30, 1920, 0x3E3C32FF);
    Wall wallSouth(Point2D(960, 1065), 30, 1920, 0x3E3C32FF);
    Wall wallWest(Point2D(15, 540), 1890, 30, 0x3E3C32FF);
    Wall wallEast(Point2D(1905, 540), 1890, 30, 0x3E3C32FF);
    Wall wallCenter(Point2D(960, 540), 30, 1500, 0x3E3C32FF);

    std::shared_ptr<Object2D> wallPtrN(new Wall(wallNorth));
    std::shared_ptr<Object2D> wallPtrS(new Wall(wallSouth));
    std::shared_ptr<Object2D> wallPtrW(new Wall(wallWest));
    std::shared_ptr<Object2D> wallPtrE(new Wall(wallEast));
    std::shared_ptr<Object2D> wallPtrC(new Wall(wallCenter));

    World world;

    world.addObject(wallPtrN);
    world.addObject(wallPtrS);
    world.addObject(wallPtrW);
    world.addObject(wallPtrE);
    world.addObject(wallPtrC);
 
    world.run();

    return 0;
}
