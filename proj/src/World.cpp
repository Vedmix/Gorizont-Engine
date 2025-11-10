#include "../headers/World.hpp"
#include <iostream>

World::World():camera(Point2D(300, 300), 30, 0xFF0000FF, &map), 
               window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Gorizont"), 
               isRunning(true),
               debugPanel(window)
{

    this->setMapOption1();

    camera.setFOV(PI / 2);                    
    camera.setRenderDistance(300);            
    camera.setNumberRaysInFov(SCREEN_WIDTH/8); 
    camera.setVelocity(300);  
    color = sf::Color::Black;
    
    debugPanel.setCamera(&camera);
    debugPanel.setMap(&map);
    
}

World::~World(){}

void World::setMap(const Map& newMap){
    // Преобразуем set в vector
    const auto& objectsSet = newMap.getObjects();
    std::vector<std::shared_ptr<Object2D>> objectsVector(objectsSet.begin(), objectsSet.end());
    map.setMap(objectsVector);
}

void World::setMapOption1(){
    Wall wallSouth(Point2D(960, 1065), 30, 1920, 0x3E3C32FF);
    Wall wallWest(Point2D(15, 540), 1890, 30, 0x3E3C32FF);
    Wall wallEast(Point2D(1850, 540), 1890, 30, 0x3E3C32FF);
    Wall wallNorth(Point2D(960, 15), 30, 1920, 0x3E3C32FF);

    std::vector<std::shared_ptr<Object2D>> objects;

    objects.push_back(std::make_shared<Wall>(wallSouth));
    objects.push_back(std::make_shared<Wall>(wallWest));
    objects.push_back(std::make_shared<Wall>(wallEast));
    objects.push_back(std::make_shared<Wall>(wallNorth));

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

        debugPanel.handleEvent(event);
    }
}

void World::update(double deltaTime){
    window.clear(color);
    camera.moveWithKeyboard(deltaTime);
    debugPanel.update(deltaTime);
}

void World::setCircleMovable(double deltaTime){
    static int currentDirection = 0;
    
    float moveSpeed = 500.0f;
    float left = 100;
    float right = SCREEN_WIDTH - 200;
    float top = 100;
    float bottom = SCREEN_HEIGHT - 200;
    
    for(auto& obj:map.getObjects()) {
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

// World.cpp
void World::display2DMap(sf::RenderWindow& window){
    // Рендерим все объекты карты
    map.render(window);
    
    // Вместо camera.render() рисуем камеру и лучи отдельно
    camera.draw(window);      // Рисуем саму камеру
    camera.drawRays(window);  // Рисуем лучи камеры
}

void World::render(){
    // Очищаем экран
    window.clear(color);
    
    // Рендерим карту и объекты
    display2DMap(window);
    
    // Рендерим DebugPanel поверх всего
    debugPanel.render();
    
    // Отображаем всё
    window.display();
}