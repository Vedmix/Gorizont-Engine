#include "../headers/MainMenuWindow.hpp"
#include "../headers/settings.hpp"
#include "../headers/World.hpp"

#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    if(USE_QT){
        QApplication app(argc, argv);
        MainMenuWindow mainWindow;
        mainWindow.show();
        return app.exec();
    } else{
        World world;
        world.run();
        return 0;
    }
}
