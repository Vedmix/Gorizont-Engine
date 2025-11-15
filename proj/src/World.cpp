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
    // Границы
    Wall wallSouth(Point2D(0, 1050), 1920, 30, 0xF0F8FFFF);
    Wall wallWest(Point2D(0, 0), 30, 1050, 0xF0F8FFFF);
    Wall wallEast(Point2D(1890, 0), 30, 1050, 0xF0F8FFFF);
    Wall wallNorth(Point2D(0, 0), 1920, 30, 0xF0F8FFFF);

    // Центральная зона: две комнаты с дверью между ними
    Wall room1Top(Point2D(520, 300), 560, 30, 0xF0F8FFFF);
    Wall room1BottomLeft(Point2D(520, 540), 250, 30, 0xF0F8FFFF);
    Wall room1BottomRight(Point2D(830, 540), 250, 30, 0xF0F8FFFF);
    // вертикальные стороны комнат (левая и правая)
    Wall room1Left(Point2D(520, 300), 30, 270, 0xF0F8FFFF);
    Wall room1Right(Point2D(1050, 300), 30, 270, 0xF0F8FFFF);
    // проход в центре между bottomLeft и bottomRight (дверь) — оставляем зазор

    // Центральный коридор / разделитель
    Wall centerVertTop(Point2D(915, 0), 30, 240, 0xF0F8FFFF);
    Wall centerVertBottom(Point2D(915, 810), 30, 240, 0xF0F8FFFF);

    // Левая секция: несколько укрытий и проходов
    Wall leftHallTop(Point2D(60, 150), 420, 30, 0xF0F8FFFF);
    Wall leftHallBottom(Point2D(60, 420), 420, 30, 0xF0F8FFFF);
    Wall leftPillarH1(Point2D(300, 180), 30, 90, 0xF0F8FFFF);
    Wall leftPillarH2(Point2D(300, 420), 30, 90, 0xF0F8FFFF);

    // Правая секция: лабиринт из узких стен
    Wall mazeRow1(Point2D(1320, 100), 460, 30, 0xF0F8FFFF);
    Wall mazeRow2(Point2D(1320, 220), 300, 30, 0xF0F8FFFF);
    Wall mazeRow3(Point2D(1320, 340), 460, 30, 0xF0F8FFFF);
    Wall mazeCol1(Point2D(1320, 100), 30, 300, 0xF0F8FFFF);
    Wall mazeCol2(Point2D(1650, 220), 30, 300, 0xF0F8FFFF);

    // Нижняя область: островки (пилоны) и короткие укрытия
    Wall lowerCover1(Point2D(420, 900), 180, 30, 0xF0F8FFFF);
    Wall lowerCover2(Point2D(840, 900), 180, 30, 0xF0F8FFFF);
    Wall lowerCover3(Point2D(1320, 900), 180, 30, 0xF0F8FFFF);

    // Центр: два параллельных барьера с проходом
    Wall centralBarrierTop(Point2D(640, 420), 560, 20, 0xF0F8FFFF);
    Wall centralBarrierBottom(Point2D(640, 660), 560, 20, 0xF0F8FFFF);

    // Небольшие укрытия/углы
    Wall smallBlock1(Point2D(200, 660), 120, 120, 0xF0F8FFFF);
    Wall smallBlock2(Point2D(1600, 540), 120, 120, 0xF0F8FFFF);

    // Круглые объекты (пилоны/колонны) — разной величины
    Circle pillar1(Point2D(320, 300), 40, 0xF0F8FFFF);   // другой оттенок (коричневый)
    Circle pillar2(Point2D(480, 300), 30, 0xF0F8FFFF);
    Circle pillar3(Point2D(720, 540), 25, 0xF0F8FFFF);
    Circle pillar4(Point2D(1100, 150), 35, 0xF0F8FFFF);
    Circle pillar5(Point2D(1500, 760), 45, 0xF0F8FFFF);
    Circle pillar6(Point2D(300, 900), 20, 0xF0F8FFFF);

    // Разбросанные небольшие препятствия (подъезды/островки)
    Circle rock1(Point2D(980, 820), 18, 0xF0F8FFFF);
    Circle rock2(Point2D(540, 820), 18, 0xF0F8FFFF);
    Circle rock3(Point2D(1400, 480), 22, 0xF0F8FFFF);

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


const Map& World::getMap() const{
    return map;
}

void World::addObject(std::shared_ptr<Object2D> object){
    map.addObject(object);
}

void World::setColor(unsigned int _color){
    color = sf::Color(_color);
}

void World::update(double deltaTime){
    //camera.setMap(map); //ПОКА НИЧЕГО НЕ ДВИЖЕТСЯ НА КАРТЕ - НЕ ЮЗАТЬ ОБНОВЛЕНИЕ КАРТЫ
    camera.moveWithKeyboard(deltaTime);
}

void World::display2DMap(sf::RenderWindow& window){
    map.render(window);
    camera.drawCameraOnMap(window);
}

void World::renderMinimapObjects(sf::RenderTexture& texture, float scale) {
    for (auto& obj : map.objectSet) {
        obj->render(texture, scale);
    }
}

void World::renderPlayerOnMinimap(sf::RenderTexture& texture) {
    camera.drawCameraOnMap(texture);
}

void World::renderMinimapBackground(sf::RenderTexture& texture) {
    sf::RectangleShape mapBackground(sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
    mapBackground.setFillColor(sf::Color(40, 40, 60));
    mapBackground.setPosition(0, 0);
    texture.draw(mapBackground);
}

void World::renderToTexture(sf::RenderTexture& texture) {
    texture.clear(sf::Color(40, 40, 60));

    camera.drawCameraView(texture);

    sf::View originalView = texture.getView();

    const float minimapWidth = 1200.0f;
    const float minimapHeight = 800.0f;
    const float minimapX = 20.0f;
    const float minimapY = 20.0f;

    sf::View minimapView(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    minimapView.setViewport(sf::FloatRect(minimapX / SCREEN_WIDTH, minimapY / SCREEN_HEIGHT, minimapWidth / SCREEN_WIDTH, minimapHeight / SCREEN_HEIGHT));

    texture.setView(minimapView);

    renderMinimapBackground(texture);
    renderMinimapObjects(texture, 1.0f);
    renderPlayerOnMinimap(texture);

    texture.setView(originalView);
}
