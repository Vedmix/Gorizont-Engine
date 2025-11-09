#include "../headers/DebugPanel.hpp"
#include "../headers/settings.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>


DebugPanel::DebugPanel(sf::RenderWindow& window) : window(window), camera(nullptr){
    font.loadFromFile("../font.ttf");
    
    PanelInit();

}

DebugPanel::~DebugPanel(){

}

void DebugPanel::setCamera(Camera* cam) {
    camera = cam;
}

void DebugPanel::PanelInit(){
    initMainFrame();

    initTextElements();

    initButtons();

    initSliders();

    defaultSliders();
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
    fpsText.setPosition(startX + 20, 20);
    fpsText.setString("FPS: 0.0");
    
    cameraPosText.setFont(font);
    cameraPosText.setCharacterSize(14);
    cameraPosText.setFillColor(sf::Color::White);
    cameraPosText.setPosition(startX + 20, 50);
    cameraPosText.setString("Position: (0, 0)");
}

void DebugPanel::initButtons(){
    double startX = static_cast<double>(window.getSize().x) - UI_PANEL_WIDTH;
    
    addButton("Reset Camera", Point2D(startX + 20, 100), [this](){ 
                if (camera){
                    camera->setFOV(PI / 2);
                    camera->setRenderDistance(100);
                    camera->setNumberRaysInFov(SCREEN_WIDTH/8);
                    camera->setVelocity(300);

                    defaultSliders();
                }
              });
}

void DebugPanel::initSliders(){
    double startX = static_cast<double>(window.getSize().x) - UI_PANEL_WIDTH;
    double startY = 150.0f;
    double verticalSpacing = 50.0f;

    //Слайдер FOV 
    addSlider("FOV", Point2D(startX + 20, startY), 30.0f, 120.0f, 90.0f, [this](double value){ 
                if (camera){
                    camera->setFOV(value * 3.14159f / 180.0f);
                }
              });
    
    //Слайдер дистанции рендера 
    addSlider("Render Distance", Point2D(startX + 20, startY + verticalSpacing), 100.0f, 1000.0f, 100.0f, [this](double value){ 
                if (camera){
                    camera->setRenderDistance(value);
                }
              });
    
    //Слайдер скорости движения
    addSlider("Move Speed", Point2D(startX + 20, startY + verticalSpacing * 2), 100.0f, 1000.0f, 300.0f, [this](double value){ 
                if (camera){
                    camera->setVelocity(value);
                }
              });
    
    //Слайдер количества лучей
    addSlider("Ray Count", Point2D(startX + 20, startY + verticalSpacing * 3), 10.0f, 500.0f, static_cast<double>(SCREEN_WIDTH/8), [this](double value){ 
                if (camera){
                    camera->setNumberRaysInFov(static_cast<int>(value));
                }
              });
}

//Конструктор кнопок
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

//Конструктор слайдеров
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
    
    updateSliderValue(slider, 0);
    
    sliders.push_back(slider);
}

//Метод для сброса слайдеров к значениям из кнопки Reset
void DebugPanel::defaultSliders(){
    if (sliders.size() >= 4){
        sliders[0].currentValue = 90.0f;
        updateSliderValue(sliders[0], sliders[0].track.getPosition().x + (90.0f - 30.0f) / (120.0f - 30.0f) * 200.0f);
        sliders[1].currentValue = 100.0f;
        updateSliderValue(sliders[1], sliders[1].track.getPosition().x + (100.0f - 100.0f) / (1000.0f - 100.0f) * 200.0f);
        sliders[2].currentValue = 300.0f;
        updateSliderValue(sliders[2], sliders[2].track.getPosition().x + (300.0f - 100.0f) / (1000.0f - 100.0f) * 200.0f);
        sliders[3].currentValue = static_cast<double>(SCREEN_WIDTH/8);
        updateSliderValue(sliders[3], sliders[3].track.getPosition().x + (static_cast<double>(SCREEN_WIDTH/8) - 10.0f) / (500.0f - 10.0f) * 200.0f);
        
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

//Обработка событий
void DebugPanel::handleEvent(const sf::Event& event){
    if (event.type == sf::Event::MouseButtonPressed){
        sf::Vector2f mousePos(static_cast<double>(event.mouseButton.x), static_cast<double>(event.mouseButton.y));
        if (!isMouseOverPanel(mousePos))
        {
            return;
        }
        // Check button click
        if (!buttons.empty() && buttons[0].isActive && buttons[0].shape.getGlobalBounds().contains(mousePos)){
            buttons[0].shape.setFillColor(sf::Color(80, 80, 90));
            if (buttons[0].action)
            {
                buttons[0].action();
            }
        }
        //Проверка начала перетаскивания слайдеров
        for (auto& slider : sliders){
            if (slider.thumb.getGlobalBounds().contains(mousePos)){
                slider.isDragging = true;
                break;  // Этот break правильный - он внутри цикла for
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased){
        //Сброс цвета кнопоки и состояния перетаскивания
        if (!buttons.empty()) {
            buttons[0].shape.setFillColor(sf::Color(60, 60, 70));
        }
        for (auto& slider : sliders){
            slider.isDragging = false;
        }
    }
    else if (event.type == sf::Event::MouseMoved){
        sf::Vector2f mousePos(static_cast<double>(event.mouseMove.x), static_cast<double>(event.mouseMove.y));
        //Обработка перетаскивания слайдеров
        for (auto& slider : sliders){
            if (slider.isDragging){
                updateSliderValue(slider, mousePos.x);
                if (slider.onChange){
                    slider.onChange(slider.currentValue);
                }
            }
        }
    }
}

//Обновление значения слайдера
void DebugPanel::updateSliderValue(Slider& slider, double mouseX){
    double trackStart = slider.track.getPosition().x;
    double trackEnd = trackStart + slider.track.getSize().x;
    
    //Ограничиваем позицию мыши в пределах трека
    double clampedX = std::max(trackStart, std::min(mouseX, trackEnd));
    
    //Вычисляем значение
    double normalized = (clampedX - trackStart) / slider.track.getSize().x;
    slider.currentValue = slider.minValue + normalized * (slider.maxValue - slider.minValue);
    
    //Обновляем позицию бегунка
    slider.thumb.setPosition(clampedX - slider.thumb.getSize().x / 2, slider.track.getPosition().y - (slider.thumb.getSize().y - slider.track.getSize().y) / 2);
}

//Проверка выхода курсора за панель
bool DebugPanel::isMouseOverPanel(const sf::Vector2f& mousePos) const{
    return debugPanel.getGlobalBounds().contains(mousePos);
}

void DebugPanel::render(){
    window.draw(debugPanel);
    window.draw(fpsText);
    window.draw(cameraPosText);
    
    //Отрисовка кнопок
    for (const auto& button : buttons){
        if (button.isActive){
            window.draw(button.shape);
            window.draw(button.text);
        }
    }
    
    //Отрисовка слайдеров
    for (const auto& slider : sliders){
        window.draw(slider.track);
        window.draw(slider.thumb);
        window.draw(slider.label);
        window.draw(slider.valueText);
    }
}