#include "../headers/DebugPanel.hpp"
#include "../headers/settings.hpp"
#include "../headers/Wall.hpp"
#include "../headers/Circle.hpp"
#include "../headers/Polygon2D.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <random>

DebugPanel::DebugPanel(sf::RenderWindow& window) : 
    window(window), camera(nullptr), map(nullptr),
    currentEditMode(EditMode::NONE), currentAddMode(AddObjectMode::NONE),
    isObjectSelected(false), isDrawingPolygon(false),
    isPlacingObject(false), currentRotation(0), currentSize(50.0)  // Добавить инициализацию
{
    font.loadFromFile("../font.ttf");
    PanelInit();
}

DebugPanel::~DebugPanel(){
}

void DebugPanel::setCamera(Camera* cam) {
    camera = cam;
}

void DebugPanel::setMap(Map* mapPtr) {
    map = mapPtr;
}

void DebugPanel::PanelInit(){
    initMainFrame();
    initTextElements();
    initButtons();
    initSliders();
}

void DebugPanel::initMainFrame(){
    debugPanel.setSize(sf::Vector2f(UI_PANEL_WIDTH, static_cast<double>(window.getSize().y)));
    debugPanel.setPosition(static_cast<double>(window.getSize().x) - UI_PANEL_WIDTH, 0);
    debugPanel.setFillColor(sf::Color(40, 40, 50, 240));
}

void DebugPanel::initTextElements(){
    double startX = static_cast<double>(window.getSize().x) - UI_PANEL_WIDTH;
    
    fpsText.setFont(font);
    fpsText.setCharacterSize(18);
    fpsText.setFillColor(sf::Color::Green);
    fpsText.setPosition(startX + 15, 20);
    fpsText.setString("FPS: 0.0");
    
    cameraPosText.setFont(font);
    cameraPosText.setCharacterSize(14);
    cameraPosText.setFillColor(sf::Color::White);
    cameraPosText.setPosition(startX + 15, 50);
    cameraPosText.setString("Position: (0, 0)");
    
    editModeText.setFont(font);
    editModeText.setCharacterSize(14);
    editModeText.setFillColor(sf::Color::Yellow);
    editModeText.setPosition(startX + 15, 80);
    editModeText.setString("Edit Mode: NONE");
    
    addModeText.setFont(font);
    addModeText.setCharacterSize(14);
    addModeText.setFillColor(sf::Color::Cyan);
    addModeText.setPosition(startX + 15, 110);
    addModeText.setString("Add Mode: NONE");
}

void DebugPanel::initButtons(){
    double startX = static_cast<double>(window.getSize().x) - UI_PANEL_WIDTH;
    
    // Кнопки управления камерой
    addButton("Reset Camera", Point2D(startX + 20, 150), [this](){ 
        if (camera){
            camera->setFOV(PI / 2);
            camera->setRenderDistance(100);
            camera->setNumberRaysInFov(SCREEN_WIDTH/8);
            camera->setVelocity(300);
            resetSlidersToDefault();
        }
    });
    
    // Кнопки добавления объектов
    addButton("Add Wall", Point2D(startX + 20, 200), [this](){ 
        currentAddMode = AddObjectMode::WALL;
        currentEditMode = EditMode::ADD_OBJECT;
        addModeText.setString("Add Mode: WALL - Click to place");
        editModeText.setString("Edit Mode: ADD OBJECT");
        isDrawingPolygon = false;
        polygonPoints.clear();
    });
    
    addButton("Add Circle", Point2D(startX + 20, 240), [this](){ 
        currentAddMode = AddObjectMode::CIRCLE;
        currentEditMode = EditMode::ADD_OBJECT;
        addModeText.setString("Add Mode: CIRCLE - Click to place");
        editModeText.setString("Edit Mode: ADD OBJECT");
        isDrawingPolygon = false;
        polygonPoints.clear();
    });
    
    addButton("Add Rectangle", Point2D(startX + 20, 280), [this](){ 
        currentAddMode = AddObjectMode::SQUARE;
        currentEditMode = EditMode::ADD_OBJECT;
        addModeText.setString("Add Mode: SQUARE - Click to place");
        editModeText.setString("Edit Mode: ADD OBJECT");
        isDrawingPolygon = false;
        polygonPoints.clear();
    });
    
    addButton("Add Triangle", Point2D(startX + 20, 320), [this](){ 
        currentAddMode = AddObjectMode::TRIANGLE;
        currentEditMode = EditMode::ADD_OBJECT;
        addModeText.setString("Add Mode: TRIANGLE - Click to place");
        editModeText.setString("Edit Mode: ADD OBJECT");
        isDrawingPolygon = false;
        polygonPoints.clear();
    });
    
    // Кнопки редактирования объектов
    addButton("Move Object", Point2D(startX + 20, 420), [this](){ 
        currentEditMode = EditMode::MOVE_OBJECT;
        currentAddMode = AddObjectMode::NONE;
        editModeText.setString("Edit Mode: MOVE OBJECT");
        addModeText.setString("Add Mode: NONE");
        isDrawingPolygon = false;
        polygonPoints.clear();
    });
    
    addButton("Delete Object", Point2D(startX + 20, 460), [this](){ 
        currentEditMode = EditMode::DELETE_OBJECT;
        currentAddMode = AddObjectMode::NONE;
        editModeText.setString("Edit Mode: DELETE OBJECT");
        addModeText.setString("Add Mode: NONE");
        isDrawingPolygon = false;
        polygonPoints.clear();
    });
    
    addButton("Clear Selection", Point2D(startX + 20, 540), [this](){ 
        selectedObject = nullptr;
        isObjectSelected = false;
        currentEditMode = EditMode::NONE;
        currentAddMode = AddObjectMode::NONE;
        editModeText.setString("Edit Mode: NONE");
        addModeText.setString("Add Mode: NONE");
        isDrawingPolygon = false;
        polygonPoints.clear();
    });
}

void DebugPanel::initSliders(){
    double startX = static_cast<double>(window.getSize().x) - UI_PANEL_WIDTH;
    double startY = 630.0f;
    double verticalSpacing = 60.0f;

    // Слайдеры камеры
    addSlider("FOV", Point2D(startX + 20, startY), 30.0f, 120.0f, 90.0f, [this](double value){ 
        if (camera){
            camera->setFOV(value * 3.14159f / 180.0f);
        }
    });
    
    addSlider("Render Distance", Point2D(startX + 20, startY + verticalSpacing), 100.0f, 1000.0f, 100.0f, [this](double value){ 
        if (camera){
            camera->setRenderDistance(value);
        }
    });
    
    addSlider("Move Speed", Point2D(startX + 20, startY + verticalSpacing * 2), 100.0f, 1000.0f, 300.0f, [this](double value){ 
        if (camera){
            camera->setVelocity(value);
        }
    });
    
    addSlider("Ray Count", Point2D(startX + 20, startY + verticalSpacing * 3), 10.0f, 500.0f, static_cast<double>(SCREEN_WIDTH/8), [this](double value){ 
        if (camera){
            camera->setNumberRaysInFov(static_cast<int>(value));
        }
    });
}

void DebugPanel::addButton(const std::string& label, const Point2D& position, std::function<void()> action){
    Button button;

    button.shape.setSize(sf::Vector2f(200, 35));
    button.shape.setPosition(static_cast<double>(position.getX()), static_cast<double>(position.getY()));
    button.shape.setFillColor(sf::Color(60, 60, 70));
    button.shape.setOutlineColor(sf::Color::White);
    button.shape.setOutlineThickness(1);

    button.text.setFont(font);
    button.text.setString(label);
    button.text.setCharacterSize(14);
    button.text.setFillColor(sf::Color::White);
    button.text.setPosition(static_cast<double>(position.getX()) + 10, static_cast<double>(position.getY()) + 8);

    button.action = action;
    button.isActive = true;
    
    buttons.push_back(button);
}

void DebugPanel::addSlider(const std::string& label, const Point2D& position, double minVal, double maxVal, double initialVal, std::function<void(double)> onChange){
    Slider slider;

    slider.track.setSize(sf::Vector2f(200, 6));
    slider.track.setPosition(static_cast<double>(position.getX()), static_cast<double>(position.getY()) + 25);
    slider.track.setFillColor(sf::Color(100, 100, 100));
    
    slider.thumb.setSize(sf::Vector2f(12, 20));
    slider.thumb.setFillColor(sf::Color::White);
    
    slider.label.setFont(font);
    slider.label.setString(label);
    slider.label.setCharacterSize(14);
    slider.label.setFillColor(sf::Color::White);
    slider.label.setPosition(static_cast<double>(position.getX()), static_cast<double>(position.getY()));
    
    slider.valueText.setFont(font);
    slider.valueText.setCharacterSize(12);
    slider.valueText.setFillColor(sf::Color::Yellow);
    slider.valueText.setPosition(static_cast<double>(position.getX()) + 210, static_cast<double>(position.getY()) + 20);
    
    slider.minValue = minVal;
    slider.maxValue = maxVal;
    slider.currentValue = initialVal;
    slider.onChange = onChange;
    slider.isDragging = false;
    
    updateSliderValue(slider, slider.track.getPosition().x + ((initialVal - minVal) / (maxVal - minVal)) * 200.0f);
    
    sliders.push_back(slider);
}

void DebugPanel::resetSlidersToDefault(){
    if (sliders.size() >= 4){
        sliders[0].currentValue = 90.0f;
        updateSliderValue(sliders[0], sliders[0].track.getPosition().x + ((90.0f - 30.0f) / (120.0f - 30.0f)) * 200.0f);
        sliders[1].currentValue = 100.0f;
        updateSliderValue(sliders[1], sliders[1].track.getPosition().x + ((100.0f - 100.0f) / (1000.0f - 100.0f)) * 200.0f);
        sliders[2].currentValue = 300.0f;
        updateSliderValue(sliders[2], sliders[2].track.getPosition().x + ((300.0f - 100.0f) / (1000.0f - 100.0f)) * 200.0f);
        sliders[3].currentValue = static_cast<double>(SCREEN_WIDTH/8);
        updateSliderValue(sliders[3], sliders[3].track.getPosition().x + ((static_cast<double>(SCREEN_WIDTH/8) - 10.0f) / (500.0f - 10.0f)) * 200.0f);
        
        for (auto& slider : sliders){
            if (slider.onChange){
                slider.onChange(slider.currentValue);
            }
        }
    }
}

void DebugPanel::update(double deltaTime){
    if (!camera)
    {
        return;
    }
    std::stringstream fpsStream;
    fpsStream << "FPS: " << std::fixed << std::setprecision(1) << (deltaTime > 0 ? 1.0 / deltaTime : 0);
    fpsText.setString(fpsStream.str());
    
    Point2D camPos = camera->getPos();
    std::stringstream posStream;
    posStream << "Position: (" << std::fixed << std::setprecision(1) << camPos.getX() << ", " << camPos.getY() << ")";
    cameraPosText.setString(posStream.str());
   
    for (auto& slider : sliders){
        std::stringstream valueStream;
        valueStream << std::fixed << std::setprecision(0) << slider.currentValue;
        slider.valueText.setString(valueStream.str());
    }
}

void DebugPanel::handleEvent(const sf::Event& event){
    if (event.type == sf::Event::MouseButtonPressed){
        sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
        
        if (!isMouseOverPanel(mousePos)) {
            // Если мы в режиме добавления и объект еще не размещается
            if (currentAddMode != AddObjectMode::NONE && !isPlacingObject && event.mouseButton.button == sf::Mouse::Left) {
                objectStartPos = Point2D(mousePos.x, mousePos.y);
                isPlacingObject = true;
                currentRotation = 0;
                currentSize = 50.0; // Начальный размер
                return;
            }
            handleObjectEditing(mousePos, event);
            return;
        }
        
        // Обработка кнопок на панели
        for (size_t i = 0; i < buttons.size(); ++i){
            if (buttons[i].isActive && buttons[i].shape.getGlobalBounds().contains(mousePos)){
                buttons[i].shape.setFillColor(sf::Color(80, 80, 90));
                if (buttons[i].action){
                    buttons[i].action();
                }
            }
        }

        // Обработка слайдеров
        for (auto& slider : sliders){
            if (slider.thumb.getGlobalBounds().contains(mousePos)){
                slider.isDragging = true;
                break;
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased){
        // Завершаем размещение объекта при отпускании кнопки мыши
        if (isPlacingObject && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            Point2D endPos(mousePos.x, mousePos.y);
            finishObjectPlacement(endPos);
            isPlacingObject = false;
        }
        
        // Сброс цвета кнопок
        for (auto& button : buttons){
            button.shape.setFillColor(sf::Color(60, 60, 70));
        }
        
        // Сброс перетаскивания слайдеров
        for (auto& slider : sliders){
            slider.isDragging = false;
        }
    }
    else if (event.type == sf::Event::MouseMoved){
        sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
        
        // Если объект размещается, обновляем предпросмотр
        if (isPlacingObject) {
            updateObjectPreview(Point2D(mousePos.x, mousePos.y));
        }
        
        // Обработка перетаскивания слайдеров
        for (auto& slider : sliders){
            if (slider.isDragging){
                updateSliderValue(slider, mousePos.x);
                if (slider.onChange){
                    slider.onChange(slider.currentValue);
                }
            }
        }
        
        // Обработка перемещения объекта
        if (currentEditMode == EditMode::MOVE_OBJECT && isObjectSelected && selectedObject && 
            sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            Point2D newPos(mousePos.x, mousePos.y);
            selectedObject->setPos(newPos);
        }
    }
    else if (event.type == sf::Event::MouseWheelScrolled){
        // Обработка вращения колесом мыши только для прямоугольников и треугольников
        if (isPlacingObject && (currentAddMode == AddObjectMode::SQUARE || currentAddMode == AddObjectMode::TRIANGLE)) {
            if (event.mouseWheelScroll.delta > 0) {
                currentRotation += 15.0; // Поворот по часовой на 15 градусов
            } else {
                currentRotation -= 15.0; // Поворот против часовой на 15 градусов
            }
            
            // Ограничиваем вращение в диапазоне 0-360 градусов
            while (currentRotation >= 360.0) currentRotation -= 360.0;
            while (currentRotation < 0.0) currentRotation += 360.0;
        }
    }
}

void DebugPanel::updateObjectPreview(const Point2D& currentMousePos){
    // Для всех объектов вычисляем размер на основе расстояния
    double dx = currentMousePos.getX() - objectStartPos.getX();
    double dy = currentMousePos.getY() - objectStartPos.getY();
    double distance = std::sqrt(dx*dx + dy*dy);
    
    switch(currentAddMode){
        case AddObjectMode::WALL:
            // Для стен вычисляем угол поворота
            currentRotation = std::atan2(dy, dx) * 180.0 / M_PI;
            currentSize = distance;
            break;
            
        case AddObjectMode::CIRCLE:
            // Для круга используем расстояние как радиус
            currentSize = std::max(10.0, distance);
            break;
            
        case AddObjectMode::SQUARE:
            // Для прямоугольника сохраняем конечную позицию
            currentSize = distance;
            break;
            
        case AddObjectMode::TRIANGLE:
            // Для треугольника используем расстояние как размер
            currentSize = std::max(10.0, distance);
            break;
            
        default:
            break;
    }
}

void DebugPanel::finishObjectPlacement(const Point2D& endPos){
    if (!map) return;
    
    // Создаем объект в зависимости от текущего режима
    switch(currentAddMode){
        case AddObjectMode::WALL:
            createWallInteractive(objectStartPos, endPos, currentSize, currentRotation);
            break;
        case AddObjectMode::CIRCLE:
            createCircleInteractive(objectStartPos, currentSize, currentRotation);
            break;
        case AddObjectMode::SQUARE:
            createSquareInteractive(objectStartPos, endPos, currentSize, currentRotation); // Изменил на квадрат
            break;
        case AddObjectMode::TRIANGLE:
            createTriangleInteractive(objectStartPos, currentSize, currentRotation);
            break;
        default:
            break;
    }
    
    // Сбрасываем режим
    currentAddMode = AddObjectMode::NONE;
    currentEditMode = EditMode::NONE;
    isPlacingObject = false;
    addModeText.setString("Add Mode: NONE");
    editModeText.setString("Edit Mode: NONE");
}

void DebugPanel::createWallInteractive(const Point2D& startPos, const Point2D& endPos, double size, double rotation){
    // Вычисляем центр стены
    Point2D center((startPos.getX() + endPos.getX()) / 2, 
                   (startPos.getY() + endPos.getY()) / 2);
    
    // Вычисляем ширину и высоту (расстояние между точками)
    double width = std::abs(endPos.getX() - startPos.getX());
    double height = std::abs(endPos.getY() - startPos.getY());
    
    // Минимальный размер
    width = std::max(10.0, width);
    height = std::max(10.0, height);
    
    // Толщина стены (фиксированная)
    float wallThickness = 10.0f;
    
    Wall* newWall = nullptr;
    
    // Определяем ориентацию стены и создаем соответствующую стену
    if (width > height) {
        // Горизонтальная стена
        newWall = new Wall(center, static_cast<int>(wallThickness), static_cast<int>(width), 0x3E3C32FF);
    } else {
        // Вертикальная стена
        newWall = new Wall(center, static_cast<int>(height), static_cast<int>(wallThickness), 0x3E3C32FF);
    }
    
    map->addObject(std::shared_ptr<Wall>(newWall));
    
    std::cout << "✅ Added wall: " << width << "x" << height << std::endl;
    
    // Убираем предупреждения о неиспользуемых параметрах
    (void)size;
    (void)rotation;
}

void DebugPanel::createCircleInteractive(const Point2D& center, double radius, double rotation){
    Circle newCircle(center, radius, 0x3E3C32FF);
    
    // Если в классе Circle есть метод setRotation, используем его
    // newCircle.setRotation(rotation);
    
    map->addObject(std::make_shared<Circle>(newCircle));
    
    // Убрать предупреждение о неиспользуемых параметрах
    (void)rotation;
}
void DebugPanel::createSquareInteractive(const Point2D& startPos, const Point2D& endPos, double size, double rotation){
    // Вычисляем центр квадрата
    Point2D center((startPos.getX() + endPos.getX()) / 2, 
                   (startPos.getY() + endPos.getY()) / 2);
    
    // Вычисляем ширину и высоту
    double width = std::abs(endPos.getX() - startPos.getX());
    double height = std::abs(endPos.getY() - startPos.getY());
    
    // Для квадрата берем максимальную сторону
    double side = std::max(width, height);
    side = std::max(20.0, side); // Минимальный размер
    
    Wall newSquare(center, static_cast<int>(side), static_cast<int>(side), 0x3E3C32FF);
    
    map->addObject(std::make_shared<Wall>(newSquare));
    
    std::cout << "✅ Added square: " << side << "x" << side << std::endl;
    
    // Убираем предупреждения о неиспользуемых параметрах
    (void)size;
    (void)rotation;
}

void DebugPanel::addSquareAtPosition(const Point2D& position){
    if (!map) return;
   
    Wall newSquare(position, 80, 80, 0x3E3C32FF); // Квадрат одинаковые width и height
    map->addObject(std::make_shared<Wall>(newSquare));
}
void DebugPanel::createTriangleInteractive(const Point2D& center, double size, double rotation){
    // Угол вращения в радианах
    double angle = rotation * M_PI / 180.0;
    
    // Вершины равностороннего треугольника
    std::vector<Point2D> trianglePoints = {
        Point2D(0, -size),                           // Верхняя вершина
        Point2D(size * 0.866, size * 0.5),          // Правая нижняя (cos30=0.866, sin30=0.5)
        Point2D(-size * 0.866, size * 0.5)          // Левая нижняя
    };
    
    // Применяем вращение ко всем точкам
    for (auto& point : trianglePoints) {
        double x = point.getX() * std::cos(angle) - point.getY() * std::sin(angle);
        double y = point.getX() * std::sin(angle) + point.getY() * std::cos(angle);
        point.setX(x);
        point.setY(y);
    }
    
    Polygon2D newTriangle(center, trianglePoints, 0x3E3C32FF);
    map->addObject(std::make_shared<Polygon2D>(newTriangle));
}

void DebugPanel::handleObjectEditing(const sf::Vector2f& mousePos, const sf::Event& event){
    Point2D worldPos(mousePos.x, mousePos.y);
    
    if (currentAddMode != AddObjectMode::NONE) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            handleAddObject(worldPos);
        }
        else if (event.mouseButton.button == sf::Mouse::Right && isDrawingPolygon) {
            if (polygonPoints.size() >= 3) {
                addPolygonFromPoints();
            }
        }
        return;
    }
    
    switch(currentEditMode){
        case EditMode::MOVE_OBJECT:
            if (event.mouseButton.button == sf::Mouse::Left) {
                selectObjectAtPosition(worldPos);
            }
            break;
            
        case EditMode::DELETE_OBJECT:
            if (event.mouseButton.button == sf::Mouse::Left) {
                selectObjectAtPosition(worldPos);
                if (isObjectSelected) {
                    deleteSelectedObject();
                }
            }
            break;
            
        case EditMode::NONE:
        default:
            break;
    }
}

void DebugPanel::handleAddObject(const Point2D& position){
    switch(currentAddMode){
        case AddObjectMode::WALL:
            addWallAtPosition(position);
            break;
            
        case AddObjectMode::CIRCLE:
            addCircleAtPosition(position);
            break;
            
        case AddObjectMode::SQUARE:
            addSquareAtPosition(position);
            break;
            
        case AddObjectMode::TRIANGLE:
            addTriangleAtPosition(position);
            break;
            
        case AddObjectMode::POLYGON:
            addPolygonPoint(position);
            break;
            
        default:
            break;
    }
}

void DebugPanel::addWallAtPosition(const Point2D& position){
    if (!map) return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> colorDist(50, 255);
    
    unsigned int color = (colorDist(gen) << 24) | (colorDist(gen) << 16) | 
                        (colorDist(gen) << 8) | 0xFF;
    
    Wall newWall(position, 80, 40, color);
    map->addObject(std::make_shared<Wall>(newWall));
}

void DebugPanel::addCircleAtPosition(const Point2D& position){
    if (!map) {
        return;
    }
    Circle newCircle(position, 30, 0x3E3C32FF);

    map->addObject(std::make_shared<Circle>(newCircle));
}

void DebugPanel::addTriangleAtPosition(const Point2D& position){
    if (!map) return;

    std::vector<Point2D> trianglePoints = {
        Point2D(0, -50),
        Point2D(50, 50),
        Point2D(-50, 50)
    };
    
    Polygon2D newTriangle(position, trianglePoints, 0x3E3C32FF);
    map->addObject(std::make_shared<Polygon2D>(newTriangle));
}

void DebugPanel::selectObjectAtPosition(const Point2D& position){
    if (!map) return;
    
    selectedObject = nullptr;
    isObjectSelected = false;
    
    // Используем getObjects() для доступа к объектам
    const auto& objects = map->getObjects();
    for (const auto& obj : objects) {
        if (obj->getObjectType() == ObjectType::CIRCLE) {
            Circle* circle = dynamic_cast<Circle*>(obj.get());
            if (circle && circle->isCrossing(position)) {
                selectedObject = obj;
                isObjectSelected = true;
                break;
            }
        }
        else if (obj->getObjectType() == ObjectType::POLYGON) {
            Polygon2D* polygon = dynamic_cast<Polygon2D*>(obj.get());
            if (polygon && polygon->isCrossing(position)) {
                selectedObject = obj;
                isObjectSelected = true;
                break;
            }
        }
    }
    
    if (isObjectSelected) {
        editModeText.setString("Edit Mode: OBJECT SELECTED");
    }
}

void DebugPanel::deleteSelectedObject(){
    if (!map || !selectedObject) return;
    
    // Используем removeObject для удаления
    map->removeObject(selectedObject);

    selectedObject = nullptr;
    isObjectSelected = false;
    currentEditMode = EditMode::NONE;
    editModeText.setString("Edit Mode: NONE");
}

void DebugPanel::updateSliderValue(Slider& slider, double mouseX){
    double trackStart = slider.track.getPosition().x;
    double trackEnd = trackStart + slider.track.getSize().x;
    
    double clampedX = std::max(trackStart, std::min(mouseX, trackEnd));
    
    double normalized = (clampedX - trackStart) / slider.track.getSize().x;
    slider.currentValue = slider.minValue + normalized * (slider.maxValue - slider.minValue);
    
    slider.thumb.setPosition(clampedX - slider.thumb.getSize().x / 2, 
                           slider.track.getPosition().y - (slider.thumb.getSize().y - slider.track.getSize().y) / 2);
}

bool DebugPanel::isMouseOverPanel(const sf::Vector2f& mousePos) const{
    return debugPanel.getGlobalBounds().contains(mousePos);
}

void DebugPanel::drawLine(sf::RenderWindow& window, const Point2D& p1, const Point2D& p2, sf::Color color){
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(p1.getX(), p1.getY()), color),
        sf::Vertex(sf::Vector2f(p2.getX(), p2.getY()), color)
    };
    window.draw(line, 2, sf::Lines);
}

void DebugPanel::render(){
    window.draw(debugPanel);
    window.draw(fpsText);
    window.draw(cameraPosText);
    window.draw(editModeText);
    window.draw(addModeText);
    
    for (const auto& button : buttons){
        if (button.isActive){
            window.draw(button.shape);
            window.draw(button.text);
        }
    }
    
    for (const auto& slider : sliders){
        window.draw(slider.track);
        window.draw(slider.thumb);
        window.draw(slider.label);
        window.draw(slider.valueText);
    }
    
    // Отрисовка предпросмотра объекта
    if (isPlacingObject) {
        drawObjectPreview();
    }
    
    if (isDrawingPolygon && !polygonPoints.empty()) {
        for (const auto& point : polygonPoints) {
            sf::CircleShape dot(3);
            dot.setFillColor(sf::Color::Yellow);
            dot.setPosition(point.getX() - 3, point.getY() - 3);
            window.draw(dot);
        }
        
        if (polygonPoints.size() > 1) {
            for (size_t i = 1; i < polygonPoints.size(); ++i) {
                drawLine(window, polygonPoints[i-1], polygonPoints[i], sf::Color::Yellow);
            }
        }
    }
}
void DebugPanel::drawObjectPreview(){
    // Начальная точка
    sf::CircleShape startDot(3);
    startDot.setFillColor(sf::Color::Yellow);
    startDot.setPosition(objectStartPos.getX() - 3, objectStartPos.getY() - 3);
    window.draw(startDot);
    
    // Получаем текущую позицию мыши для предпросмотра
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    Point2D currentMousePos(static_cast<double>(mousePos.x), static_cast<double>(mousePos.y));
    
    // Предпросмотр в зависимости от типа объекта
    switch(currentAddMode){
        case AddObjectMode::WALL: {
            // Прямоугольник от начальной до текущей позиции мыши
            double minX = std::min(objectStartPos.getX(), currentMousePos.getX());
            double minY = std::min(objectStartPos.getY(), currentMousePos.getY());
            double width = std::abs(currentMousePos.getX() - objectStartPos.getX());
            double height = std::abs(currentMousePos.getY() - objectStartPos.getY());
            
            // Для стены определяем ориентацию
            sf::RectangleShape wallPreview;
            if (width > height) {
                // Горизонтальная стена
                wallPreview.setSize(sf::Vector2f(static_cast<float>(width), 10.0f));
            } else {
                // Вертикальная стена
                wallPreview.setSize(sf::Vector2f(10.0f, static_cast<float>(height)));
            }
            
            wallPreview.setFillColor(sf::Color(62, 60, 50, 128));
            wallPreview.setOutlineColor(sf::Color::Yellow);
            wallPreview.setOutlineThickness(2);
            wallPreview.setPosition(static_cast<float>(minX), static_cast<float>(minY));
            
            window.draw(wallPreview);
            break;
        }
        
        case AddObjectMode::SQUARE: { // Теперь это квадрат
            // Квадрат от начальной до текущей позиции мыши
            double minX = std::min(objectStartPos.getX(), currentMousePos.getX());
            double minY = std::min(objectStartPos.getY(), currentMousePos.getY());
            double width = std::abs(currentMousePos.getX() - objectStartPos.getX());
            double height = std::abs(currentMousePos.getY() - objectStartPos.getY());
            
            // Для квадрата берем максимальную сторону
            double side = std::max(width, height);
            
            sf::RectangleShape squarePreview(sf::Vector2f(static_cast<float>(side), static_cast<float>(side)));
            squarePreview.setFillColor(sf::Color(62, 60, 50, 128));
            squarePreview.setOutlineColor(sf::Color::Yellow);
            squarePreview.setOutlineThickness(2);
            squarePreview.setPosition(static_cast<float>(minX), static_cast<float>(minY));
            
            window.draw(squarePreview);
            break;
        }
        
        // ... остальные case остаются без изменений
        case AddObjectMode::CIRCLE: {
            double radius = objectStartPos.distance(currentMousePos);
            sf::CircleShape circlePreview(static_cast<float>(radius));
            circlePreview.setFillColor(sf::Color(62, 60, 50, 128));
            circlePreview.setOutlineColor(sf::Color::Yellow);
            circlePreview.setOutlineThickness(2);
            circlePreview.setPosition(objectStartPos.getX() - static_cast<float>(radius), 
                                    objectStartPos.getY() - static_cast<float>(radius));
            window.draw(circlePreview);
            break;
        }
        
        case AddObjectMode::TRIANGLE: {
            // Равносторонний треугольник
            sf::ConvexShape trianglePreview;
            trianglePreview.setPointCount(3);
            
            float sizeFloat = static_cast<float>(objectStartPos.distance(currentMousePos));
            
            // Вершины равностороннего треугольника
            trianglePreview.setPoint(0, sf::Vector2f(0, -sizeFloat));
            trianglePreview.setPoint(1, sf::Vector2f(sizeFloat * 0.866f, sizeFloat * 0.5f));
            trianglePreview.setPoint(2, sf::Vector2f(-sizeFloat * 0.866f, sizeFloat * 0.5f));
            
            trianglePreview.setFillColor(sf::Color(62, 60, 50, 128));
            trianglePreview.setOutlineColor(sf::Color::Yellow);
            trianglePreview.setOutlineThickness(2);
            trianglePreview.setPosition(objectStartPos.getX(), objectStartPos.getY());
            trianglePreview.setRotation(static_cast<float>(currentRotation));
            
            window.draw(trianglePreview);
            break;
        }
        
        default:
            break;
    }
}


void DebugPanel::addPolygonPoint(const Point2D& position){
    polygonPoints.push_back(position);
    
    std::stringstream ss;
    ss << "Add Mode: POLYGON - Points: " << polygonPoints.size() << " (RMB to finish)";
    addModeText.setString(ss.str());
    
    if (polygonPoints.size() >= 8) {
        addPolygonFromPoints();
    }
}

void DebugPanel::addPolygonFromPoints(){
    if (!map || polygonPoints.size() < 3) return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> colorDist(50, 255);
    
    unsigned int color = (colorDist(gen) << 24) | (colorDist(gen) << 16) | 
                        (colorDist(gen) << 8) | 0xFF;
    
    Point2D center = polygonPoints[0];
    std::vector<Point2D> relativePoints;
    
    for (const auto& point : polygonPoints) {
        relativePoints.push_back(point - center);
    }
    
    Polygon2D newPolygon(center, relativePoints, color);
    map->addObject(std::make_shared<Polygon2D>(newPolygon));
    
    polygonPoints.clear();
    isDrawingPolygon = false;
    currentAddMode = AddObjectMode::NONE;
    currentEditMode = EditMode::NONE;
    addModeText.setString("Add Mode: NONE");
    editModeText.setString("Edit Mode: NONE");
    
    std::cout << "✅ Added polygon with " << relativePoints.size() << " points" << std::endl;
}

void DebugPanel::changeSelectedObjectColor(){
    if (!selectedObject) return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> colorDist(50, 255);
    
    unsigned int newColor = (colorDist(gen) << 24) | (colorDist(gen) << 16) | 
                           (colorDist(gen) << 8) | 0xFF;
    
    selectedObject->setColor(newColor);
    std::cout << "🎨 Changed object color" << std::endl;
}

void DebugPanel::changeSelectedObjectSize(double newSize){
    if (!selectedObject) return;
    
    if (selectedObject->getObjectType() == ObjectType::CIRCLE) {
        Circle* circle = dynamic_cast<Circle*>(selectedObject.get());
        if (circle) {
            circle->setRadius(newSize);
            std::cout << "📏 Changed circle radius to " << newSize << std::endl;
        }
    }
}