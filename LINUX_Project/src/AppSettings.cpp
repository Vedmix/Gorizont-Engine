#include <../headers/AppSettings.hpp>


AppSettings& AppSettings::instance() {
    static AppSettings instance;
    return instance;
}

//Параметры экрана
int AppSettings::screenWidth() const {
    return m_settings.value("Display/Width", 1920).toInt();
}
int AppSettings::screenHeight() const {
    return m_settings.value("Display/Height", 1080).toInt();
}

//Сеттеры экрана
void AppSettings::setScreenWidth(int width) {
    m_settings.setValue("Display/Width", width);
}
void AppSettings::setScreenHeight(int height) {
    m_settings.setValue("Display/Height", height);
}

//Параметры игры
double AppSettings::fov() const {
    return m_settings.value("Game/FOV", 1.5708).toDouble();
}
double AppSettings::renderDistance() const {
    return m_settings.value("Game/RenderDistance", 1000.0).toDouble();
}
int AppSettings::numberOfRays() const {
    return m_settings.value("Game/NumberOfRays", 1920).toInt();
}
double AppSettings::playerSpeed() const {
    return m_settings.value("Game/PlayerSpeed", 150.0).toDouble();
}

//Сеттеры игры
void AppSettings::setRenderDistance(double dist) {
    m_settings.setValue("Game/RenderDistance", dist);
}
void AppSettings::setFOV(double fov) {
    m_settings.setValue("Game/FOV", fov);
}
void AppSettings::setNumberOfRays(int rays) {
    m_settings.setValue("Game/NumberOfRays", rays);
}
void AppSettings::setPlayerSpeed(double speed) {
    m_settings.setValue("Game/PlayerSpeed", speed);
}

//Сохранение настроек
void AppSettings::sync() {
    m_settings.sync();
}

QString AppSettings::mapPath() const {
    return m_settings.value("Last/MapPath", "maps/map2.xml").toString();
}

void AppSettings::setMapPath(const QString& path) {
    m_settings.setValue("Last/MapPath", path);
}
