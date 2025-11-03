#include "../headers/Object2D.hpp"
#include "../headers/Circle.hpp"
#include "../headers/Polygon2D.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "ZOMBE_BAZOOKA_GTA");

    std::vector<Point2D> rectPoints = {Point2D(0, 0), Point2D(100, 0), Point2D(100, 100), Point2D(80, 120),Point2D(0, 100)};
    Polygon2D pol(Point2D(50, 50), rectPoints, 0xFF0000FF);
    Circle crl(Point2D(100, 100), 25, 0x00FF00FF);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        pol.draw(window);
        crl.draw(window);
        window.display();
    }

    return 0;
}
