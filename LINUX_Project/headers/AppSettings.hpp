#pragma once

#include <QSettings>
#include <QByteArray>
#include <QString>

class AppSettings {
public:

    static constexpr double DEFAULT_FOV = 1.5708;
    static constexpr double DEFAULT_RENDER_DISTANCE = 1000.0;
    static constexpr int DEFAULT_NUMBER_OF_RAYS = 1920;
    static constexpr double DEFAULT_PLAYER_SPEED = 150.0;

    static AppSettings& instance();

    //Параметры экрана
    int screenWidth() const;
    int screenHeight() const;

    //Сеттеры экрана
    void setScreenWidth(int width);
    void setScreenHeight(int height);

    //Параметры игры
    double fov() const;
    double renderDistance() const;
    int numberOfRays() const;
    double playerSpeed() const;

    //Сеттеры игры
    void setRenderDistance(double dist);
    void setFOV(double fov);
    void setNumberOfRays(int rays);
    void setPlayerSpeed(double speed);

    //Сохранение настроек
    void sync();

    QString mapPath() const;

    void setMapPath(const QString& path);

private:
    AppSettings() : m_settings("Gorizont", "Game") {}
    QSettings m_settings;
};
