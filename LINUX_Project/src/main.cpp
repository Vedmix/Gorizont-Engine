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

        QFile styleFile(":/styles/styles/cyber_theme.qss");
        if (styleFile.open(QFile::ReadOnly)) {
            QString styleSheet = QLatin1String(styleFile.readAll());
            app.setStyleSheet(styleSheet);
            styleFile.close();
        }

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