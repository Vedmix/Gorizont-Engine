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
    return m_settings.value("Game/FOV", DEFAULT_FOV).toDouble();
}
double AppSettings::renderDistance() const {
    return m_settings.value("Game/RenderDistance", DEFAULT_RENDER_DISTANCE).toDouble();
}
int AppSettings::numberOfRays() const {
    return m_settings.value("Game/NumberOfRays", DEFAULT_NUMBER_OF_RAYS).toInt();
}
double AppSettings::playerSpeed() const {
    return m_settings.value("Game/PlayerSpeed", DEFAULT_PLAYER_SPEED).toDouble();
}
QString AppSettings::mapPath() const {
    return m_settings.value("Last/MapPath", "maps/map2.xml").toString();
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
void AppSettings::setMapPath(const QString& path) {
    m_settings.setValue("Last/MapPath", path);
}

//Соxранение параметров
void AppSettings::toDefaultSettings(){
    setRenderDistance(DEFAULT_RENDER_DISTANCE);
    setFOV(DEFAULT_FOV);
    setNumberOfRays(DEFAULT_NUMBER_OF_RAYS);
    setPlayerSpeed(DEFAULT_PLAYER_SPEED);
    setTheme(DEFAULT_THEME);
    sync();
}

//Сохранение параметров
void AppSettings::sync() {
    m_settings.sync();
}



//Параметры интерфейса
QString AppSettings::theme() const {
    return m_settings.value("Interface/Theme", DEFAULT_THEME).toString();
}

QStringList AppSettings::availableThemes() const {
    return m_availableThemes;
}

//Сеттеры интерфейса
void AppSettings::setTheme(const QString& theme) {
    if (m_availableThemes.contains(theme)) {
        m_settings.setValue("Interface/Theme", theme);
    }
}

void AppSettings::applyTheme(const QString& theme) {
    QString styleFile;
    if (theme == THEME_LIGHT) {
        styleFile = ":/styles/styles/light_theme.qss";
    } else if (theme == THEME_DARK) {
        styleFile = ":/styles/styles/dark_theme.qss";
    } else if (theme == THEME_CYBER) {
        styleFile = ":/styles/styles/cyber_theme.qss";
    } else {
        styleFile = ":/styles/styles/dark_theme.qss";
    }

    QFile file(styleFile);
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
        file.close();
    }
}