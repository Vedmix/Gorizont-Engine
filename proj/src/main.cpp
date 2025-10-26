#include "../headers/Object2D.hpp"
#include "../headers/Circle.hpp"
#include "../headers/Polygon2D.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "ZOMBE_BAZOOKA_GTA");

    Point2D blueCirclePos(200, 300);
    Circle blueCircle(blueCirclePos, 80);
    blueCircle.setFillColor(sf::Color::Blue);

    Point2D redCirclePos(500, 300);
    Circle redCircle(redCirclePos, 60);
    redCircle.setFillColor(sf::Color::Red);

    std::vector<Point2D> rectPoints = {Point2D(0, 0), Point2D(100, 0), Point2D(100, 100), Point2D(80, 120),Point2D(0, 100)};
    Polygon2D pol(Point2D(0, 0), rectPoints);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        blueCircle.draw(window);
        redCircle.draw(window);
        pol.draw(window);
        window.display();
    }

    return 0;
}
