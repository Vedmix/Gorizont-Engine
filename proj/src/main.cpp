#include "../headers/settings.hpp"
#include "../headers/Object2D.hpp"
#include "../headers/Circle.hpp"
#include "../headers/Polygon2D.hpp"
#include "../headers/Map.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "ZOMBE_BAZOOKA_GTA");

    std::vector<Point2D> rectPoints = {Point2D(0, 0), Point2D(100, 0), Point2D(100, 100), Point2D(80, 120),Point2D(0, 100)};
    Polygon2D pol(Point2D(50, 50), rectPoints, 0xFF0000FF);
    Circle crl(Point2D(500, 500), 25, 0x00FF00FF);

    Map map;
    
    std::shared_ptr<Object2D> polPtr(new Polygon2D(pol));
    std::shared_ptr<Object2D> crlPtr(new Circle(crl));

    map.addObject(polPtr);
    map.addObject(crlPtr);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        map.draw(window);
        window.display();
    }

    return 0;
}
