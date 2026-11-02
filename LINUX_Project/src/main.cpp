#include "../headers/MainMenuWindow.hpp"
#include "../headers/World.hpp"
#include "../headers/AppSettings.hpp"

#include <QApplication>
#include <cstdlib>
#include <QWidget>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qputenv("QT_QPA_PLATFORM", "xcb;wayland");

    app.setOrganizationName("Gorizont");
    app.setApplicationName("Gorizont");

    auto& settings = AppSettings::instance();

    AppSettings::applyTheme(AppSettings::instance().theme());

    MainMenuWindow mainWindow;
    mainWindow.resize(settings.screenWidth(), settings.screenHeight());
    mainWindow.show();

    return app.exec();

}
