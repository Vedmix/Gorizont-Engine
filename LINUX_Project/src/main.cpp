#include "../headers/MainMenuWindow.hpp"
#include "../headers/World.hpp"
#include "../headers/AppSettings.hpp"

#include <QApplication>
#include <QWidget>
#include <QFile>

int main(int argc, char *argv[])
{
    if(USE_QT){
        QApplication app(argc, argv);

        app.setOrganizationName("Gorizont");
        app.setApplicationName("Gorizont");

        auto& settings = AppSettings::instance();

        AppSettings::applyTheme(AppSettings::instance().theme());

        MainMenuWindow mainWindow;
        mainWindow.resize(settings.screenWidth(), settings.screenHeight());
        mainWindow.show();

        return app.exec();
    } else{
        World world;
        world.run();
        return 0;
    }
}