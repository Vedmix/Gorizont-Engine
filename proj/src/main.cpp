#include "../headers/settings.hpp"
#include "../headers/Object2D.hpp"
#include "../headers/Circle.hpp"
#include "../headers/Polygon2D.hpp"
#include "../headers/World.hpp"
int main()
{
    std::vector<Point2D> rectPoints = {Point2D(0, 0), Point2D(100, 0), Point2D(100, 100), Point2D(80, 120),Point2D(0, 100)};
    Polygon2D pol(Point2D(50, 50), rectPoints, 0xFF0000FF);
    Circle crl(Point2D(500, 500), 25, 0x00FF00FF);

    std::shared_ptr<Object2D> polPtr(new Polygon2D(pol));
    std::shared_ptr<Object2D> crlPtr(new Circle(crl));

    World world;

    world.addObject(polPtr);
    world.addObject(crlPtr);

    world.run();

    return 0;
}
