#include "../headers/World.hpp"

World::World():map(), player(), window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "GORIZONT"), isRunning(true)
{
    color = sf::Color::Black;
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

void World::setColor(unsigned int _color){
    color = sf::Color(_color);
}

void World::run(){
    while (isRunning && window.isOpen()){
        float deltaTime = clock.restart().asSeconds();
        handleEvents();
       // update(deltaTime);
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

void World::update(float deltaTime){
    auto& objects = map.getObjects();
    
    static int currentDirection = 0;
    
    float moveSpeed = 500.0f;
    float left = 100;
    float right = SCREEN_WIDTH - 200;
    float top = 100;
    float bottom = SCREEN_HEIGHT - 200;
    
    for(auto& obj : objects) {
        if (obj->getObjectType() != ObjectType::CIRCLE) {
            continue;
        }
        
        Point2D currentPos = obj->getPos();
        float x = currentPos.getX();
        float y = currentPos.getY();
        
        switch(currentDirection) {
            case 0: x += moveSpeed * deltaTime;
                    if (x >= right) { x = right; currentDirection = 1; }
                    break;
            case 1: y += moveSpeed * deltaTime;
                    if (y >= bottom) { y = bottom; currentDirection = 2; }
                    break;
            case 2: x -= moveSpeed * deltaTime;
                    if (x <= left) { x = left; currentDirection = 3; }
                    break;
            case 3: y -= moveSpeed * deltaTime;
                    if (y <= top) { y = top; currentDirection = 0; }
                    break;
        }
        
        obj->setPos(Point2D(x, y));
        break;
    }
}

void World::render(){
    window.clear(color);
    map.render(window);
    window.display();
}