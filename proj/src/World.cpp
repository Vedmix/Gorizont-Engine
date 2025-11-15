#include "../headers/World.hpp"

World::World():camera(Point2D(300, 300), 30, 0xFF0000FF, this->map), isRunning(true)
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
    Wall wallSouth(Point2D(0, 1050), 1920, 30, 0x3E3C32FF);
    Wall wallWest(Point2D(0, 0), 30, 1050, 0x3E3C32FF);
    Wall wallEast(Point2D(1890, 0), 30, 1050, 0x3E3C32FF);
    Wall wallCenter1(Point2D(690, 540), 30, 500, 0x3E3C32FF);
    Wall wallCenter2(Point2D(1290, 540), 30, 500, 0x3E3C32FF);

    //Wall wallcorner(Point2D(0, 0), 300, 300, 0x0000FFFF);

    //Wall cornerWall(Point2D(100, 100), 100, 100, 0x0000FFFF);
    Circle circle(Point2D(0, 0), 100, 0x3E3C32FF);
    Circle circle1(Point2D(600, 600), 100, 0x3E3C32FF);
    Circle circle2(Point2D(345, 876), 100, 0x3E3C32FF);
    Circle circle3(Point2D(999,34), 100, 0x3E3C32FF);
    Circle circle4(Point2D(2, 600), 100, 0x3E3C32FF);


    std::vector<std::shared_ptr<Object2D>> objects;

    //objects.push_back(std::shared_ptr<Object2D>(new Wall(wallcorner)));
    objects.push_back(std::shared_ptr<Object2D>(new Wall(wallSouth)));
    objects.push_back(std::shared_ptr<Object2D>(new Wall(wallWest)));
    objects.push_back(std::shared_ptr<Object2D>(new Wall(wallEast)));
    objects.push_back(std::shared_ptr<Object2D>(new Wall(wallCenter1)));
    objects.push_back(std::shared_ptr<Object2D>(new Wall(wallCenter2)));
    objects.push_back(std::shared_ptr<Object2D>(new Circle(circle)));
    objects.push_back(std::shared_ptr<Object2D>(new Circle(circle1)));
    objects.push_back(std::shared_ptr<Object2D>(new Circle(circle2)));
    objects.push_back(std::shared_ptr<Object2D>(new Circle(circle3)));
    objects.push_back(std::shared_ptr<Object2D>(new Circle(circle4)));
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
/*
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
*/
void World::update(double deltaTime){
    // Убираем window.clear(color); - очистка теперь в renderToTexture

    camera.setMap(map); //ПОКА НИЧЕГО НЕ ДВИЖЕТСЯ НА КАРТЕ - НЕ ЮЗАТЬ ОБНОВЛЕНИЕ КАРТЫ
    camera.moveWithKeyboard(deltaTime);
    //setCircleMovable(deltaTime);
}

void World::display2DMap(sf::RenderWindow& window){
    map.render(window);
    camera.drawCameraOnMap(window);
}
void World::renderToTexture(sf::RenderTexture& texture) {
    texture.clear(sf::Color(40, 40, 60)); // Серо-синий фон

    // ТОЛЬКО 3D вид от камеры
    camera.drawCameraView(texture);

    // Информация
    sf::Text text;
    text.setString("3D View - WASD to move, Arrows to rotate\nObjects: " + std::to_string(map.objectSet.size()));
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, 10);
    texture.draw(text);
}

void World::render(){
    // Этот метод больше не используется, т.к. рендеринг в renderToTexture
    // Можно оставить пустым или закомментировать
    /*
    camera.drawCameraView(window);
    display2DMap(window);
    window.display();
    */
}
