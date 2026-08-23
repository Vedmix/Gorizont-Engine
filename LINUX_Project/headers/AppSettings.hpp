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

    static constexpr const char* DEFAULT_THEME = "Dark";
    static constexpr const char* THEME_DARK = "Dark";
    static constexpr const char* THEME_LIGHT = "Light";
    static constexpr const char* THEME_CYBER= "Cyber";

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

    //Откат к настройкам по умолчанию
    void toDefaultSettings();

    //Сохранение настроек
    void sync();

    QString mapPath() const;
    QString theme() const;

    void setMapPath(const QString& path);
    void setTheme(const QString& theme);
    QStringList availableThemes() const;

private:
    AppSettings() : m_settings("Gorizont", "Game") {}
    QSettings m_settings;
    QStringList m_availableThemes = { THEME_DARK, THEME_LIGHT, THEME_CYBER };





};
