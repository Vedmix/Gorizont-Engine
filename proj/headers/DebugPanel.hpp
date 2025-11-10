// DebugPanel.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.hpp"
#include "Point2D.hpp"
#include "Map.hpp"
#include "ObjectType.hpp"
#include <vector>
#include <string>
#include <functional>
#include <memory>

class DebugPanel{
private:
    sf::RenderWindow& window;
    Camera* camera;
    Map* map;

    sf::Font font;
    sf::Text fpsText;
    sf::RectangleShape debugPanel;
    sf::Text cameraPosText;
    sf::Text editModeText;
    sf::Text addModeText;

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
    
    // Режимы редактирования
    enum class EditMode {
        NONE,
        ADD_OBJECT,
        MOVE_OBJECT,
        DELETE_OBJECT,
        CHANGE_COLOR,
        CHANGE_SIZE
    };
    
    // Режимы добавления объектов
    enum class AddObjectMode {
        NONE,
        WALL,
        CIRCLE,
        POLYGON,
        SQUARE,
        TRIANGLE
    };
    
    EditMode currentEditMode;
    AddObjectMode currentAddMode;
    std::shared_ptr<Object2D> selectedObject;
    bool isObjectSelected;
    std::vector<Point2D> polygonPoints;
    bool isDrawingPolygon;
    
    // Поля для интерактивного создания объектов
    Point2D objectStartPos;
    bool isPlacingObject;
    double currentRotation;
    double currentSize;
    
public:
    DebugPanel(sf::RenderWindow& window);
    ~DebugPanel();

    void setCamera(Camera* cam);
    void setMap(Map* mapPtr);

    void update(double deltaTime);
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
    void resetSlidersToDefault();
    
    // Методы для управления объектами
    void handleObjectEditing(const sf::Vector2f& mousePos, const sf::Event& event);
    void handleAddObject(const Point2D& position);
    
    void addWallAtPosition(const Point2D& position);
    void addCircleAtPosition(const Point2D& position);
    void addSquareAtPosition(const Point2D& position);
    void addTriangleAtPosition(const Point2D& position);
    void addPolygonPoint(const Point2D& position);
    void addPolygonFromPoints();
    
    void selectObjectAtPosition(const Point2D& position);
    void deleteSelectedObject();
    void changeSelectedObjectColor();
    void changeSelectedObjectSize(double newSize);
    
    // Методы для интерактивного создания объектов
    void updateObjectPreview(const Point2D& currentMousePos);
    void finishObjectPlacement(const Point2D& endPos);
    void createWallInteractive(const Point2D& startPos, const Point2D& endPos, double size, double rotation);
    void createCircleInteractive(const Point2D& center, double radius, double rotation);
    void createSquareInteractive(const Point2D& startPos, const Point2D& endPos, double size, double rotation);
    void createTriangleInteractive(const Point2D& center, double size, double rotation);
    void drawObjectPreview();
    
    void drawLine(sf::RenderWindow& window, const Point2D& p1, const Point2D& p2, sf::Color color);
};