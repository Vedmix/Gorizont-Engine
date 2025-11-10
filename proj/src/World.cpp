#include "../headers/World.hpp"

World::World():camera(Point2D(300, 300), 30, 0xFF0000FF, this->map),window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "TUT KAK V ZHIZNI"), isRunning(true)
{
    this->setMapOption2();
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

void World::setMapOption2(){
    // Границы
    Wall wallSouth(Point2D(0, 1050), 1920, 30, 0x3E3C32FF);
    Wall wallWest(Point2D(0, 0), 30, 1050, 0x3E3C32FF);
    Wall wallEast(Point2D(1890, 0), 30, 1050, 0x3E3C32FF);
    Wall wallNorth(Point2D(0, 0), 1920, 30, 0x3E3C32FF);

    // Центральная зона: две комнаты с дверью между ними
    Wall room1Top(Point2D(520, 300), 560, 30, 0x3E3C32FF);
    Wall room1BottomLeft(Point2D(520, 540), 250, 30, 0x3E3C32FF);
    Wall room1BottomRight(Point2D(830, 540), 250, 30, 0x3E3C32FF);
    // вертикальные стороны комнат (левая и правая)
    Wall room1Left(Point2D(520, 300), 30, 270, 0x3E3C32FF);
    Wall room1Right(Point2D(1050, 300), 30, 270, 0x3E3C32FF);
    // проход в центре между bottomLeft и bottomRight (дверь) — оставляем зазор

    // Центральный коридор / разделитель
    Wall centerVertTop(Point2D(915, 0), 30, 240, 0x3E3C32FF);
    Wall centerVertBottom(Point2D(915, 810), 30, 240, 0x3E3C32FF);

    // Левая секция: несколько укрытий и проходов
    Wall leftHallTop(Point2D(60, 150), 420, 30, 0x3E3C32FF);
    Wall leftHallBottom(Point2D(60, 420), 420, 30, 0x3E3C32FF);
    Wall leftPillarH1(Point2D(300, 180), 30, 90, 0x3E3C32FF);
    Wall leftPillarH2(Point2D(300, 420), 30, 90, 0x3E3C32FF);

    // Правая секция: лабиринт из узких стен
    Wall mazeRow1(Point2D(1320, 100), 460, 30, 0x3E3C32FF);
    Wall mazeRow2(Point2D(1320, 220), 300, 30, 0x3E3C32FF);
    Wall mazeRow3(Point2D(1320, 340), 460, 30, 0x3E3C32FF);
    Wall mazeCol1(Point2D(1320, 100), 30, 300, 0x3E3C32FF);
    Wall mazeCol2(Point2D(1650, 220), 30, 300, 0x3E3C32FF);

    // Нижняя область: островки (пилоны) и короткие укрытия
    Wall lowerCover1(Point2D(420, 900), 180, 30, 0x3E3C32FF);
    Wall lowerCover2(Point2D(840, 900), 180, 30, 0x3E3C32FF);
    Wall lowerCover3(Point2D(1320, 900), 180, 30, 0x3E3C32FF);

    // Центр: два параллельных барьера с проходом
    Wall centralBarrierTop(Point2D(640, 420), 560, 20, 0x3E3C32FF);
    Wall centralBarrierBottom(Point2D(640, 660), 560, 20, 0x3E3C32FF);

    // Небольшие укрытия/углы
    Wall smallBlock1(Point2D(200, 660), 120, 120, 0x3E3C32FF);
    Wall smallBlock2(Point2D(1600, 540), 120, 120, 0x3E3C32FF);

    // Круглые объекты (пилоны/колонны) — разной величины
    Circle pillar1(Point2D(320, 300), 40, 0x8B4513FF);   // другой оттенок (коричневый)
    Circle pillar2(Point2D(480, 300), 30, 0x8B4513FF);
    Circle pillar3(Point2D(720, 540), 25, 0x8B4513FF);
    Circle pillar4(Point2D(1100, 150), 35, 0x8B4513FF);
    Circle pillar5(Point2D(1500, 760), 45, 0x8B4513FF);
    Circle pillar6(Point2D(300, 900), 20, 0x8B4513FF);

    // Разбросанные небольшие препятствия (подъезды/островки)
    Circle rock1(Point2D(980, 820), 18, 0x3E3C32FF);
    Circle rock2(Point2D(540, 820), 18, 0x3E3C32FF);
    Circle rock3(Point2D(1400, 480), 22, 0x3E3C32FF);

    // Собираем объекты в вектор (в том же порядке — сначала большие стены)
    std::vector<std::shared_ptr<Object2D>> objects;
    // границы
    objects.push_back(std::make_shared<Wall>(wallSouth));
    objects.push_back(std::make_shared<Wall>(wallWest));
    objects.push_back(std::make_shared<Wall>(wallEast));
    objects.push_back(std::make_shared<Wall>(wallNorth));

    // центральная зона
    objects.push_back(std::make_shared<Wall>(room1Top));
    objects.push_back(std::make_shared<Wall>(room1BottomLeft));
    objects.push_back(std::make_shared<Wall>(room1BottomRight));
    objects.push_back(std::make_shared<Wall>(room1Left));
    objects.push_back(std::make_shared<Wall>(room1Right));

    // центральный коридор
    objects.push_back(std::make_shared<Wall>(centerVertTop));
    objects.push_back(std::make_shared<Wall>(centerVertBottom));

    // левая зона
    objects.push_back(std::make_shared<Wall>(leftHallTop));
    objects.push_back(std::make_shared<Wall>(leftHallBottom));
    objects.push_back(std::make_shared<Wall>(leftPillarH1));
    objects.push_back(std::make_shared<Wall>(leftPillarH2));

    // правая лабиринт
    objects.push_back(std::make_shared<Wall>(mazeRow1));
    objects.push_back(std::make_shared<Wall>(mazeRow2));
    objects.push_back(std::make_shared<Wall>(mazeRow3));
    objects.push_back(std::make_shared<Wall>(mazeCol1));
    objects.push_back(std::make_shared<Wall>(mazeCol2));

    // нижняя область
    objects.push_back(std::make_shared<Wall>(lowerCover1));
    objects.push_back(std::make_shared<Wall>(lowerCover2));
    objects.push_back(std::make_shared<Wall>(lowerCover3));

    // центральные барьеры и укрытия
    objects.push_back(std::make_shared<Wall>(centralBarrierTop));
    objects.push_back(std::make_shared<Wall>(centralBarrierBottom));
    objects.push_back(std::make_shared<Wall>(smallBlock1));
    objects.push_back(std::make_shared<Wall>(smallBlock2));

    // круги / колонны
    objects.push_back(std::make_shared<Circle>(pillar1));
    objects.push_back(std::make_shared<Circle>(pillar2));
    objects.push_back(std::make_shared<Circle>(pillar3));
    objects.push_back(std::make_shared<Circle>(pillar4));
    objects.push_back(std::make_shared<Circle>(pillar5));
    objects.push_back(std::make_shared<Circle>(pillar6));

    // мелкие препятствия
    objects.push_back(std::make_shared<Circle>(rock1));
    objects.push_back(std::make_shared<Circle>(rock2));
    objects.push_back(std::make_shared<Circle>(rock3));

    // финально применяем карту
    map.setMap(objects);
}

void World::setMapOption3(){
    // Границы (как всегда)
    Wall wallSouth(Point2D(0, 1050), 1920, 30, 0x3E3C32FF);
    Wall wallWest(Point2D(0, 0), 30, 1050, 0x3E3C32FF);
    Wall wallEast(Point2D(1890, 0), 30, 1050, 0x3E3C32FF);
    Wall wallNorth(Point2D(0, 0), 1920, 30, 0x3E3C32FF);

    // Центр — плотный кластер небольших укрытий
    Wall c1(Point2D(760, 460), 60, 20, 0x3E3C32FF);
    Wall c2(Point2D(820, 460), 60, 20, 0x3E3C32FF);
    Wall c3(Point2D(760, 500), 20, 60, 0x3E3C32FF);
    Wall c4(Point2D(880, 500), 20, 60, 0x3E3C32FF);
    Circle cc1(Point2D(820, 520), 18, 0x3E3C32FF);
    Circle cc2(Point2D(740, 520), 18, 0x3E3C32FF);

    // Левая кучка — маленькие блоки и колонны
    Wall l1(Point2D(160, 260), 80, 20, 0x3E3C32FF);
    Wall l2(Point2D(220, 300), 20, 80, 0x3E3C32FF);
    Circle lC1(Point2D(200, 350), 16, 0x3E3C32FF);
    Circle lC2(Point2D(140, 320), 12, 0x3E3C32FF);

    // Правая кучка — узкие проходы
    Wall r1(Point2D(1500, 220), 100, 18, 0x3E3C32FF);
    Wall r2(Point2D(1560, 260), 18, 100, 0x3E3C32FF);
    Circle rC1(Point2D(1520, 300), 14, 0x3E3C32FF);

    // Нижняя область — несколько мелких укрытий по кругу (плотно)
    Circle b1(Point2D(520, 880), 14, 0x3E3C32FF);
    Circle b2(Point2D(580, 900), 14, 0x3E3C32FF);
    Circle b3(Point2D(640, 880), 14, 0x3E3C32FF);
    Wall b4(Point2D(580, 840), 120, 18, 0x3E3C32FF);

    // Собираем (меньше объектов, но плотнее)
    std::vector<std::shared_ptr<Object2D>> objects;
    // границы
    objects.push_back(std::make_shared<Wall>(wallSouth));
    objects.push_back(std::make_shared<Wall>(wallWest));
    objects.push_back(std::make_shared<Wall>(wallEast));
    objects.push_back(std::make_shared<Wall>(wallNorth));

    // центр
    objects.push_back(std::make_shared<Wall>(c1));
    objects.push_back(std::make_shared<Wall>(c2));
    objects.push_back(std::make_shared<Wall>(c3));
    objects.push_back(std::make_shared<Wall>(c4));
    objects.push_back(std::make_shared<Circle>(cc1));
    objects.push_back(std::make_shared<Circle>(cc2));

    // левая кучка
    objects.push_back(std::make_shared<Wall>(l1));
    objects.push_back(std::make_shared<Wall>(l2));
    objects.push_back(std::make_shared<Circle>(lC1));
    objects.push_back(std::make_shared<Circle>(lC2));

    // правая кучка
    objects.push_back(std::make_shared<Wall>(r1));
    objects.push_back(std::make_shared<Wall>(r2));
    objects.push_back(std::make_shared<Circle>(rC1));

    // нижняя плотная группа
    objects.push_back(std::make_shared<Circle>(b1));
    objects.push_back(std::make_shared<Circle>(b2));
    objects.push_back(std::make_shared<Circle>(b3));
    objects.push_back(std::make_shared<Wall>(b4));

    // Применяем карту
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
    //camera.setMap(map); //ПОКА НИЧЕГО НЕ ДВИЖЕТСЯ НА КАРТЕ - НЕ ЮЗАТЬ ОБНОВЛЕНИЕ КАРТЫ
    camera.moveWithKeyboard(deltaTime);
    //setCircleMovable(deltaTime);
}

void World::setCircleMovable(double deltaTime){
    static int currentDirection = 0;
    
    float moveSpeed = 100.0f;
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
    camera.drawCameraOnMap(window);
}

void World::render(){
    camera.drawCameraView(window);
    display2DMap(window);
    window.display();
}