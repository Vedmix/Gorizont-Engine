#include <QApplication>
#include <QMainWindow>
#include "main_window.h"
#include <QDebug>
#include <QTimer>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qDebug() << "Application starting...";

    MainWindow window;
    window.setWindowTitle("SFML + Qt5 Integration Test");
    window.resize(800, 600);
    window.show();

    qDebug() << "MainWindow shown";

    // Принудительно обновляем окно через короткое время
    QTimer::singleShot(100, []() {
        qDebug() << "Application started successfully";
    });

    return app.exec();
}
