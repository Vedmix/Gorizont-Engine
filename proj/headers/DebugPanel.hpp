#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.hpp"
#include "Point2D.hpp"
#include <vector>
#include <string>
#include <functional>

class DebugPanel{
private:
    sf::RenderWindow& window;
    Camera* camera; 

    sf::Font font;
    sf::Text fpsText;
    sf::RectangleShape debugPanel;
    sf::Text cameraPosText;

    struct Button{
        sf::RectangleShape shape;
        sf::Text text;
        std::function<void()> action;
        bool isActive;
    };

    struct Slider{
        sf::RectangleShape track;
        sf::RectangleShape thumb;
        sf::Text label;
        sf::Text valueText;
        double minValue;
        double maxValue;
        double currentValue;
        std::function<void(double)> onChange;
        bool isDragging;
    };

    std::vector<Button> buttons;
    std::vector<Slider> sliders;

public:
    DebugPanel(sf::RenderWindow& window);
    ~DebugPanel();

    void update(double deltaTime);
    void setCamera(Camera* cam);
    void render();
    void handleEvent(const sf::Event& event);
    
private:
    void PanelInit();
    void initMainFrame();
    void initTextElements();
    void initButtons();
    void initSliders();

    void addButton(const std::string& label, const Point2D& pos, std::function<void()> action);
    void addSlider(const std::string& label, const Point2D& pos, double minVal, double maxVal, double initVal, std::function<void(double)> onChange);
    void updateSliderValue(Slider& slider, double mouseX);
    bool isMouseOverPanel(const sf::Vector2f& mousePos) const;
    void defaultSliders(); 
};