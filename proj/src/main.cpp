#include <QApplication>
#include "../headers/MainMenuWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainMenuWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
