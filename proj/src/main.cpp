#include "../headers/Point.hpp"
#include "../headers/Object2D.hpp"
#include "../headers/Circle.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Two Circles");

    Point blueCirclePos(200, 300);
    Circle blueCircle(blueCirclePos, 80);
    blueCircle.setFillColor(sf::Color::Blue);

    Point redCirclePos(500, 300);
    Circle redCircle(redCirclePos, 60);
    redCircle.setFillColor(sf::Color::Red);

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

        window.display();
    }

    return 0;
}
