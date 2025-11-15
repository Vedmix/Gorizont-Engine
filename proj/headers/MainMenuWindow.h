#ifndef MAINMENUWINDOW_H
#define MAINMENUWINDOW_H

#include "World.hpp"
#include "GameWindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QTimer>
#include <QButtonGroup>
#include <QMainWindow>
#include <QPushButton>

class MainMenuWindow : public QMainWindow
{
    Q_OBJECT
private:
    QStringList buttonNames = {
        "Играть",
        "Настройки",
        "Авторы",
        "Выход"
    };
    void initMenu();
    GameWindow *gameWindow;


public:
    MainMenuWindow(QWidget *parent = nullptr, int choice = 0);
    ~MainMenuWindow();

private slots:
    void handleButton(int id);
    void onGameFinished();

};
#endif // MAINMENUWINDOW_H
