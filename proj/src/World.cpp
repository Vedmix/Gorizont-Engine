#include "../headers/World.hpp"

World::World():map(), player(), window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "GORIZONT"), isRunning(true){

}

World::~World(){}

void World::setMap(const Map& newMap){
    map = newMap;
}

const Map& World::getMap() const{
    return map;
}

void World::addObject(std::shared_ptr<Object2D> object){
    map.addObject(object);
}

void World::run(){
    while (isRunning && window.isOpen()){
        handleEvents();
        render();
    }
}

void World::handleEvents(){
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            isRunning = false;
    }
}
void World::render(){
    window.clear(sf::Color::Black);
    map.render(window);
    window.display();
}
