#include "../headers/Map.hpp"

Map::Map(){}

Map::Map(const Map& other){
    this->objectSet = other.objectSet;
}

Map::Map(std::vector<std::shared_ptr<Object2D>> objects){
    for(auto& obj:objects){
        objectSet.insert(obj);
    }
}

Map& Map::operator=(const Map& other){
    if(this != &other){
        this->objectSet = other.objectSet;
        return *this;
    }
    return *this;
}

void Map::addObject(std::shared_ptr<Object2D> object){
    objectSet.insert(object);
}

void Map::render(sf::RenderTarget& window){
    // Сохраняем текущий вид
    sf::View originalView = window.getView();

    // Устанавливаем вид для мини-карты в координатах ЭКРАНА
    sf::View minimapView(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    window.setView(minimapView);

    // Фон мини-карты
    if(SCREEN_WIDTH == 1920){
        sf::RectangleShape minimapBg(sf::Vector2f(SCREEN_WIDTH * MAP_SCALE, SCREEN_HEIGHT * MAP_SCALE));
        minimapBg.setFillColor(sf::Color::Black);
        minimapBg.setPosition(0, 0);
        window.draw(minimapBg);
    }else if(SCREEN_WIDTH == 3840){
        sf::RectangleShape minimapBg(sf::Vector2f(SCREEN_WIDTH * MAP_SCALE * 0.5, SCREEN_HEIGHT * MAP_SCALE * 0.5));
        minimapBg.setFillColor(sf::Color::Black);
        minimapBg.setPosition(0, 0);
        window.draw(minimapBg);
    }

    for (const auto& obj : objectSet) {
        if(obj->getObjectType() == ObjectType::CAMERA){
            continue;
        }

        if (auto circle = std::dynamic_pointer_cast<Circle>(obj)) {
            Point2D pos = circle->getPos();
            double radius = circle->getRadius();

            sf::CircleShape shape(radius * MAP_SCALE);
            shape.setFillColor(sf::Color(200, 200, 200));
            shape.setPosition(pos.getX() * MAP_SCALE, pos.getY() * MAP_SCALE);
            shape.setOrigin(radius * MAP_SCALE, radius * MAP_SCALE);
            window.draw(shape);
        }
        else if (auto polygon = std::dynamic_pointer_cast<Polygon2D>(obj)) {
            const std::vector<Point2D>& points = polygon->getPoints();
            if (points.empty()) continue;

            Point2D pos = polygon->getPos();

            sf::ConvexShape shape;
            shape.setPointCount(points.size());

            for (size_t i = 0; i < points.size(); ++i) {
                shape.setPoint(i, sf::Vector2f((pos.getX() + points[i].getX()) * MAP_SCALE, (pos.getY() + points[i].getY()) * MAP_SCALE));
            }

            shape.setFillColor(sf::Color(150, 150, 150));
            window.draw(shape);
        }
    }
    window.setView(originalView);
}

void Map::setMap(std::vector<std::shared_ptr<Object2D>> objects){
    objectSet.clear();
    for(auto& obj:objects){
        objectSet.insert(obj);
    }
}

const std::set<std::shared_ptr<Object2D>>& Map::getObjects() const {
    return objectSet;
}

Map::~Map(){}
