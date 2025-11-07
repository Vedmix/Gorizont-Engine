#include "../headers/World.hpp"

World::World():camera(Point2D(300, 300), 30, 0xFF0000FF, this->map),window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "TUT KAK V ZHIZNI"), isRunning(true)
{
    this->setMapOption1();
    camera.setMap(this->map);
    color = sf::Color::Black;
}

World::~World(){}

void World::setMap(const Map& newMap){
    map = newMap;
}

void World::setMapOption1(){
    //Wall wallNorth(Point2D(960, 15), 30, 1920, 0x3E3C32FF);
    Wall wallSouth(Point2D(960, 1065), 30, 1920, 0x3E3C32FF);
    Wall wallWest(Point2D(15, 540), 1890, 30, 0x3E3C32FF);
    Wall wallEast(Point2D(1850, 540), 1890, 30, 0x3E3C32FF);
    Wall wallCenter1(Point2D(690, 540), 30, 500, 0x3E3C32FF);
    Wall wallCenter2(Point2D(1290, 540), 30, 500, 0x3E3C32FF);

    //Wall cornerWall(Point2D(100, 100), 100, 100, 0x0000FFFF);
    Circle circle(Point2D(0, 0), 100, 0x3E3C32FF);

    std::vector<std::shared_ptr<Object2D>> objects;

    objects.push_back(std::shared_ptr<Object2D>(new Wall(wallSouth)));
    objects.push_back(std::shared_ptr<Object2D>(new Wall(wallWest)));
    objects.push_back(std::shared_ptr<Object2D>(new Wall(wallEast)));
    objects.push_back(std::shared_ptr<Object2D>(new Wall(wallCenter1)));
    objects.push_back(std::shared_ptr<Object2D>(new Wall(wallCenter2)));
    objects.push_back(std::shared_ptr<Object2D>(new Circle(circle)));
    //objects.push_back(std::shared_ptr<Object2D>(new Wall(cornerWall)));

    map.setMap(objects);
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
        double deltaTime = clock.restart().asSeconds();
        handleEvents();
        update(deltaTime);
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

void World::update(double deltaTime){
    window.clear(color);
    camera.setMap(map);
    camera.moveWithKeyboard(deltaTime);
}

void World::setCircleMovable(double deltaTime){
    static int currentDirection = 0;
    
    float moveSpeed = 500.0f;
    float left = 100;
    float right = SCREEN_WIDTH - 200;
    float top = 100;
    float bottom = SCREEN_HEIGHT - 200;
    
    for(auto& obj:map.objectSet) {
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

void World::display2DMap(sf::RenderWindow& window){
    map.render(window);
    camera.render(window);
}

void World::render(){
    display2DMap(window);
    window.display();
}