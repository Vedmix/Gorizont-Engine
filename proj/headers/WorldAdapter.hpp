#pragma once

#include "World.hpp"
#include "Camera.hpp"
#include "Map.hpp"
#include <SFML/Graphics.hpp>
#include "settings.hpp"

class WorldAdapter
{
private:
    World m_world;

    // Приватные методы адаптера
    void drawMiniMap(sf::RenderTexture& texture);
    void drawFallback3D(sf::RenderTexture& texture);

public:
    WorldAdapter();

    void update(double deltaTime);
    void renderToTexture(sf::RenderTexture& texture);
    void handleEvents();

    // Адаптеры для ваших классов
    static void drawCameraView(Camera& camera, sf::RenderTexture& texture);
    static void drawCameraOnMap(Camera& camera, sf::RenderTexture& texture);
    static void renderMap(Map& map, sf::RenderTexture& texture);
    static void drawObject(Object2D& obj, sf::RenderTexture& texture, double mapScale);

    World& getWorld();
    const World& getWorld() const;
};
