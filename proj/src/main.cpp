#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow(nullptr,0);//1-запуск без GUI, 0- запуск с GUI
    mainWindow.show();

    return app.exec();
}
