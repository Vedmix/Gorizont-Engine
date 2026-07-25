#ifndef APPSETTINGS_HPP
#define APPSETTINGS_HPP

#include <QSettings>
#include <QByteArray>
#include <QString>

class AppSettings {
public:
    static AppSettings& instance() {
        static AppSettings instance;
        return instance;
    }

    //Параметры экрана
    int screenWidth() const {
        return m_settings.value("Display/Width", 1920).toInt();
    }
    int screenHeight() const {
        return m_settings.value("Display/Height", 1080).toInt();
    }

    //Сеттеры экрана
    void setScreenWidth(int width) {
        m_settings.setValue("Display/Width", width);
    }
    void setScreenHeight(int height) {
        m_settings.setValue("Display/Height", height);
    }

    //Параметры игры
    double fov() const {
        return m_settings.value("Game/FOV", 1.5708).toDouble();
    }
    double renderDistance() const {
        return m_settings.value("Game/RenderDistance", 1000.0).toDouble();
    }
    int numberOfRays() const {
        return m_settings.value("Game/NumberOfRays", 1920).toInt();
    }
    double playerSpeed() const {
        return m_settings.value("Game/PlayerSpeed", 150.0).toDouble();
    }

    //Сеттеры игры
    void setRenderDistance(double dist) {
        m_settings.setValue("Game/RenderDistance", dist);
    }
    void setFOV(double fov) {
        m_settings.setValue("Game/FOV", fov);
    }
    void setNumberOfRays(int rays) {
        m_settings.setValue("Game/NumberOfRays", rays);
    }
    void setPlayerSpeed(double speed) {
        m_settings.setValue("Game/PlayerSpeed", speed);
    }

    //Сохранение настроек
    void sync() {
        m_settings.sync();
    }

    QString mapPath() const {
        return m_settings.value("Last/MapPath", "maps/map2.xml").toString();
    }

    void setMapPath(const QString& path) {
        m_settings.setValue("Last/MapPath", path);
    }

private:
    AppSettings() : m_settings("Gorizont", "Game") {}
    QSettings m_settings;
};

#endif
