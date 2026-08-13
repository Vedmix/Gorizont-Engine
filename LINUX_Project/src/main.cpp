#include "../headers/MainMenuWindow.hpp"
#include "../headers/settings.hpp"
#include "../headers/World.hpp"
#include "../headers/AppSettings.hpp"

#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    if(USE_QT){
        QApplication app(argc, argv);

        app.setOrganizationName("Gorizont");
        app.setApplicationName("Gorizont");

        auto& settings = AppSettings::instance();

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
