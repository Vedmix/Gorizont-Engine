#include <QApplication>
#include "MainMenuWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainMenuWindow MainMenu(nullptr,0);//1-запуск без GUI, 0- запуск с GUI
    MainMenu.show();

    return app.exec();
}
