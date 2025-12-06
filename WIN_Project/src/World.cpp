#include "../headers/World.hpp"
#include "../headers/settings.hpp"

World::World():camera(Point2D(300, 300), 30, 0xFF0000FF, this->map),window(), isRunning(true), XMLFilePath("C:/Users/vedmix/Gorizont-Engine/proj/maps/map2.xml")
{
    if(!USE_QT){
        window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Gorizont(SFML Mode)");
        window.setFramerateLimit(60);
    }
    this->loadMapFromXML();
    font.loadFromFile("C:/Users/vedmix/Gorizont-Engine/proj/fonts/font.ttf");
    camera.setMap(this->map);
    color = sf::Color::Black;
}

World::~World(){}

void World::run(){
    if(!USE_QT){
        while (isRunning && window.isOpen()){
            double deltaTime = clock.restart().asSeconds();
            handleEvents();
            update(deltaTime);
            render();
        }
    }
}

void World::update(double deltaTime){
    if(!USE_QT){
        window.clear(color);
        updateFPS();
    }

    camera.moveWithKeyboard(deltaTime);
   // camera.setMap(map); //ПОКА НИЧЕГО НЕ ДВИЖЕТСЯ НА КАРТЕ - НЕ ЮЗАТЬ ОБНОВЛЕНИЕ КАРТЫ
   // setCircleMovable(deltaTime);
}

void World::updateFPS() {
    if(!USE_QT){
        static int frameCount = 0;
        static sf::Clock fpsClock;

        frameCount++;

        if (fpsClock.getElapsedTime().asMilliseconds() >= 100) {
            currentFPS = frameCount / (fpsClock.restart().asMilliseconds() / 1000.0f);
            frameCount = 0;
        }
    }
}

void World::drawFPS() {
    if(!USE_QT){
        sf::Text fpsText;
        fpsText.setFont(font);
        fpsText.setCharacterSize(30);
        fpsText.setFillColor(sf::Color::Green);

        fpsText.setPosition(SCREEN_WIDTH - 200, 20);
        fpsText.setString("FPS: " + std::to_string(static_cast<int>(currentFPS)));

        window.draw(fpsText);
    }
}

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


void World::handleEvents(){
    if(!USE_QT){
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                isRunning = false;
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                isRunning = false;
        }
    }
}



void World::setCircleMovable(double deltaTime){
    static int currentDirection = 0;
    
    float moveSpeed = 100.0f;
    float left = 100;
    float right = SCREEN_WIDTH - 200;
    float top = 100;
    float bottom = SCREEN_HEIGHT - 200;
    
    for(auto& obj:map.objectSet) {
        if (obj->getObjectType() != ObjectType::POLYGON) {
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

void World::display2DMap(sf::RenderTarget& target){
    map.render(target);
    camera.drawCameraOnMap(target);
}

void World::render(){
    if(!USE_QT){
        camera.drawCameraView(window);
        display2DMap(window);
        drawFPS();
        window.display();
    }
}

void World::renderToTexture(sf::RenderTexture& texture) {
    if(USE_QT){
        texture.clear(color);
        camera.drawCameraView(texture);
        display2DMap(texture);
        texture.display();
    }
}

void World::readWallsXML(){
    std::ifstream file(XMLFilePath);
    std::string line;
    std::regex wallPattern("<wall\\s+x=\"([0-9.]+)\"\\s+y=\"([0-9.]+)\"\\s+width=\"([0-9.]+)\"\\s+height=\"([0-9.]+)\"\\s+color=\"(0x[0-9A-Fa-f]+)\"\\s*\\/?>");
    bool inWallsSection = false;

    Point2D wallPos;

    if(!file.is_open()){
        std::cout << "!!!!!FILE - V S E!!!!!";
        return;
    }

    while(std::getline(file, line)){
        if(line.find("<walls>") != std::string::npos){
            inWallsSection = true;
            continue;
        } else if(line.find("</walls>") != std::string::npos){
            inWallsSection = false;
            continue; 
        }

        if(inWallsSection){
            std::smatch matches;
            if(std::regex_search(line, matches, wallPattern)){
                wallPos.setPoint(std::stod(matches[1]), std::stod(matches[2]));

                //adding wall object with constructor: 
                //Wall(const Point2D& _position, const double _width, const double _lenght, unsigned int _color);
                map.addObject(std::make_shared<Wall>(wallPos, std::stod(matches[3]), std::stod(matches[4]), std::stoul(matches[5], nullptr, 16)));
            }
        }
    }
}

void World::readCirclesXML(){
    std::ifstream file(XMLFilePath);
    std::string line;
    std::regex circlePattern("<circle\\s+x=\"([0-9.]+)\"\\s+y=\"([0-9.]+)\"\\s+radius=\"([0-9.]+)\"\\s+color=\"(0x[0-9A-Fa-f]+)\"\\s*\\/?>");
    bool inCirclesSection = false; 
    
    if(!file.is_open()){
        std::cout << "!!!!!FILE - V S E!!!!!";
        return;
    }

    Point2D circlePos;
    while(std::getline(file, line)){
        if(line.find("<circles>") != std::string::npos){
            inCirclesSection = true;
            continue;
        } else if(line.find("</circles>") != std::string::npos){
            inCirclesSection = false;
            continue; 
        }

        if(inCirclesSection){
            std::smatch matches;
            if(std::regex_search(line, matches, circlePattern)){
                circlePos.setPoint(std::stod(matches[1]), std::stod(matches[2]));

                //adding circle object with constructor: 
                //Circle(const Point2D& _position, double _radius, unsigned int _color);
                map.addObject(std::make_shared<Circle>(circlePos, std::stod(matches[3]), std::stoul(matches[5], nullptr, 16)));
            }
        }
    }
}

void World::loadMapFromXML(){
    readWallsXML();
    readCirclesXML();
}
