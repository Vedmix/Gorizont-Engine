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

void Map::render(sf::RenderTarget& window, double scale){
    // Сохраняем текущий вид
    sf::View originalView = window.getView();

    // Устанавливаем вид для мини-карты в координатах ЭКРАНА
    auto& settings = AppSettings::instance();
    sf::View minimapView(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(settings.screenWidth(), settings.screenHeight())));
    window.setView(minimapView);

    // Фон мини-карты
    sf::RectangleShape minimapBg(sf::Vector2f(settings.screenWidth() * scale, settings.screenHeight() * scale));
    minimapBg.setFillColor(sf::Color::Black);
    minimapBg.setPosition(sf::Vector2f(0, 0));
    window.draw(minimapBg);

    for (const auto& obj : objectSet) {
        if(obj->getObjectType() == ObjectType::CAMERA){
            continue;
        }

        if (auto circle = std::dynamic_pointer_cast<Circle>(obj)) {
            Point2D pos = circle->getPos();
            double radius = circle->getRadius();

            sf::CircleShape shape(radius * scale);
            shape.setFillColor(sf::Color(200, 200, 200));
            shape.setPosition(sf::Vector2f(pos.getX() * scale, pos.getY() * scale));
            shape.setOrigin(sf::Vector2f(radius * scale, radius * scale));
            window.draw(shape);
        }
        else if (auto polygon = std::dynamic_pointer_cast<Polygon2D>(obj)) {
            const std::vector<Point2D>& points = polygon->getPoints();
            if (points.empty()) continue;

            Point2D pos = polygon->getPos();

            sf::ConvexShape shape;
            shape.setPointCount(points.size());

            for (size_t i = 0; i < points.size(); ++i) {
                shape.setPoint(i, sf::Vector2f((pos.getX() + points[i].getX()) * scale, (pos.getY() + points[i].getY()) * scale ));
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
